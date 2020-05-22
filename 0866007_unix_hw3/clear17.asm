; nc aup.zoolab.org 2503
; FLAG: ASM{495071da9e7398c46f7b924f4038a597d3952314}
; ======
; clear17: clear bit-17 in eax (zero-based index)
; ======

clear17:
        and     eax, 0xfffdffff     ; eax = eax & 0xfffdffff
done: