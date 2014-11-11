
.486p
.MODEL flat,stdcall
option casemap:none
ASSUME fs:NOTHING

include \masm32\include\gdi32.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\advapi32.inc
include \masm32\include\comctl32.inc
include \masm32\include\imagehlp.inc
include \masm32\include\windows.inc
include \masm32\include\masm32.inc
include \masm32\include\shell32.inc
include macros\macros.inc              ; includes for some macros like m2m

include GameWall.inc                            ; includes for the GameWall's Dll's exports.
includelib GameWall.lib

include IMAGE.inc
includelib IMAGE.lib
include common.inc
include data.inc                                ; the datas and consts of the program

includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\comctl32.lib
includelib \masm32\lib\imagehlp.lib
includelib \masm32\lib\shell32.lib
includelib \masm32\lib\masm32.lib


.code
a01                                 dd 0
include Md5.asm
include WndProc.asm
disasm_init:
include LDECrypted1.asm
disasm_main:
include LDECrypted2.asm
disasm_end:
include modules\ExeName2PID.asm         ; some included modules
include HideMe9x.asm            
include HideMeKMD.asm

IsUserAdmin proc
    invoke OpenSCManagerA,NULL,NULL,SC_MANAGER_ALL_ACCESS
    .if eax == 0
        invoke GetLastError
        .if eax == ERROR_ACCESS_DENIED
            xor    eax, eax
            ret
        .endif
    .else
        invoke CloseHandle,eax
        mov eax, 1
        ret
    .endif
IsUserAdmin endp

TraqueTitres proc
    invoke Sleep,80
    mov    ecx, Count_Titles
    mov    edx, dword ptr [TitlesOTable+ecx*4]     
    invoke FindWindow,NULL,edx
    cmp    eax, NULL
    jz     @F
    invoke GetWindowThreadProcessId,eax,addr pID
    mov    eax, pID
    invoke OpenProcess,PROCESS_TERMINATE,0,eax
    invoke TerminateProcess,eax,0
@@: 
    inc    Count_Titles
    cmp    Count_Titles, 10
    jnz    @F
    mov    Count_Titles, 0
@@:
    ret
TraqueTitres endp

start:
    freed  DataBegin,DataEnd-DataBegin
    lea    esi, disasm_init
    mov    ecx, disasm_end-disasm_init
@@:
    not    byte ptr [esi]
    sub    byte ptr [esi], 13h
    xor    byte ptr [esi], 13h
    inc    esi
    dec    ecx
    jnz    @B
    invoke GetModuleHandle,addr szU32Dll
    invoke GetProcAddress,eax,addr szMgBox
    mov    U32Hnd, eax
;-------------------------------------------------------------------------;
;   Here begins the source code of my craziest project : the GameWall !   ;
;-------------------------------------------------------------------------;
;----------------------------------------------- Detects if another intance of the GameWall is running...
    invoke CreateMutex,NULL,TRUE,addr MutexName
    .if eax != 0
        mov    hMutex, eax
        invoke GetLastError
        .if eax == ERROR_ALREADY_EXISTS
            invoke IsUserAdmin
            .if eax==1
                invoke MessageBoxA,NULL,addr YetLoaded,addr AppName,NULL
            .endif
            invoke ExitProcess,0
        .endif
    .else
        invoke IsUserAdmin
        .if eax == 1
            invoke MessageBoxA,NULL,addr szErrorUnexp,addr AppName,NULL
        .endif
        invoke ExitProcess,0
    .endif
    mov    ecx, cs                 ; first of all, determine the OS
    xor    cl, cl
    jecxz  WinNT
;----------------------------------------------- HideMe9x and HideMeKMD are functions used to hide the GameWall
    invoke HideMe9x
    jmp    @F    
WinNT:
    mov    NTOs,1
    invoke HideMeKMD   
    cmp    eax, 0
    jnz    @F
;---------------------------------------------- If there was a problem while calling the invisibility device driver...
    invoke IsUserAdmin
    .if eax == 1
        invoke MessageBoxA,0,addr szErrorUnexp,addr AppName,NULL
    .endif
    invoke ExitProcess,0
@@:    
    invoke GetModuleHandle,0
    mov    hInst,eax
;--------------------------------------------- Tries to find the INI file of the GameWall.
    invoke GetAppPath,addr pIniDir
    invoke lstrcat,addr pIniDir,addr GWallIni
    invoke CreateFile,addr pIniDir,GENERIC_READ or GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL
    cmp    eax, 0FFFFFFFFh
    jnz    NotTheFirstTime
    invoke GetLastError
    cmp    eax,ERROR_SHARING_VIOLATION
    jnz    @F
;--------------------------------------------- If one instance of the GameWall had been previous executed, we exit NOW.
    invoke IsUserAdmin
    .if eax == 1
        invoke MessageBoxA,NULL,addr YetLoaded,addr AppName,NULL
    .endif
    invoke ExitProcess,0
@@:
    invoke CreateFile,addr pIniDir,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL
    cmp    eax, 0FFFFFFFFh
    jnz    NotTheFirstTime
;------------------------------------------------------ First launch : creates the GameWall.ini containing the crypted password
;    invoke IsUserAdmin
;    .if eax == 1
        invoke MessageBoxA,NULL,addr FirstTimeText,addr AppName,NULL
;    .endif
__setdefault:
    invoke CreateFile,addr pIniDir,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
    mov    hInit, eax
    invoke SetFilePointer,hInit,0,0,FILE_BEGIN
    invoke WriteFile,hInit,addr IniDefault,sizeof INIFILE,addr read, NULL
    mcopyd IniDefault,IniFile,sizeof INIFILE
    invoke SaveCheckSum,CHECKSUM_RSET
    jmp    FirstDone
NotTheFirstTime:
;------------------------------------------------------ Else reads the password in the existing GameWall.ini
    mov    hInit, eax
    invoke RtlZeroMemory,addr IniFile,sizeof IniFile
    invoke SetFilePointer,hInit,0,0,FILE_BEGIN
    invoke ReadFile,hInit,addr IniFile,sizeof INIFILE,addr read,NULL
    invoke SaveCheckSum,CHECKSUM_RGET
    cmp    eax, 0
    jz     @F
    mov    ebx, eax
    invoke SaveCheckSum,CHECKSUM_FGET
    cmp    eax, 0
    jz     @F
    cmp    eax, ebx
    jnz    @F
    jmp    FirstDone
@@:
    invoke IsUserAdmin
    .if eax == 1
        invoke MessageBoxA,0,addr FileModified,addr AppName,0
    .endif
    jmp    __setdefault
FirstDone:
    invoke SetFilePointer,hInit,0,0,FILE_BEGIN
;--------------------------------------------- Try to find the demo file of the gamewall.
;int 3
    invoke SetDemoOrNot    
    invoke DemoReminder,U32Hnd
;------------------------------------------------------ Just opens the \run key

    invoke RegCreateKey,HKEY_LOCAL_MACHINE,addr SubKey,addr Khnd
    invoke GetModuleFileNameA,0,addr value, sizeof value
    invoke CharLower,addr value
;------------------------------------------------------ We extract our dll from the executable, copy it in the system directory
    invoke GetSystemDirectory,addr DllPath,sizeof DllPath
    invoke lstrcat,addr DllPath,addr DllName
    invoke CreateFile,addr DllPath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    mov    hDll, eax
    invoke FindResource,hInst,200,addr Typ
    push   eax
    invoke SizeofResource,hInst,eax
    mov    sDll, eax
    pop    eax
    invoke LoadResource,hInst,eax
    push   eax
    invoke LockResource,eax
    mov    pDll, eax
    invoke WriteFile,hDll,eax,sDll,addr read,NULL
    pop    eax
    invoke FreeResource, eax
    invoke CloseHandle,hDll
;---------------------------------------------------- Creates the Nag-Screen
    .if IniFile.IsNagOff == 0
        invoke CreateThread,NULL,NULL,addr CreateNag,NULL,NULL,addr NagThID
    .endif
;------------------------------------------------------ And we finally install the hooks.
    invoke LoadLibrary,addr DllPath
    mov    hGDll, eax
    invoke GetProcAddress,eax,addr IHStr
    mov    ebx, eax
    invoke GetCurrentProcessId
    push   offset IniFile
    push   offset topatch
    push   eax
    call   ebx
    m2m    a01, offset MyKillAddress
    mov    Count_Titles,10
    lea    edi, ForbiddenFiles
    xor    ecx, ecx
@@:
    mov    dword ptr [TitlesOTable+ecx*4], edi              ; minesweeper = démineur anglais
    push   ecx
    invoke lstrlen, edi
    pop    ecx
    add    edi, eax
    inc    edi
    inc    ecx
    cmp    ecx, Count_Titles
    jnz    @B
    mov    Count_Titles, 0
;------------------------------------------------------ Infinite loop to be broken with the hooks when pressing F12
re: 
    .if IniFile.AreWinGamesBlocked == 1
        invoke TraqueTitres
    .else
        invoke Sleep,450
    .endif
    invoke nrandom, 1000
    cmp eax, 13
    jnz @F
    invoke DemoReminder,U32Hnd
@@:
    
;------------------------------------------------------ Now we do the necessary to be loaded at startup
    .if IniFile.IsAutoRestarted == 1
        invoke RegSetValueEx,Khnd,addr ServName,NULL,REG_SZ,addr value,sizeof value
    .else
        invoke RegDeleteValue,Khnd,addr ServName
    .endif
MyKillAddress:
    jmp    topatch
    invoke DoUnHookAndClose
topatch:
    jmp    re
;------------------------------------------------------ Calls a dialogbox to verify the password
    invoke DialogBoxParam,hInst,101,NULL,addr DlgVerif,NULL
;------------------------------------------------------ If it's the onliest right pass, we can display the main Control Dialog
    .if Pass == 1
        invoke DialogBoxParam,hInst,102,NULL,addr TabProc,NULL
    .endif
;------------------------------------------------------ re-assembles the jump to restart the infinite loop
    mov    byte ptr [topatch], 0EBh
    xor    al, al
    sub    al, offset topatch-offset re
    sub    al,2
           
    mov    byte ptr [topatch+1], al
    jmp    re
    invoke ExitProcess,0                            ; just for safety purposes :D
    
end start
