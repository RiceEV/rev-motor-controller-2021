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

#include <stdbool.h>
#include "pac5xxx.h"
#include "pac5xxx_driver_system.h"
#include "code_protect_func.h"
#include "flash_func.h"

eCodeProtectStatus get_code_protect_status(void)
{
    eCodeProtectStatus cp_enabled = DISABLED;
    
    // If the SECEN Byte is still 0xFF, then code protection is not enabled
    if(PAC55XX_INFO2->SECEN != 0xFF)
    {
        cp_enabled = ENABLED;
    }
    
    return cp_enabled;
}

PAC5XXX_RAMFUNC void enable_code_protection(void)
{
 	// To enable code protection, write the SECEN Byte to something other than 0xFF
	PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_SECEN ;        // Enable Write Access to SECEN Byte

	PAC55XX_INFO2->SECEN = 0xAA ;

    // Make sure were not still busy writing and add delay before allowing read/fetch access to flash
    while(PAC55XX_MEMCTL->MEMSTATUS.WBUSY) { }  // wait for Flash Write WBUSY bit to be 0
    pac_delay_asm_ramfunc(320);                 // delay 20uS after WBUSY=0

	PAC55XX_MEMCTL->FLASHLOCK = 0 ;

}

void set_code_protection_security_level(uint32_t level)
{
    uint8_t level_byte;
    //uint8_t *p_seclevel = (uint8_t *)&(PAC55XX_INFO3->SECLEVEL);

    switch (level) {
        case 0:
            // Byte should already be 0xFF = Security Level 0
            // If it's not 0xFF, the only way back to level 0 is Mass Program and Info Erase
            break;

        case 1:
            //0xFE = Security Level 1
            level_byte = 0xFE;

            // Write the byte to INFO3 Sec Level to set the new level
            flash_write((uint8_t *)&(PAC55XX_INFO3->SECLEVEL), &level_byte, 1);
            break;

        case 2:
            //0xFC = Security Level 1
            level_byte = 0xFC;

            // Write the byte to INFO3 Sec Level to set the new level
            flash_write((uint8_t *)&(PAC55XX_INFO3->SECLEVEL), &level_byte, 1);
            break;

        case 3:
            set_swd_disable_permanently();
            break;
        default:
            break;
    }


}


void set_code_protection_masks(uint32_t *rmask, uint32_t *wmask, uint8_t *imask)
{

    flash_write((uint8_t *) &(PAC55XX_INFO3->WMASK), (uint8_t *)wmask, 16);
    flash_write((uint8_t *) &(PAC55XX_INFO3->RMASK), (uint8_t *)rmask, 16);
    flash_write((uint8_t *) &(PAC55XX_INFO3->IMASK), (uint8_t *)imask, 1);

}

PAC5XXX_RAMFUNC void write_swdunlock_key(uint32_t key_ms, uint32_t key_ls)
{
    // key_ms - key most significant 16 bits
    // key_ls - key least significant 16 bits
    // key_ms and key_ls are stored speparetly in the application so they are never stored together in memory
    // They are only combined together in ARM registers within this function
    
    // SWDUNLOCK must be written from code running in RAM
    //PAC55XX_MEMCTL->SWDUNLOCK = key = (key_ms << 16) + key_ls;
    write_swdunlock_key_ramfunc_asm(key_ms, key_ls);
    
    // Make sure were not still busy writing and add delay before allowing read/fetch access to flash
    while(PAC55XX_MEMCTL->MEMSTATUS.WBUSY) { }  // wait for Flash Write WBUSY bit to be 0
    pac_delay_asm_ramfunc(320);                 // delay 20uS after WBUSY=0
}

PAC5XXX_RAMFUNC void set_swd_disable_permanently(void)
{
    //===== Enable Write Access to SWD FUSE =====
    PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_SWDFUSE;

    // Write the SWDFUSE to permanently disable SWD
    PAC55XX_INFO2->SWDFUSE =SWD_DISABLE_PERMANENTLY;

    // Make sure were not still busy writing and add delay before allowing read/fetch access to flash
    while(PAC55XX_MEMCTL->MEMSTATUS.WBUSY) { }  // wait for Flash Write WBUSY bit to be 0
    pac_delay_asm_ramfunc(320);                 // delay 20uS after WBUSY=0
    
    PAC55XX_MEMCTL->FLASHLOCK = 0 ;
}


