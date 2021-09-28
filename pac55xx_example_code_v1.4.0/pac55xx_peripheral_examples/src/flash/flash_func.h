//=============================================================================
// Copyright (C) 2018-2019, Qorvo, Inc.
//
// THIS SOFTWARE IS SUBJECT TO A SOURCE CODE LICENSE AGREEMENT WHICH PROVIDES,
// AMONG OTHER THINGS:  (i) THAT IT CAN BE USED ONLY TO ADAPT THE LICENSEE'S
// APPLICATION TO PAC PROCESSORS SUPPLIED BY QORVO, INC.;
// (ii) THAT  IT IS PROVIDED "AS IS" WITHOUT WARRANTY;  (iii) THAT
// QORVO, INC. IS NOT LIABLE FOR ANY INDIRECT DAMAGES OR FOR DIRECT
// DAMAGES EXCEEDING US$1,500;  AND (iv) THAT IT CAN BE DISCLOSED TO AND USED
// ONLY BY CERTAIN AUTHORIZED PERSONS.
//
//=============================================================================

#include "app.h"

#ifndef FLASH_FUNC_H
#define FLASH_FUNC_H

extern PAC5XXX_RAMFUNC void flash_erase_page(uint32_t page_num);
extern PAC5XXX_RAMFUNC void flash_erase_key(uint32_t key);
extern PAC5XXX_RAMFUNC void flash_write(uint8_t *p_dest, uint8_t *p_src, uint32_t size_bytes);
extern PAC5XXX_RAMFUNC void flash_write_word(uint32_t * p_dest, uint32_t value);
extern PAC5XXX_RAMFUNC uint32_t flash_write_16byte_aligned(uint32_t *p_dest, uint32_t *p_src, uint32_t size_32bit_words);

void print_mem_format_hex_32bit(uint8_t * p_mem, uint32_t size_bytes);

extern PAC5XXX_RAMFUNC uint32_t write_flash(uint32_t dest_addr, uint32_t num_words, uint32_t *src);
#endif
