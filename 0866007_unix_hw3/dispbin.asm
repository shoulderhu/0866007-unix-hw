; nc aup.zoolab.org 2505
; FLAG: ASM{7cd0546f6f0ab256514c5426156e0642a0c5ec08}
; ======
; dispbin:
;         given a number in AX, store the corresponding bit string in str1.
;         for example, if AX = 0x1234, the result should be:
;         str1 = 0001001000111000
; ======
;       str1 @ 0x600000-600014
; ======

dispbin:
        mov     ecx, 16                     ; ecx = 16
l1:
        mov     bx, ax                      ; bx = ax
        and     bl, 0x1                     ; bx = bx & 0x1
        or      bl, 0x30                    ; bx = bx | 0x30
        mov     [0x600000 + ecx - 1], bl    ; str1[ecx - 1] = bl
        shr     ax, 0x1                     ; ax = ax / 2
        loop l1
done: