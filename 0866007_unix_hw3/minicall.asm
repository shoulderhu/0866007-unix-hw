; nc aup.zoolab.org 2515
; FLAG: ASM{6a409552c7451c41c5e961cc5a9af9ced510ce98}
; ======
; minicall: implement a minimal function call in the emulator
; ===== THE CODE
;     call   a
;     jmp    exit
;
; a:  ; function a - read ret-addr in rax
;     pop    rax
;     push   rax
;     ret
; exit:
; ======

minicall:
        call    a
        jmp     exit
a:
        pop     rax
        push    rax
        ret
exit:
done: