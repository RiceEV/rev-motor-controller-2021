//=============================================================================
///
/// @note
/// Copyright (C) 2019, Qorvo, Inc.
///
/// THIS SOFTWARE IS SUBJECT TO A SOURCE CODE LICENSE AGREEMENT WHICH PROVIDES,
/// AMONG OTHER THINGS:  (i) THAT IT CAN BE USED ONLY TO ADAPT THE LICENSEE'S
/// APPLICATION TO PAC PROCESSORS SUPPLIED BY QORVO, INC.@
/// (ii) THAT  IT IS PROVIDED "AS IS" WITHOUT WARRANTY@  (iii) THAT
/// QORVO, INC. IS NOT LIABLE FOR ANY INDIRECT DAMAGES OR FOR DIRECT
/// DAMAGES EXCEEDING US$1,500@  AND (iv) THAT IT CAN BE DISCLOSED TO AND USED
/// ONLY BY CERTAIN AUTHORIZED PERSONS.
///
//=============================================================================

#define MEMCTL_SWDUNLOCK_ADDR 0x400D0010

//===========================================================================================================
//  Function:  write_swdunlock_key_ramfunc_asm()
///
/// @brief  This function combines the 2 parts of the SWDUNLOCK key and writes the key to SWDUNLOCK
///         The key is cleared from the register before returning
///
/// @param  r0 = key_ms - most significant 16 bits of the key
/// @param  r1 = key_ls - least significant 16 bits of the key
///
/// @return None
///
//============================================================================================================


//===== IAR Assembly ===== 
// process below if  IAR
#if defined(__IAR_SYSTEMS_ASM__)    //__IAR_SYSTEMS_ASM__ = IAR Assembler

        #define SHT_PROGBITS 0x1
        #define SHF_WRITE 0x1
        #define SHF_EXECINSTR 0x4
        SECTION .text:CODE:REORDER:NOROOT(2)
        SECTION_TYPE SHT_PROGBITS, SHF_WRITE | SHF_EXECINSTR
        THUMB

        PUBLIC  write_swdunlock_key_ramfunc_asm
        
write_swdunlock_key_ramfunc_asm:
        // Combine key_ms and key_ls
        lsls  r0,r0,#16                     // Shift key_ms into upper 16 bits;  key_ms is most significant 16 bits of key
        add   r0,r0,r1                      // Add key_ls to previous result;  key_ls is least significant 16 bits of key

        // Store key to SWDUNLOCK register
        ldr   r1, =MEMCTL_SWDUNLOCK_ADDR

        str   r0,[r1]

        // Clear key from r1
        movs  r0,#0

        // Return to calling function
        bx    lr

        END
        
#endif // #if defined(__IAR_SYSTEMS_ASM__)




//===== Keil Assembly ===== 
// process below if not GCC and not IAR
#if !defined(__ASSEMBLER__) && !defined(__IAR_SYSTEMS_ASM__)        //__ASSEMBLER__ = GCC assembler  __IAR_SYSTEMS_ASM__ = IAR Assembler

// For Keil, this file must be preprocessed by the ARM C Compiler to process #if directives.
// The use of --cpreproc by the assembler will only resolve #defines and not #if/#ifdef.  Therefore, it must be run through armcc first.
// The file should be configured as a custom file in the project so it will be preprocessed by the C compiler to create preproc_{filename}.s.
// The custom arguments below are then used to run the C preprocessor
// Custom Arguments: $K\ARM\ARMCC\bin\armcc -c --cpu Cortex-M4.fp -E  #F -I ..\src -o "$Ppreproc_%F"
//                                      |    |   |                 |   |  |    |    |    |
//                                      |    |   |                 |   |  |    |    |    +---- Output Filename = $P=ProjectPath + "preproc_" + %F=Original Filename  => preproc_swd_unlock.s
//                                      |    |   |                 |   |  |    |    +---- Specify Output filename
//                                      |    |   |                 |   |  |    +---- Include Path (Optional)
//                                      |    |   |                 |   |  +---- Add Include Path (Optional)
//                                      |    |   |                 |   +---- Input File to be preprocessed
//                                      |    |   |                 +---- Only Preprocess the file
//                                      |    |   +---- Specify the CPU type
//                                      |    +---- Treat as a C file
//                                      +---- ARM C Compiler

        PRESERVE8
        THUMB
        AREA    |.data|, CODE, READWRITE
        EXPORT  write_swdunlock_key_ramfunc_asm
        ALIGN   2

write_swdunlock_key_ramfunc_asm

        // Combine key_ms and key_ls 
        lsls  r0,r0,#16                     // Shift key_ms into upper 16 bits;  key_ms is most significant 16 bits of key
        add   r0,r0,r1                      // Add key_ls to previous result;  key_ls is least significant 16 bits of key
        
        // Store key to SWDUNLOCK register
        ldr   r1, =MEMCTL_SWDUNLOCK_ADDR
        
        str   r0,[r1]
        
        // Clear key from r1
        movs  r0,#0

        // Return to calling function
        bx    lr

        END
                    
#endif // #if !defined(__ASSEMBLER__) && !defined(__IAR_SYSTEMS_ASM__)            




//===== GCC Assembly =====
// process below if GCC
#if defined(__ASSEMBLER__)      //__ASSEMBLER__ = GCC assembler 

		.section  .data,"aw",%progbits @    // Places in RAM
		.thumb_func
		.code   16
		.syntax unified
		.type write_swdunlock_key_ramfunc_asm, %function
		.globl  write_swdunlock_key_ramfunc_asm
		.align 1

write_swdunlock_key_ramfunc_asm:
        // Combine key_ms and key_ls
        lsls  r0,r0,#16                     // Shift key_ms into upper 16 bits;  key_ms is most significant 16 bits of key
        add   r0,r0,r1                      // Add key_ls to previous result;  key_ls is least significant 16 bits of key

        // Store key to SWDUNLOCK register
        ldr   r1, =MEMCTL_SWDUNLOCK_ADDR

        str   r0,[r1]

        // Clear key from r1
        movs  r0,#0

        // Return to calling function
        bx    lr

        .end
        
#endif // #if defined(__ASSEMBLER__)

