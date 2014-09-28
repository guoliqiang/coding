#include "../public/clientsock_manager.h"
#include "base/public/logging.h"

ClientSockManager::ClientSockManager() { }

bool ClientSockManager::init() {
	//init rwlock
	if (pthread_rwlock_init(&m_lock,NULL)) return false;
	return true;
}

ClientSock* ClientSockManager::allocClientSock() {
	return new ClientSock();
}

bool ClientSockManager::releaseClientSock(ClientSock* sock) {
	delete sock;
	return true;
}

bool ClientSockManager::mapClientSock(ClientSock* sock) {
	pthread_rwlock_wrlock(&m_lock);
	uint64_t key = sock->snapshot_nolock();
	this->m_keySockMap[key] = sock;
	this->m_fdSockMap[sock->getFd()] = sock;
	pthread_rwlock_unlock(&m_lock);
	return true;
}

ClientSock* ClientSockManager::unmapClientSockNoLock(int fd, bool& delay) {
	ClientSock* sock = NULL;
	std::map<uint64_t, ClientSock*>::iterator keyit;
	std::map<int, ClientSock*>::iterator fdit;
	uint64_t key = 0;
	delay = false;

	bool fdok = false;
	bool keyok = false;

	fdit = this->m_fdSockMap.find(fd);
	if (fdit != this->m_fdSockMap.end()) {
		sock = fdit->second;
		if (!sock->wantDestroy()) {
			LOG(WARNING) << "ClientSockManager::UNmapClientSockSlot fd="
                   << fd << " ClientSock="
                   << sock << " Cannot be destroyed for now";
			delay = true;
			return NULL;
		}

		key = sock->snapshot_nolock();
		keyit = this->m_keySockMap.find(key);
		if (keyit != this->m_keySockMap.end()) {
			keyok = true;
			this->m_keySockMap.erase(keyit);
		}

		fdok = true;
		this->m_fdSockMap.erase(fdit);
		return sock;
	} else {
		return NULL;
	}
	LOG(INFO) << "ClientSockManager::UNmapClientSockSlot fd=" << fd << "key="
            << key << " ClientSock = " << sock << " , fdok = " << fdok
            << " keyok = " << keyok;
}

bool ClientSockManager::onAcceptClientSock(int fd, uint32_t ip,
                                           uint16_t port, bufferevent* ev) {
	ClientSock* sock = allocClientSock();
	if (!sock) return false;
	if (!sock->init()) return false;
	if (!(sock->set(ip, port, fd, ev))) return false;
	return mapClientSock(sock);
}

uint64_t ClientSockManager::onDropClientSock(int fd) {
	uint64_t sockkey = 0;
	pthread_rwlock_wrlock(&m_lock);

	bool delay = false;
	ClientSock* sock = unmapClientSockNoLock(fd, delay);
	if (delay) { //can not unmap socket for now
		sockkey = 0;
	} else if (NULL == sock) { //can not find socket of fd
		LOG(WARNING) << "Cannot find sock for fd=" << fd;
		sockkey = 1;
	} else { //unmap ok
		sockkey = sock->snapshot_nolock();
		sock->destroy();
		delete sock;
	}

	pthread_rwlock_unlock(&m_lock);
	return sockkey;
}

bool ClientSockManager::sendToClientSock(uint64_t sockkey,
                                         const char* data, int len) {
	ClientSock* sock = NULL;
	pthread_rwlock_rdlock(&m_lock);

	sock = getClientSockByIpportKeyNoLock(sockkey);
	if (sock!=NULL) sock->incRefCount();

	pthread_rwlock_unlock(&m_lock);

	if (sock) {
		sock->write(data, len);
		sock->decRefCount();
		return true;
	}

	return false;
}

ClientSock* ClientSockManager::getClientSockByFdAndIncRefCount(int fd,
    bool& delay) {
	ClientSock* sock = NULL;
	pthread_rwlock_rdlock(&m_lock);

	delay = false;
	sock = getClientSockByFdNoLock(fd);
	if (sock==NULL) goto _GET_SOCK_END;

	sock->incRefCount();

_GET_SOCK_END:
	pthread_rwlock_unlock(&m_lock);
	return sock;
}

ClientSock* ClientSockManager::getClientSockByFdNoLock(int fd) {
	ClientSock* sock = NULL;
	std::map<int, ClientSock*>::const_iterator it;

	it = m_fdSockMap.find(fd);
	if (it != m_fdSockMap.end()) sock = it->second;

	return sock;
}

ClientSock* ClientSockManager::getClientSockByIpportKeyNoLock(uint64_t key) {
	ClientSock* sock = NULL;
	std::map<uint64_t, ClientSock*>::const_iterator it;

	it = m_keySockMap.find(key);
	if (it != m_keySockMap.end()) sock = it->second;

	return sock;
}
