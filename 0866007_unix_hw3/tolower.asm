; nc aup.zoolab.org 2521
;
; ======
; tolower: convert the single character in val1 to uppercase and store in val2
; FLAG: ASM{a2ae0dfc6e3d5f1a32bdc8bc9a12382a2c8fbb8b}
; ======
;       val1 @ 0x600000-600001
;       val2 @ 0x600001-600002

tolower:
        mov     al, [0x600000]      ; al = *val1
        and     al, 0x5f            ; al = al & 0x5f
        mov     [0x600001], al      ; *val2 = al
done:

