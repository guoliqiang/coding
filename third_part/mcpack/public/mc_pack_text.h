/***************************************************************************
 *
 * Copyright (c) 2008 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/



/**
 * @file mc_pack_text.h
 * @author zhouzr(zhouzerui@baidu.com)
 * @brief pack text description
 *
 **/


#ifndef  __MC_PACK_TEXT_H_
#define  __MC_PACK_TEXT_H_

#include "mc_pack_c.h"

/**
 * @brief convert text to pack
 * 仅用于debug用途。否则请使用mc_pack_pack2json
 *
 * @param [in] txt   : const char*
 * @param [in] pack   : mc_pack_t*
 * @return  int
 * @retval   errno
 *	- 0 : succeed
 *	- <0 : failed
 *	- >0 : position of parse failure, start from 1
 * @see
 * @note
 * @author zhouzr
 * @date 2008/03/11 18:26:00
 **/
int mc_pack_text2pack(const char *txt, mc_pack_t *pack);

/**
 * @brief convert pack to text
 * 仅用于debug用途。否则请使用mc_pack_pack2json
 *
 * @param [in] pack   : const mc_pack_t*
 * @param [out] txt_buf   : char*
 * @param [in] buf_size   : u_int
 * @param [in] json   : u_int
 *		- 0 : pack desc text
 *		- 1 : json (not support raw, lost type info)
 * @return  int
 * @retval   errno
 *	- 0 : succeed
 *	- <0 : failed
 * @see
 * @note
 * @author zhouzr
 * @date 2008/03/11 18:27:22
 **/
int mc_pack_pack2text(const mc_pack_t *pack, char *txt_buf, u_int buf_size, int json);

#endif  //__MC_PACK_TEXT_H_
