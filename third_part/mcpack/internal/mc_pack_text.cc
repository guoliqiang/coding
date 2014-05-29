#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>
#include <errno.h>
#include "../public/mc_pack_text.h"
/*
 * BFS:
 *
 * PACK ::= OBJECT
 * OBJECT ::= {} | { MEMBERS }
 * MEMBERS ::= PAIR | PAIR, MEMBERS
 * PAIR ::= KEY:VALUE
 * KEY ::= STRING
 * VALUE ::= SIMPLE_VALUE | COMPOUND_VALUE
 * SIMPLE_VALUE ::= (SIMPLE_TYPE)BASIC_VALUE | BASIC_VALUE | EMPTY_RAW
 * COMPOUND_VALUE ::= (OBJ_TYPE)OBJECT | (ARR_TYPE)ARRAY | OBJECT | ARRAY
 * ARRAY ::= [] | [ ELEMENTS ]
 * ELEMENTS ::= VALUE | VALUE, ELEMENTS
 * SIMPLE_TYPE ::= int32 | uint32 | int64 | uint64 | str | raw | rawfile
 * ARR_TYPE ::= array
 * OBJ_YPE ::= object
 * BASIC_VALUE ::= STRING | NUMBER
 * EMPTY_RAW ::= (raw)
 * STRING ::= [^ \n\r\t:.,{}()[]]+ | ".*"
 * NUMBER ::= [+-]?[0-9]+
 *
 */

typedef enum _lex_t {
    LEX_INT32,		  /**< (int32)       */
    LEX_UINT32,		  /**< (uint32)       */
    LEX_INT64,		  /**< (int64)       */
    LEX_UINT64,		  /**< (uint64)       */
    LEX_STR,		  /**< (str)       */
    LEX_RAW,		  /**< (raw)       */
    LEX_RAWFILE,	  /**< (rawfile)       */
    LEX_ARRAY,		  /**< (array)       */
    LEX_OBJECT,		  /**< (object)       */
    LEX_LBRACE,		  /**< {       */
    LEX_RBRACE,		  /**< }       */
    LEX_LBRACKET,	  /**< [       */
    LEX_RBRACKET,	  /**< ]       */
    LEX_COMMA,		  /**< ,       */
    LEX_COLON,		  /**< :       */
    LEX_STRING,
    LEX_NUMBER,
    LEX_FLOAT,		  /**< (int32)       */
    LEX_DOUBLE,		  /**< (uint32)       */
} lex_t;

typedef struct _token_t {
    lex_t lex;		  /**< lex       */
    u_int vlen;		  /**< value string length       */
    const char *vstr;		  /**< value string       */
} token_t;

#define IS_RESERVED_CHAR(ch) (',' == ch || ':' == ch || '{' == ch || '}' == ch \
		|| '[' == ch || ']' == ch || '(' == ch)

#define PUT_LEX(_lex) do { \
	token_q[qlen].vstr = txt + pos; \
	token_q[qlen++].lex = _lex; \
	if (qlen > *qsiz) { \
		return MC_PE_NO_SPACE; \
	} \
} while (0)

static int parse_type(const char *s, u_int *off) {
    if (strncmp(s + *off, "(int32)", 7) == 0) {
        *off += 6;
        return LEX_INT32;
    } else if (strncmp(s + *off, "(uint32)", 8) == 0) {
        *off += 7;
        return LEX_UINT32;
    } else if (strncmp(s + *off, "(int64)", 7) == 0) {
        *off += 6;
        return LEX_INT64;
    } else if (strncmp(s + *off, "(uint64)", 8) == 0) {
        *off += 7;
        return LEX_UINT64;
    } else if (strncmp(s + *off, "(str)", 5) == 0) {
        *off += 4;
        return LEX_STR;
    } else if (strncmp(s + *off, "(raw)", 5) == 0) {
        *off += 4;
        return LEX_RAW;
    } else if (strncmp(s + *off, "(rawfile)", 9) == 0) {
        *off += 8;
        return LEX_RAWFILE;
    } else if (strncmp(s + *off, "(array)", 7) == 0) {
        *off += 6;
        return LEX_ARRAY;
    } else if (strncmp(s + *off, "(object)", 8) == 0) {
        *off += 7;
        return LEX_OBJECT;
    }
    return -1;
}

static int parse_value(const char *s, u_int *off, token_t *tok) {
    int slash_count=0;
    u_int cur = *off;
    if ('"' == s[*off]) {
        ++cur;
        while ((s[cur] != '"' && s[cur] != '\0')
                || ('"' == s[cur] && '\\' == s[cur - 1] && slash_count%2)) { // not \"
            if('\\' == s[cur]) {
                slash_count ++;
            } else {
                slash_count = 0;
            }
            ++cur;
        }
        if ('"' == s[cur]) {
            tok->lex = LEX_STRING;
            tok->vstr = s + *off + 1;
            tok->vlen = cur - *off - 1;
            *off += cur - *off; // pos at '"'
        } else {
            return -1;
        }
    } else {
        while (! (IS_RESERVED_CHAR(s[cur])) && s[cur] != '\0' && (!isspace(s[cur]))) {
            ++cur;
        }
        if (IS_RESERVED_CHAR(s[cur]) || '\0' == s[cur] || isspace(s[cur])) {
            char *end=NULL;
            strtod(s+*off,&end);
            if(end==(s+cur)) {
                tok->lex=LEX_NUMBER;
            } else {
                tok->lex=LEX_STRING;
            }
            tok->vstr=s+*off;
            tok->vlen=cur-*off;
            *off+=tok->vlen-1;
        } else {
            return -1;
        }
    }
    return 0;
}

/**
 * @brief lex parse
 * @param [in] txt   : const char*
 *	text
 * @param [out] token_q   : token_t*
 *	token queue
 * @param [in/out] qsiz   : int*
 *	in: length of token queue, out: length used
 * @return  int
 * @retval  errno
 *	- 0 : succeed
 *	- <0 : failed
**/
static int lex_parse(const char *txt, token_t *token_q, int *qsiz) {
    int ret = 0;
    int qlen = 0;
    u_int tlen = strlen(txt);
    u_int pos = 0;

    if (0 == tlen) {
        *qsiz = 0;
        return MC_PE_SUCCESS;
    }

    for (pos = 0; pos < tlen; ++pos) {
        while (pos < tlen && isspace(txt[pos])) { // ignore spaces
            ++pos;
        }

        if (pos >= tlen) {
            break;
        }

        switch (txt[pos]) {
        case '{':
            PUT_LEX(LEX_LBRACE);
            break;
        case '}':
            PUT_LEX(LEX_RBRACE);
            break;
        case '[':
            PUT_LEX(LEX_LBRACKET);
            break;
        case ']':
            PUT_LEX(LEX_RBRACKET);
            break;
        case ',':
            PUT_LEX(LEX_COMMA);
            break;
        case ':':
            PUT_LEX(LEX_COLON);
            break;
        case '(':
            ret = parse_type(txt, &pos);
            if (ret < 0) {
                token_q[qlen].vstr = txt + pos;
                *qsiz = qlen;
                return MC_PE_BAD_DATA;
            }
            PUT_LEX((lex_t)ret);
            break;
        default:
            ret = parse_value(txt, &pos, token_q + qlen);
            if (ret < 0) {
                token_q[qlen].vstr = txt + pos;
                *qsiz = qlen;
                return MC_PE_BAD_DATA;
            }
            ++qlen;
            if (qlen > *qsiz) {
                *qsiz = qlen;
                return MC_PE_NO_SPACE;
            }
        }
    }
    *qsiz = qlen;
    return MC_PE_SUCCESS;
}

#define CHK_RANGE(s, e) do { \
	if (s > e) { \
		return e; \
	} \
} while (0)

#define NOT_KEY(tok) (LEX_STRING != tok.lex && LEX_NUMBER != tok.lex)
#define IS_BASIC_VALUE(tok) (LEX_STRING == tok.lex || LEX_NUMBER == tok.lex)
#define IS_EMPTY_RAW(tok) (LEX_RAW == tok.lex)
#define IS_SIMPLE_TYPE(tok) (LEX_INT32 == tok.lex || LEX_UINT32 == tok.lex \
		|| LEX_INT64 == tok.lex || LEX_UINT64 == tok.lex || LEX_STR == tok.lex \
		|| LEX_RAW == tok.lex || LEX_RAWFILE == tok.lex || LEX_FLOAT == tok.lex \
		|| LEX_DOUBLE == tok.lex)
#define IS_OBJECT(toklst, off) ((LEX_OBJECT == toklst[off].lex \
		&& LEX_LBRACE == toklst[off + 1].lex) || (LEX_LBRACE == toklst[off].lex))
#define IS_ARRAY(toklst, off) ((LEX_ARRAY == toklst[off].lex \
		&& LEX_LBRACKET == toklst[off + 1].lex) || (LEX_LBRACKET == toklst[off].lex))

#define MAKE_NTSTR(kn, tok) do { \
	kn = (char *)alloca(tok.vlen + 1); \
	strncpy(kn, tok.vstr, tok.vlen); \
	kn[tok.vlen] = 0; \
} while (0)

/*
 * match_* func
 *	match token_q[s, e]
 * retval:
 *  >=0 : failed token pos
 *  -1 : succ
 */
#define MATCH_OK (-19831126)
static int match_pack(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_object(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_members(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_array(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_elements(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_pair(mc_pack_t *pack, token_t *token_q, int s, int e);
static int match_value(mc_pack_t *pack, token_t *token_q, int s, int e, int key);
static int match_compound_value(mc_pack_t *pack, token_t *token_q, int s, int e, int key);
static int match_simple_value(mc_pack_t *pack, token_t *token_q, int s, int e);

static inline char DHEX(char c) {
    if ('0' <= c && c <= '9') {
        return c - '0';
    }
    if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    }
    if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
    }
    return 0;
}
/*
 * get raw value from xdigit string
 * retval:
 *	-1 : fail
 *	>=0 : bytes count
 */
static int get_raw(const char *s, u_int len, char *rawbuf, u_int bufsiz) {
    u_int rcnt = 0; // result bytes count
    u_int hcnt = 0; // half bytes count
    u_int i = 0;
    char byte = 0;
    for (i = 0; i < len; ++i) {
        if (isspace(s[i])) {
            continue;
        }
        if (isxdigit(s[i])) {
            ++hcnt;
            if (hcnt % 2 == 1) {
                byte = DHEX(s[i]);
            } else {
                if (rcnt + 1 > bufsiz) {
                    return -1;
                }
                byte = (byte << 4) | DHEX(s[i]);
                rawbuf[rcnt++] = byte;
            }
        } else {
            return -1;
        }
    }
    return rcnt;
}

/*
 * get length of raw file
 * retval:
 *	-1 : fail
 *	>=0 : bytes count
 */
static int get_rawfile_len(const char *fn) {
    struct stat file_stat;
    if (stat(fn, &file_stat) == 0) {
        return file_stat.st_size;
    }
    return -1;
}

/*
 * escape \" \\ \/ \b \f \n \r \t in str
 * retval:
 *	>=0 : result length
 *	-1 : fail
 */
static int escape_str(const char *str, u_int slen, char *buf, u_int bsiz) {
    u_int i = 0;
    u_int used = 0;
    for (i = 0; i < slen; ++i) {
        if ('\\' == str[i]) {
            switch (str[i + 1]) {
            case '"':
                buf[used++] = '"';
                break;
            case '\\':
                buf[used++] = '\\';
                break;
            case '/':
                buf[used++] = '/';
                break;
            case 'b':
                buf[used++] = '\b';
                break;
            case 'f':
                buf[used++] = '\f';
                break;
            case 'n':
                buf[used++] = '\n';
                break;
            case 'r':
                buf[used++] = '\r';
                break;
            case 't':
                buf[used++] = '\t';
                break;
            case '\'':
                buf[used++] = '\'';
                break;
            default:
                return -1;
            }
            ++i;
            continue;
        }
        buf[used++] = str[i];
        if (used > bsiz) {
            return -1;
        }
    }
    buf[used] = 0;
    return used;
}

/*
 * put item into pack
 * retval:
 *	0 : succ
 *	-1 : fail
 */
static int
put_key_value(mc_pack_t *pack, token_t *token_q, int key, int value) {
    int ret = -1;
    int rawfd = -1;
    mc_int64_t sign = 0;
    mc_uint64_t unsign = 0;
    float fn;
    double dn;
    char *k = NULL;
    char *rawfn = NULL;
    char *rawbuf = NULL;
    u_int rawlen = 0;

    if (key >= 0) {
        MAKE_NTSTR(k, token_q[key]);
    }

    switch (token_q[value].lex) {
    case LEX_INT32:
        sign = strtol(token_q[value].vstr, NULL, 10);
        ret = mc_pack_put_int32(pack, k, sign);
        break;
    case LEX_UINT32:
        unsign = strtoul(token_q[value].vstr, NULL, 10);
        ret = mc_pack_put_uint32(pack, k, unsign);
        break;
    case LEX_INT64:
        sign = strtoll(token_q[value].vstr, NULL, 10);
        ret = mc_pack_put_int64(pack, k, sign);
        break;
    case LEX_UINT64:
        unsign = strtoull(token_q[value].vstr, NULL, 10);
        ret = mc_pack_put_uint64(pack, k, unsign);
        break;
    case LEX_FLOAT:
        fn = strtof(token_q[value].vstr, NULL);
        ret = mc_pack_put_float(pack, k, fn);
        break;
    case LEX_DOUBLE:
        dn = strtod(token_q[value].vstr, NULL);
        ret = mc_pack_put_double(pack, k, dn);
        break;
    case LEX_NUMBER:
        dn=strtod(token_q[value].vstr,NULL);
        ret=mc_pack_put_double(pack,k,dn);
        break;
    case LEX_STRING:
    case LEX_STR:
        rawlen = token_q[value].vlen + 1;
        if(rawlen<256){
            rawbuf = (char *)alloca(rawlen);
        }else{
            rawbuf=(char*)malloc(rawlen);
            if(rawbuf==0){
                return MC_PE_BAD_DATA;
            }
        }
        ret = escape_str(token_q[value].vstr, token_q[value].vlen, rawbuf, rawlen);
        if (ret >= 0) {
            ret=mc_pack_put_str(pack,k,rawbuf);
        }
        if(rawlen>=256){
            free(rawbuf);
        }
        break;
    case LEX_RAW:
        rawlen = token_q[value].vlen / 2;
        if(rawlen<256){
            rawbuf = (char *)alloca(rawlen);
        }else{
            rawbuf=(char*)malloc(rawlen);
            if(rawbuf==0){
                return MC_PE_BAD_DATA;
            }
        }
        ret = get_raw(token_q[value].vstr, token_q[value].vlen, rawbuf, rawlen);
        if (ret >= 0) {
            ret = mc_pack_put_raw(pack, k, rawbuf, ret);
        }
        if(rawlen>=256){
            free(rawbuf);
        }
        break;
    case LEX_RAWFILE:
        MAKE_NTSTR(rawfn, token_q[value]);
        rawlen = get_rawfile_len(rawfn);
        if (rawlen <= 0) {
            return MC_PE_BAD_DATA;
        }
        rawfd = open(rawfn, O_RDONLY);
        if (rawfd < 0) {
            return MC_PE_BAD_DATA;
        }
        rawbuf = (char *)mmap(0, rawlen, PROT_READ, MAP_SHARED, rawfd, 0);
        if (rawbuf != MAP_FAILED) {
            ret = mc_pack_put_raw(pack, k, rawbuf, rawlen);
            munmap(rawbuf, rawlen);
        } else {
            return MC_PE_BAD_DATA;
        }
        break;
    default:
        return MC_PE_BAD_DATA;
    }
    return ret;
}

/*
 * find comma after a value
 * retval:
 *	-1 : not found
 *	>=0 : pos
 */
static int find_comma(token_t *token_q, int s, int e) {
    int brace = 0;
    int i = 0;
    for (i = s; i <= e; ++i) {
        switch (token_q[i].lex) {
        case LEX_LBRACE:
        case LEX_LBRACKET:
            ++brace;
            break;
        case LEX_RBRACE:
        case LEX_RBRACKET:
            --brace;
            break;
        case LEX_COMMA:
            if (0 == brace) {
                return i;
            }
            break;
        default:
            continue;
        }
    }
    return -1;
}

static int match_simple_value(mc_pack_t * pack, token_t *token_q, int s, int e) {
    pack = 0;
    /*
     * will return lex index of value
     * will modify lex to type of value
     * retval:
     *	<0 : fail
     *	>=0 : lex index
     */
    switch (e - s) {
    case 0:
        // BASIC_VALUE
        if (IS_BASIC_VALUE(token_q[s])) {
            //不要修改成为这个...
            if(token_q[s].lex!=LEX_NUMBER) {
                token_q[s].lex = LEX_STR;
            }
            return e;
        }
        // EMPTY_RAW
        if (IS_EMPTY_RAW(token_q[s])) {
            token_q[e].lex = LEX_RAW;
            token_q[e].vlen = 0;
            return e;
        }
        break;
    case 1:
        // (SIMPLE_TYPE)BASIC_VALUE
        if (IS_SIMPLE_TYPE(token_q[s]) && IS_BASIC_VALUE(token_q[e])) {
            token_q[e].lex = token_q[s].lex;
            return e;
        }
        break;
    default:
        return -1;
    }
    return -1;
}

#define PACK_PUT_OBJ(_obj, _pck, _key) do { \
	_obj = mc_pack_put_object(_pck, _key); \
	if (MC_PACK_PTR_ERR(_obj)) { \
		return MC_PACK_PTR_ERR(_obj); \
	} \
} while (0)

#define PACK_PUT_ARR(_arr, _pck, _key) do { \
	_arr = mc_pack_put_array(_pck, _key); \
	if (MC_PACK_PTR_ERR(_arr)) { \
		return MC_PACK_PTR_ERR(_arr); \
	} \
} while (0)

static int match_compound_value(mc_pack_t *pack, token_t *token_q, int s, int e, int key) {
    mc_pack_t *sub_pack = NULL;
    char *name = NULL;
    int ret=0;
    CHK_RANGE(s, e);
    if (key >= 0) {
        MAKE_NTSTR(name, token_q[key]);
    }
    // (OBJ_TYPE)OBJECT | OBJECT
    if (IS_OBJECT(token_q, s)) {
        if (key < 0) { // no key
            PACK_PUT_OBJ(sub_pack, pack, NULL);
        } else { // have key
            PACK_PUT_OBJ(sub_pack, pack, name);
        }
        if (LEX_OBJECT == token_q[s].lex) { // (OBJ_TYPE)OBJECT
            ret=match_object(sub_pack, token_q, s + 1, e);
        } else { // OBJECT
            ret=match_object(sub_pack, token_q, s, e);
        }
        mc_pack_finish(sub_pack);
        return ret;
        // (ARR_TYPE)ARRAY | ARRAY
    } else if (IS_ARRAY(token_q, s)) {
        if (key < 0) { // no key
            PACK_PUT_ARR(sub_pack, pack, NULL);
        } else { // have key
            PACK_PUT_ARR(sub_pack, pack, name);
        }
        if (LEX_ARRAY == token_q[s].lex) { // (ARR_TYPE)ARRAY
            ret=match_array(sub_pack, token_q, s + 1, e);
        } else { // ARRAY
            ret=match_array(sub_pack, token_q, s, e);
        }
        mc_pack_finish(sub_pack);
        return ret;
    }
    return s;
}

static int match_value(mc_pack_t *pack, token_t *token_q, int s, int e, int key) {
    int value = 0;
    int ret = 0;
    CHK_RANGE(s, e);
    // SIMPLE_VALUE
    value = match_simple_value(pack, token_q, s, e);
    if (value >= 0) {
        if (key >= 0) {
            ret = put_key_value(pack, token_q, key, value);
        } else {
            ret = put_key_value(pack, token_q, key, value);
        }
        if (MC_PE_SUCCESS == ret) {
            return MATCH_OK;
        }
        return ret;
    }
    // COMPOUND_VALUE
    return match_compound_value(pack, token_q, s, e, key);
}

static int match_pair(mc_pack_t *pack, token_t *token_q, int s, int e) {
    CHK_RANGE(s, e);
    // KEY:VALUE
    if (NOT_KEY(token_q[s])) {
        return s;
    }
    if (LEX_COLON != token_q[s + 1].lex) {
        return s + 1;
    }
    return match_value(pack, token_q, s + 2, e, s);
}

static int match_elements(mc_pack_t *pack, token_t *token_q, int s, int e) {
    int pos = 0;
    int ret = 0;
    CHK_RANGE(s, e);
    pos = find_comma(token_q, s, e);
    if (pos >= 0) { // VALUE, ELEMENTS
        ret = match_value(pack, token_q, s, pos - 1, -1);
        if (ret != MATCH_OK) {
            return ret;
        }
        ret = match_elements(pack, token_q, pos + 1, e);
        return ret;
    }
    // VALUE
    return match_value(pack, token_q, s, e, -1);
}

static int match_array(mc_pack_t *pack, token_t *token_q, int s, int e) {
    CHK_RANGE(s, e);
    if (LEX_LBRACKET != token_q[s].lex) {
        return s;
    }
    if (LEX_RBRACKET != token_q[e].lex) {
        return e;
    }
    if (s + 1 == e) { // []
        return MATCH_OK;
    }
    // [ELEMENTS]
    return match_elements(pack, token_q, s + 1, e - 1);
}

static int match_members(mc_pack_t *pack, token_t *token_q, int s, int e) {
    int pos = 0;
    int ret = 0;
    CHK_RANGE(s, e);
    pos = find_comma(token_q, s, e);
    if (pos >= 0) { // PAIR, MEMBERS
        ret = match_pair(pack, token_q, s, pos - 1);
        if (ret != MATCH_OK) {
            return ret;
        }
        ret = match_members(pack, token_q, pos + 1, e);
        return ret;
    }
    // PAIR
    return match_pair(pack, token_q, s, e);
}

static int match_object(mc_pack_t *pack, token_t *token_q, int s, int e) {
    CHK_RANGE(s, e);
    if (LEX_LBRACE != token_q[s].lex) {
      return s;
    }
    if (LEX_RBRACE != token_q[e].lex) {
      return e;
    }
    if (s + 1 == e) { // {}
      return MATCH_OK;
    }
    // {MEMBERS}
    return match_members(pack, token_q, s + 1, e - 1);
}

static int match_pack(mc_pack_t *pack, token_t *token_q, int s, int e) {
  return match_object(pack, token_q, s, e);
}

int mc_pack_text2pack(const char *txt, mc_pack_t *pack) {
    const int TOKEN_NUM_MAX = 40960;
    // sizeof(token_t) * 40960 = 655360 = 650k
    // if this isn't enough, you should split
    // txt to several txts
    token_t token_q[TOKEN_NUM_MAX];
    int tok_cnt = TOKEN_NUM_MAX;
    int ret = 0;
    if (NULL == txt || NULL == pack) {
      return MC_PE_BAD_PARAM;
    }
    ret = lex_parse(txt, token_q, &tok_cnt);
    //守护情况zhangyan04@baidu.com
    if(tok_cnt==0){
      return MC_PE_SUCCESS;
    }
    if (MC_PE_BAD_DATA == ret) {
      return (int)(token_q[tok_cnt].vstr - txt + 1);
    } else if (ret < 0) {
      return ret;
    }
    ret = match_pack(pack, token_q, 0, tok_cnt - 1);
    if (MATCH_OK == ret) {
      return MC_PE_SUCCESS;
    }
    if (ret >= 0) {
      return (int)(token_q[ret].vstr - txt + 1);
    }
    return ret;
}

static const char * get_subkey(const char *key) {
    int len = strlen(key);
    int pos;
    for (pos = len - 1; pos >= 0; --pos) {
        if (MC_PACK_OBJDELIM == key[pos] || MC_PACK_ARRDELIM == key[pos]) {
            break;
        }
    }
    return key + pos + 1;
}

#define BUF_PRINT(_buf, _siz, _used, _fmt, _arg...) do { \
	if (used >= _siz) { \
		return used; \
	} \
	_used += snprintf(_buf + _used, _siz - _used, _fmt, ##_arg); \
} while (0)

#define NEED_ESCAPE(ch) ('"' == ch || '\n' == ch || '\\' == ch \
		|| '/' == ch || '\b' == ch || '\f' == ch || '\r' == ch || '\t' == ch)

static int sprint_str(char *buf, int size, const char *str) {
    int used = 0;
    BUF_PRINT(buf, size, used, "%c", '"');
    while (*str) {
        if (NEED_ESCAPE(*str)) {
            BUF_PRINT(buf, size, used, "%c", '\\');
          switch (*str) {
            case '\n':
                BUF_PRINT(buf, size, used, "%c", 'n');
                break;
            case '\b':
                BUF_PRINT(buf, size, used, "%c", 'b');
                break;
            case '\f':
                BUF_PRINT(buf, size, used, "%c", 'f');
                break;
            case '\r':
                BUF_PRINT(buf, size, used, "%c", 'r');
                break;
            case '\t':
                BUF_PRINT(buf, size, used, "%c", 't');
                break;
            default:
                BUF_PRINT(buf, size, used, "%c", *str);
            }
        } else {
            BUF_PRINT(buf, size, used, "%c", *str);
        }
        ++str;
    }
    BUF_PRINT(buf, size, used, "%c", '"');
    return used;
}

static int print_item(const mc_pack_item_t *pitem, char *buf, int size, int is_arr, int json) {
    int used = 0;
    int i, bytes;
  switch (pitem->type) {
    case MC_IT_TXT:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        used += sprint_str(buf + used, size - used, pitem->value);
        break;
    case MC_IT_I32:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        if (json) {
            BUF_PRINT(buf, size, used, "%d", *(int32_t *)pitem->value);
        } else {
            BUF_PRINT(buf, size, used, "(int32)%d", *(int32_t *)pitem->value);
        }
        break;
    case MC_IT_U32:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        if (json) {
            BUF_PRINT(buf, size, used, "%u", *(u_int32_t *)pitem->value);
        } else {
            BUF_PRINT(buf, size, used, "(uint32)%u", *(u_int32_t *)pitem->value);
        }
        break;
    case MC_IT_I64:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        if (json) {
            BUF_PRINT(buf, size, used, "%lld", *(long long *)pitem->value);
        } else {
            BUF_PRINT(buf, size, used, "(int64)%lld", *(long long *)pitem->value);
        }
        break;
    case MC_IT_U64:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        if (json) {
            BUF_PRINT(buf, size, used, "%llu", *(unsigned long long *)pitem->value);
        } else {
            BUF_PRINT(buf, size, used, "(uint64)%llu", *(unsigned long long *)pitem->value);
        }
        break;
    case MC_IT_BIN:
        if (json) {
            return 0;
        }
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        bytes = (int)pitem->value_size;
        BUF_PRINT(buf, size, used, "(raw)%s", "");
        for (i = 0; i < bytes; ++i) {
            BUF_PRINT(buf, size, used, "%02hhX", pitem->value[i]);
        }
        break;
    case MC_IT_FLOAT:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        //恶心的处理.因为mcpack1不能够正常读取(float)...
        //当初要是把(float)处理了去就没有这么多破事情了..
        //而且事实上代码框架里面也写好了处理float类型的代码
        //但是就是没有写处理float值的代码...aaaaa
        BUF_PRINT(buf,size,used,"%f",*(float*)pitem->value);
        break;
    case MC_IT_DOUBLE:
        if (! is_arr) {
            BUF_PRINT(buf, size, used, "\"%s\":", get_subkey(pitem->key));
        }
        BUF_PRINT(buf,size,used,"%lf",*(double*)pitem->value);
        break;
    default:
        return 0;
    }
    return used;
}

static int output_pack(const mc_pack_t *pack, char *str, int size, int is_arr, int json) {
    mc_pack_item_t itm;
    int ret = 0;
    int is_first = 1;
    int used = 0;
    int result;
    const mc_pack_t *p;
    if (0 == size) {
        return 0;
    }
    ret = mc_pack_first_item(pack, &itm);
    while (MC_PE_SUCCESS == ret) {
        if (! is_first) {
            BUF_PRINT(str, size, used, ", ");
        }
        if (MC_PT_OBJ == itm.type) {
            if (! is_arr) {
                BUF_PRINT(str, size, used, "\"%s\":", get_subkey(itm.key));
            }
            BUF_PRINT(str, size, used, "{");
            result = mc_pack_get_pack_from_item(&itm, &p);
            if(result < 0)
                return size;
            used += output_pack(p, str + used, size - used, 0, json);
            BUF_PRINT(str, size, used, "}");
            //需要显示释放...
            mc_pack_finish(p);
        } else if (MC_PT_ARR == itm.type) {
            if (! is_arr) {
                BUF_PRINT(str, size, used, "\"%s\":", get_subkey(itm.key));
            }
            BUF_PRINT(str, size, used, "[");
            result = mc_pack_get_array_from_item(&itm, &p);
            if(result < 0)
                return size;
            used += output_pack(p, str + used, size - used, 1, json);
            BUF_PRINT(str, size, used, "]");
            //需要显示释放...
            mc_pack_finish(p);
        } else {
            used += print_item(&itm, str + used, size - used, is_arr, json);
        }
        if (used >= size) {
            return used;
        }
        ret = mc_pack_next_item(&itm, &itm);
        is_first = 0;
    }
    return used;
}

int mc_pack_pack2text(const mc_pack_t *pack, char *txt_buf, u_int buf_size, int json) {
    u_int used = 0;
    if (NULL == pack || NULL == txt_buf) {
        return MC_PE_BAD_PARAM;
    }
    txt_buf[0] = 0;
    used += snprintf(txt_buf + used, buf_size - used, "{");
    if (used < buf_size) {
        used += output_pack(pack, txt_buf + used, buf_size - used, 0, json);
        if (used < buf_size) {
            used += snprintf(txt_buf + used, buf_size - used, "}");
        }
    }
    return (used < buf_size ? MC_PE_SUCCESS : MC_PE_NO_SPACE);
}
