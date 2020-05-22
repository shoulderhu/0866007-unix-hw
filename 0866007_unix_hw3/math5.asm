; nc aup.zoolab.org 2514
; FLAG: ASM{d094846539de42b83a8c47494e8ad906f4642340}
; ======
; math5: 32-bit signed arithmetic
;         var3 = (var1 * -var2) / (var3 - ebx)
;         note: overflowed part should be truncated
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
; ======

math5:
        mov     eax, [0x600000]     ; eax = *var1
        mov     ecx, [0x600004]     ; ecx = *var2
        neg     ecx                 ; ecx = -ecx
        imul    ecx                 ; eax = eax * ecx

        mov     ecx, [0x600008]     ; ecx = *var3
        sub     ecx, ebx            ; ecx = ecx - ebx

        mov     edx, 0x0            ; edx = 0
        idiv    ecx                 ; eax = eax / ecx
        mov     [0x600008], eax     ; *var3 = eax
done:
