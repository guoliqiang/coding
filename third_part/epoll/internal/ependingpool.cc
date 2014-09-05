#include "../public/ependingpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include "base/public/basictypes.h"
#include "base/public/logging.h"
#include "base/public/net.h"

DEFINE_int32(DEF_CONN_TIMEO, 500, "");
DEFINE_int32(DEF_READ_TIMEO, 500, "");
DEFINE_int32(DEF_WRITE_TIMEO, 500, "");
DEFINE_int32(DEF_EPOLL_TIMEO, 500, "");
DEFINE_int32(DEF_MAX_SOCK, 500, "");
DEFINE_int32(DEF_QUEUE_LEN, 500, "");


ependingpool::ependingpool() {
  m_sock_len = 0;
  m_free_thr = 0;
  m_listen_fd = -1;

  m_conn_timeo = FLAGS_DEF_CONN_TIMEO;
  m_read_timeo = FLAGS_DEF_READ_TIMEO;
  m_write_timeo = FLAGS_DEF_WRITE_TIMEO;
  m_min_timeo = m_read_timeo;

  m_ep_timeo = FLAGS_DEF_EPOLL_TIMEO;
  m_queue_len = FLAGS_DEF_QUEUE_LEN;
  m_sock_num = FLAGS_DEF_MAX_SOCK;

  m_insert_item_sock_thrsafe = 0;
  m_least_last_update = INT_MAX;

  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_condition, NULL);
  pthread_mutex_init(&m_insert_sock, NULL);

  queue_create(m_queue_len);
  m_ay_sock = (sock_item_t *)calloc((size_t)m_sock_num, sizeof(sock_item_t));
  CHECK(NULL != m_ay_sock) << "calloc m_ay_sock fail";

  m_ay_events = (struct epoll_event *)calloc((size_t)m_sock_num,
                 sizeof(struct epoll_event));
  CHECK(NULL != m_ay_events) << "calloc m_ay_events fail";

  m_epfd = epoll_create(m_sock_num);
  for (int i = 0; i < SOCK_EVENT_NUM; ++i) m_callbacklist[i] = NULL;
  m_todo_event_callback = NULL;
  pool_run = 1;
}

ependingpool::~ependingpool() {
  if (m_epfd > 0) close(m_epfd);
  if (m_ay_sock) {
    for (int i = 0; i < m_sock_len; ++i) {
      if (NOT_USED != m_ay_sock[i].sock_status) {
        reset_item(i, false);
      }
    }
    free(m_ay_sock);
  }
  if (m_ay_events) free(m_ay_events);
  queue_destroy();
}
// @brief 设置监听句柄
// @param [in] lis_fd   : 需要监听的listen 句柄
// @return  设置成功与否
// @retval   -1 设置失败 0 设置成功
int ependingpool::set_listen_fd(int lis_fd) {
  if (lis_fd <= 0) {
    LOG(INFO) << "invalid listen fd:" << lis_fd;
    return -1;
  }
  m_listen_fd = lis_fd;
  return pool_epoll_add(lis_fd, -1, (EPOLLIN | EPOLLHUP | EPOLLERR));
}

int ependingpool::set_queue_len(int len) {
  if (m_ay_ready) {
    free(m_ay_ready);
    m_ay_ready = NULL;
  }
  if (len > 2) m_queue_len = len;
  else m_queue_len = 2;
  return queue_create(m_queue_len);
}

int ependingpool::set_event_callback(int event, event_callback_t* callback) {
  if (NULL == m_callbacklist) {
    LOG(FATAL) << "m_callbacklist is NULL";
    return -1;
  }
  if (event < 0 || event >= SOCK_EVENT_NUM) {
    LOG(WARNING) << "event must between 0 ~ " << SOCK_EVENT_NUM;
    return -1;
  }
  if (m_callbacklist[event] != NULL) {
    LOG(WARNING) << "event " << event <<  " is no NULL";
  }
  m_callbacklist[event] = callback;
  return 0;
}

int ependingpool::set_todo_event_ex(event_callback_ex_t* callback, void *user_arg) {
  m_todo_event_callback = callback;
  m_todo_event_user_arg = user_arg;
  return 0;
}

int ependingpool::get_queue_len() {
  int len = m_put - m_get;
  if (len < 0) len += m_queue_len;
  return len;
}

int ependingpool::set_sock_num(int num) {
  if (num <= 0) {
    LOG(WARNING) << "invalid socket number " << num
                 << " sock_num keep unchanged " << m_sock_num;
    return -1;
  }
  if (m_ay_sock) {
    free(m_ay_sock);
    m_ay_sock = NULL;
  }
  m_sock_num = num;
  m_ay_sock = (sock_item_t *)calloc((size_t)m_sock_num, sizeof(sock_item_t));
  CHECK (NULL != m_ay_sock) << "m_ay_sock calloc memory failed " << errno;

  if (m_epfd > 0) {
    while (close(m_epfd) < 0 && errno == EINTR) {};
    m_epfd = -1;
  }

  if (m_ay_events) {
    free(m_ay_events);
    m_ay_events = NULL;
  }

  m_ay_events = (struct epoll_event *)calloc((size_t)m_sock_num, sizeof(struct epoll_event));
  CHECK(NULL != m_ay_events) << "m_ay_events calloc memory failed " << errno;

  m_epfd = epoll_create(m_sock_num);
  if (m_epfd < 0) {
    LOG(WARNING) << "create epoll descriptor failed " << errno;
    return -1;
  }
  return 0;
}

int ependingpool::set_conn_timeo(int timeo) {
  if (timeo <= 0) {
    LOG(WARNING) << "invalid timeout" << timeo << " timeout. timeout keep unchanged "
                 << m_conn_timeo;
    return -1;
  }
  m_conn_timeo = timeo;
  if (m_conn_timeo < m_read_timeo && m_conn_timeo < m_write_timeo) {
    m_min_timeo = m_conn_timeo;
  }
  return 0;
}

int ependingpool::set_read_timeo(int timeo) {
  if (timeo <= 0) {
    LOG(WARNING) << "invalid timeout" << timeo << " timeout. timeout keep unchanged "
                 << m_conn_timeo;
    return -1;
  }

  m_read_timeo = timeo;
  if (m_read_timeo < m_conn_timeo && m_read_timeo < m_write_timeo) {
    m_min_timeo = m_read_timeo;
  }
  return 0;
}

int ependingpool::set_write_timeo(int timeo) {
  if (timeo <= 0) {
    LOG(WARNING) << "invalid timeout" << timeo << " timeout. timeout keep unchanged "
                 << m_conn_timeo;
    return -1;
  }
  m_write_timeo = timeo;
  if (m_write_timeo < m_read_timeo && m_write_timeo < m_conn_timeo) {
    m_min_timeo = m_write_timeo;
  }
  return 0;
}

int ependingpool::get_conn_timeo() {
  return m_conn_timeo;
}

int ependingpool::get_read_timeo() {
  return m_read_timeo;
}

int ependingpool::get_write_timeo() {
  return m_write_timeo;
}

int ependingpool::set_epoll_timeo(int timeo) {
  if (timeo < 0) {
    LOG(WARNING) << "invalid timeout" << timeo << " timeout. timeout keep unchanged "
                 << m_conn_timeo;
    return -1;
  }
  m_ep_timeo = timeo;
  return 0;
}

int ependingpool::get_epoll_timeo() {
  return m_ep_timeo;
}

int ependingpool::get_free_thread() {
  return m_free_thr;
}

int ependingpool::get_offset() {
  int ret;
  if (NULL == m_ay_sock) return -1;
  for (int i = 0; i < m_sock_len; ++i) {
    if (NOT_USED == m_ay_sock[i].sock_status) {
      return i;
    }
  }
  if (m_sock_len >= m_sock_num) return -1;
  ret = m_sock_len++;
  return ret;
}

int ependingpool::insert_item(int sock) {
  if (sock < 0) {
    LOG(FATAL) << "insert socket fail, invalid socket " << sock;
    return -1;
  }
  int current_offset = 0;
  int safe = m_insert_item_sock_thrsafe;
  if (safe) pthread_mutex_lock(&m_insert_sock);
  // 获取sock池中空闲位置
  current_offset = get_offset();
  // 获取失败
  if (-1 == current_offset) {
    LOG(WARNING) << "insert socket" << sock << " fail, socket array[max_size:"
                 << m_sock_num << "] overflow.";
    if (safe) pthread_mutex_unlock(&m_insert_sock);
    return -1;
  }
  // 设置sock状态
  m_ay_sock[current_offset].last_active = time(NULL);
  m_ay_sock[current_offset].sock = sock;
  m_ay_sock[current_offset].sock_status = READY;
  LOG(INFO) << "socket " << sock << " inserted into sock array[offset:"
            << current_offset << "] [sock_len:" << m_sock_len << "].";
  if (safe) pthread_mutex_unlock(&m_insert_sock);
  return current_offset;
}

int ependingpool::restart_run() {
  pool_run = 1;
  return 0;
}

int ependingpool::stop() {
  if (1 == pool_run) {
    pool_run = 0;
  }
  pthread_cond_broadcast(&m_condition);
  return 0;
}

