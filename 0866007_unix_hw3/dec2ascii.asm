; nc aup.zoolab.org 2504
; FLAG: ASM{067d91cabbe48ae0d378b9f06e09a3f5dd19f192}
; ======
; dec2ascii: convert the value (0-9) in AL to its ASCII character
; ======

dec2ascii:
        or      al, 0x30    ; al = al | 0x30
done: