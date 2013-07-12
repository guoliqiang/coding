/*
  100% free public domain implementation of the SHA-1 algorithm
  by Dominik Reichl <dominik.reichl@t-online.de>
  Web: http://www.dominik-reichl.de/
  Copyright [2010] <jimchung1221@gmail.com>

  Version 1.6 - 2005-02-07 (thanks to Howard Kapustein for patches)
  - You can set the endianness in your files, no need to modify the
  header file of the CSHA1 class any more
  - Aligned data support
  - Made support/compilation of the utility functions (ReportHash
  and HashFile) optional (useful, if bytes count, for example in
  embedded environments)

  Version 1.5 - 2005-01-01
  - 64-bit compiler compatibility added
  - Made variable wiping optional (define SHA1_WIPE_VARIABLES)
  - Removed unnecessary variable initializations
  - ROL32 improvement for the Microsoft compiler (using _rotl)

  ======== Test Vectors (from FIPS PUB 180-1) ========

  SHA1("abc") =
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D

  SHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq") =
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1

  SHA1(A million repetitions of "a") =
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/
#include "../public/sha1.h"
#include "base/public/basictypes.h"
#ifdef SHA1_UTILITY_FUNCTIONS
#define SHA1_MAX_FILE_BUFFER 8000
#endif

// Rotate x bits to the left
#ifndef ROL32
#ifdef _MSC_VER
#define ROL32(_val32, _nBits) _rotl(_val32, _nBits)
#else
#define ROL32(_val32, _nBits) \
        (((_val32) << (_nBits))|((_val32) >> (32 - (_nBits))))
#endif
#endif

#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (block_->l[i] = \
        (ROL32(block_->l[i], 24)  &  0xFF00FF00) \
        | (ROL32(block_->l[i], 8)  &  0x00FF00FF))
#else
#define SHABLK0(i) (block_->l[i])
#endif

#define SHABLK(i) (block_->l[i & 15] = ROL32(block_->l[(i+13) & 15]\
                                            ^ block_->l[(i+8) & 15] \
                                            ^ block_->l[(i+2) & 15] \
                                            ^ block_->l[i & 15], 1))

// SHA-1 rounds
#define _R0(v, w, x, y, z, i) { \
  z += ((w & (x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v, 5);\
  w = ROL32(w, 30);\
}
#define _R1(v, w, x, y, z, i) {\
  z += ((w & (x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v, 5);\
  w = ROL32(w, 30);\
}
#define _R2(v, w, x, y, z, i) {\
  z += (w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v, 5);\
  w = ROL32(w, 30);\
}
#define _R3(v, w, x, y, z, i) {\
  z += (((w|x) & y)|(w & x))+SHABLK(i)+0x8F1BBCDC+ROL32(v, 5);\
  w = ROL32(w, 30);\
}
#define _R4(v, w, x, y, z, i) {\
  z += (w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v, 5);\
  w = ROL32(w, 30);\
}

CSha1::CSha1() {
  block_ = reinterpret_cast<Shas1WorkspaceBlock *>(workspace_);

  Reset();
}

CSha1::~CSha1() {
  Reset();
}

void CSha1::Reset() {
  // SHA1 initialization constants
  state_[0] = 0x67452301;
  state_[1] = 0xEFCDAB89;
  state_[2] = 0x98BADCFE;
  state_[3] = 0x10325476;
  state_[4] = 0xC3D2E1F0;

  count_[0] = 0;
  count_[1] = 0;
}

void CSha1::Transform(unsigned int *state_, char *buffer) {
  unsigned int a = state_[0];
  unsigned int b = state_[1];
  unsigned int c = state_[2];
  unsigned int d = state_[3];
  unsigned int e = state_[4];

  memcpy(block_, buffer, 64);

  // 4 rounds of 20 operations each. Loop unrolled.
  _R0(a, b, c, d, e, 0);
  _R0(e, a, b, c, d, 1);
  _R0(d, e, a, b, c, 2);
  _R0(c, d, e, a, b, 3);
  _R0(b, c, d, e, a, 4);
  _R0(a, b, c, d, e, 5);
  _R0(e, a, b, c, d, 6);
  _R0(d, e, a, b, c, 7);
  _R0(c, d, e, a, b, 8);
  _R0(b, c, d, e, a, 9);
  _R0(a, b, c, d, e, 10);
  _R0(e, a, b, c, d, 11);
  _R0(d, e, a, b, c, 12);
  _R0(c, d, e, a, b, 13);
  _R0(b, c, d, e, a, 14);
  _R0(a, b, c, d, e, 15);
  _R1(e, a, b, c, d, 16);
  _R1(d, e, a, b, c, 17);
  _R1(c, d, e, a, b, 18);
  _R1(b, c, d, e, a, 19);
  _R2(a, b, c, d, e, 20);
  _R2(e, a, b, c, d, 21);
  _R2(d, e, a, b, c, 22);
  _R2(c, d, e, a, b, 23);
  _R2(b, c, d, e, a, 24);
  _R2(a, b, c, d, e, 25);
  _R2(e, a, b, c, d, 26);
  _R2(d, e, a, b, c, 27);
  _R2(c, d, e, a, b, 28);
  _R2(b, c, d, e, a, 29);
  _R2(a, b, c, d, e, 30);
  _R2(e, a, b, c, d, 31);
  _R2(d, e, a, b, c, 32);
  _R2(c, d, e, a, b, 33);
  _R2(b, c, d, e, a, 34);
  _R2(a, b, c, d, e, 35);
  _R2(e, a, b, c, d, 36);
  _R2(d, e, a, b, c, 37);
  _R2(c, d, e, a, b, 38);
  _R2(b, c, d, e, a, 39);
  _R3(a, b, c, d, e, 40);
  _R3(e, a, b, c, d, 41);
  _R3(d, e, a, b, c, 42);
  _R3(c, d, e, a, b, 43);
  _R3(b, c, d, e, a, 44);
  _R3(a, b, c, d, e, 45);
  _R3(e, a, b, c, d, 46);
  _R3(d, e, a, b, c, 47);
  _R3(c, d, e, a, b, 48);
  _R3(b, c, d, e, a, 49);
  _R3(a, b, c, d, e, 50);
  _R3(e, a, b, c, d, 51);
  _R3(d, e, a, b, c, 52);
  _R3(c, d, e, a, b, 53);
  _R3(b, c, d, e, a, 54);
  _R3(a, b, c, d, e, 55);
  _R3(e, a, b, c, d, 56);
  _R3(d, e, a, b, c, 57);
  _R3(c, d, e, a, b, 58);
  _R3(b, c, d, e, a, 59);
  _R4(a, b, c, d, e, 60);
  _R4(e, a, b, c, d, 61);
  _R4(d, e, a, b, c, 62);
  _R4(c, d, e, a, b, 63);
  _R4(b, c, d, e, a, 64);
  _R4(a, b, c, d, e, 65);
  _R4(e, a, b, c, d, 66);
  _R4(d, e, a, b, c, 67);
  _R4(c, d, e, a, b, 68);
  _R4(b, c, d, e, a, 69);
  _R4(a, b, c, d, e, 70);
  _R4(e, a, b, c, d, 71);
  _R4(d, e, a, b, c, 72);
  _R4(c, d, e, a, b, 73);
  _R4(b, c, d, e, a, 74);
  _R4(a, b, c, d, e, 75);
  _R4(e, a, b, c, d, 76);
  _R4(d, e, a, b, c, 77);
  _R4(c, d, e, a, b, 78);
  _R4(b, c, d, e, a, 79);

  // Add the working vars back into state_
  state_[0] += a;
  state_[1] += b;
  state_[2] += c;
  state_[3] += d;
  state_[4] += e;

  // Wipe variables
#ifdef SHA1_WIPE_VARIABLES
  a = b = c = d = e = 0;
#endif
}

// Use this function to hash in binary data and strings
void CSha1::Update(char *data, unsigned int len) {
  unsigned int i, j;

  j = (count_[0] >> 3)  &  63;

  if ((count_[0] += len << 3) < (len << 3))
    count_[1]++;

  count_[1] += (len >> 29);

  if ((j + len) > 63) {
    i = 64 - j;
    memcpy(&buffer[j], data, i);
    Transform(state_, buffer);

    for (; i + 63 < len; i += 64)
      Transform(state_,  & data[i]);

    j = 0;
  } else {
    i = 0;
  }
  memcpy(&buffer[j],  &data[i], len - i);
}

#ifdef SHA1_UTILITY_FUNCTIONS
// Hash in file contents
bool CSha1::HashFile(char *file_name) {
  uint64 ulFileSize, ulRest, ulBlocks;
  uint64 i;
  char uData[SHA1_MAX_FILE_BUFFER];
  FILE *fIn;

  if (file_name == NULL)
    return false;

  fIn = fopen(file_name, "rb");
  if (fIn == NULL)
    return false;

  fseek(fIn, 0, SEEK_END);
  ulFileSize = static_cast<uint64>(ftell(fIn));
  fseek(fIn, 0, SEEK_SET);

  if (ulFileSize != 0) {
    ulBlocks = ulFileSize / SHA1_MAX_FILE_BUFFER;
    ulRest = ulFileSize % SHA1_MAX_FILE_BUFFER;
  } else {
    ulBlocks = 0;
    ulRest = 0;
  }

  for (i = 0; i < ulBlocks; i++) {
    ignore_result(fread(uData, 1, SHA1_MAX_FILE_BUFFER, fIn));
    Update(uData, SHA1_MAX_FILE_BUFFER);
  }

  if (ulRest != 0) {
    ignore_result(fread(uData, 1, ulRest, fIn));
    Update(uData, ulRest);
  }

  fclose(fIn);
  fIn = NULL;
  return true;
}
#endif

void CSha1::Final() {
  unsigned int i;
  char finalcount_[8];

  for (i = 0; i < 8; i++)
    finalcount_[i] = static_cast<char>(((count_[((i >= 4) ? 0 : 1)]
                                         >> ((3 - (i  &  3)) * 8)) & 255));

  char p_1[] = {"\200"};
  Update(p_1, 1);

  char p_2[] = {"\0"};
  while ((count_[0]  &  504) != 448)
    Update(p_2, 1);

  Update(finalcount_, 8);  // Cause a SHA1Transform()

  for (i = 0; i < 20; i++) {
    digest_[i] = static_cast<char>(((state_[i >> 2] >> ((3 - (i  &  3)) * 8))
                                    & 255));
  }

  // Wipe variables for security reasons
#ifdef SHA1_WIPE_VARIABLES
  i = 0;
  memset(buffer, 0, 64);
  memset(state_, 0, 20);
  memset(count_, 0, 8);
  memset(finalcount_, 0, 8);
  Transform(state_, buffer);
#endif
}

#ifdef SHA1_UTILITY_FUNCTIONS
// Get the final hash as a pre-formatted string
void CSha1::ReportHash(char *report, unsigned char report_type) {
  unsigned char i;
  char szTemp[16];
  char *iter = NULL;

  if (report == NULL)
    return;

  if (report_type == REPORT_HEX) {
    snprintf(szTemp, sizeof(szTemp), "%02X", digest_[0]);
    iter = report + sizeof(report);
    snprintf(iter, sizeof(iter), "%s", szTemp);

    for (i = 1; i < 20; i++) {
      snprintf(szTemp, sizeof(szTemp), " %02X", digest_[i]);
      iter += strlen(report);
      snprintf(iter, sizeof(iter), "%s", szTemp);
    }
  } else if (report_type == REPORT_DIGIT) {
    snprintf(szTemp, sizeof(szTemp), "%u", digest_[0]);
    iter += strlen(report);
    snprintf(iter, sizeof(iter), "%s", szTemp);

    for (i = 1; i < 20; i++) {
      snprintf(szTemp, sizeof(szTemp), " %u", digest_[i]);
      iter += strlen(report);
      snprintf(iter, sizeof(iter), "%s", szTemp);
    }
  } else {
    strncpy(report, "Error: Unknown report type!", sizeof(report));
  }
}
#endif

// Get the raw message digest_
void CSha1::GetHash(char *dest) {
  memcpy(dest, digest_, 20);
}
