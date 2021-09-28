#line 1 "C:\\Users\\smagee\\source\\git\\pac55xx\\pac55xx_example_code.top\\pac55xx_example_code.git\\pac55xx_example_code\\pac55xx_peripheral_examples\\src\\code_protect\\swd_unlock.S"

































#line 64 "C:\\Users\\smagee\\source\\git\\pac55xx\\pac55xx_example_code.top\\pac55xx_example_code.git\\pac55xx_example_code\\pac55xx_peripheral_examples\\src\\code_protect\\swd_unlock.S"
























        PRESERVE8
        THUMB
        AREA    |.data|, CODE, READWRITE
        EXPORT  write_swdunlock_key_ramfunc_asm
        ALIGN   2

write_swdunlock_key_ramfunc_asm

        
        lsls  r0,r0,#16                     
        add   r0,r0,r1                      
        
        
        ldr   r1, =0x400D0010
        
        str   r0,[r1]
        
        
        movs  r0,#0

        
        bx    lr

        END
                    







#line 149 "C:\\Users\\smagee\\source\\git\\pac55xx\\pac55xx_example_code.top\\pac55xx_example_code.git\\pac55xx_example_code\\pac55xx_peripheral_examples\\src\\code_protect\\swd_unlock.S"

