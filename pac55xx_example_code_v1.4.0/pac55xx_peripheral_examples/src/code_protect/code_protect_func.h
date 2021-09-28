//=============================================================================
// Copyright (C) 2019, Qorvo, Inc.
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
 
#include "pac5xxx.h"

#ifndef CODE_PROTECT_FUNC_H
#define CODE_PROTECT_FUNC_H
#include <stdbool.h>

// Code Protect Status Enum
typedef enum {
    DISABLED = 0,       // Code Protection is Disabled; SECEN = 0xFF
    ENABLED = 1         // Code Protection is Enabled; SECEN != 0xFF
} eCodeProtectStatus;


eCodeProtectStatus get_code_protect_status(void);
PAC5XXX_RAMFUNC void enable_code_protection(void);
void set_code_protection_security_level(uint32_t level);
void set_code_protection_masks(uint32_t *rmask, uint32_t *wmask, uint8_t *imask);
PAC5XXX_RAMFUNC void write_swdunlock_key(uint32_t key_ms, uint32_t key_ls);
PAC5XXX_RAMFUNC void write_swdunlock_key_ramfunc_asm(uint32_t key_ms, uint32_t key_ls);
PAC5XXX_RAMFUNC void set_swd_disable_permanently(void);

#endif  // CODE_PROTECT_FUNC_H






