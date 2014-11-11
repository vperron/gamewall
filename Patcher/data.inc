MyReminderBox PROTO :DWORD

.const
    ToMove              equ 0D23h
    ToRead              equ 015CEh+2Ch-ToMove               ; 28h = 10*4 bytes = 10 dwords farther
    
    Key                 equ 45896329       

.data
;------------------------------------------------    general datas
    hInstance           dd 0
    szFileToPatch       db 150 dup(0)
    ErrorOpening        db "Sorry but the file could not be opened. Please select another.",0
    Successful          db "Patch successful !!",0

    AppName             db "Patcher for the GameWall",0
    

    szLicName           db 300 dup(0)

    LicFile             db "License.key",0
    BlankFile           db "License.INFO",0

    FileHandle          dd 0

    read                dd 0
    hLic                dd 0
    hINFO               dd 0
    
    ReadBuf             db ToRead dup(0)
    newbuf              db 150 dup(0)
    
    VarKey              dd 0

    filter              db 'Executables (*.exe)',0,'*.exe',0,0
    info                db 'Fichier INFO',0,'*.INFO',0,0
    Format              db "%08d.License.key",0