int ependingpool::is_run() {
  return pool_run;
}

// @brief 从已就绪队列中获取已经就绪的sock
// @param [in/out] offset   : int*
// @param [in/out] sock   : int * 实际使用的句柄
// @return  int
int ependingpool::fetch_item(int *handle, int *sock) {
  int fetch_handle = -1;
  int fetch_sock;
  int ret = -1;
  if (NULL == m_ay_sock || NULL == handle || NULL == sock) {
    return -1;
  }
  pthread_mutex_lock(&m_mutex);
  ++m_free_thr;
  // 队列为空的时候
  while (queue_empty() && pool_run) {
    struct timeval now;
    struct timespec timeout;
    gettimeofday(&now, NULL);
    timeout.tv_sec = now.tv_sec + 5;
    timeout.tv_nsec = now.tv_usec * 1000;
    pthread_cond_timedwait(&m_condition, &m_mutex, &timeout);
  }
  --m_free_thr;
  if (!pool_run) {
    LOG(INFO) << "pool is stop";
    goto failure;
  }
  // 获取句柄
  if (queue_pop(&fetch_handle) < 0) {
    LOG(WARNING) << "get invalid handle [" << fetch_handle << "] from queue.";
    goto failure;
  }
  fetch_sock = m_ay_sock[fetch_handle].sock;
  if (fetch_sock < 0) {
    LOG(WARNING) << "get invalid socket " << fetch_sock << " from queue.";
    goto failure;
  }
  if (m_ay_sock[fetch_handle].sock_status != BUSY) {
    LOG(WARNING) << "get error handle " << fetch_handle
                 << "socket" << fetch_sock << " from queue, status is "
                 << m_ay_sock[fetch_handle].sock_status;
    goto failure;
  }
  if (m_free_thr > 0 && !queue_empty()) {
    pthread_cond_signal(&m_condition);
  }
  *handle = fetch_handle;
  *sock = fetch_sock;
  if (0 == pool_run) goto failure;

  pthread_mutex_unlock(&m_mutex);
  LOG(INFO) << "handle:" << fetch_handle << " socket : "
            << fetch_sock << " geted, head="
            << m_get << ", tail=" << m_put << "free_thr:"
            << m_free_thr;
  ret = 0;
  if (m_callbacklist[SOCK_FETCH] != NULL) {
    ret = m_callbacklist[SOCK_FETCH](fetch_sock, (void**)&(m_ay_sock[fetch_handle].arg));
    switch (ret) {
      case 0:
        break;
      case 1:
        reset_item(fetch_handle, false);
        break;
      case 2:
        reset_item(fetch_handle, true);
        break;
      case 3:
        clear_item(fetch_handle);
        break;
      default:
        LOG(WARNING) << "SOCK_FETCH callback return unknown ret:" << ret;
        reset_item(fetch_handle, false);
        break;
    }
  }
  return ret;

failure:
  pthread_mutex_unlock(&m_mutex);
  *handle = -1;
  *sock = -1;
  return ret;
}

int ependingpool::fetch_item(int *handle, int *sock, long long *staytime) {
  int ret = 0;
  if (NULL == staytime) {
    return -1;
  }
  ret = fetch_item(handle, sock);
  if (ret != -1 && *handle >= 0) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    *staytime = (long long)(tv.tv_sec - m_ay_sock[*handle].queue_time.tv_sec) * 1000000LL
                + (tv.tv_usec-m_ay_sock[*handle].queue_time.tv_usec);
  }
  return ret;
}

int ependingpool::close_ready_socket() {
  LOG(INFO) << "close ready socket m_sock_len:" << m_sock_len;
  int ready = 0;
  for (int i = 0; i < m_sock_len; ++i) {
    if (READY == m_ay_sock[i].sock_status) {
      ++ready;
      reset_item(i, false);
    }
  }
  return ready;
}

int ependingpool::waitforstop(int timeout) {
  struct timeval tv_old;
  struct timeval tv;
  LOG(INFO) << "queue_len is :" << get_queue_len();
  stop();
  int handle = -1;
  pthread_mutex_lock(&m_mutex);
  while (!queue_empty()) {
    queue_pop(&handle);
    reset_item(handle, false);
  }
  pthread_mutex_unlock(&m_mutex);
  LOG(INFO) << "queue all close, queue is empty";
  gettimeofday(&tv_old, NULL);
  while (1) {
    gettimeofday(&tv, NULL);
    if ((tv.tv_sec - tv_old.tv_sec) * 1000L +
        (tv.tv_usec - tv_old.tv_usec) / 1000L > timeout) {
      break;
    }
    check_item();
    if (0 == m_read_socket_num && 0 == m_write_socket_num) {
      break;
    }
  }
  int close_sock = close_ready_socket();
  LOG(INFO) << "close socket num is " << close_sock
            << ", queue_len is " << get_queue_len();
  return 0;
}

int ependingpool::clear_item(int handle) {
  int sock = -1;
  if (handle < 0 || handle >= m_sock_len) {
    LOG(WARNING) << "invalid handle:" << handle;
    return -1;
  }
  if (NULL == m_ay_sock) {
    LOG(WARNING) << "reset_item invalid m_ay_sock.";
    return -1;
  }
  if (NOT_USED == m_ay_sock[handle].sock_status) {
    LOG(WARNING) << "no found socket in handle:" << handle;
    return 0;
  }
  sock = m_ay_sock[handle].sock;
  if (sock < 0) {
    LOG(WARNING) << "invalid handle:" << handle << " sock :" << sock;
    return -1;
  }
  // 不判断epoll del成功与否,这种错误没有影响
  pool_epoll_offset_del(handle);
  if (m_callbacklist[SOCK_CLEAR] != NULL) {
    m_callbacklist[SOCK_CLEAR](m_ay_sock[handle].sock, (void**)(&m_ay_sock[handle].arg));
    m_ay_sock[handle].arg = NULL;
  }
  m_ay_sock[handle].sock = -1;
  m_ay_sock[handle].sock_status = NOT_USED;
  return 0;
}

int ependingpool::reset_item(int handle, bool keep_alive) {
  int ret = 0;
  int sock = -1;
  if (handle < 0 || handle >= m_sock_len) {
    LOG(WARNING) << "invalid handle :" << handle;
    return -1;
  }
  if (NULL == m_ay_sock) {
    LOG(FATAL) << "reset_item invalid m_ay_sock.";
    return -1;
  }
  if (NOT_USED == m_ay_sock[handle].sock_status) {
    LOG(WARNING) << "no found socket in handle:" << handle;
    return 0;
  }
  sock = m_ay_sock[handle].sock;
  if (sock < 0) {
    LOG(WARNING) << "invalid handle:" << handle << " sock :" << sock;
    return -1;
  }
  if (!(keep_alive && pool_run)) { // 不保持连接
    // 关闭句柄
    LOG(INFO) << "close sock:" << sock;
    if (m_callbacklist[SOCK_CLEAR] != NULL) {
      m_callbacklist[SOCK_CLEAR](m_ay_sock[handle].sock, (void**)(&m_ay_sock[handle].arg));
      m_ay_sock[handle].arg = NULL;
    }
    while ((ret = close(sock)) < 0 && errno == EINTR) { };
    if (ret < 0) {
      LOG(WARNING) << "close socket handle:" << handle << " failed, "
                   << "sock:" << sock << " errno:" << errno;
      return -1;
    }
    m_ay_sock[handle].sock = -1;
    m_ay_sock[handle].sock_status = NOT_USED;
  } else { // keep alive
    m_ay_sock[handle].last_active = time(NULL);
    m_ay_sock[handle].sock_status = READY;
    pool_epoll_offset_mod(handle, EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLONESHOT);
  }
  return 0;
}

int ependingpool::fetch_handle_arg(int handle, void **arg) {
  if (handle < 0 || handle >= m_sock_len) {
    LOG(WARNING) << "invalid handle :" << handle;
    return -1;
  }
  if (NULL == m_ay_sock) {
    LOG(FATAL) << "reset_item invalid m_ay_sock.";
    return -1;
  }
  *arg = m_ay_sock[handle].arg;
  return 0;
}

int ependingpool::write_reset_item(int handle) {
  if (handle < 0 || handle >= m_sock_len) {
    LOG(WARNING) << "invalid handle : " << handle;
    return -1;
  }
  if (NULL == m_ay_sock) {
    LOG(FATAL) << "reset_item invalid m_ay_sock.";
    return -1;
  }
  if (NOT_USED == m_ay_sock[handle].sock_status) {
    LOG(WARNING) << "no found socket in handle :" << handle;
    return 0;
  }
  LOG(INFO) << "write rest handle:" << handle;
  m_ay_sock[handle].last_active = time(NULL);
  m_ay_sock[handle].sock_status = WRITE_BUSY;
  pool_epoll_offset_mod(handle, EPOLLOUT | EPOLLHUP | EPOLLERR);
  return 0;
}

int ependingpool::set_insert_item_sock_thrsafe(int safe) {
  m_insert_item_sock_thrsafe = safe;
  return 0;
}

int ependingpool::insert_item_sock(int sock, void *arg, int flags) {
  int offset = -1;
  int events = 0;
  if (0 == pool_run) {
    return -1;
  }
  offset = insert_item(sock);
  if (offset < 0) {
    if (m_callbacklist[SOCK_INSERTFAIL] != NULL) {
      m_callbacklist[SOCK_INSERTFAIL](sock, NULL);
    }
    return -1;
  }
  if (arg != NULL) {
    m_ay_sock[offset].arg = arg;
  } else {
    if (m_callbacklist[SOCK_INIT] != NULL) {
      if (m_callbacklist[SOCK_INIT](sock, (void**)(&(m_ay_sock[offset].arg))) < 0) {
        LOG(WARNING) << "SOCK_INIT callback return < 0";
        clear_item(offset);
        return -1;
      }
    }
  }
  if (0 == flags) {
    events = EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR;
  } else {
    events = EPOLLOUT | EPOLLHUP | EPOLLERR;
  }
  if (pool_epoll_offset_add(offset, events) < 0) {
    if (arg != NULL) {
      m_ay_sock[offset].arg = NULL;
    }
    LOG(WARNING) << "epoll_del offset:" << offset << " error";
    clear_item(offset);
    return -1;
  }
  return 0;
}

void ependingpool::do_read_event(int offset) {
  int ret = 0;
  if (offset < 0 || offset >= m_sock_len) {
    LOG(WARNING) << "invalid offset:" << offset;
    return;
  }
  ++m_read_socket_num;
  if (m_callbacklist[SOCK_READ] != NULL) {
    LOG(INFO) << "run read callback..., arg :" << m_ay_sock[offset].arg;
    if (READ_BUSY != m_ay_sock[offset].sock_status) {
      m_ay_sock[offset].last_active = time(NULL);
      m_ay_sock[offset].sock_status = READ_BUSY;
    }
    ret = m_callbacklist[SOCK_READ](m_ay_sock[offset].sock,
                                    (void**)(&m_ay_sock[offset].arg));
  } else ret = 0;

  switch (ret) {
    case 0:
      ret = queue_in(offset);
      if (0 == ret) break;
      LOG(WARNING) << "queue_in offset:" << offset
                   << " error:" << ret << " sock:" << m_ay_sock[offset].sock;
      if (m_callbacklist[SOCK_QUEUEFAIL] != NULL) {
        ret = m_callbacklist[SOCK_QUEUEFAIL](m_ay_sock[offset].sock,
                                             (void**)(&m_ay_sock[offset].arg));
      }
      switch (ret) {
        case 0:
          reset_item(offset, false);
          break;
        case 1:
          clear_item(offset);
          break;
        default:
          LOG(WARNING) << "SOCK_QUEUEFAIL callback return ret:" << ret
                       << ", sock:" << m_ay_sock[offset].sock;
          reset_item(offset, false);
          break;
      }
      break;
    case 1:
      LOG(INFO) << "read no end ret:" << ret << ", sock reset to EPOLLIN";
      pool_epoll_offset_mod(offset, EPOLLIN | EPOLLHUP | EPOLLERR |
                            EPOLLONESHOT);
      break;
    case 2:
      if (m_callbacklist[SOCK_TODO] != NULL) {
        LOG(INFO) << "read call success, not put into epoll, call SOCK_TODO";
        m_ay_sock[offset].sock_status = BUSY;
        ret = m_callbacklist[SOCK_TODO](m_ay_sock[offset].sock,
            (void**)(&m_ay_sock[offset].arg));
      } else {
        ret = 0;
      }
      if (0 == ret) {
        if (NULL != m_todo_event_callback) {
          ependingpool_task_t todo_arg;
          m_ay_sock[offset].sock_status = BUSY;
          todo_arg.sock = m_ay_sock[offset].sock;
          todo_arg.offset = offset;
          todo_arg.arg = m_ay_sock[offset].arg;
          todo_arg.user_arg = m_todo_event_user_arg;
          ret = m_todo_event_callback(&todo_arg);
        }
      }

      switch (ret) {
        case 0:
          write_reset_item(offset);
          break;
        case 1:
          reset_item(offset, true);
          break;
        case 2:
          clear_item(offset);
          break;
        case 3:
          break;
        default:
          LOG(WARNING) << "call back SOCK_TODO run result is err:" << ret;
          reset_item(offset, false);
          break;
      }
      break;
    case 3:
      LOG(INFO) << "read end call clear item";
      clear_item(offset);
      break;
    default:
      LOG(WARNING) << "read call error :" << ret;
      reset_item(offset, false);
      break;
  }
}

void ependingpool::do_write_event(int offset) {
  int ret = 0;
  if (offset < 0 || offset >= m_sock_len) {
    LOG(WARNING) << "invalid offset :" << offset;
    return;
  }
  ++m_write_socket_num;
  if (m_callbacklist[SOCK_WRITE] != NULL) {
    LOG(INFO) << "run write callback..., arg :" << m_ay_sock[offset].arg;
    if (WRITE_BUSY != m_ay_sock[offset].sock_status) {
      m_ay_sock[offset].last_active = time(NULL);
      m_ay_sock[offset].sock_status = WRITE_BUSY;
    }
    ret = m_callbacklist[SOCK_WRITE](m_ay_sock[offset].sock,
        (void**)(&m_ay_sock[offset].arg));
  } else ret = 0;
  switch (ret) {
    case 0:
      LOG(INFO) << "write end sock, close sock:" << m_ay_sock[offset].sock;
      reset_item(offset, true);
      break;
    case 1:
      LOG(INFO) << "write no end ret, continue";
      break;
    case 2:
      LOG(INFO) << "write end sock, no close sock:" << m_ay_sock[offset].sock;
      reset_item(offset, false);
      break;
    case 3:
      LOG(INFO) << "write end sock, clear item";
      clear_item(offset);
      break;
    default:
      LOG(WARNING) << "write call error :" << ret;
      reset_item(offset, false);
      break;
  }
}

int ependingpool::check_item() {
  // 先检查超时事件
  check_time_out();
  // 等待epoll事件;
  int num = pool_epoll_wait(m_ep_timeo);
  if (num <= 0) { // 没有触发事件
    return num;
  }
  int offset = -1;
  m_read_socket_num = 0;
  m_write_socket_num = 0;
  for (int i = 0; i < num; i++) {
    // 如果是监听端口则accept出来
    if (m_ay_events[i].data.fd == -1 && m_listen_fd > 0) {
      int work_sock = accept_sock();
      if (work_sock < 0) {
        LOG(WARNING) << "accept error";
        continue;
      }
      if (insert_item_sock(work_sock) < 0) {
        while (close(work_sock) < 0 && errno == EINTR) {};
        LOG(WARNING) << "insert_item_sock work_sock:" << work_sock << " fail";
      }
    } else if (m_ay_events[i].data.fd >= 0) {
      offset = m_ay_events[i].data.fd;
      // 检查发生的事件
      LOG(INFO) << "events handle is :" << offset;
      if (m_ay_events[i].events & EPOLLHUP) {  // 断开
        LOG(WARNING) << "socket: " << m_ay_sock[offset].sock << " closed by peer.";
        reset_item(offset, false);
      } else if (m_ay_events[i].events & EPOLLERR) {  // 出错
        LOG(WARNING) << "socket :" << m_ay_sock[offset].sock << "error.";
        reset_item(offset, false);
      } else if ((m_ay_events[i].events & EPOLLIN)&&pool_run) {  // 可读
        do_read_event(offset);
      } else if (m_ay_events[i].events & EPOLLOUT) {
        do_write_event(offset);
      } else {
        LOG(WARNING) << "offset :" << offset << " is close";
        reset_item(offset, false);
      }
    }
  }
  return 0;
}

int ependingpool::accept_sock() {
  int work_sock = -1;
  // 用户定义的回调函数
  if (m_callbacklist[SOCK_ACCEPT] != NULL) {
    work_sock = m_callbacklist[SOCK_ACCEPT](m_listen_fd, NULL);
    if (work_sock < 0) {
      LOG(WARNING) << "accept sock callback fail";
      return -1;
    }
    return work_sock;
  }
  // accept connection
  work_sock = base::TcpAccept(m_listen_fd, NULL, NULL);
  if (work_sock < 0) {  // accept failed.
    LOG(WARNING) << "accept sock fail";
    return -1;
  }
  int client_port = 0;
  std::string client_ip;
  if (base::TcpClientIpPort(work_sock, &client_ip, &client_port)) {
    LOG(INFO) << "connect from client " << client_ip << ":"
              << client_port << " in socket :" << work_sock;
  }
  LOG(INFO) << "accept:" << work_sock << " success";
  return work_sock;
}

int ependingpool::queue_in(int offset) {
  if (!pool_run) {
    LOG(WARNING) << "pool is stop";
    return -1;
  }
  int sock = m_ay_sock[offset].sock;
  if (m_ay_sock[offset].sock_status != READY &&
      m_ay_sock[offset].sock_status != READ_BUSY) {
    LOG(WARNING) << "status of socket " << sock << "offset "
                 << offset << " is not ready!";
    return -1;
  }
  pthread_mutex_lock(&m_mutex);
  // sock放入就绪队列
  int ret = queue_push(offset);
  if (ret < 0) {  // 放入失败
    LOG(WARNING) << "queue overflow socket" << sock << ", queue len "
                 << m_queue_len;
    pthread_mutex_unlock(&m_mutex);
    return -1;
  } else {
    // 设置状态为 BUSY
    m_ay_sock[offset].sock_status = BUSY;
    // 设置最后活动时间
    gettimeofday(&(m_ay_sock[offset].queue_time), NULL);
    m_ay_sock[offset].last_active = m_ay_sock[offset].queue_time.tv_sec; 
    if (m_free_thr > 0) {
      pthread_cond_signal(&m_condition);
    }
  }
  pthread_mutex_unlock(&m_mutex);
  LOG(INFO) << "socket:" << sock << " offset:" << offset
            << "added into ready queue, head=" << m_get << ", tail="
            << m_put;
  return 0;
}

int ependingpool::queue_create(int queue_size) {
  m_get = 0;
  m_put = 0;
  m_ay_ready = (int *)calloc((size_t)queue_size, sizeof(int));
  if (NULL == m_ay_ready) {
    LOG(WARNING) << "calloc queue fail";
    return -1;
  }
  return 0;
}
int ependingpool::queue_push(int val) {
  if (NULL == m_ay_ready) return -1;
  if (queue_full()) return -1;
  m_ay_ready[m_put] = val;
  if (++m_put >= m_queue_len) m_put = 0;
  return 0;
}

int ependingpool::queue_pop(int *val) {
  if (NULL == m_ay_ready) return -1;
  if (queue_empty()) return -1;
  *val = m_ay_ready[m_get];
  if (++m_get >= m_queue_len) m_get = 0;
  return 0;
}

int ependingpool::queue_empty() {
  return (m_put == m_get);
}

int ependingpool::queue_full() {
  int pos = m_put + 1;
  if (pos >= m_queue_len) {
    pos -= m_queue_len;
  }
  if (pos == m_get) {  // overflow
    return 1;
  }
  return 0;
}

int ependingpool::queue_destroy() {
  if (m_ay_ready != NULL) {
    free(m_ay_ready);
    m_ay_ready = NULL;
  }
  return 0;
}

int ependingpool::check_time_out() {
  int current_time = time(NULL);
  if (m_least_last_update != INT_MAX &&
    current_time - m_least_last_update <= m_min_timeo) {
    return 0;
  }
  if (NULL == m_ay_sock || m_sock_len < 0) {
    return -1;
  }
  int ready_num = 0;
  int busy_num = 0;
  int read_num = 0;
  int write_num = 0;
  int last_active_offset = -1;
  m_least_last_update = INT_MAX;
  for (int i = 0; i < m_sock_len; ++i) {
    switch (m_ay_sock[i].sock_status) {
      case NOT_USED:
        break;
      case READY:
        if (current_time >= m_ay_sock[i].last_active + m_conn_timeo) {
          LOG(WARNING) << "handle:" << i << " time out last_active: " << m_ay_sock[i].last_active
                       << ", m_conn_timeo:" << m_conn_timeo << ", current_time:" << current_time;
          if (m_callbacklist[SOCK_LISTENTIMEOUT] != NULL) {
            m_callbacklist[SOCK_LISTENTIMEOUT](m_ay_sock[i].sock,
                                               (void**)(&m_ay_sock[i].arg));
          }
          reset_item(i, false);
          continue;
        }
        ready_num ++;
        if (m_least_last_update > m_ay_sock[i].last_active) {
          m_least_last_update = m_ay_sock[i].last_active;
        }
        last_active_offset = i;
        break;
      case READ_BUSY:
        if (current_time >= m_ay_sock[i].last_active + m_read_timeo) {
          LOG(WARNING) << "handle:" << i << " read time out last_active:" << m_ay_sock[i].last_active
                       << ", m_conn_timeo:" << m_conn_timeo << ", current_time:" << current_time;
          if (m_callbacklist[SOCK_READTIMEOUT] != NULL) {
            m_callbacklist[SOCK_READTIMEOUT](m_ay_sock[i].sock,
                                             (void**)(&m_ay_sock[i].arg));
          }
          reset_item(i, false);
          continue;
        }
        read_num ++;
        if (m_least_last_update > m_ay_sock[i].last_active) {
          m_least_last_update = m_ay_sock[i].last_active;
        }
        last_active_offset = i;
        break;
      case WRITE_BUSY:
        if (current_time >= m_ay_sock[i].last_active + m_write_timeo) {
          LOG(WARNING) << "handle" << i << " write time out last_active:" << m_ay_sock[i].last_active
                       << " m_conn_timeo:" << m_conn_timeo << ", current_time:" << current_time;
          if (m_callbacklist[SOCK_WRITETIMEOUT] != NULL) {
            m_callbacklist[SOCK_WRITETIMEOUT](m_ay_sock[i].sock,
                                              (void**)(&m_ay_sock[i].arg));
          }
          reset_item(i, false);
          continue;
        }
        write_num++;
        if (m_least_last_update > m_ay_sock[i].last_active) {
          m_least_last_update = m_ay_sock[i].last_active;
        }
        last_active_offset = i;
        break;
      case BUSY:
        busy_num++;
        m_ay_sock[i].last_active = current_time;
        last_active_offset = i;
        break;
      default:
        LOG(WARNING) << "unknow status " << m_ay_sock[i].sock_status;
        break;
    }
  }
  // 为保证多线程insert的安全, m_sock_len不减少
  if (!m_insert_item_sock_thrsafe) {
    m_sock_len = last_active_offset + 1;
  }
  if (ready_num == 0 && (busy_num > 0 || read_num > 0 || write_num > 0)) {
    LOG(INFO) << ready_num << " socket READY " << busy_num << " BUSY "
              << read_num << " READ WRITE" << write_num << " Total";
  }
  return 0;
}

int ependingpool::pool_epoll_wait(int timeout) {
  int  nfds;
  if (m_epfd <= 0) {
    LOG(FATAL) << "invalid epoll fd:" << m_epfd;
    return -1;
  }
  while (1) {
    // epool_wait return the ready event count those are save in
    // m_ay_events.
    nfds = epoll_wait(m_epfd, m_ay_events, m_sock_num, timeout);
    if (nfds < 0 && errno == EINTR) continue;
    if (nfds < 0) LOG(WARNING) << "epoll_wait failed. errno:" << errno;
    break;
  }
  return nfds;
}

int ependingpool::pool_epoll_offset_add(int offset, int events) {
  if (offset < 0) return -1;
  int ret = pool_epoll_add(m_ay_sock[offset].sock, offset, events);
  return ret;
}

int ependingpool::pool_epoll_offset_del(int offset) {
  if (offset < 0) return -1;
  int ret = pool_epoll_del(m_ay_sock[offset].sock, offset);
  return ret;
}

int ependingpool::pool_epoll_add(int sock, int fd, int events) {
  struct epoll_event ev;
  if (m_epfd < 0) {
    LOG(FATAL) << "invalid epoll fd:" << m_epfd;
    return -1;
  }
  ev.data.u64 = 0UL;
  ev.data.fd = fd;
  ev.events = events;
  if (epoll_ctl(m_epfd, EPOLL_CTL_ADD, sock, &ev) < 0) {
    LOG(FATAL) << "epoll_ctl add socket:" << sock << " failed. errno:" << errno;
    return -1;
  }
  LOG(INFO) << "socket:" << sock << " fd:" << fd << " add into epoll";
  return 0;
}

int ependingpool::pool_epoll_del(int sock, int fd) {
  struct epoll_event ev;
  if (m_epfd < 0) {
    LOG(FATAL) << "invalid epoll fd:" << m_epfd;
    return -1;
  }
  ev.data.u64 = 0UL;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLHUP | EPOLLERR;
  if (epoll_ctl(m_epfd, EPOLL_CTL_DEL, sock, &ev) < 0) {
    LOG(FATAL) << "epoll_ctl del socket:" << sock << " failed. errno:" << errno;
    return -1;
  }
  LOG(INFO) << "socket:" << sock << "fd:" << fd << " delete form epoll";
  return 0;
}

int ependingpool::pool_epoll_offset_mod(int offset, int event) {
  if (offset < 0) return -1;
  int ret = pool_epoll_mod(m_ay_sock[offset].sock, offset, event);
  return ret;
}

int ependingpool::pool_epoll_mod(int sock, int fd, int event) {
  struct epoll_event ev;
  if (m_epfd < 0) {
    LOG(FATAL) << "invalid epoll fd:" << m_epfd;
    return -1;
  }
  ev.data.u64 = 0ULL;
  ev.data.fd = fd;
  ev.events = event;
  if (epoll_ctl(m_epfd, EPOLL_CTL_MOD, sock, &ev) < 0) {
    LOG(FATAL) << "epoll_mod mod socket" << sock << " failed << errno:" << errno;
    return -1;
  }
  LOG(INFO) << "socket:" << sock << "fd:" << fd << " mod in epoll";
  return 0;
}
