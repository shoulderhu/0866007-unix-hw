; nc aup.zoolab.org 2511
; FLAG: ASM{a12829814c0c3681044eaf4fee319c66994e2fda}
; ======
; math2: signed arithmetic
;         eax = (-var1 * var2) + var3
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
; ======

math2:
        mov     eax, [0x600000]     ; eax = *var1
        neg     eax                 ; eax = -eax
        mov     ebx, [0x600004]     ; ebx = *var2
        imul    ebx                 ; eax = eax * ebx
        add     eax, [0x600008]     ; eax = eax + *var3
done:
