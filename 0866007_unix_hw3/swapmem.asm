; nc aup.zoolab.org 2519
; FLAG: ASM{bfc6b67fad445d161d540cfd75a5f57bbabb31ea}
; ======
; swapmem: swap the values in val1 and val2
; ======
;       val1 @ 0x600000-600008
;       val2 @ 0x600008-600010
; ======

swapmem:
        mov     eax, [0x600008]     ; eax = *val2
        xchg    [0x600000], eax     ; *val1, eax = eax, *val1
        mov     [0x600008], eax     ; *val2 = eax
done: