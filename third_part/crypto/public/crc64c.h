// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-12-07 11:16:02
// File  : crc64c.h
// Brief :

#ifndef  __CRC64C_H_
#define  __CRC64C_H_

#include "base/public/basictypes.h"

namespace crypto {

uint64_t Value64(uint64_t crc, const unsigned char *s, uint64_t l);

}  // namepsace crypto

#endif  // __CRC64C_H_
