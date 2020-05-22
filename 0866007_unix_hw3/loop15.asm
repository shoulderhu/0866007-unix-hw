; nc aup.zoolab.org 2509
; FLAG: ASM{44f8c5d95bfc5a2cebf28e8a0016bf0c898e42b7}
; ======
; loop15:
;         str1 is a string contains 15 lowercase and uppercase alphbets.
;         implement a loop to convert all alplabets to lowercase,
;         and store the result in str2.
; ======
;       str1 @ 0x600000-600010
;       str2 @ 0x600010-600020
; ======

loop15:
        mov     ecx, 15                     ; ecx = 15
l1:
        mov     al, [0x600000 + ecx - 1]    ; al = str1[ecx - 1]
        or      al, 0x20                    ; al = al | 0x20
        mov     [0x600010 + ecx - 1], al    ; str2[ecx - 1] = al
        loop l1                             ;
done: