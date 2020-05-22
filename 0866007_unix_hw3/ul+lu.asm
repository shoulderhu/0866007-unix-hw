; nc aup.zoolab.org 2522
; FLAG: ASM{8c9f0650d45e9a8cf1a0c6696f486025e30005ad}
; ======
; ul+lu: convert the alphabet in CH from upper to lower or from lower to upper
; ======

ullu:
        xor     ch, 0x20    ; ch = ch ^ 0x20
done: