#ifndef _CLIENTSOCK_MANAGER_H_
#define _CLIENTSOCK_MANAGER_H_

#include "clientsock.h"
#include <map>

#define CLIENT_SOCK_MAX_COUNT 1000000

class ClientSockManager {
 public:
	ClientSockManager();
	~ClientSockManager(){}

	bool init();
	bool onAcceptClientSock(int fd, uint32_t ip, uint16_t port, bufferevent* ev);
	uint64_t onDropClientSock(int fd);
	bool sendToClientSock(uint64_t sockkey, const char* data, int len);
	ClientSock* getClientSockByFdAndIncRefCount(int fd, bool& delay);

 private:
	ClientSock* allocClientSock();
	bool mapClientSock(ClientSock* slot);
	ClientSock* unmapClientSockNoLock(int fd, bool& delay);
	bool releaseClientSock(ClientSock* sock);
	ClientSock* getClientSockByFdNoLock(int fd);
	ClientSock* getClientSockByIpportKeyNoLock(uint64_t key);

 private:
	pthread_rwlock_t m_lock;
	std::map<int, ClientSock*> m_fdSockMap;
	std::map<uint64_t, ClientSock*> m_keySockMap;
};

#endif
