armcca1 MACRO
    db 60h,33h,0C0h,75h,02,0EBh,15h,0EBh,33h,0C0h,75h,18h,7Ah,0Ch,70h,0Eh
    db 0EBh,0Dh,0E8h,72h,0Eh,79h,0F1h,0FFh,15h,00,79h,09h,74h,0F0h,0EBh
    db 87h,0DBh,7Ah,0F0h,0A0h,33h,61h
ENDM

armcca2 MACRO
    db 50h,0F7h,0D0h,0Fh,0C8h,58h,73h,00,9Ch,60h,0EBh,2Bh,0D2h,70h,0Eh,0EBh
    db 1Dh,33h,0C9h,74h,00,0EBh,10h,0F9h,83h,0F1h,03h,74h,90h,0EBh,05h,0B9h
    db 0EBh,10h,74h,0EFh,73h,0FAh,0F2h,1Ah,0DBh,33h,0DBh,0EBh,0EAh,0E9h,0EBh
    db 0E1h,0EBh,0DDh,0F2h,1Ch,0DBh,0EBh,03h,0EBh,0D2h,0B8h,61h,9Dh,0Fh,0C8h,0F7h,0D1h
ENDM

armcca3 MACRO
    db 9Ch,60h,33h,0DBh,74h,03h,0EBh,22h,0EBh,33h,0DBh,74h,00,0EBh,0Dh,0B8h
    db 0EBh,0Fh,0B9h,87h,0C9h,0F9h,34h,90h,0F9h,74h,05h,0EBh,33h,0C0h,74h,0F2h
    db 0B8h,87h,0C9h,40h,48h,85h,0C0h,75h,0DDh,0E9h,61h,9Dh
ENDM

ccatrace MACRO          ; CCA with Tracer detection   (c) Syn 2oo4
	db  60h,9Ch,0EBh,29h,0B8h,0EBh,50h,0FFh,0FFh,0Ch,24h,0C3h,0E8h,2Bh,4,24h,0E8h,0,0,0,0,0Fh,77h,0EBh,4,0,0EBh,0E9h,0E8h
	db  5Bh,83h,0EBh,3,66h,0C7h,3,66h,3Dh,53h,0C3h,0Fh,31h,50h,0EBh,1Ch,0E8h,0FFh,0FFh,0FFh,0FFh,4,24h,4,0C3h,0EBh,1,0EBh
	db  0E8h,0CAh,0FFh,0FFh,0FFh,34h,24h,83h,2Ch,24h,0Bh,0C3h,0E8h,0E3h,0FFh,0FFh,0FFh,34h,24h,0E8h,0B7h,0FFh,0FFh,0FFh
	db  34h,24h,0Fh,31h,0EBh,0B6h,83h,0C4h,10h,9Dh,61h
ENDM

cca equ db 60h,33h,0C9h,75h,02h,0EBh,15h,0EBh,33h,0C9h,75h,18h,7Ah,0Ch,70h,0Eh,0EBh,0Dh,0E8h,72h,0Eh,79h,0F1h,0FFh,15h,00h,79h,09h,74h,0F0h,0EBh,87h,0DBh,7Ah,0F0h,0A0h,33h,61h
cca1 equ db 50h,51h,0EBh,0Fh,0B9h,0EBh,0Fh,0B8h,0EBh,07h,0B9h,0EBh,0Fh,90h,0EBh,08h,0FDh,0EBh,0Bh,0F2h,0EBh,0F5h,0EBh,0F6h,0F2h,0EBh,08h,0FDh,0EBh,0E9h,0F3h,0EBh,0E4h,0FCh,0E9h,59h,58h
    
a equ     db 0Ebh,02,0CDh,20h
b equ     db 0EBh,01h,0EBh