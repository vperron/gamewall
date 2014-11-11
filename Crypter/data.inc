

.const
    

.data
;------------------------------------------------    general datas
    hInstance           dd 0
    buf                 db 150 dup(0)
    szFileName          db "Hex.dmp",0
    hFile               dd 0
    read                dd 0
    
    szCrypt1            db "Crypted1.dmp",0
    szCrypt2            db "Crypted2.dmp",0
    hCrypt1             dd 0
    hCrypt2             dd 0
    
    ReadBuf             db 273h dup(0)