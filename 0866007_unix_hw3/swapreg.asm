; nc aup.zoolab.org 2520
; FLAG: ASM{534a511af0ecbeb0f7f367a81d416d27749c2642}
; ======
; swapreg: swap the values in RAX and RBX
; ======

swapreg:
        xchg    rax, rbx    ; rax, rbx = rbx, rax
done: