#ifndef _D5_IPacketRouter_H__
#define _D5_IPacketRouter_H__

#include "protocal_package.h"

class IPacketRouter {
 protected:
	typedef uint64_t socketClientData_t;

 public:
	virtual ~IPacketRouter(){}

 public:
	virtual void OnReceivePacket(socketClientData_t sClient,
                               const char * pData ,int nDataLen )=0;
};

#endif // _D5_IPacketRouter_H__
