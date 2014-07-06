#ifndef HC_CACHE_H_
#define HC_CACHE_H_

#include "cache_base.h"
#include <stdio.h>
#include <fcntl.h>
#include "base/public/basictypes.h"

#define BYTES_PER_MB 1048576 //  1024 * 1024
#define BYTES_PER_KB 1024
#define MAX_FILE_NUM 20000
#define HAVE_NOASSISTANT 0x200
#define HAVE_ASSISTANT 0x100
#define NOT_USE 0x0
#define USING_IN_WRITE 0x1
#define USING_IN_READ 0x2
#define NEED_DELETE 0x3
#define NEED_PHYSICAL_DELETE 0x4
#define HC_NULL 0xFFFFFFFF
#define MAX_FILE_SIZE 2000
#define PATH_SIZE 1000

struct hc_hashtable {
	u_int hashnum;
	u_int *table;
};
// 节点结构
struct hc_indexnode {
	// 64位签名
	u_int sign1;
	u_int sign2;
	// 位置信息
	u_int file_offset; // 文件位置
	u_int pos_offset; // 文件中的位置
	u_int length; // 长度
	// hash chain
	u_int before;
	u_int next;
	// file chain
	u_int left;
	u_int right;
	// 命中的次数
	u_int click_time;
};
// hc_cache中的文件结构
struct hc_file {
	int handle;			// 句柄
	int status;			// 状态
	u_int entry;		// 入口
	u_int tail; 		// 在文件中的最后一个节点
	u_int free_size;	// 空闲的文件大小
	int node_num_inlist;// node列表中的节点数
	int thread_in_read;	// 正在读该文件的线程数
	char *buf;			// mmap的指针
	time_t est_time;	// 建立时间
};
// file list结构
struct hc_filelist {
	u_int file_size;		// 每个文件的大小(Bytes)
	int file_num;			// 文件个数
	int oldest_offset;		// 最老的文件位置
	int newest_offset;		// 最新的文件位置
	hc_file * files;			// 文件数组指针
};
//  cache统计信息
struct hc_cache_stat_t {
	int add_count;
	int seek_count;
	int hit_count;
	int remove_count;
};
// hc_cache结构
struct hc_cache {   
	char file_name[PATH_SIZE];	// 使用的文件名
	u_int capacity;					// 节点个数
	hc_hashtable hashtable;			// hash表
	hc_indexnode *nodelist;			// 节点个数
	hc_filelist filelist;			// 文件列表
	u_int rs_head;					// 回收栈顶
	u_int in_curpos;				// 当前位置
	u_int total_count;				// 总数
	int use_mode;					// 使用模式
	time_t valid_time;				// 有效时间
	hc_cache_stat_t cache_stat;
	pthread_mutex_t hc_mutex_count;	// 互斥锁
};

/*
 *  @brief [create and initialize a hc_cache structure]
 *  @param capacity [number of nodes of the hc_cache]
 *  @param file_size [the size of each file in the hc_cache(unit:MB)]
 *  @param file_num [the number of file the hc_cache uses]
 *  @param use_mode [use mode of the hc_cache]
 *  @param char *file_name [file_name including path of the files]
 *  @param valid_time [timeout for cache files(unit_size:s)]
 *  @return pointer of the hc_cache: NULL for error; others for success
 */ 
hc_cache *hc_create_cache(u_int capacity, int file_size, int file_num,
		int use_mode, char *file_name, time_t valid_time);

/*			  
 *  @brief [destroy and free a hc_cache structure]
 *  @param pcache [pointer of the hc_cache]
 *  @return: this function always returns RT_NOTICE_PROCESSED for success
 */ 
int hc_free_cache(hc_cache *pcache);

/*
 * @brief [remove a item by sign from hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param click_time [output the click times of the removed item if it's not NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_removeitem(hc_cache *pcache, u_int *sign, int *click_time);

/*
 * @brief [add a item to hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_additem(hc_cache *pcache, u_int *sign, void *pdata, u_int size);

/*
 * @brief [read a item from hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @param fact_size [output the fact size of the seeked item if fact_size != NULL]
 * @param click_time [output the number of the item seekd if click_time != NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_seekitem(hc_cache *pcache, u_int *sign, 
		void *pdata, u_int size, u_int *fact_size, int *click_time);

/*
 * @brief [seek item from hc_cache and only return info]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param size [the size of item, unitsize: bit]
 * @param fact_size [output the fact size of the seeked item]
 * @param file_offset [output the offset of the files that the item saved]
 * @param pos_offset [output the offset of the item in the file]
 * @param click_time [output the number of the item seekd if click_time != NULL]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_seekitem_r(hc_cache *pcache, u_int *sign, u_int size, 
		u_int *fact_size, u_int *handle, u_int *offset, int *click_time);

/*
 *  @brief [reset click time of the node assigned by sign]
 *  @param pcache [pointer of the hc_cache]
 *  @param sign [pointer of the 64bit sign of the item]
 *  return RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_reset_click_time(hc_cache *pcache, u_int *sign);

/*
 * @brief [modify a item that in hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @param sign [pointer of the 64bit sign of the item]
 * @param pdata [pointer of the data of the item]
 * @param size [the size of item, unitsize: bit]
 * @return: RT_NOTICE_PROCESSED for success, others for failure
 */
int hc_modifyitem(hc_cache *pcache, u_int *sign, void *pdata, u_int size);

/*
 * @brief [clear the hc_cache]
 * @param pcache [pointer of the hc_cache]
 * @return: this function always returns RT_NOTICE_PROCESSED for success
 */
int hc_clean_cache(hc_cache *pcache);

/**
 * @brief dump cache to file
 * @param[in] pcache    pointer to cache
 * @param[in] file_name  file name
 * @return   
 *  @li @c RT_NOTICE_PROCESSED        on success
 *  @li @c RT_NOTICE_NONE_PROCESSED   on failure
 */
int hc_dump_cache(hc_cache* pcache, char *file_name);

/**
 * load cache from file
 * @param file_name     file name
 * @param max_interval   cache expire time
 * @return   
 *  @li a pointer to the cache  on success
 *  @li @c NULL                 on failure
 */
hc_cache* hc_load_cache(char *file_name, time_t max_interval);

/**
 * remove expired files. should be called periodically.
 * @param hc_cache      pointer to cache
 * @param expired_file_name_buf[out] buffer holding the name of expired file
 * @param buf_len		length of buffer
 * @return
 *  @li RT_NOTICE_PROCESSED if any file removed
 *  @li RT_NOTICE_NONE_PROCESSED if no file removed
 * @note this function DO NOT physically unlink the file from file system. 
 *	The name of expired file to be deleted is returned instead so that the caller 
 *	can unlink the file without holding the mutex.
 */
int hc_cache_clean_expired_files(hc_cache *cache, int *expired_file_offset);
#endif
