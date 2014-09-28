#include "../public/protocal_package.h"
#include <memory.h>

ProtocalPackege::ProtocalPackege() {
	packlength = 0;
	opcode = 0;
	databuf=NULL;
	cap = 0;
	datalen = 0;
}

ProtocalPackege::~ProtocalPackege() {
	if(databuf) delete []databuf;
}

bool ProtocalPackege::init(int _cap) {
	databuf = new char[_cap];
	if (!databuf) return false;
	cap = _cap;
	return true;
}

char* ProtocalPackege::getData() {
	return databuf;
}

int ProtocalPackege::getDataLen() {
	return datalen;
}

int ProtocalPackege::getDataCap() {
	return cap;
}

void ProtocalPackege::setOpCode(unsigned char _opcode) {
	opcode = _opcode;
}

void ProtocalPackege::setDataLen(int len) {
	datalen = len;
}
