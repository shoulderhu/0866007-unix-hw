; nc aup.zoolab.org 2508
; FLAG: ASM{4dcff917b5e26def74682294894946603a1e6b6b}
; ======
; leax:
;         eax = edi * 2
;         ebx = edi * 3
;         ecx = edi * 5
;         edx = edi * 9
; ======

leax:
        lea     eax, [edi * 2]          ; eax = edi * 2
        lea     ebx, [edi * 2 + edi]    ; ebx = edi * 2 + edi
        lea     ecx, [edi * 4 + edi]    ; ecx = edi * 4 + edi
        lea     edx, [edi * 8 + edi]    ; edx = edi * 8 + edi
done: