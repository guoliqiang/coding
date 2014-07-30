#include <stdlib.h>
#include "../public/hc_cache.h"
#include "base/public/logging.h"
#include "base/public/hash.h"
#include "file/public/file.h"

#define FS_BLOCKSIZE 4096
#define PATH_SIZE 1000

void creat_sign_mds64(const char * head_buf, int head_buf_len,
                      u_int * sign0, u_int * sign1) {
  uint64 t = base::Fingerprint(head_buf, head_buf_len);
  *sign0 = t & 0xfffffffff;
  *sign1 = t >> 32;
}

// @brief [create and initialize a hc_cache structure]
// @param capacity [number of nodes of the hc_cache]
// @param file_size [the size of each file in the hc_cache(unit:MB)]
// @param file_num [the number of file the hc_cache uses]
// @param use_mode [use mode of the hc_cache]
// @param char *file_name [file_name including path of the files]
// @param valid_time [timeout for cache files(unit_size:s)]
// @return pointer of the hc_cache: NULL for error; others for success
hc_cache *hc_create_cache(u_int capacity, int file_size, int file_num,
                          int use_mode, char *file_name, time_t valid_time) {
  if (HC_NULL == capacity || 0 == capacity ||
      file_size > MAX_FILE_SIZE || file_size <= 0 ||
      file_num > MAX_FILE_NUM || file_num <= 0 ||
      NULL == file_name  || '\0' == file_name[0] ||
      (HAVE_ASSISTANT != use_mode && HAVE_NOASSISTANT != use_mode) ||
      strlen(file_name) >= PATH_SIZE - 5 || 0 == valid_time) {
    LOG(ERROR) << "param error!";
    return NULL;
  } else if (HAVE_ASSISTANT == use_mode) {
    LOG(ERROR) << "now this module can not support USE_ASSISTANT mode!";
    return NULL;
  }
  hc_cache *pcache = NULL;
  hc_indexnode *nodelist = NULL;
  hc_file *files = NULL;
  u_int *table = NULL;
  pcache = static_cast<hc_cache*>(malloc(1 * sizeof(hc_cache)));
  CHECK(pcache != NULL) << "Memory allocated for hd_cache Error!";

  u_int hash_num = capacity % 2 == 0 ?
      capacity + 1 : capacity;  // set hash_num
  table = static_cast<u_int*>(malloc(hash_num * sizeof(u_int)));
  CHECK(NULL != table) << "Memory allocated for hash_table Error!";
  memset(table, HC_NULL, hash_num*sizeof(u_int));  // set table to HC_NULL

  pthread_mutex_init(&pcache->hc_mutex_count, NULL);  // init_mutex

  nodelist = static_cast<hc_indexnode*>(
      malloc(capacity * sizeof(hc_indexnode)));
  CHECK(nodelist != NULL) << "Memory allocated for nodelist Error!";
  for (u_int i = 0; i < capacity; ++i) {
    nodelist[i].before = HC_NULL;
    nodelist[i].next = HC_NULL;
    nodelist[i].left = HC_NULL;
    nodelist[i].right = HC_NULL;
  }

  file_num = (file_num > 2) ? file_num : 2;  // 最少使用2个文件
  files = static_cast<hc_file *>(malloc(file_num * sizeof(hc_file)));
  CHECK(files != NULL) << "Memory allocated for filelist Error!";

  for (int j = 0; j < file_num; ++j) {
    files[j].handle = -1;
    files[j].status = NOT_USE;
    files[j].entry = HC_NULL;
    files[j].tail = HC_NULL;
    files[j].free_size = file_size * BYTES_PER_MB;
    files[j].buf = NULL;
  }
  memcpy(pcache->file_name, file_name, strlen(file_name) + 1);
  pcache->capacity = capacity;
  pcache->hashtable.hashnum = hash_num;
  pcache->hashtable.table = table;
  pcache->nodelist = nodelist;
  pcache->filelist.file_size = file_size * BYTES_PER_MB;
  pcache->filelist.file_num = file_num;
  pcache->filelist.files = files;
  pcache->rs_head = HC_NULL;
  pcache->in_curpos = 0;
  pcache->total_count = 0;
  pcache->use_mode = (use_mode == HAVE_ASSISTANT) ?
      HAVE_ASSISTANT : HAVE_NOASSISTANT;
  pcache->valid_time = valid_time;
  memset(&pcache->cache_stat, 0, sizeof(pcache->cache_stat));
  return pcache;
}

// @brief [destroy and free a hc_cache structure]
// @param pcache [pointer of the hc_cache]
// @return: this function always returns RT_NOTICE_PROCESSED for success
int hc_free_cache(hc_cache *pcache) {
  if (pcache != NULL) {
    if (pcache->nodelist != NULL) {
      free(pcache->nodelist);
      pcache->nodelist = NULL;
    }
    if (pcache->filelist.files != NULL) {
      for (int i = 0; i < pcache->filelist.file_num; ++i) {
        if (pcache->filelist.files[i].handle >= 0) {
          close(pcache->filelist.files[i].handle);
          pcache->filelist.files[i].handle = -1;
        }
      }
      free(pcache->filelist.files);
    }
    if (pcache->hashtable.table != NULL) {
      free(pcache->hashtable.table);
      pcache->hashtable.table = NULL;
    }
    pthread_mutex_destroy(&pcache->hc_mutex_count);
    free(pcache);
  }
  return RT_NOTICE_PROCESSED;
}

// @brief [get the pos of the sign from hash chain]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer to the 64bit sign of the item]
// @param last_pos [pointer to the tail of hash chain]
// @return: the pointer to the pos
inline u_int * hc_hash_getpos(hc_cache * pcache, u_int * sign, u_int * last_pos) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  CHECK(last_pos != NULL);
  u_int hashentry;  // hash entry
  u_int * ptmppos;  // the pointer to the sign, which will return
  hc_indexnode * pin;
  // calculate hash entry of the hash table
  hashentry = (sign[0] + sign[1]) % pcache->hashtable.hashnum;
  // check the node exist in the cache or not;
  ptmppos = pcache->hashtable.table + hashentry;
  while (*ptmppos != HC_NULL) {
    CHECK(*ptmppos <= pcache->capacity);
    pin = pcache->nodelist + *ptmppos;
    if ((pin->sign1 == sign[0]) && (pin->sign2 == sign[1])) break;
    else {
      *last_pos = *ptmppos;
      ptmppos = &(pin->next);
    }
  }
  return ptmppos;
}

// @brief [remove the oldest file from the cache]
// @param pcache [pointer of the hc_cache]
// @return: RT_NOTICE_PROCESSED for success; other for failure
static int hc_removefile(hc_cache *pcache) {
  CHECK(pcache);
  int offset = pcache->filelist.oldest_offset;  // get oldest_offset
  // get the pointer of the oldest file
  hc_file *fin = &(pcache->filelist.files[offset]);
  if (fin->status != NEED_DELETE || fin->thread_in_read > 0) {
    LOG(ERROR) << "the file can't bd deleted due to status " << fin->status
               << " or thread_in_read " << fin->thread_in_read;
    return RT_NOTICE_NONE_PROCESSED;
  }
  int node_num_inlist = fin->node_num_inlist;
  u_int pos = fin->entry;
  hc_indexnode * pin = NULL;
  while ((pos=fin->entry) != HC_NULL) {
    pin = pcache->nodelist + pos;
    u_int hash_entry = (pin->sign1+pin->sign2) % pcache->hashtable.hashnum; 
    if (pin->before != HC_NULL) {  // modify hash chain
      pcache->nodelist[pin->before].next = pin->next;
    } else {  // pin是hash_chian中的第一个节点，需要修改hash_entry
      pcache->hashtable.table[hash_entry] = pin->next;
    }
    if (pin->next != HC_NULL) {  // modify hash chain
      pcache->nodelist[pin->next].before = pin->before;
    }
    if (pin->left != HC_NULL) {  // modify file chain
      // in this function, pin->left should be HC_NULL
      // so it should core here
      CHECK(pin->left == HC_NULL);
      pcache->nodelist[pin->left].right = pin->right;
    }
    if (pin->right != HC_NULL) {  // modify file chain
      pcache->nodelist[pin->right].left = pin->left;
    }
    // modify rs_head
    pin->next = pcache->rs_head;
    pcache->rs_head = pos;
    // modify file entry
    fin->entry = pin->right;
    // modyfy node info
    pin->before = HC_NULL;
    pin->left = HC_NULL;
    pin->right = HC_NULL;
    pin->sign1 = 0;
    pin->sign2 = 0;
    pin->file_offset = 0;
    pin->pos_offset = 0;
    pin->length = 0;
    pin->click_time = 0;
    // modify node_num_inlist
    --fin->node_num_inlist;
    //  modigy total count
    --pcache->total_count;
  }
  CHECK(0 == fin->node_num_inlist);
  // close the file
  if (fin->handle >= 0) {
    close(fin->handle);
    fin->handle = -1;
  }
  fin->status = NOT_USE;
  fin->entry = HC_NULL;
  fin->tail = HC_NULL;
  fin->free_size = pcache->filelist.file_size;
  fin->est_time = 0;
  pcache->filelist.oldest_offset =
    (pcache->filelist.oldest_offset+1) % pcache->filelist.file_num;
  LOG(INFO) << "hc_cache deleted file "<< offset;
  pcache->cache_stat.remove_count += node_num_inlist;
  return RT_NOTICE_PROCESSED;
}

// @brief [remove num nodes from the cache]
// @param pcache [pointer of the hc_cache]
// @param pcache [the num of node that will be deleted]
// @return: RT_NOTICE_PROCESSED for success; other for failure
static int hc_removenum(hc_cache *pcache, int num) {
  CHECK(pcache != NULL);
  int offset = pcache->filelist.oldest_offset;  // get oldest_offset
  hc_file *fin = &(pcache->filelist.files[offset]);  // get the pointer of the oldest file
  if (NOT_USE == fin->status) {
    LOG(ERROR) << "the file of offset " << offset << " is not used";
    return RT_NOTICE_NONE_PROCESSED;
  }
  // 由于只是删除num个节点，因此可以允许文件状态为USING_IN_WRITE
  // 此时如果num>node_num_inlist,则会调用hc_removefile
  // 会返回RT_NOTICE_NONE_PROCESSED
  if (num <= 0) {
    LOG(ERROR) << "the number " << num << " that want to delete is invalid";
    return RT_NOTICE_NONE_PROCESSED;
  }
  if (num > fin->node_num_inlist) {  // num > node_num_inlist
    if (fin->node_num_inlist > 0) {  // node_num_inlist > 0 则返回hc_removefile
      return hc_removefile(pcache);
    }
    CHECK(0 == fin->node_num_inlist);
    if (fin->status == NEED_DELETE && fin->thread_in_read == 0) {
      if (fin->handle >= 0) {
        close(fin->handle);
        fin->handle = -1;
      }
      fin->status = NOT_USE;
      fin->entry = HC_NULL;
      fin->tail = HC_NULL;
      fin->free_size = pcache->filelist.file_size;
      fin->est_time = 0;
      pcache->filelist.oldest_offset =
        (pcache->filelist.oldest_offset+1) % pcache->filelist.file_num;
    } else if (fin->status == USING_IN_READ) {
      fin->status = NEED_DELETE;
    }
    LOG(INFO) << "hc_cache deleted empty file :" << offset;
    // 上述操作没有实质删除节点，因而返回RT_NOTICE_NONE_PROCESSED
    return RT_NOTICE_NONE_PROCESSED;
  }
  // 由于只是删除num个节点，因此可以允许文件状态为USING_IN_WRITE
  // 此时如果num>node_num_inlist,则会调用hc_removefile
  // 会返回RT_NOTICE_NONE_PROCESSED
  u_int pos = fin->entry;
  hc_indexnode *pin = NULL;
  int i = 0;
  while (i<num) {
    pin = pcache->nodelist + pos;
    if (pin->before != HC_NULL) {  // modify hash chain
      pcache->nodelist[pin->before].next = pin->next;
    } else {  // pin是hash_chian中的第一个节点，需要修改hash_entry
      u_int hash_entry = (pin->sign1+pin->sign2)
        % pcache->hashtable.hashnum;
      pcache->hashtable.table[hash_entry] = pin->next;
    }
    if (pin->next != HC_NULL) {  // modify hash chain
      pcache->nodelist[pin->next].before = pin->before;
    }
    if (pin->left != HC_NULL) {  // modify file chain
      // in this function, pin->left should be HC_NULL
      // so it should core here
      CHECK(pin->left == HC_NULL);
      pcache->nodelist[pin->left].right = pin->right;
    }
    if (pin->right != HC_NULL) {  // modify file chain
      pcache->nodelist[pin->right].left = pin->left;
    }
    fin->entry = pin->right;  // update_entry
    pin->next = pcache->rs_head;
    pcache->rs_head = pos;
    pos = fin->entry;  // update pos
    // modify node
    pin->before = HC_NULL;
    pin->left = HC_NULL;
    pin->right = HC_NULL;
    pin->sign1 = 0;
    pin->sign2 = 0;
    pin->file_offset = 0;
    pin->pos_offset = 0;
    pin->length = 0;
    pin->click_time = 0;
    //  modify node_num_inlist
    --fin->node_num_inlist;
    //  modify total count
    --pcache->total_count;
    ++i;
  }
  // 处理恰好删光节点的情况
  if (HC_NULL == fin->entry) {
    fin->tail = HC_NULL;
  }
  if (0 == fin->node_num_inlist && fin->status != USING_IN_WRITE) {
    if (fin->status == NEED_DELETE && fin->thread_in_read == 0) {
      if (fin->handle >= 0) {
        close(fin->handle);
        fin->handle = -1;
      }
      fin->status = NOT_USE;
      fin->entry = HC_NULL;
      fin->tail = HC_NULL;
      fin->free_size = pcache->filelist.file_size;
      fin->est_time = 0;
      pcache->filelist.oldest_offset =
        (pcache->filelist.oldest_offset+1) % pcache->filelist.file_num;
    } else if (fin->status != NEED_DELETE) {
      CHECK(USING_IN_READ == fin->status);
      fin->status = NEED_DELETE;
    }
  }
  pcache->cache_stat.remove_count += num;
  return RT_NOTICE_PROCESSED;
  // 上述操作都删除了num个节点，因此返回RT_NOTICE_PROCESSED
}

// @brief [write item to the file]
// @param pcache [the pointer of hc_cache]
// @param pdata [the data will be written to the file]
// @param size [size of pdata(unitsize: bytes)]
// @return RT_NOTICE_PROCESSED for success, others for failure
static int hc_writeitem(hc_cache *pcache, void *pdata, u_int size) {
  char pathname[PATH_SIZE];
  int offset = pcache->filelist.newest_offset;
  int offset_new;
  int ret;
  char tmp = '\0';
  hc_file *fin = &(pcache->filelist.files[offset]);
  if (fin->status != USING_IN_WRITE) {  //  如果处于最新位置的文件不可写
    if (NOT_USE == fin->status) {  // 查看该文件是否未被使用
      sprintf(pathname, "%s.%d", pcache->file_name, offset);// 生成文件名
      if ((fin->handle = open(pathname, O_CREAT | O_RDWR,
              S_IRUSR | S_IWUSR)) < 0) {  // 以可读写,非截断的方式打开文件
        LOG(ERROR) << "open file" << pathname << " failure!";
        return RT_ERROR_GENERAL_ERROR;
      }
      // 占位
      if (pwrite(fin->handle, &tmp, sizeof(char),
          pcache->filelist.file_size - sizeof(char)) != sizeof(char)) {
        LOG(ERROR) << "pwrite() error!";
        close(fin->handle);  // 占位失败,需要关闭socket
        fin->handle = -1;
        return RT_ERROR_GENERAL_ERROR;
      }
      fin->status = USING_IN_WRITE;
    } else {
      LOG(ERROR) << "the status" << fin->status
                 << " of the newest_offset " << offset << " file is wrong!";
      return RT_ERROR_GENERAL_ERROR;
    }
  }
  if (size > fin->free_size) {  // 如果要写的长度大于可写长度
    if (size > pcache->filelist.file_size) {  // 进一步的,如果大于文件长度
      LOG(ERROR) << "the size " << pcache->filelist.file_size
                 << " of the item is too large!";
      return RT_NOTICE_NONE_PROCESSED;
    }
    offset_new = (pcache->filelist.newest_offset + 1)
      % pcache->filelist.file_num; //  get newest offset
    // 如果所有文件都在使用
    if (offset_new == pcache->filelist.oldest_offset) {
      CHECK(pcache->filelist.files[offset_new].status != USING_IN_WRITE);
      CHECK(pcache->filelist.files[offset_new].status != NOT_USE);
      if ((ret = hc_removefile(pcache)) != RT_NOTICE_PROCESSED) { //  删除文件失败
        pcache->filelist.files[offset_new].status = NEED_DELETE; // 置位
        return ret;
      }
      LOG(ERROR) << "no file can write, delete the oldestone : " << offset_new;
    }
    pcache->filelist.newest_offset = offset_new;
    // 关闭当前文件
    fin->est_time = time((time_t*)NULL); //  获取文件建立时间
    close(fin->handle);
    // 以只读模式重新打开该文件
    sprintf(pathname, "%s.%d", pcache->file_name, offset);
    if ((fin->handle = open(pathname, O_RDONLY, S_IRUSR)) < 0) {
      LOG(ERROR) << "open file:" << pathname << " failure!";
      return RT_ERROR_GENERAL_ERROR;
    }
    fin->status = USING_IN_READ;
    // 打开一个新的文件,可读写,非截断(截断可能耗时过长)
    offset = pcache->filelist.newest_offset;  // get newest offset
    fin = &(pcache->filelist.files[offset]);  // get pointer of the file
    sprintf(pathname, "%s.%d", pcache->file_name, offset);
    if ((fin->handle = open(pathname, O_CREAT | O_RDWR,
            S_IRUSR | S_IWUSR)) < 0) {
      LOG(ERROR) << "open file:" << pathname << " failure!";
      return RT_ERROR_GENERAL_ERROR;
    }
    // 占位
    if (pwrite(fin->handle, &tmp, sizeof(char),
        pcache->filelist.file_size - sizeof(char)) != sizeof(char)) {
      LOG(ERROR) << "pwrite() error!";
      close(fin->handle); // 占位失败,需要关闭socket
      fin->handle = -1;
      return RT_ERROR_GENERAL_ERROR;
    }
    fin->status = USING_IN_WRITE;
  }
  //  写文件
  ssize_t num;
  CHECK(fin->handle >= 0);
  if ((num = pwrite(fin->handle, (void*)pdata, size,
      pcache->filelist.file_size - fin->free_size)) != size) {
    LOG(ERROR) << "pwrite() error! size=" << size << " num=" << num;
    return RT_ERROR_GENERAL_ERROR;
  }
  return RT_NOTICE_PROCESSED;
}

// @brief [add a item to hc_cache]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// @param pdata [pointer of the data of the item]
// @param size [the size of item, unitsize: bit]
// @return: RT_NOTICE_PROCESSED for success, others for failure
int hc_additem(hc_cache *pcache, u_int *sign, void *pdata, u_int size) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  CHECK(pdata != NULL);
  CHECK(size > 0);
  int ret;
  u_int * ptmppos;
  u_int hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK(ptmppos != NULL);
  if (*ptmppos != HC_NULL) {
    LOG(ERROR) << "the item is already in the hc_cache";
    return RT_NOTICE_NONE_PROCESSED;
  }
  // find insert position
  u_int curpos;
  if (pcache->rs_head != HC_NULL) {
    // get pos from reclaim stack
    curpos = pcache->rs_head;
    pcache->rs_head = (pcache->nodelist + curpos)->next;
  } else if (pcache->in_curpos < pcache->capacity) {
    // get pos from in_curpos
    curpos = pcache->in_curpos;
    pcache->in_curpos++;
  } else {
    // the nodelist is full
    if ((ret=hc_removenum(pcache, 1)) != RT_NOTICE_PROCESSED) {
      // delete the oldest one
      LOG(ERROR) << "remove one node from nodelist failure!";
      return ret;
    }
    CHECK(pcache->rs_head != HC_NULL);  // now rs_head mustn't be HC_NULL
    curpos = pcache->rs_head;
    pcache->rs_head = HC_NULL;
  }
  hc_indexnode * pin = &(pcache->nodelist[curpos]);
  ret = hc_writeitem(pcache, pdata, size);
  if (ret != RT_NOTICE_PROCESSED) {  // pin未使用，将该节点放回回收栈
    pin->next = pcache->rs_head;
    pcache->rs_head = curpos;
    return ret;
  }
  // 前面的操作中可能会有删除操作，会改变hash_chain中的前后关系
  // 因此这里需要重新计算一次hash_chain
  hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);
  CHECK(ptmppos != NULL && HC_NULL == *ptmppos);
  *ptmppos = curpos;  // add to hashtable or chage next
  int offset = pcache->filelist.newest_offset;
  // hc_writeitem可能改变newest offset
  hc_file *fin = &(pcache->filelist.files[offset]);
  // update node information
  pin->sign1 = sign[0];
  pin->sign2 = sign[1];
  pin->file_offset = pcache->filelist.newest_offset;
  pin->pos_offset = pcache->filelist.file_size - fin->free_size;
  pin->length = size;
  pin->before = hash_tail;  // before
  if (hash_tail != HC_NULL) {  // update hash chain
    pcache->nodelist[hash_tail].next = curpos;
    // unnecessary due to update *ptmppos already
  }
  pin->next = HC_NULL;  // next
  pin->left = fin->tail;  // left
  if (pin->left != HC_NULL) {  // update file chain
    pcache->nodelist[pin->left].right = curpos;
  }
  pin->right = HC_NULL;  // right
  pin->click_time = 0;
  if (pin->left == HC_NULL) {
    CHECK(fin->node_num_inlist == 0);
  }
  // update newest file information
  if (HC_NULL == fin->entry) {
    CHECK(0 == fin->node_num_inlist);
    fin->entry = curpos;  // entry
  }
  fin->tail = curpos;  // tail
  fin->free_size -= size;  // free_size
  fin->node_num_inlist++;  // node_num_inlis
  pcache->total_count++;
  pcache->cache_stat.add_count++;
  return RT_NOTICE_PROCESSED;
}

// @brief [read a item from hc_cache]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// @param pdata [pointer of the data of the item]
// @param size [the size of item, unitsize: bit]
// @param fact_size [output the fact size of the seeked item if fact_size != NULL]
// @param click_time [output the number of the item seekd if click_time != NULL]
// @return: RT_NOTICE_PROCESSED for success, others for failure
int hc_seekitem(hc_cache *pcache, u_int * sign,
                void *pdata, u_int size, u_int *fact_size, int *click_time) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  CHECK(pdata != NULL);
  u_int *ptmppos = NULL;
  u_int hash_tail = HC_NULL;
  pcache->cache_stat.seek_count++;
  // 获取节点位置
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK(ptmppos != NULL);
  if (HC_NULL == *ptmppos) { //  如果节点不在nodelist中
    return RT_NOTICE_NONE_PROCESSED;
  }
  //  get pointer of node
  hc_indexnode *pin = &(pcache->nodelist[*ptmppos]);
  //  get pointer of file
  hc_file *fin = &(pcache->filelist.files[pin->file_offset]);
  if (NEED_DELETE == fin->status || NEED_PHYSICAL_DELETE == fin->status) {
    //  检查节点有效性
    LOG(ERROR) << "the file:" <<  pin->file_offset << " that saved the item sign1:"
               << sign[0] << " sign2:" << sign[1] << " is overdue";
    return RT_NOTICE_NONE_PROCESSED;
  }
  if (size < pin->length) {  //  输入buf的size不够
    LOG(ERROR) << "the size " << size << " of pdata is less than the length "
               <<  pin->length << " of the node!";
    return RT_NOTICE_NONE_PROCESSED;
  }
  CHECK((pin->pos_offset + pin->length)
        <= (pcache->filelist.file_size - fin->free_size));  // 总长度不应越界
  CHECK(fin->status != NOT_USE);  // 状态不能为 NOT_USED
  CHECK(fin->handle >= 0);
  ssize_t num;
  num = pread(fin->handle, pdata, pin->length, pin->pos_offset);  // 读文件
  if (num != (ssize_t)pin->length) {
    if (num < 0) {
      LOG(ERROR) << "read error: " << strerror(errno);
      return RT_ERROR_GENERAL_ERROR;
    } else {
      LOG(ERROR) << "read size notmatch:" << size << "!=" << num;
      return RT_ERROR_GENERAL_ERROR;
    }
  }
  ++pin->click_time; // update click_time
  CHECK(fact_size != NULL);
  if (fact_size != NULL) *fact_size = pin->length;
  if (click_time != NULL) *click_time = pin->click_time;
  pcache->cache_stat.hit_count++;
  return RT_NOTICE_PROCESSED;
}

// @brief [seek item from hc_cache and only return info]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// @param size [the size of item, unitsize: bit]
// @param fact_size [output the fact size of the seeked item]
// @param file_offset [output the offset of the files that the item saved]
// @param pos_offset [output the offset of the item in the file]
// @param click_time [output the number of the item seekd if click_time != NULL]
// @return: RT_NOTICE_PROCESSED for success, others for failure
int hc_seekitem_r(hc_cache *pcache, u_int *sign, u_int size,
        u_int *fact_size, u_int *file_offset,
        u_int *pos_offset, int *click_time) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  CHECK(fact_size != NULL);
  CHECK(file_offset != NULL);
  CHECK(pos_offset != NULL);
  pcache->cache_stat.seek_count++;
  u_int *ptmppos = NULL;
  u_int hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);  // 获取节点位置
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK (ptmppos != NULL);
  if (HC_NULL == *ptmppos) { // 如果节点不在nodelist中
    return RT_NOTICE_NONE_PROCESSED;
  }
  // get pointer of node
  hc_indexnode *pin = &(pcache->nodelist[*ptmppos]);
  // get pointer of file
  hc_file *fin = &(pcache->filelist.files[pin->file_offset]);
  if (NEED_DELETE == fin->status || NEED_PHYSICAL_DELETE == fin->status) {
    //  检查节点有效性
    LOG(ERROR) << "the file " <<  pin->file_offset << " that saved the item sign1:"
               << sign[0] << " sign2:" << sign[1] << " is overdue!";
    return RT_NOTICE_NONE_PROCESSED;
  }
  if (size < pin->length) {  // 输入buf的size不够
    LOG(ERROR) << "the size " << size << " of pdata is less than the length "
               << pin->length << " of the node!";
    return RT_NOTICE_NONE_PROCESSED;
  }
  CHECK((pin->pos_offset+pin->length)
      <= (pcache->filelist.file_size-fin->free_size));  // 总长度不应越界
  CHECK(fin->status != NOT_USE);  // 状态不能为 NOT_USE
  CHECK(fin->handle >= 0);
  *file_offset = pin->file_offset;  // output file_offset
  *pos_offset = pin->pos_offset;  // output pos_offset
  *fact_size = pin->length;  // output length
  ++pin->click_time;  // update click_time
  if (click_time != NULL) {
    *click_time = pin->click_time;  // output click_time
  }
  pthread_mutex_lock(&pcache->hc_mutex_count);
  ++fin->thread_in_read;
  pthread_mutex_unlock(&pcache->hc_mutex_count);
  pcache->cache_stat.hit_count++;
  return RT_NOTICE_PROCESSED;
}

// @brief [reset click time of the node assigned by sign]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// return RT_NOTICE_PROCESSED for success, others for failure
int hc_reset_click_time(hc_cache *pcache, u_int *sign) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  u_int *ptmppos = NULL;
  u_int hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);// 获取节点位置
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK(ptmppos != NULL);
  if (HC_NULL == *ptmppos) {  // 如果节点不在nodelist中
    return RT_NOTICE_NONE_PROCESSED;
  }
  // get pointer of node
  hc_indexnode *pin = &(pcache->nodelist[*ptmppos]);
  pin->click_time = 0;
  return RT_NOTICE_PROCESSED;
}

// @brief [remove a item by sign from hc_cache]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// @param click_time [output the click times of the removed item if it's not NULL]
// @return: RT_NOTICE_PROCESSED for success, others for failure
int hc_removeitem(hc_cache *pcache, u_int *sign, int *click_time) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  u_int *ptmppos = NULL;
  u_int hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);// 获取节点位置
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK(ptmppos != NULL);
  if (HC_NULL == *ptmppos) {  // 如果节点不在nodelist中
    return RT_NOTICE_NONE_PROCESSED;
  }
  u_int rm_pos = *ptmppos;
  // get pointer of node
  hc_indexnode *pin = &(pcache->nodelist[rm_pos]);
  // get pointer of file
  hc_file *fin = &(pcache->filelist.files[pin->file_offset]);
  if (pin->before != HC_NULL) {  // modify hash chain
    pcache->nodelist[pin->before].next = pin->next;
  } else { //  pin是hash_chian中的第一个节点，需要修改hash_entry
    u_int hash_entry = (pin->sign1+pin->sign2)
      % pcache->hashtable.hashnum;
    pcache->hashtable.table[hash_entry] = pin->next;
  }
  if (pin->next != HC_NULL) {  // modify hash chain
    pcache->nodelist[pin->next].before = pin->before;
  }
  if (pin->left != HC_NULL) {  // modify file chain
    pcache->nodelist[pin->left].right = pin->right;
  } else {
    fin->entry = pin->right;  // pin是文件的第一个节点
  }
  if (pin->right != HC_NULL) {  // modify file chain
    pcache->nodelist[pin->right].left = pin->left;
  } else {
    fin->tail = pin->left;  // pin是文件的最后一个节点
  }
  // inters to reclaim stack
  pin->next = pcache->rs_head;
  pcache->rs_head = rm_pos;
  --pcache->total_count;  // update total_count
  // init node
  pin->before = HC_NULL;
  pin->left = HC_NULL;
  pin->right = HC_NULL;
  pin->sign1 = 0;
  pin->sign2 = 0;
  pin->file_offset = 0;
  pin->pos_offset = 0;
  pin->length = 0;
  // modify fin->node_num_inlist
  --fin->node_num_inlist;
  if (click_time != NULL) {  // output click time
    *click_time = pin->click_time;
  }
  pin->click_time = 0;
  pcache->cache_stat.remove_count++;
  return RT_NOTICE_PROCESSED;
}

// @brief [modify a item that in hc_cache]
// @param pcache [pointer of the hc_cache]
// @param sign [pointer of the 64bit sign of the item]
// @param pdata [pointer of the data of the item]
// @param size [the size of item, unitsize: bit]
// @return: RT_NOTICE_PROCESSED for success, others for failure
int hc_modifyitem(hc_cache *pcache, u_int *sign, void *pdata, u_int size) {
  CHECK(pcache != NULL);
  CHECK(sign != NULL);
  CHECK(pdata != NULL);
  CHECK(size > 0);
  int ret;
  u_int *ptmppos = NULL;
  u_int hash_tail = HC_NULL;
  ptmppos = hc_hash_getpos(pcache, sign, &hash_tail);  // 获取节点位置
  // the logic of hc_hash_getpos will not return NULL
  // so (ptmppos == NULL) shows serious error occurs
  CHECK(ptmppos != NULL);
  if (HC_NULL == *ptmppos) {  // 如果节点不在nodelist中
    return RT_NOTICE_NONE_PROCESSED;
  }
  u_int md_pos = *ptmppos;
  hc_indexnode *pin = &(pcache->nodelist[md_pos]);  // get pointer of node
  int offset_old = pin->file_offset;  // get offset old
  hc_file *finold = &(pcache->filelist.files[offset_old]);  // get pointer of file
  // 如果old node满足下面的条件
  // 则通过删除-添加的方式进行更新
  // 因为在写文件时，有可能将节点本身删除
  // 同时该节点也不可能再被应用，即使add失败也无需保留
  if (offset_old == pcache->filelist.oldest_offset
      && NEED_DELETE == finold->status) {
    int click_time;
    ret = hc_removeitem(pcache, sign, &click_time);
    if (RT_NOTICE_PROCESSED == ret) {
      return hc_additem(pcache, sign, pdata, size);
    } else {
      return ret;
    }
  }
  // 进行写操作，同时该节点在写的过程中不会被删除
  ret = hc_writeitem(pcache, pdata, size);
  if (ret != RT_NOTICE_PROCESSED) {
    // 如果写失败,则保留原节点内容,直接返回交由调用函数处理
    return ret;
  }
  int offset = pcache->filelist.newest_offset;// get newest offset
  hc_file *fin = &(pcache->filelist.files[offset]);// get pointer of file
  // 更新节点在旧文件中的信息
  if (pin->left != HC_NULL) {
    pcache->nodelist[pin->left].right = pin->right;
  } else {
    finold->entry = pin->right;  // pin是文件的第一个节点
  }
  if (pin->right != HC_NULL) {
    pcache->nodelist[pin->right].left = pin->left;
  } else {
    finold->tail = pin->left;  // pin是文件的最后一个节点
  }
  --finold->node_num_inlist;
  if (HC_NULL == pin->left && HC_NULL == pin->right) {
    CHECK(finold->node_num_inlist == 0);
  }
  // update node information
  // hash chain 不用更改(before, next不变)
  // click_time保持原来的
  pin->file_offset = pcache->filelist.newest_offset;// file_offset
  pin->pos_offset = pcache->filelist.file_size - fin->free_size;// pos_offset
  pin->length = size;
  pin->left = fin->tail;  // left:写操作永远是添加到最新文件的队尾
  if (pin->left != HC_NULL) {
    pcache->nodelist[pin->left].right = md_pos;
  }
  pin->right = HC_NULL;  // right
  if (pin->left == HC_NULL) {
    CHECK(0 == fin->node_num_inlist);
  }
  // update newest file information
  if (HC_NULL == fin->entry) {
    CHECK(0 == fin->node_num_inlist);
    fin->entry = md_pos;  // entry
  }
  fin->tail = md_pos;  // tail
  fin->free_size -= size;  // free_size
  ++fin->node_num_inlist;  // node_num_inlist
  return RT_NOTICE_PROCESSED;
}

// @brief [clear the hc_cache]
// @param pcache [pointer of the hc_cache]
// @return: this function always returns RT_NOTICE_PROCESSED for success
#define INITIAL_OLDEST_OFFSET 5
int hc_clean_cache(hc_cache *pcache) {
  CHECK(pcache != NULL);
  // 清理文件
  for (int i=0; i<pcache->filelist.file_num; ++i) {
    if (pcache->filelist.files[i].handle>=0) {
      close(pcache->filelist.files[i].handle);
      pcache->filelist.files[i].handle = -1;
    }
    pcache->filelist.files[i].status =
      (i < INITIAL_OLDEST_OFFSET ? NOT_USE : NEED_DELETE);
    pcache->filelist.files[i].entry = HC_NULL;
    pcache->filelist.files[i].tail = HC_NULL;
    pcache->filelist.files[i].free_size = pcache->filelist.file_size;
    pcache->filelist.files[i].node_num_inlist = 0;
    pcache->filelist.files[i].buf = NULL;
    pcache->filelist.files[i].est_time = 0;
  }
  pcache->filelist.newest_offset = 0;
  pcache->filelist.oldest_offset = std::min(INITIAL_OLDEST_OFFSET, pcache->filelist.file_num - 1);
  // 清理nodelist
  memset(pcache->nodelist, 0, pcache->capacity * sizeof(hc_indexnode));
  for (u_int j=0; j < pcache->capacity; ++j) {
    pcache->nodelist[j].before = HC_NULL;
    pcache->nodelist[j].next = HC_NULL;
    pcache->nodelist[j].left = HC_NULL;
    pcache->nodelist[j].right = HC_NULL;
  }
  // 清理hashtable
  memset(pcache->hashtable.table, 0xff,
      pcache->hashtable.hashnum*sizeof(u_int));
  // 清理hc_cache
  pcache->rs_head = HC_NULL;
  pcache->in_curpos = 0;
  pcache->total_count = 0;
  return RT_NOTICE_PROCESSED;
}

#define APPEND_FIELD(buf, pos, field) do {\
    memcpy(buf + pos, &field, sizeof(field)); \
    pos += sizeof(field); \
} while (0)

int hc_dump_cache(hc_cache* pcache, char *file_name) {
    FILE *fp = NULL;
    time_t now = time(NULL);
    unsigned int sign[2];
    char head_buf[sizeof(hc_cache)];
    int head_buf_len = 0;
    if (pcache == NULL || file_name == NULL) {
        return RT_NOTICE_NONE_PROCESSED;
    }
    if ((fp = fopen(file_name, "w")) == NULL) {
        return RT_NOTICE_NONE_PROCESSED;
    }
    // write timestamp
    CHECK(fwrite(&now, sizeof(now), 1, fp) >= 1);
    // fill buffer with necessary fields in cache head
    APPEND_FIELD(head_buf, head_buf_len, pcache->file_name);
    APPEND_FIELD(head_buf, head_buf_len, pcache->capacity);
    APPEND_FIELD(head_buf, head_buf_len, pcache->hashtable.hashnum);
    APPEND_FIELD(head_buf, head_buf_len, pcache->filelist.file_size);
    APPEND_FIELD(head_buf, head_buf_len, pcache->filelist.file_num);
    APPEND_FIELD(head_buf, head_buf_len, pcache->filelist.oldest_offset);
    APPEND_FIELD(head_buf, head_buf_len, pcache->filelist.newest_offset);
    APPEND_FIELD(head_buf, head_buf_len, pcache->rs_head);
    APPEND_FIELD(head_buf, head_buf_len, pcache->in_curpos);
    APPEND_FIELD(head_buf, head_buf_len, pcache->total_count);
    APPEND_FIELD(head_buf, head_buf_len, pcache->use_mode);
    APPEND_FIELD(head_buf, head_buf_len, pcache->valid_time);
    // write cache head with length and signature
    CHECK(fwrite(&head_buf_len, sizeof(int), 1, fp) >= 1);
    CHECK(fwrite(head_buf, head_buf_len, 1, fp) >= 1);
    creat_sign_mds64(head_buf, head_buf_len, &sign[0], &sign[1]);
    CHECK(fwrite(sign, sizeof(sign), 1, fp) >= 1);
    // write hash table with signature
    CHECK(fwrite(pcache->hashtable.table, sizeof(u_int), pcache->hashtable.hashnum, fp)
        >= pcache->hashtable.hashnum);
    creat_sign_mds64((char*)pcache->hashtable.table, sizeof(u_int) * pcache->hashtable.hashnum, &sign[0], &sign[1]);
    CHECK(fwrite(sign, sizeof(sign), 1, fp) >= 1);
    // write nodelist with signature
    CHECK(fwrite(pcache->nodelist, sizeof(hc_indexnode), pcache->capacity, fp) >= pcache->capacity);
    creat_sign_mds64((char*)pcache->nodelist, sizeof(hc_indexnode) * pcache->capacity, &sign[0], &sign[1]);
    CHECK(fwrite(sign, sizeof(sign), 1, fp) >= 1);
    // write filelist with signature
    CHECK(fwrite(pcache->filelist.files, sizeof(hc_file), pcache->filelist.file_num, fp)
        >= (u_int)pcache->filelist.file_num);
    creat_sign_mds64((char*)pcache->filelist.files, sizeof(hc_file) * pcache->filelist.file_num, &sign[0], &sign[1]);
    CHECK(fwrite(sign, sizeof(sign), 1, fp) >= 1);
    fclose(fp);
    return RT_NOTICE_PROCESSED;
}

#define EXTRACT_FIELD(buf, pos, field) do {\
    memcpy(&field, buf + pos, sizeof(field)); \
    pos += sizeof(field); \
} while (0)

hc_cache* hc_load_cache(char *file_name, time_t max_interval) {
  hc_cache *cache = NULL;
  time_t now = time(NULL);
  time_t timestamp = 0;
  FILE *fp = NULL;
  int i = 0;
  char pathname[PATH_SIZE] = "\0";
  unsigned int sign[2];
  unsigned int sign_saved[2];
  char head_buf[sizeof(hc_cache)];
  int head_buf_len = 0;
  if (file_name == NULL) return NULL;
  if ((fp = fopen(file_name, "r")) == NULL) return NULL;
  // read timestamp and check whether expired or not
  if (fread(&timestamp, sizeof(timestamp), 1, fp) < 1) {
    fclose(fp);
    return NULL;
  }
  if (now - timestamp > max_interval) {
    LOG(ERROR) << "hc_cache expired " << file_name;
    fclose(fp);
    return NULL;
  }
  // read cache head and verify signature
  CHECK((cache = (hc_cache*)malloc(1 * sizeof(hc_cache))) != NULL);
  memset(cache, 0, sizeof(*cache));
  CHECK(fread(&head_buf_len, sizeof(int), 1, fp) >= 1);
  CHECK(fread(head_buf, head_buf_len, 1, fp) >= 1);
  CHECK(fread(sign_saved, sizeof(sign_saved), 1, fp) >= 1);
  creat_sign_mds64(head_buf, head_buf_len, &sign[0], &sign[1]);
  CHECK(sign[0] != sign_saved[0] || sign[1] != sign_saved[1]);
  // extract fields from buffer to cache head
  head_buf_len = 0;
  EXTRACT_FIELD(head_buf, head_buf_len, cache->file_name);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->capacity);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->hashtable.hashnum);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->filelist.file_size);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->filelist.file_num);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->filelist.oldest_offset);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->filelist.newest_offset);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->rs_head);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->in_curpos);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->total_count);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->use_mode);
  EXTRACT_FIELD(head_buf, head_buf_len, cache->valid_time);
  cache->hashtable.table = NULL;
  cache->nodelist = NULL;
  cache->filelist.files = NULL;
  // read hashtable and verify signature
  CHECK((cache->hashtable.table = (u_int*)malloc(cache->hashtable.hashnum * sizeof(u_int))) != NULL);
  CHECK(fread(cache->hashtable.table, sizeof(u_int), cache->hashtable.hashnum, fp) >= cache->hashtable.hashnum);
  CHECK(fread(sign_saved, sizeof(sign_saved), 1, fp) >= 1);
  creat_sign_mds64((char*)cache->hashtable.table, sizeof(u_int) * cache->hashtable.hashnum, &sign[0], &sign[1]);
  CHECK(sign[0] != sign_saved[0] || sign[1] != sign_saved[1]);
  // read nodelist and verify signature
  CHECK((cache->nodelist = (hc_indexnode*)malloc(cache->capacity * sizeof(hc_indexnode))) != NULL);
  CHECK(fread(cache->nodelist, sizeof(hc_indexnode), cache->capacity, fp) >= cache->capacity);
  CHECK(fread(sign_saved, sizeof(sign_saved), 1, fp) >= 1);
  creat_sign_mds64((char*)cache->nodelist, sizeof(hc_indexnode) * cache->capacity, &sign[0], &sign[1]);
  CHECK(sign[0] != sign_saved[0] || sign[1] != sign_saved[1]);
  // read filelist and verify signature
  CHECK((cache->filelist.files = (hc_file*)malloc(cache->filelist.file_num * sizeof(hc_file))) != NULL);
  CHECK(fread(cache->filelist.files, sizeof(hc_file), cache->filelist.file_num, fp) >= (u_int)cache->filelist.file_num);
  CHECK(fread(sign_saved, sizeof(sign_saved), 1, fp) >= 1);
  creat_sign_mds64((char*)cache->filelist.files, sizeof(hc_file) * cache->filelist.file_num, &sign[0], &sign[1]);
  CHECK(sign[0] != sign_saved[0] || sign[1] != sign_saved[1]);
  for (i = 0; i < cache->filelist.file_num; i++) {
    cache->filelist.files[i].handle = -1;
  }
  // reopen file descriptors
  for (i = 0; i < cache->filelist.file_num; i++) {
    sprintf(pathname, "%s.%d", cache->file_name, i);  // 生成文件名
    if (cache->filelist.files[i].status == USING_IN_READ) {
      cache->filelist.files[i].handle = open(pathname, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
    } else if (cache->filelist.files[i].status == USING_IN_WRITE) {
      cache->filelist.files[i].handle = open(pathname, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    } else {
      continue;
    } if (cache->filelist.files[i].handle < 0) {
      LOG(ERROR) << "open file " << pathname << " failure!";
      CHECK(false);
    }
    cache->filelist.files[i].thread_in_read = 0;
    cache->filelist.files[i].buf = NULL;
  }
  pthread_mutex_init(&cache->hc_mutex_count, NULL);  // init_mutex
  fclose(fp);
  fp = NULL;
  // unlink dumped file to avoid being loaded again in future
  if (unlink(file_name) == -1) {
    LOG(ERROR) << "unlink cache index file" << pathname << " failed: "
               << strerror(errno);
  }
  return cache;
}

int hc_cache_clean_expired_files(hc_cache * cache, int * expired_file_offset) {
  int ret;
  time_t now = time(NULL);
  hc_file * fin = NULL;
  if (cache == NULL || expired_file_offset == NULL) {
    return RT_NOTICE_NONE_PROCESSED;
  }
  fin = &cache->filelist.files[cache->filelist.oldest_offset];
  if (cache->filelist.oldest_offset != cache->filelist.newest_offset
      && now - fin->est_time > cache->valid_time) {
    fin->status = NEED_DELETE;
    *expired_file_offset = cache->filelist.oldest_offset;
    if ((ret = hc_removefile(cache)) == RT_NOTICE_PROCESSED) {
      // NOTE: A little bit tricky here. Temporarily mark the file as NEED_PHYSICAL_DELETE.
      // The file will be marked as NOT_USE later after the file is physically deleted.
      cache->filelist.files[*expired_file_offset].status = NEED_PHYSICAL_DELETE;
    }
  } else ret = RT_NOTICE_NONE_PROCESSED;
  return ret;
}

