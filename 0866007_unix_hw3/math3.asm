; nc aup.zoolab.org 2512
; FLAG: ASM{29fefc836582fc2520b7c1943a4f222d13c867bd}
; ======
; math3: 32-bit unsigned arithmetic
;         var4 = (var1 * 5) / (var2 - 3)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var4 @ 0x600008-60000c
; ======

math3:
        mov     eax, [0x600000]     ; eax = *var1
        mov     ebx, 0x5            ; ebx = 0x5
        mul     ebx                 ; eax = eax * ebx

        mov     ebx, [0x600004]     ; ebx = *var2
        sub     ebx, 0x3            ; ebx = ebx - 3;

        mov     edx, 0x0            ; edx = 0x0
        div     ebx                 ; eax = eax / ebx
        mov     [0x600008], eax     ; *var4 = eax
done: