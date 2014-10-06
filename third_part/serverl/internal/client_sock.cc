#include "../public/client_sock.h"
#include <endian.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event-config.h>
#include <event2/bufferevent_struct.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "base/public/logging.h"

uint64_t get_sock_ipport_key(uint32_t ip, uint16_t port) {
	uint64_t k = port;
	k = k<<32;
	k += ip;
	return k;
}

void fetch_ipport(uint64_t key, uint32_t* ip, uint16_t* port) {
	uint64_t _ip = key & 0xffffffff;
	uint64_t _port = (key >> 32);
	*ip = (uint32_t) _ip;
	*port = (uint16_t) _port;
}

bool ClientSock::init() {
	if (pthread_mutex_init(&m_refcount_mutex, NULL)) return false;
	m_refcount = 0;
	return true;
}

uint64_t ClientSock::snapshot_nolock() {
	return get_sock_ipport_key(this->m_ip, this->m_port);
}

bool ClientSock::destroy() {
	bufferevent_free(this->m_ev);
	close(this->m_fd);
	this->m_ev = NULL;
	this->m_fd = -1;
	this->m_ip = 0;
	this->m_port = 0;
	return true;
}

bool ClientSock::wantDestroy() {
	bool r;
	pthread_mutex_lock(&m_refcount_mutex);
	m_needDestroy = true;
	r = (this->m_refcount == 0);
	pthread_mutex_unlock(&m_refcount_mutex);
	return r;
}

bool ClientSock::set(uint32_t ip, uint16_t port, int fd, bufferevent *ev) {
	this->m_ev = ev;
	this->m_fd = fd;
	this->m_ip = ip;
	this->m_port = port;
	return true;
}

int ClientSock::read(ProtocalPackege* req) {
	struct evbuffer *input;
	int rc = CLIENTSOCK_READ_DELAY;
	int read;

	uint32_t* _len_le;
	uint32_t len;
	uint32_t total_len;
	unsigned char opcode;
	uint32_t avail_len;

	if (this->m_ev == NULL || this->m_fd < 0) {
		// this means this socket has been revoked by other threads(main thread)
		// so we should not do anything on this
		LOG(WARNING) << "ClientSock " << this << "::read socket has been revoked";
		goto _SOCK_READ_END;
	}

	bufferevent_lock(this->m_ev);
	input = bufferevent_get_input(this->m_ev);
	if (NULL == input) {
		LOG(WARNING) << "ClientSock[ip=" << this->m_ip
                 << ", port=" << this->m_port
                 << ", fd=" << this->m_fd
                 << "]::read get input buffer from event failed";
		goto _SOCK_READ_RELEASE_LOCK;
	}

	unsigned char *tmp;
	tmp = evbuffer_pullup(input, 5);

	if (tmp == NULL) {
		goto _SOCK_READ_RELEASE_LOCK; // not enough data
	}

	// little_endian_int32 | char | ...
	// byte length of following data | opcode | probuf_data...
	_len_le = (uint32_t*) (tmp);
	len = le32toh(*_len_le);
	total_len = len + 4;
	opcode = *(tmp + 4);
	avail_len = evbuffer_get_length(input);

	if (avail_len < total_len) {
  // this means we still have to wait a complete request
		LOG(INFO) << "ClientSock[ip=" << this->m_ip << ", port="
               << this->m_port << ", fd=" << this->m_fd
               << "]::read not enough data, ignore";
		goto _SOCK_READ_RELEASE_LOCK;
	}

	if (total_len > (uint32_t) req->getDataCap()) {
		// request too long, maybe an error. just drain buffer
		// need close socket??
		evbuffer_drain(input, total_len);
		LOG(WARNING) << "ClientSock[ip=" << this->m_ip << ", port=" << this->m_port
                 << ", fd=" << this->m_fd << "::read total_len="
                 << total_len << ", too large, drain_buffer";
		goto _SOCK_READ_RELEASE_LOCK;
	}

	read = evbuffer_remove(input, req->getData(), total_len);
	if ((uint32_t)read == total_len) {
		req->setOpCode(opcode);
		req->setDataLen(total_len);
		rc = CLIENTSOCK_READ_OK;
	} else {
		LOG(WARNING) << "ClientSock[ip=" << this->m_ip << ", port=" << this->m_port
                 << ", fd=" << this->m_fd << "]::read " << read
                 << " bytes, expected: " << total_len << ", just return false";
	}

_SOCK_READ_RELEASE_LOCK:
	bufferevent_unlock(this->m_ev);
_SOCK_READ_END:
	return rc;
}

bool ClientSock::write(const char * pData , int nDataLen) {
	int r;
	bool ret = false;
	if (this->m_ev == NULL || this->m_fd < 0) {
	  LOG(WARNING) << "ClientSock " << this 
                 << "::write, socket has bee revoked, ignore";
	  goto _SOCK_WRITE_END;
	}
	bufferevent_lock(this->m_ev);
	r = bufferevent_write(m_ev, pData, nDataLen);
	if (0 == r) {
    ret = true;
	} else {
		ret = false;
		LOG(WARNING) << "ClientSock[ip=" << this->m_ip << ", port=" << this->m_port
                 << ", fd=" << this->m_fd
                 << "::write bufferevent_write failed";
	}
	bufferevent_unlock(this->m_ev);
_SOCK_WRITE_END:
	return ret;
}

void ClientSock::incRefCount() {
	pthread_mutex_lock(&m_refcount_mutex);
	this->m_refcount ++;
	pthread_mutex_unlock(&m_refcount_mutex);
}

void ClientSock::decRefCount() {
	pthread_mutex_lock(&m_refcount_mutex);
	this->m_refcount --;
	if (this->m_refcount==0 && this->m_needDestroy) {
		// when refcount==0 and needDestroy flg is true,
    // we just notify the event_base that this ev can be removed
		// the main event loop of libevent will unlock base_event before locking bufferevent
		// and this code will lock bufferevent first and then lock the base_event
		// @see file: event.c, function: event_process_active_single_queue
		// so this would not cause deadlock, at least in current libevent version
		bufferevent_lock(this->m_ev);
		event_active(&(this->m_ev->ev_read),EV_READ,0);
		bufferevent_unlock(this->m_ev);
	}
	pthread_mutex_unlock(&m_refcount_mutex);
}

void ClientSock::getRefCount(int& r) {
	pthread_mutex_lock(&m_refcount_mutex);
	r = this->m_refcount;
	pthread_mutex_unlock(&m_refcount_mutex);
}
