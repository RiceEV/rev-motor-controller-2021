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
#include <stdbool.h>

//!!!!! Define below one at a time in order; Ground PE1 in between each test to recover the device and start at Security Level 0
//#define ENABLE_CODE_PROTECTION  // Define this to run code that enables PAC55XX Code Protection - Once enabled, it will always be enabled; Initially at Secure Level 0
//#define SECURITY_LEVEL_1        // Define this to go to security level 1; after a reset, certain pages will be inaccessible when a debugger is connected
#define SECURITY_LEVEL_2        // Define this to go to security level 2; after a reset, SWD will be disabled until PB-S2 is pushed or PE0 is grounded

//!!!!!! WARNING !!!!!! Defining SECURITY_LEVEL_3 will permanently disable SWD - no going back
//#define SECURITY_LEVEL_3        // !!!!! WARNING !!!!! - NO RETURN, Define this to go to security level 3; after a reset, SWD will be permanently disabled and there is no way to get it back


// Configure Masks to be written to code protection area in INFO3 FLASH for Security Level 1 or 2
uint32_t rmask[4] = {0xFFFFFFAA,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};  // First 8 pages, every other page is read protected
uint32_t wmask[4] = {0xFFFFAA00,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};  // First 8 pages write protected, next 8 every other page
uint8_t  imask = 0xFF;                                              // INFO Pages not protected


// The 32-bit SWDUNLOCK are stored as separate 32-bit values, which are assembled only at SWDUNLOCK write time
uint32_t swdunlock_key_ms = 0xFEED;     // Aribitray Value - Most significant 16-bits of SWDUNLOCK key for Security level 2
uint32_t swdunlock_key_ls = 0xCAFE;     // Aribitray Value - Least significant 16-bits of SWDUNLOCK key for Security level 2

void code_protect_gpio_init(void);
void enable_code_protection_app(void);
void security_level_1_app(void);
void security_level_2_app(void);
void security_level_3_app(void);
void verify_code_protection_masks(uint32_t *rmask, uint32_t *wmask, uint8_t *imask);

void code_protect_app(void)
{
	uint8_t security_level;

    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);

    code_protect_gpio_init();

    printf("\n===========================================================================\n");
    printf(  "===== Code Protection Example App =========================================\n");
    printf(  "===========================================================================\n");
    printf(  "Ground PE1 to Recover Device from Secuirty Level 1 or 2\n");
    printf(  "Device wll be mass erased and return to Security Level 0 after Reset\n\n");
    printf(  "Note:\n");
    printf(  "    1) Some EVKs have an optional S1 Push Button, use this to Ground E1\n");
    printf(  "    2) If recovery doesn't occur during test, Ground PE1 during power on\n");
    printf(  "    3) !!!!! WARNING !!!!! There is no recovery for Security Level 3\n");
    printf(  "===========================================================================\n");
    //===== Recovery =====
    // If PE1 is grounded, then perform Mass Program/Info Erase to get back to Security Level 0 (SL0)
    if (PAC55XX_GPIOE->IN.P1 == 0)
    {
        printf("\n===== PE1 Grounded, performing recovery, aborting test =====\n");
        printf(  "    Performing Mass Program/Info Erase Device...\n");
        printf("\n    ACTION REQUIRED => Reboot device now to Flash again\n");
        flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
    }

    // If Code Protection is not Enabled, then enable it; won't take affect until after a reset
    if (get_code_protect_status() == DISABLED)
    {
        printf("\nCode Protection Not Enabled\n");
    }
    else
    {
        printf("\nCode Protection Already Enabled\n");

        // Determine which security level we are in
        switch (PAC55XX_INFO3->SECLEVEL)
        {
            case 0xFF:
                security_level = 0;
                break;
            case 0xFE:
                security_level = 1;
                break;
            case 0xFC:
                security_level = 2;
                break;
            default:
                security_level = 2;
                break;
        }

        printf("Security Level is currently set to %d\n", security_level);
        printf("    SECLEVEL = 0x%X\n", PAC55XX_INFO3->SECLEVEL);
    }


#ifdef ENABLE_CODE_PROTECTION
    enable_code_protection_app();
#endif  // #ifdef ENABLE_CODE_PROTECTION

#ifdef SECURITY_LEVEL_1
    security_level_1_app();
#endif

#ifdef SECURITY_LEVEL_2
    security_level_2_app();
#endif

#ifdef SECURITY_LEVEL_3
    security_level_3_app();
#endif

    printf("    PF4 will be toggling");

    // As long as SWD is not permanently disabled, print Recovery message
    if (PAC55XX_INFO2->SWDFUSE != SWD_DISABLE_PERMANENTLY)
    {
        printf(" - Ground PE1 to recover via mass erase\n");
    }
    while(1) {
        // Toggle PF4
        PAC55XX_GPIOF->OUT.P4 ^= 1;

        // Delay after toggle
        for(volatile uint32_t i=0; i<1000000; i++);

        // If PE1 get's grounded, then perform recovery by doing a Mass Program/Info Erase
        if (PAC55XX_GPIOE->IN.P1 == 0)
        {
            printf("\n\n===== PE1 Grounded, performing recovery, aborting test =====\n");
            printf(    "    Performing Mass Program/Info Erase Device...\n");
            printf(  "\n    ACTION REQUIRED => Reboot device before Flashing again\n");
            flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
        }

    }

}

void enable_code_protection_app(void)
{
    printf("\n===== Enable Code Protection App =====\n");

    // Only need to enable code protection if it's not already enabled
    if(get_code_protect_status() == DISABLED)
    {
        // Erase INFO3 to make sure we start from clean code protection settings
        printf("    Erasing INFO3 to start fresh prior to enabling security...\n");
        flash_erase_key(FLASH_ERASE_INFO_3);                              // Erase INFO3

        printf("    Enabling Code Protection - SECEN byte will be written to non 0xFF\n");
        enable_code_protection();


        printf(  "    Code Protection Enabled\n");
        printf("\n    ACTION REQUIRED => Reboot now for settings to take effect\n\n");
    }


}


void security_level_1_app(void)
{

    printf("\n===== Security Level 1 App =====\n");

    // Only setup Security Level 1 if not already there
    if( (PAC55XX_INFO3->SECLEVEL == 0xFF) && (get_code_protect_status()==ENABLED) )
    {
        // Note: configuration below won't take effect until after a reset
        printf("    Setting to Security Level 1 with Read, Write, Info Masks...\n");

        // Set Securty Level to 1
        set_code_protection_security_level(1);

        // Write masks to INFO3; won't take effect until after a reset
        set_code_protection_masks(rmask, wmask, &imask);

        verify_code_protection_masks(rmask, wmask, &imask);

        printf("\n    Security Level 1 has been set\n");
        printf(  "    SECLEVEL = 0x%X\n", PAC55XX_INFO3->SECLEVEL);
        printf("\n    ACTION REQUIRED => Reboot now for settings to take effect\n\n");
    }
    else if (get_code_protect_status()==DISABLED)
    {
        printf("    Must Define ENABLE_CODE_PROTECTION to enable Code Protection before you can enter Security Level 1\n");
    }
    // After reboot, SECLEVEL should be set to 0xFE (security level 1) and settings of Masks will be in effect
    else if (PAC55XX_INFO3->SECLEVEL == 0xFE)
    {
        printf("    Device is in Security Level 1\n");
        printf("    SECLEVEL = 0x%X\n", PAC55XX_INFO3->SECLEVEL);
        verify_code_protection_masks(rmask, wmask, &imask);
        printf("\n    ACTION REQUIRED => Connect Debugger without Reseting, Erasing,or Downloading\n\n");
        printf(  "    View pages based on MASK settings to see effect\n");
        printf(  "    See .\\resources\\Code Protection Debugger Configuration.pdf to set debugger\n");
    }
    else
    {
        printf("    Device is in too high of a security level to set Security Level 1\n");
        printf("    Use Recovery mechanism to get back to Security Level 0\n");
    }

}


void security_level_2_app(void)
{
    printf("\n===== Security Level 2 App =====\n");

    // Only setup Security Level 2 if Security is Enabled, and we aren't already in Security Level 2
    if( (get_code_protect_status() == ENABLED) && ( (PAC55XX_INFO3->SECLEVEL == 0xFF) || (PAC55XX_INFO3->SECLEVEL == 0xFE) ) )
    {
        // Note: configuration below won't take effect until after a reset
        printf("    Setting to Security Level 2 with Read, Write, Info Masks...\n");

        // Set Securty Level to 1
        set_code_protection_security_level(2);

        // Write masks to INFO3; won't take effect until after a reset
        set_code_protection_masks(rmask, wmask, &imask);

        verify_code_protection_masks(rmask, wmask, &imask);

        printf(  "    Security Level 2 has been Set\n");
        printf(  "    SECLEVEL = 0x%X\n", PAC55XX_INFO3->SECLEVEL);
        printf("\n    ACTION REQUIRED => Reboot now for settings to take effect\n\n");
    }
    else if (get_code_protect_status()==DISABLED)
    {
        printf("    Must Define ENABLE_CODE_PROTECTION to enable Code Protection before you can enter Security Level 2\n");
    }
    // After reboot, device will be in Security Level 2 SECLEVEL = 0xFC
    else
    {
        printf(  "    Device is in Security Level 2\n");
        printf(  "    SECLEVEL = 0x%X\n", PAC55XX_INFO3->SECLEVEL);
        printf("\n    ACTION REQUIRED => Ground PE0 briefly to write Key to SWDUNLOCK\n");
        printf(  "                       for the first time.\n\n");
        printf(  "    Optional: Push S2 push button on some EVKs to ground PE0\n");

        // Wait for PE0 to be grounded before proceeding
        while(PAC55XX_GPIOE->IN.P0 != 0)
        {
                // Toggle PF4
                PAC55XX_GPIOF->OUT.P4 ^= 1;

                // Delay after toggle
                for(volatile uint32_t i=0; i<10000000; i++) 
                {                
                    // If PE0 becomes grounded, then exit
                    if (PAC55XX_GPIOE->IN.P0 == 0)
                    {
                        break;
                    }
                }

                // If PE1 get's grounded, then perform recovery by doing a Mass Program/Info Erase
                if (PAC55XX_GPIOE->IN.P1 == 0)
                {
                    printf("\n\n===== PE1 Grounded, performing recovery, aborting test =====\n");
                    printf(    "    Performing Mass Program/Info Erase Device...\n");
                    printf(  "\n    ACTION REQUIRED => Reboot device now to Flash again\n\n");
                    flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
                }
        }

        // Write SWDUNLOCK key for the first time, SWD will remain disabled
        printf("    Writing Key to SWDUNLOCK Reg for the first time\n");
        write_swdunlock_key(swdunlock_key_ms, swdunlock_key_ls);
        
        printf("    SWDUNLOCK Bit = %d\n", (*((uint32_t *)0x400D0150) & 0x4)>>2);

        verify_code_protection_masks(rmask, wmask, &imask);

        printf("\n===== SWD is currently disabled! =====\n");
        printf(  "    An SWD debugger can NOT be connected until the signal is given\n");
        printf("\n    ACTION REQUIRED => Ground PE0 to give signal to enable SWD\n\n");
        printf(  "    Optional: Push S2 push button on some EVKs to ground PE0\n");


        printf("    PF4 will be toggling slowly until the signal is given\n");
        for(volatile uint32_t i=0; i<5000000; i++);  // Short delay before sampling

        // Wait until PE0 is Grounded before proceeding
        while(PAC55XX_GPIOE->IN.P0 != 0)
        {
                // Toggle PF4
                PAC55XX_GPIOF->OUT.P4 ^= 1;

                // Delay after toggle
                for(volatile uint32_t i=0; i<5000000; i++)
                {                
                     // If PE0 becomes grounded, then exit
                     if (PAC55XX_GPIOE->IN.P0 == 0)
                     {
                         break;
                     }
                }
                // If PE1 get's grounded, then perform recovery by doing a Mass Program/Info Erase
                if (PAC55XX_GPIOE->IN.P1 == 0)
                {
                    printf("\n\n===== PE1 Grounded, performing recovery, aborting test =====\n");
                    printf("    Performing Mass Program/Info Erase Device...\n");
                    printf("    ACTION REQUIRED => Reboot device now to Flash again\n");
                    flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
                }
        }

        // Write SWDUNLOCK key for the second time, SWD will now be enabled, but Masks will be in effect
        printf("\n===== SWD being enabled; writing SWDUNLOCK Key a second time =====\n");
        write_swdunlock_key(swdunlock_key_ms, swdunlock_key_ls);

        printf("\n    ACTION REQUIRED => Connect Debugger without Reseting, Erasing,or Downloading\n\n");
        printf(  "    View pages based on MASK settings to see effect\n");
        printf(  "    See .\\resources\\Code Protection Debugger Configuration.pdf to set debugger\n");
    }

}

void security_level_3_app(void)
{
    int32_t i;

    printf("\n===== Security Level 3 App =====\n");
    // If alread in Security Level 3, then Exit
    if (PAC55XX_INFO2->SWDFUSE == SWD_DISABLE_PERMANENTLY)
    {
        printf("    Already at Security Level 3, SWD Permanently Disabled!\n");
        return;
    }

    printf(  "    !!!!! WARNING !!!!!!\n");
    printf(  "    SWD will be permanently Disabled; There is no recovery from Security Level 3\n");
    printf("\n    ACTION REQUIRED => Ground PE0 to proceed with Security Level 3\n");
    printf(  "                       Or, Ground PE1 to abort with a Mass Erase\n\n");
    printf(  "    Optional: Push S2 push button on some EVKs to ground PE0\n");
    printf(  "    Optional: Push S1 push button on some EVKs to ground PE1\n");

    // Wait until PE0 is Grounded before proceeding
    while(PAC55XX_GPIOE->IN.P0 != 0)
    {
            // Toggle PF4
            PAC55XX_GPIOF->OUT.P4 ^= 1;

            // Delay after toggle
            for(volatile uint32_t i=0; i<5000000; i++);

            // If PE1 get's grounded, then perform recovery by doing a Mass Program/Info Erase
            if (PAC55XX_GPIOE->IN.P1 == 0)
            {
                printf("\n\n===== PE1 Grounded, performing recovery, aborting test =====\n");
                printf("    Performing Mass Program/Info Erase Device...\n");
                printf("    ACTION REQUIRED => Reboot device now to Flash again\n");
                flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
            }
    }

    printf("===== Security Level 3 will be initiated ======\n");
    printf("    !!!!!! Ground PE1 within 10 seconds to abort (Push S1)\n   ");

    for(i=10; i >= 0; i--)
    {
        // If PE1 get's grounded, then perform recovery by doing a Mass Program/Info Erase
        if (PAC55XX_GPIOE->IN.P1 == 0)
        {
            printf("\n\n===== PE1 Grounded, performing recovery, aborting test =====\n");
            printf("    Performing Mass Program/Info Erase Device...\n");
            printf("\n    ACTION REQUIRED => Reboot device now to Flash again\n\n");
            flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);                              // Perform Mass Program and Info Flash Erase
        }

        pac_delay_asm(0x1800000);
        printf(" %d", i);

    }

    printf("\n\n    Setting to Security Level 3...\n");
    set_swd_disable_permanently();

    printf("\n    ACTION REQUIRED => Reboot device now for setting to take effect\n\n");
}

void verify_code_protection_masks(uint32_t *rmask, uint32_t *wmask, uint8_t *imask)
{
    uint32_t i;
    printf("\n    Security Mask Settings:\n");

    printf("        RMASK = ");
    for(i=0; i<4; i++)
    {
        printf("%x ", PAC55XX_INFO3->RMASK[i]);
        if (PAC55XX_INFO3->RMASK[i] != rmask[i])
        {
            printf("    !!!!!! rmask incorrect !!!!!\n");
            break;
        }

    }
    printf(" \n");

    printf("        WMASK = ");
    for(i=0; i<4; i++)
    {
        printf("%x ", PAC55XX_INFO3->WMASK[i]);
        if (PAC55XX_INFO3->WMASK[i] != wmask[i])
        {
            printf("    !!!!! wmask incorrect !!!!!\n");
            break;
        }
    }
    printf(" \n");

    printf("        IMASK = %x\n", PAC55XX_INFO3->IMASK);
    if (PAC55XX_INFO3->IMASK != *imask)
    {
        printf("    !!!!! imask incorrect !!!!!\n");
    }
}

void code_protect_gpio_init(void)
{
    // Setup PE0 as input
    PAC55XX_GPIOE->MODE.P0 = IO_HIGH_IMPEDENCE_INPUT;
    PAC55XX_SCC->PEMUXSEL.P0 = 0;
    PAC55XX_SCC->PEPUEN.P0 = 1;

    // Setup PE1 as input
    PAC55XX_GPIOE->MODE.P1 = IO_HIGH_IMPEDENCE_INPUT;
    PAC55XX_SCC->PEMUXSEL.P1 = 0;
    PAC55XX_SCC->PEPUEN.P1 = 1;

    // Set PF4 as output
    PAC55XX_GPIOF->MODE.P4 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOF->OUTMASK.P4 = 0;
    PAC55XX_GPIOF->DOCLEAR.P4 = 1;
    PAC55XX_GPIOF->DOSET.P4 = 1;
}
