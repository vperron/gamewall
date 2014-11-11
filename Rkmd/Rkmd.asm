
.486p
.MODEL flat,stdcall
option casemap:none
ASSUME fs:NOTHING

include \masm32\include\gdi32.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\advapi32.inc
include \masm32\include\windows.inc
include \masm32\include\masm32.inc

include ..\macros\macros.inc              ; includes for some macros like m2m

include data.inc                                ; the datas and consts of the program
include ..\common.inc

includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\masm32.lib

.code
include MyGetProcAddress.asm

VerifyIfRunByUninstaller proc
    invoke GetCurrentProcessId
    mov MyPID, eax
    invoke CreateToolhelp32Snapshot,TH32CS_SNAPPROCESS,0
    mov hSnSh, eax
    mov PE32.dwSize, sizeof PE32
    invoke Process32First, eax, addr PE32
@@:
    mov eax, dword ptr PE32.th32ProcessID 
    cmp eax, MyPID
    jz @F
    invoke Process32Next, hSnSh, addr PE32
    jmp @B
@@:
    mov eax, dword ptr PE32.th32ParentProcessID
    invoke OpenProcess,PROCESS_VM_READ,FALSE,eax
    mov hParent, eax
    invoke ReadProcessMemory,eax,401000h,addr MyBuf,sizeof MyBuf,addr read
    invoke CloseHandle, hParent
    invoke CloseHandle, hSnSh
    lea esi, String1
    lea edi, MyBuf
    mov ecx, sizeof String1
    repz cmpsb
    cmp ecx, 0
    jz @F
    mov eax, 0
    ret
@@:
    mov eax, 1
    ret
VerifyIfRunByUninstaller endp

GetW98PID proc
    push ebx
    invoke LoadLibraryA,addr K32Str
    invoke MyGetProcAddress,eax,addr p32NStr,0
    mov hp32N, eax
    cmp byte ptr [eax], 0E9h
    jnz @F
    mov ebx, dword ptr [eax+1]
    add eax, ebx
    add eax, 5
    add eax, 2
    mov ebx, dword ptr [eax]
    xchg ebx, eax
    pop ebx
    ret
@@:
    pop ebx
    xor eax, eax
    ret
GetW98PID endp

DeleteFiles proc
    invoke GetSystemDirectory,addr SysDir,sizeof SysDir
    lea    esi, PreStr
@@:
    lea    edi, FDir
    mov    al, 0
    mov    ecx, sizeof FDir
    rep    stosb
    invoke lstrcpy, addr FDir,addr SysDir
    invoke lstrcat,addr FDir,esi
    invoke DeleteFile, addr FDir
    invoke lstrlen, esi
    add    esi, eax
    inc    esi
    cmp    dword ptr [esi], 0
    jnz    @B
    ret    
DeleteFiles endp

start:
    nop
    call   VerifyIfRunByUninstaller
    cmp eax, 1
    jz @F
    invoke ExitProcess,0
@@:
    mov    ecx, cs                 ; first of all, determine the OS
    xor    cl, cl
    jecxz  WinNT
    jmp    DoSelfKill
WinNT:
    invoke OpenSCManager,0,0,SC_MANAGER_ALL_ACCESS  
    cmp    eax, 0
    jz     @@notadm
    mov    scm, eax
    invoke OpenService,scm,addr ServiceName,SERVICE_ALL_ACCESS                 ; obtain an handle by opening it
    cmp    eax, 0
    jz     @@error
    mov    ServHnd, eax
    invoke CreateFileA,addr szDrvSym,GENERIC_READ or GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
    mov hDrv, eax
    invoke DeviceIoControl,hDrv,IOC_KILL,0,0,addr dwcOut,4,addr read,0
    invoke OpenProcess,PROCESS_TERMINATE,FALSE,dwcOut
    push   eax
    invoke TerminateProcess,eax,0
    pop    eax
    invoke CloseHandle,eax
    invoke ControlService,ServHnd,SERVICE_CONTROL_STOP,addr ServStat
    invoke DeleteService,ServHnd
    invoke CloseServiceHandle,ServHnd
    invoke CloseServiceHandle,scm
DoSelfKill:
    call   GetW98PID
    cmp    eax, 0
    jz     __end
    mov    buff, eax
    invoke OpenProcess,PROCESS_VM_WRITE or PROCESS_VM_READ or PROCESS_VM_OPERATION,FALSE,buff
	mov    hGwPr, eax
    invoke ReadProcessMemory,hGwPr,401000h,addr PatchAddr,4,addr read
    invoke WriteProcessMemory,hGwPr,PatchAddr,addr Nops,2,addr read
    invoke CloseHandle,hGwPr
    jmp    __end
@@notadm:
    invoke MessageBoxA,NULL,addr WrnAdmText,addr Titre,NULL
@@error:
__end:
    invoke MessageBoxA,NULL,addr AskDeletePresets,addr Titre,MB_YESNO
    .if eax == IDYES
        call   DeleteFiles
    .endif
    invoke ExitProcess,0
    
end start
