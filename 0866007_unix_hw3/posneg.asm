; nc aup.zoolab.org 2517
; FLAG: ASM{4a637ac9bd775a6f7ac8c15290a7bae3c5f492e1}
; ======
; posneg : test if registers are positive or negative.
;          if ( eax >= 0 ) { var1 = 1 } else { var1 = -1 }
;          if ( ebx >= 0 ) { var2 = 1 } else { var2 = -1 }
;          if ( ecx >= 0 ) { var3 = 1 } else { var3 = -1 }
;          if ( edx >= 0 ) { var4 = 1 } else { var4 = -1 }
; ======
;       var1 @ 0x600000-600004
;       var2 @ 0x600004-600008
;       var3 @ 0x600008-60000c
;       var4 @ 0x60000c-600010
; ======

posneg:
        cmp     eax, 0x0            ;
        mov     eax, 0x1            ; eax = 1
        jl      negvar1             ;
        mov     [0x600000], eax     ; *var1 = eax
        jmp     var2                ;
negvar1:
        neg     eax                 ; eax = -eax
        mov     [0x600000], eax     ; *var1 = eax
        neg     eax                 ; eax = -eax

var2:
        cmp     ebx, 0x0            ;
        jl      negvar2             ;
        mov     [0x600004], eax     ; *var2 = eax
        jmp     var3
negvar2:
        neg     eax                 ; eax = -eax
        mov     [0x600004], eax     ; *var2 = eax
        neg     eax                 ; eax = -eax

var3:
        cmp     ecx, 0x0            ;
        jl      negvar3             ;
        mov     [0x600008], eax     ; *var3 = eax
        jmp     var4                ;
negvar3:
        neg     eax                 ; eax = -eax
        mov     [0x600008], eax     ; *var3 = eax
        neg     eax                 ; eax = -eax

var4:
        cmp     edx, 0x0            ;
        jl      negvar4             ;
        mov     [0x60000c], eax     ; *var4 = eax
        jmp     end                 ;
negvar4:
        neg     eax                 ; eax = -eax
        mov     [0x60000c], eax     ; *var4 = eax
        neg     eax                 ; eax = -eax
end:
done: