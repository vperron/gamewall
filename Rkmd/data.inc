PUSHA_STRUCT STRUCT
	_EDI                            DWORD ?
	_ESI                            DWORD ?
	_EBP                            DWORD ?
	_ESP                            DWORD ?
	_EBX                            DWORD ?
	_EDX                            DWORD ?
	_ECX                            DWORD ?
	_EAX                            DWORD ?
PUSHA_STRUCT ENDS

; ------------------------
; inline memory copy macro
; ------------------------
mcopy MACRO lpSource,lpDest,len
    mov esi, lpSource
    mov edi, lpDest
    mov ecx, len
    rep movsb
ENDM

.const
    Titre               db "GameWall UnInstaller",0
    WrnAdmText          db "WARNING :",10,13,"You are not logged in as administrator. The UnInstall process will not be complete.",0
    K32Str              db "KERNEL32.dll",0
    p32NStr             db "Process32Next",0
    AskDeletePresets    db "Delete configuration information ?",0
    PreStr              db "\pPriv32.txt",0
                        db "\pLoadEx32.txt",0
                        db "\gamewall.dll",0
                        db "\pSecExt32.txt",0
                        dd 0
    

.data
;------------------------------------------------    general datas
    SysDir              db 250 dup(0)
    FDir                db 300 dup(0)
    hp32N               dd 0
    hInit               dd 0
    scm                 dd 0
    ServHnd             dd 0
    hDrv                dd 0
    dwcOut              dd 0
    buff                dd 0
    ServStat            dd 0
    hSnSh               dd 0
    PE32                PROCESSENTRY32 <0>
    MyPID               dd 0
    hParent             dd 0
    MyBuf               db 10 dup(0)
    read                dd 0
    String1             db 0Ah,6,'string',0
    hGwPr               dd 0
    PatchAddr           dd 0
    Nops                db 90h,90h
    