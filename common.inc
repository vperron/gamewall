;--------------------------------------------- some structures
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
	
MD5RESULT		STRUCT
	dtA		                        DWORD ?
	dtB		                        DWORD ?
	dtC		                        DWORD ?
	dtD		                        DWORD ?
MD5RESULT		ENDS

DAYAUTH struct
    Duration                        DWORD ?
    BaseHour                        DWORD ?
    AuthEd                          DWORD ?
DAYAUTH ends

WEEKAUTH struct
    Monday                          DAYAUTH <>
    Tuesday                         DAYAUTH <>
    Wednesday                       DAYAUTH <>
    Thursday                        DAYAUTH <>
    Friday                          DAYAUTH <>
    Saturday                        DAYAUTH <>
    Sunday                          DAYAUTH <>
    Week                            DAYAUTH <>
WEEKAUTH ends

INIFILE struct
    Md5Password                     MD5RESULT <?>
    IsTextVisible                   DWORD ?
    IsUserPrevented                 DWORD ?
    IsCplPrevented                  DWORD ?
    IsNagOff                        DWORD ?
    IsAutoRestarted                 DWORD ?
    IsUserInformed                  DWORD ?
    AreWinGamesBlocked              DWORD ?
    IsNetwork                       DWORD ?
    IsServer                        DWORD ?
    IsClient                        DWORD ?
    ServCompName                    BYTE 40 dup(?)
    WeekAuth                        WEEKAUTH <>
    DemoPass                        DWORD ?
INIFILE ends

DAYHND struct
    ButtonHnd                       DWORD ?
    DurationHnd                     DWORD ?
    HourHnd                         DWORD ?
    IsActive                        DWORD ?
DAYHND ends

WEEKHND struct
    Monday                          DAYHND <>
    Tuesday                         DAYHND <>
    Wednesday                       DAYHND <>
    Thursday                        DAYHND <>
    Friday                          DAYHND <>
    Saturday                        DAYHND <>
    Sunday                          DAYHND <>
    Week                            DAYHND <>
WEEKHND ends

; ------------------------
; inline memory copy macro
; ------------------------
mcopy MACRO lpSource,lpDest,len
    pushad
    mov esi, lpSource
    mov edi, lpDest
    mov ecx, len
    rep movsb
    popad
ENDM

freed MACRO aDest,alen
    pushad
    lea    edi, aDest
    mov    al, 0
    mov    ecx, alen
    rep    stosb
    popad
ENDM

freed2 MACRO aDest,alen
    pushad
    mov    edi, aDest
    mov    al, 0
    mov    ecx, alen
    rep    stosb
    popad
ENDM

   crypt MACRO aDest
   
           lea edi, aDest
   @@:     
           cmp dword ptr [edi],0
           jz @F
           mov al, byte ptr [edi]
           xor al, 'S'
           mov byte ptr [edi], al
           inc edi
           jmp @B
   @@:     
 ;          mov byte ptr [edi], 53h
   ENDM 
      
   GetSSize MACRO aDest
            xor ecx, ecx
           mov edi, aDest
   @@:     
           cmp dword ptr [edi],0
           jz @F
           inc ecx
           inc edi
           jmp @B
   @@:  
   ENDM         
   
   crypt2 MACRO aDest
   
           mov edi, aDest
   @@:     
           cmp dword ptr [edi],0
           jz @F
           mov al, byte ptr [edi]
           xor al, 'S'
           mov byte ptr [edi], al
           inc edi
           jmp @B
   @@:     
 ;          mov byte ptr [edi], 53h
   ENDM    
           
          

mcopyd MACRO Source,Dest,len
    pushad
    lea esi, Source
    lea edi, Dest
    mov ecx, len
    rep movsb
    popad
ENDM

.const

; Defining I/O Control Codes
IO_USER_FUNC_CODE_BASE                  EQU (0800h)                                    
METHOD_BUFFERED                         EQU (000h)
METHOD_IN_DIRECT                        EQU (001h)
METHOD_OUT_DIRECT                       EQU (002h)
METHOD_NEITHER                          EQU (003h)


LOAD_READ_WHOLE                         EQU 1 
PRIV_READ_WHOLE                         EQU 2
LOAD_WRITE_STRING                       EQU 3
PRIV_WRITE_STRING                       EQU 4
LOAD_WRITE_ALL                          EQU 5
PRIV_WRITE_ALL                          EQU 6
KILL_SERV                               EQU 7
LOAD_SECONDARY                          EQU 8
WRITE_SECONDARY                         EQU 9


; our user defined IO control codes
IOC_GETAPIID                            EQU (IO_USER_FUNC_CODE_BASE + 0100h) or METHOD_BUFFERED
IOC_GETCURPID                           EQU (IO_USER_FUNC_CODE_BASE + 0200h) or METHOD_BUFFERED


IOC_HOOK                                EQU (IO_USER_FUNC_CODE_BASE + 01000h) or METHOD_BUFFERED
IOC_UNHOOK                              EQU (IO_USER_FUNC_CODE_BASE + 01100h) or METHOD_BUFFERED

IOC_KILL                                EQU (IO_USER_FUNC_CODE_BASE + 01200h) or METHOD_BUFFERED

ServiceName                             db "GWallDev",0
Slash                                   db "\drivers\",0
ServPath                                db "GWallDev.sys",0
szDrvSym                                db "\\.\GWallDev",0
ServPipeName                            db "\\.\pipe\GameWallPipe",0
ClientPipePre                           db "\\",0
ClientPipeEnd                           db "\pipe\GameWallPipe",0
szServProg                              db "ServProg.exe",0

AppName                                 db "GW v3.0",0

IniDefault                              INIFILE < <0BDCBBF9Eh,6E2D05D2h,9A66A73Eh,0B1DE0A89h>, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, ,<<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>,<0,0,0>>,0 >
    
CURRENT_INIDEFAULT_BETWEEN_VALUES       equ 80