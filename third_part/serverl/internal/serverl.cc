#include "../public/serverl.h"
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <event2/thread.h>
#include <event2/event-config.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include "base/public/logging.h"

#define SOMAXCONN 128

ServerL g_serverl;

void jobcb(void* pJob, void* pWorker) {
	JobL* theJob = (JobL*) pJob;
	WorkerQueueL::WorkerL* theWorker = (WorkerQueueL::WorkerL*) pWorker;
	ClientSock* sock = theJob->sock;
	ServerL* theServer = (ServerL*)theWorker->workerQueue->getServer();

	int rc = CLIENTSOCK_READ_DELAY;
	while (true) {
		if (theServer->getProtocol() == SERVERL_PROTOCOL_RAW) {
			rc = sock->read(theWorker->inputBuffer);
		} else {
			rc = CLIENTSOCK_READ_UNKNOWN_PROTO;
		}

		if (rc != CLIENTSOCK_READ_OK) break;
		theWorker->pcnt++;
		if (theServer->getProtocol()==SERVERL_PROTOCOL_RAW) {
			theJob->packetRouter->OnReceivePacket(theJob->sockKeySnapshot,
					theWorker->inputBuffer->getData(),
					theWorker->inputBuffer->getDataLen());
		} else {
			LOG(WARNING) << "unknown proto";
		}
	}
	int fd = sock->getFd();
	sock->decRefCount();
  LOG(INFO) << "server close socket = " << fd;
	if (rc < 0) theServer->onDrop(fd);
}

void jobcb_client_tcp_close(void* pJob, void* pWorker) {
	JobL* theJob = (JobL*) pJob;
	SpecialJobCB cb = theJob->specialCB;
	if (NULL != cb) {
		cb((void*)(&(theJob->sockKeySnapshot)));
	}
}

void client_fd_readcb(struct bufferevent *bev, void *ctx) {
	ServerL* server = (ServerL*) ctx;
	server->onReceiveRequest(bev);
}

void client_fd_writecb(struct bufferevent *bev, void *ctx) {}

void client_fd_evcb(struct bufferevent *bev, short error, void *ctx) {
	//TODO: clean up?
	if (error & BEV_EVENT_EOF) {
		// connection has been closed, do any clean up here
	} else if (error & BEV_EVENT_ERROR) {
		// check errno to see what error occurred
	} else if (error & BEV_EVENT_TIMEOUT) {
		// must be a timeout event handle, handle it
	}
	ServerL* server = (ServerL*) ctx;
	server->onDrop(bev);
}

void do_accept(evutil_socket_t listener, short event, void *arg) {
	ServerL* server = (ServerL*) arg;
	event_base * evbase = server->getEventBase();

	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(listener, (struct sockaddr*)(&client_addr),
                         &client_len);

	if (client_fd < 0) {
		LOG(ERROR) << "accept error";
		return ;
	}

	bufferevent * bev;
	evutil_make_socket_nonblocking (client_fd);

	// keepalive, by default, linux system use 7200s as keepalive idle
	// 75 seconds as keepalive interval, and allows 9 times of retry
	// which is too long
	// we override these parameters to more reasonable values
	int optval;
	socklen_t optlen = sizeof(int);
  // http://blog.chinaunix.net/uid-26575352-id-3483808.html
	optval = 1;
	if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
		LOG(WARNING) << "set SO_KEEPALIVE on client fd="
                 << client_fd << " failed";
		return;
	}

	optval = server->getClientSocketKeepaliveOffset();
	if (setsockopt(client_fd, SOL_TCP, TCP_KEEPIDLE, &optval, optlen) < 0) {
		LOG(WARNING) << "set TCP_KEEPIDLE on client fd="
                 << client_fd << " failed";
		return;
	}

	optval = server->getClientSocketKeepaliveInterval();
	if (setsockopt(client_fd, SOL_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) {
		LOG(WARNING) << "set TCP_KEEPINTVL on client fd="
                 << client_fd << " failed";
		return;
	}

	optval = server->getClientSocketKeepaliveProbes();
	if (setsockopt(client_fd, SOL_TCP, TCP_KEEPCNT, &optval, optlen) < 0) {
		LOG(WARNING) << "set TCP_KEEPCNT on client fd="
                 << client_fd << " failed";
		return;
	}

	optval = 1;
	if (setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &optval,
                 sizeof(optval)) < 0) {
		LOG(WARNING) << "set TCP_NODELAY on client fd="
                 << client_fd << " failed";
		return;
	}

	bev = bufferevent_socket_new(evbase, client_fd, BEV_OPT_THREADSAFE);
	if (NULL == bev) {
		LOG(WARNING) << "create bufferevent on client fd="
              << client_fd << " failed";
		close(client_fd);
		return;
	}
	bufferevent_setcb(bev, client_fd_readcb, client_fd_writecb,
                    client_fd_evcb, (void*)server);
	if (bufferevent_enable(bev, EV_READ | EV_WRITE) != 0) {
		LOG(WARNING) << "enable bufferevent on client fd="
                 << client_fd << " failed";
		bufferevent_free(bev);
		return;
	}

	bool br = server->onAccept(ntohl(client_addr.sin_addr.s_addr),
                             ntohs(client_addr.sin_port), client_fd, bev);
	if (!br) {
		// error
		LOG(WARNING) << "accept client sock(ip="
                 << ntohl(client_addr.sin_addr.s_addr)
                 << ", port=" << ntohs(client_addr.sin_port)
                 << "fd=" << client_fd << " failed";
		bufferevent_free(bev);
		// no need to close since we are using BEV_OPT_CLOSE_ON_FREE
		// close(client_fd);
	} else {
		LOG(INFO) << "accept client sock(ip="
              << ntohl(client_addr.sin_addr.s_addr)
              << ", port=" << ntohs(client_addr.sin_port)
              << "fd=" << client_fd << " ok";
	}
}

ServerL::ServerL() {
	m_port = 0;
	m_packetRouter = NULL;
	m_evbase = NULL;
	m_listenFd = -1;
	m_workerQueue = NULL;
	m_clientSockManager = NULL;
	m_nWorkerCount = 0;
	m_workerReservedInitFn = NULL;
	m_clientTcpDisconnectCB = NULL;

	// begin keepalive prob when there is no data for 90 seconds
	// probe every 20 seconds, after no response 5 times,
  // consider connection close
	m_sockKeepaliveOffset = 90;
	m_sockKeepaliveInterval = 20;
	m_sockKeepaliveProbes = 5;
	protocol = SERVERL_PROTOCOL_RAW;
}

ServerL::~ServerL() {
	event_base_free(m_evbase);
}

bool ServerL::start() {
	signal(SIGPIPE, SIG_IGN);
	if (NULL == m_packetRouter ||
      0 == m_port ||
      m_nWorkerCount < 1) {
		LOG(ERROR) << "not all necessary fields are set, start serverl failed";
		return false;
	}
	m_clientSockManager = new ClientSockManager();
	CHECK(m_clientSockManager != NULL);
	if (!m_clientSockManager->init()) {
		LOG(ERROR) << "init data rwlock failed, start serverl failed";
		return false;
	}
	if (evthread_use_pthreads() != 0) {
		LOG(WARNING) << "enable pthreads for libevent failed";
		return false;
	}
	if (!startWorkerQueue()) {
		LOG(ERROR) << "start worker queue failed";
		return false;
	}
	if (!startListen()) {
		LOG(ERROR) << "start listen failed";
		return false;
	}
	if (!startEvents()) {
		LOG(ERROR) << "start events failed";
		return false;
	}
	return true;
}

void ServerL::stop() {
	LOG(INFO) << "ServerL::stop";
	close(this->m_listenFd);
	event_base_loopbreak(this->m_evbase);
	this->m_workerQueue->stop();
}

bool ServerL::startEvents() {
	m_evbase = event_base_new();
	if (NULL == m_evbase) {
		LOG(ERROR) << "create event base for serverl failed";
		return false;
	}
	event * evlisten = event_new(m_evbase, m_listenFd, EV_READ|EV_PERSIST,
                               do_accept, (void*)this);

	event_add(evlisten, NULL);
	int r = event_base_dispatch(m_evbase);
	LOG(INFO) << "event base dispatch return=" << r;
	return true;
}

bool ServerL::startListen() {
	m_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenFd < 0) {
		LOG(ERROR) << "create listen sock error, start listen failed";
		return false;
	}

	if (evutil_make_socket_nonblocking(m_listenFd)) {
		LOG(ERROR) << "set linsten sock non block error, start serverl failed";
		return false;
	}

	int optval = 1;
	setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	struct sockaddr_in listen_addr;
	bzero(&listen_addr, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = 0;
	listen_addr.sin_port = htons(m_port);

	if (bind(m_listenFd, (struct sockaddr *) &listen_addr,
           sizeof(listen_addr))!= 0) {
		LOG(ERROR) << "bind listen sock error, start listen failed";
		return false;
	}
	if (listen(m_listenFd, SOMAXCONN) < 0) {
		LOG(ERROR) << "begin listen sock error, start listen failed";
		return false;
	}
	return true;
}

bool ServerL::startWorkerQueue() {
	this->m_workerQueue = new WorkerQueueL();
	CHECK(NULL != this->m_workerQueue);
	return this->m_workerQueue->init(m_nWorkerCount,
      m_workerReservedInitFn, (void*)this);
}

bool ServerL::onAccept(uint32_t ip, uint16_t port,
    int fd, bufferevent *bufev) {
	return this->m_clientSockManager->onAcceptClientSock(fd, ip, port, bufev);
}

//this function should only be invoked in main thread
bool ServerL::onDrop(struct bufferevent *bev) {
	int clientFd = bufferevent_getfd(bev);
	return onDrop(clientFd);
}

bool ServerL::onDrop(int clientFd) {
	uint64_t sockKey = this->m_clientSockManager->onDropClientSock(clientFd);
	if (sockKey == 0) {
		// this means we try lock sockmanager error,
    // there are some other threads using it,
		// we just bypass this case, and send a read event to the bufferevent,
    // so that it could take care of this sock in the next event loop
		// this could cause infinite loop when current bev was held by
    // a worker thread for too long
		// event_active(&(bev->ev_read), EV_READ,0);
		// we move this to ClientSock::decRefCount()
	} else if (sockKey == 1) {
		LOG(WARNING) << "Cannot find sock of clientFd=" << clientFd;
	} else if (NULL != m_clientTcpDisconnectCB) {
		JobL job;
		job.sock = NULL;
		job.fn = jobcb_client_tcp_close;
		job.packetRouter = NULL;
		job.sockKeySnapshot = sockKey;
		job.specialCB = m_clientTcpDisconnectCB;
		job.specialParam = NULL;
		return this->m_workerQueue->pushJob(job);
	}
	return true;
}

bool ServerL::onReceiveRequest(struct bufferevent *bev) {
	int clientFd = bufferevent_getfd(bev);
	bool delay = false;
	ClientSock* sock =
  this->m_clientSockManager->getClientSockByFdAndIncRefCount(clientFd, delay);
  if (sock != NULL) {
		JobL job;
		job.sock = sock;
		job.fn = jobcb;
		job.packetRouter = this->m_packetRouter;
		job.sockKeySnapshot = sock->snapshot_nolock();
		job.specialCB = NULL;
		job.specialParam = NULL;
		return this->m_workerQueue->pushJob(job);
	} else {
		LOG(WARNING) << "ServerL::onReceiveRequest: clientFd="
                 << clientFd << " with no sock, maybe revoked";
		return false;
	}
}

void ServerL::send(uint64_t skey, const char * data , int datalen) {
	WorkerQueueL::workerl_t* worker = (WorkerQueueL::workerl_t*)
      pthread_getspecific(WorkerQueueL::m_workerKey);
	if (worker && worker->currentJob &&
      worker->currentJob->sockKeySnapshot==skey) {
		worker->currentJob->sock->write(data, datalen);
	} else {
		bool b = this->m_clientSockManager->sendToClientSock(skey, data, datalen);
		if (!b) {
			LOG(WARNING) << "ServerL::send client sock with key="
                << skey << ", cmd " << *(data + 4)
                << " failed, sock may be revoked";
		}
	}
}

void ServerL::setClientSocketKeepaliveParams(int offset,
    int interval, int probes) {
	m_sockKeepaliveOffset = offset;
	m_sockKeepaliveInterval = interval;
	m_sockKeepaliveProbes = probes;
}

void ServerL::setPacketRouter(PacketRouter* router) {
	m_packetRouter = router;
}

void ServerL::setPort(uint16_t _port) {
	m_port = _port;
}

void ServerL::setWorkerCount(int c) {
	m_nWorkerCount = c;
}

void ServerL::setWorkerReservedInitFunction(WorkerReservedInitFunction fn) {
	m_workerReservedInitFn = fn;
}

void ServerL::setClientTcpDisconnectCB(SpecialJobCB cb) {
	m_clientTcpDisconnectCB = cb;
}

void ServerL::setProtocol(int prot) {
	protocol = prot;
}

int ServerL::getClientSocketKeepaliveOffset() {
	return m_sockKeepaliveOffset;
}

int ServerL::getClientSocketKeepaliveInterval() {
	return m_sockKeepaliveInterval;
}

int ServerL::getClientSocketKeepaliveProbes() {
	return m_sockKeepaliveProbes;
}

int ServerL::getProtocol() {
	return protocol;
}
