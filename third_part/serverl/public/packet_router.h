#ifndef _PacketRouter_H__
#define _PacketRouter_H__

#include "protocal_package.h"

class PacketRouter {
 protected:
	typedef uint64_t socketClientData_t;

 public:
	virtual ~PacketRouter(){}

 public:
	virtual void OnReceivePacket(socketClientData_t sClient,
                               const char * pData ,int nDataLen )=0;
};

#endif // _PacketRouter_H__
