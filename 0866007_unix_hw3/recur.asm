; nc aup.zoolab.org 2518
; FLAG: ASM{432ea21a5fc13ac8e625fa2d22ae02745289a4d8}
; ======
; recur: implement a recursive function
;
;    r(n) = 0, if n <= 0
;         = 1, if n == 1
;         = 2*r(n-1) + 3*r(n-2), otherwise
;
;    please call r(30) and store the result in RAX
; ======

recur:
        mov     edi, 30     ; edi = 30
        call    r           ;
        jmp     exit        ;
r:
        push    rbp
        mov     rbp, rsp

        cmp     edi, 0
        jle     zero

        cmp     edi, 1
        je      one

        push    rdi
        sub     edi, 0x1    ; edi = edi - 1
        call    r
        pop     rdi

        mov     rbx, 0x2    ; rbx = 0x2
        mul     rbx         ; rax = rax * rbx

        push    rax
        sub     edi, 0x2    ; edi = edi - 2
        call    r

        mov     rbx, 0x3    ; rbx = 0x3
        mul     rbx         ; rax = rax * rbx

        pop     rbx
        add     rax, rbx    ; rax = rax + rbx
        jmp     end
one:
        mov     rax, 0x1    ; rax, 1
        jmp     end
zero:
        mov     rax, 0x0    ; rax, 0
end:
        pop     rbp
        ret
exit:
done:
