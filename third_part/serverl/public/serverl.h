#ifndef _SERVERL_H_
#define _SERVERL_H_

#include "IPacketRouter.h"
#include <stdint.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_struct.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <map>
#include <vector>
#include <string>
#include "workerqueuel.h"
#include <pthread.h>
#include "clientsock_manager.h"

#define SERVERL_PROTOCOL_RAW 0
#define SERVERL_PROTOCOL_MEMCACHED 1

class ServerL {
public:
	ServerL();
	~ServerL();
	void setPacketRouter(IPacketRouter* router);
	void setPort(uint16_t _port);
	void setWorkerCount(int c);
	void setWorkerReservedInitFunction(WorkerReservedInitFunction fn);
	void setClientTcpDisconnectCB(SpecialJobCB cb);
	bool start();
	void stop();

	struct event_base * getEventBase() {
    return m_evbase;
  }

	IPacketRouter* getPacketRouter() {
    return m_packetRouter;
  }

	WorkerQueueL* getWorkerQueue() {
    return m_workerQueue;
  }

	void send(uint64_t skey, const char * data , int datalen);
	bool onAccept(uint32_t ip, uint16_t port, int fd, bufferevent *bufev);
	bool onDrop(struct bufferevent *bev);
	bool onDrop(int clientFd);
	bool onReceiveRequest(struct bufferevent *bev);

	void setClientSocketKeepaliveParams(int offset, int interval, int probes);
	int getClientSocketKeepaliveOffset();
	int getClientSocketKeepaliveInterval();
	int getClientSocketKeepaliveProbes();

	void setProtocol(int prot);
	int getProtocol();

private:
	bool startEvents();
	bool startListen();
	bool startWorkerQueue();

private:
	uint16_t m_port;
	IPacketRouter* m_packetRouter;
	int m_nWorkerCount;

	event_base *m_evbase;

	int m_listenFd;
	WorkerQueueL* m_workerQueue;

	ClientSockManager* m_clientSockManager;
	WorkerReservedInitFunction m_workerReservedInitFn;
	SpecialJobCB m_clientTcpDisconnectCB;

	//client socket keepalive parameters.
	int m_sockKeepaliveOffset;
	int m_sockKeepaliveInterval;
	int m_sockKeepaliveProbes;
	int protocol;
};
extern ServerL g_serverl;
#endif
