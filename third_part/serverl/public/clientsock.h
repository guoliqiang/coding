#ifndef CLIENTSOCK_H_
#define CLIENTSOCK_H_

#include <stdint.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "protocal_package.h"
#include <string>

#define CLIENTSOCK_READ_OK 0
#define CLIENTSOCK_READ_DELAY 1
#define CLIENTSOCK_READ_ERROR -1
#define CLIENTSOCK_READ_UNKNOWN_PROTO -2

#define CLIENTSOCK_READ_STATUS_MEM_WAIT_FIRSTLINE 0 //waiting first line
#define CLIENTSOCK_READ_STATUS_MEM_WAIT_DATA 1 //waiting data block
//#define CLIENTSOCK_READ_STATUS_MEM_ERROR (-1) //waiting data block

uint64_t get_sock_ipport_key(uint32_t ip, uint16_t port);
void fetch_ipport(uint64_t key, uint32_t* ip, uint16_t* port);

class ClientSock {
 public:
	class ReadStatusMemcache {
	 public:
		unsigned char status;
		std::string strcmd;
		std::string strkey;
		uint32_t flags;
		uint32_t exptime;
		uint32_t datalen;
		bool noreply;
	};
 public:
	ClientSock() {
		m_ev=NULL;
		m_ip = 0;
		m_port = 0;
		m_fd = -1;
		m_refcount = 0;
		m_needDestroy = false;
	}
	virtual ~ClientSock(){}

	bool init();
	bool destroy();
	bool wantDestroy();
	bool set(uint32_t ip, uint16_t port, int fd, bufferevent *ev);

	int read(ProtocalPackege* req);
	bool write(const char * pData , int nDataLen);

	void incRefCount();
	void decRefCount();
	void getRefCount(int &r);

	uint32_t getIp() {
    return m_ip;
  }
	uint16_t getPort() {
    return m_port;
  }
	int getFd() {
    return m_fd;
  }
	bufferevent* getEvent() {
    return m_ev;
  }
	uint64_t snapshot_nolock();

 private:
	uint32_t m_ip;
	uint16_t m_port;
	int m_fd;
	bufferevent *m_ev;
	volatile int m_refcount;
	pthread_mutex_t m_refcount_mutex;
	bool m_needDestroy;

 private:
	ReadStatusMemcache readStatusMemcached;
};
#endif /* CLIENTSOCK_H_ */
