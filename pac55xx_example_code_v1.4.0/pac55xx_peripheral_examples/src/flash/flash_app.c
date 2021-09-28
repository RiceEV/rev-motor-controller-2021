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

#define BUFF_SIZE_BYTES 64
uint8_t data_buffer[BUFF_SIZE_BYTES];
#define BUFF_SIZE_WORDS 48
uint32_t word_buffer[BUFF_SIZE_WORDS];

//===============================================================================================================================
// PAC55XX devices have 128k bytes of Flash, which can be used to store the FW and user data
// Also there are 3 banks of INFO FLash
//      INFO1 and  INFO2 are reserved, but INFO3 can be used to store data separate from the 128k bytes
// This app shows how to operate, write, and read the main Flash and INFO3
//      Flash data can be accessed directly, or by using the macro READ_UINT32(address)
//      To write data to an address,
//          first the page needs to be erased, unless it has already been erased
//          then, Flash data must be written 16 bytes (128-bits) at a time.
//===============================================================================================================================

void flash_app(void)
{
    uint32_t i=0;
    uint32_t data_value;
    int32_t retval;

    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    // Fill byte data buffer with incrementing values
    for (i=0; i < BUFF_SIZE_BYTES; i++)
    {
        data_buffer[i] = i;
    }

    // Fill Word buffer with incrementing values
    for (i=0; i < BUFF_SIZE_WORDS; i++)
    {
        word_buffer[i] = i;
    }

    //=================================================
    // Test various Flash Erase and Write functions
    //=================================================

    // Erase a page and write a buffer of data
    printf("\nErase a page and write a buffer of data to addr 0x0001E000\n");
    printf("Erase page 120\n");
    flash_erase_page(120);
    printf("Write page 120 with data_buffer\n");
    flash_write((uint8_t *)0x0001E000, data_buffer, BUFF_SIZE_BYTES);           // Destination, Source, size_bytes
    print_mem_format_hex_32bit((uint8_t *)0x0001E000, BUFF_SIZE_BYTES + 16);

    // Erase page and write buffer not aligned to boundaries
    printf("\nErase page, write odd byte buffer to non 16byte algined address 0x0001E403\n");
    printf("Erase page 121\n");
    flash_erase_page(121);
    printf("Write page 121 with misaligned address and odd number of bytes\n");
    flash_write((uint8_t *)0x0001E403, data_buffer, BUFF_SIZE_BYTES-1);         // Destination, Source, size_bytes
    print_mem_format_hex_32bit((uint8_t *)0x0001E403, BUFF_SIZE_BYTES + 16);

    // Write single 32-bit variable to Flash using flash_write()
    printf("\nWrite a single 32-bit variable to 0x0001E514 using flash_write()\n");
    data_value = 0x44332211;
    flash_write((uint8_t *)0x0001E514, (uint8_t *)&data_value, 4);              // Destination, Source, size_bytes
    print_mem_format_hex_32bit((uint8_t *)0x0001E510, 32);

    // Write single 32-bit value Flash using flash_write_word()
    printf("\nWrite a single 32-bit value to 0x0001E528 using flash_write_word()\n");
    flash_write_word((uint32_t *)0x0001E528, 0x88776655);                       // Destination, Data Value
    print_mem_format_hex_32bit((uint8_t *)0x0001E510, 32);

    // Write a buffer using flash_write_16byte_aligned()
    // The buffer to write must be aligned to a 32-bit boundary
    // The Flash location to write must be aligned to a 16 byte boundary
    // The number of words must be a multiple of 4
    printf("\nWrite buffer 16 byte aligned & multiple of 16 bytes using flash_write_word()\n");
    retval = flash_write_16byte_aligned((uint32_t *)0x0001E600, word_buffer, BUFF_SIZE_WORDS);           // Destination, Source, Size Words
    if(retval != PAC5XXX_OK)
    {
        printf("Error in flash_write_16byte_aligned()\n");
    }
    print_mem_format_hex_32bit((uint8_t *)0x0001E5F0, (BUFF_SIZE_WORDS * 4) + 32);

    //Erase INFO3 and Write a buffer
    printf("\nErase INFO3 Flash and Write a buffer of data to addr 0x00100800\n");
    flash_erase_key(FLASH_ERASE_INFO_3);
    flash_write((uint8_t *)PAC55XX_INFO3_FLASH_BASE, data_buffer, BUFF_SIZE_BYTES);           // Destination, Source, size_bytes
    print_mem_format_hex_32bit((uint8_t *)PAC55XX_INFO3_FLASH_BASE, BUFF_SIZE_BYTES + 16);


    // These two function calls will erase all the Program Flash and so this example code
    // will not continue to run after the call.  So these functions remain commented out.
    //flash_erase_key(FLASH_START_MASS_PAGE_ERASE);         // Mass erase of Program Flash pages 0 to 127
    //flash_erase_key(FLASH_START_MASS_PROG_INFO_ERASE);    // Mass erase Program Flash and Info3 Flash

}

void print_mem_format_hex_32bit(uint8_t * p_mem, uint32_t size_bytes)
{
    uint32_t * p_mem32;
    uint32_t total_words;
    uint32_t i;
    uint32_t addr_label;

    // Get 32-bit aligned address
    p_mem32 = (uint32_t *)((uint32_t)p_mem & ~3);
    addr_label = (uint32_t)p_mem32 % 16;

    // Calculate total bytes words to print
    //  total   = size bytes  +    alignment bytes  +  pad bytes (ensures last complete word);  then shift to obtain words
    total_words = (size_bytes + ((uint32_t)p_mem % 4) + 3) >> 2;

    // Print memory contents to uart
    printf("\nMemory Listing (Word Format) Addr: 0x%08X Size = %d Bytes\n", p_mem, size_bytes);
    printf("Offset:                    %X          %X          %X          %X",
            addr_label, (addr_label+4)%16, (addr_label+8)%16, (addr_label+12)%16);
    // Print 4 words per line with address at beginning
    for(i = 0; i < total_words; i++)
    {
        // Print Address before each set of 4 words
        if( (i & 3) == 0 )
        {
            printf("\nAddr 0x%08x:  ", p_mem32);
            printf("0x%08x ", *p_mem32);
        }
        else
        {
            printf("0x%08x ", *p_mem32);
        }

        p_mem32++;
    }
    printf("\n\n");

}
