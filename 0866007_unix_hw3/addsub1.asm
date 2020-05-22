; nc aup.zoolab.org 2500
; FLAG: ASM{12d30a57b6fd8c9779eb4d64428fb064653a7e00}
; ======
; addsub1:
;         eax = 0xbf19a23b
;         eax = eax + 0x5421c1dc
;         eax = eax - 0x9628417d
; ======

addsub1:
        mov     eax, 0xbf19a23b     ; eax = 0xbf19a23b
        add     eax, 0x5421c1dc     ; eax = eax + 0x5421c1dc
        sub     eax, 0x9628417d     ; eax = eax - 0x9628417d
done: