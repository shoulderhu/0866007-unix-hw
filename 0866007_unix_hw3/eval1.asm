; nc aup.zoolab.org 2506
; FLAG: ASM{e683b6b39c904540bf724e1e8a8e9ef030f01e29}
; ======
; eval1:
;         Rval = -Xval + (Yval – Zval)
; ======
;       Xval @ 0x600000-600004
;       Yval @ 0x600004-600008
;       Zval @ 0x600008-60000c
;       Rval @ 0x60000c-600010
; ======

eval1:
        mov     eax, [0x600000]     ; eax = *Xval
        neg     eax                 ; eax = -eax
        add     eax, [0x600004]     ; eax = eax + *Yval
        sub     eax, [0x600008]     ; eax = eax - *Zval
        mov     [0x60000c], eax     ; *Rval = eax
done: