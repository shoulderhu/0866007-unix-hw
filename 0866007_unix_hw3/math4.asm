; nc aup.zoolab.org 2513
; FLAG: ASM{5df848046cc0a617a07df92cc063abe6eb9fab33}
; ======
; math4: 32-bit signed arithmetic
;         var4 = (var1 * -5) / (-var2 % var3)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

math4:
        mov     eax, [0x600000]     ; eax = *var1
        mov     ebx, 0x5            ; ebx = 0x5
        neg     ebx                 ; ebx = -ebx
        imul    ebx                 ; eax = eax * ebx
        mov     ebx, eax            ; ebx = eax

        mov     eax, [0x600004]     ; eax = *var2
        neg     eax                 ; eax = -eax
        mov     ecx, [0x600008]     ; ecx = *var3
        mov     edx, 0x0            ; edx = 0x0
        idiv    ecx                 ; edx = eax % ecx

        mov     eax, ebx            ; eax = ebx
        mov     ecx, edx            ; ecx = edx
        mov     edx, 0x0            ; edx = 0
        idiv    ecx                 ; eax = eax / ecx
        mov     [0x60000c], eax     ; *var4 = eax
done: