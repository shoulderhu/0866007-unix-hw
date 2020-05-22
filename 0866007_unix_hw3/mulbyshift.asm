; nc aup.zoolab.org 2516
; FLAG: ASM{34b9caa05654006ec6a14db1a60ea145b2689b20}
; ======
; mulbyshift: multiply val1 by 26 and store the result in val2
; ======
;       val1 @ 0x600000-600004
;       val2 @ 0x600004-600008
; ======

mulbyshift:
        mov     eax, [0x600000]     ; eax = *val1

        mov     ebx, eax            ; ebx = eax
        shl     ebx, 0x4            ; ebx = ebx * 2^4
        mov     ecx, ebx            ; ecx = ebx

        mov     ebx, eax            ; ebx = eax
        shl     ebx, 0x3            ; ebx = ebx * 2^3
        add     ecx, ebx            ; ecx = ecx + ebx

        mov     ebx, eax            ; ebx = eax
        shl     ebx, 0x1            ; ebx = ebx * 2
        add     ecx, ebx            ; ecx = ecx + ebx

        mov     [0x600004], ecx     ; *val2 = ecx
done: