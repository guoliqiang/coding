#ifndef PROTOCAL_PACKAGE_H_
#define PROTOCAL_PACKAGE_H_

#include <stdio.h>
#include <stdint.h>

class ProtocalPackege {
 public:
	ProtocalPackege();
	~ProtocalPackege();
	bool init(int _cap);
	char* getData();
	int getDataLen();
	int getDataCap();
	void setOpCode(unsigned char _opcode);
	void setDataLen(int len);

 public:
	int32_t packlength;
	unsigned char opcode;

 private:
	char* databuf;
	int datalen;
	int cap;
};

#endif // PROTOCAL_PACKAGE_H_
