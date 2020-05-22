; nc aup.zoolab.org
; FLAG: ASM{c68c96817b844512055b5aa636fc71eba803a77c}
; ======
; isolatebit:
;         get the value bit-11 ~ bit-5 in AX and store the result in val1
;         (zero-based bit index)
; ======
;       val1 @ 0x600000-600001
;       val2 @ 0x600001-600002
; ======

isolatebit:
        shl     ax, 0x4;        ; ax = ax * 2^4
        shr     ax, 0x9;        ; ax = ax / 2^9
        mov     [0x600000], al  ; *val1 = al
done: