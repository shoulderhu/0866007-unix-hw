; nc aup.zoolab.org 2502
; FLAG: ASM{ed11e3273e0a8845d42afa3f901074df13e12d0e}
; ======
; bubble: bubble sort for 10 integers
; ======
;       a[0] @ 0x600000-600004
;       a[1] @ 0x600004-600008
;       a[2] @ 0x600008-60000c
;       a[3] @ 0x60000c-600010
;       a[4] @ 0x600010-600014
;       a[5] @ 0x600014-600018
;       a[6] @ 0x600018-60001c
;       a[7] @ 0x60001c-600020
;       a[8] @ 0x600020-600024
;       a[9] @ 0x600024-600028
; ======

bubble:
        mov     ecx, 10                             ; ecx = 10
l1:
        push    rcx
        mov     edx, ecx                            ; edx = ecx

l2:
        mov     eax, [0x600000 + edx * 4 - 4]       ; eax = a[edx - 1]
        cmp     eax, [0x600000 + ecx * 4 - 4]       ;
        jge     l2cont                              ;
        xchg    eax, [0x600000 + ecx * 4 - 4]       ; eax, a[ecx - 1] = a[ecx - 1], eax
        mov     [0x600000 + edx * 4 - 4], eax       ; a[edx - 1] = eax
l2cont:
        loop    l2

        pop     rcx
        loop    l1
done: