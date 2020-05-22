; nc aup.zoolab.org 2501
; FLAG: ASM{9f4b9cc948960da32302135229f622472f7d8713}
; ======
; addsub2:
;         final = val1 + val2 - val3
; ======
;       val1 @ 0x600000-600004
;       val2 @ 0x600004-600008
;       val3 @ 0x600008-60000c
;      final @ 0x60000c-600010
; ======

addsub2:
        mov     eax, [0x600000]     ; eax = *val1
        add     eax, [0x600004]     ; eax = eax + *val2
        sub     eax, [0x600008]     ; eax = eax - *val3
        mov     [0x60000c], eax     ; *final = eax
done:
