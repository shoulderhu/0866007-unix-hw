; nc aup.zoolab.org 2510
; FLAG: ASM{329d6b2d2b60dbe275c2fadfab47326ec7ce8032}
; ======
; math1: unsigned arithmetic
;         var4 = (var1 + var2) * var3
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

math1:
        mov     eax, [0x600000]     ; eax = *var1
        add     eax, [0x600004]     ; eax = eax + *var2
        mov     ebx, [0x600008]     ; ebx = *var3
        mul     ebx                 ; eax = eax * ebx
        mov     [0x60000c], eax     ; *var4 = eax
done: