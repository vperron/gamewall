.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\gdi32.inc
include \masm32\include\advapi32.inc

include \masm32\include\masm32.inc
include ..\macros\macros.inc
include ..\common.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\advapi32.lib
includelib \masm32\lib\masm32.lib

include data.inc
include ..\ccas.inc

.code
disasm_init:
include ..\LDECrypted1.asm
disasm_main:
include ..\LDECrypted2.asm
disasm_end:

include procs.asm

DllEntry proc hInsta:HINSTANCE, reason:DWORD, reserved1:DWORD
    .if reason == DLL_PROCESS_ATTACH
        pushad
        push hInsta
        pop hInstance
        lea    esi, disasm_init
        mov    ecx, disasm_end-disasm_init
@@:     
        not    byte ptr [esi]
        sub    byte ptr [esi], 13h
        xor    byte ptr [esi], 13h
        inc    esi
        dec    ecx
        jnz    @B
;------------------------------------------------------------------------------ Get our privilegied names file
        invoke AccessConfigFiles,2,addr PrivBuf,sizeof PrivBuf
        invoke AccessConfigFiles,1,addr LaunchBuf,sizeof LaunchBuf
        popad
        mov eax, TRUE
    .elseif reason == DLL_PROCESS_DETACH
        .if ImAGame == 1
            dec    Counter
            .if Counter == 0
                m2m    TimeElapsedBefore, SharedDuration
            .endif
        .endif
    .endif
    ret
DllEntry ENDP
    
UnInstallHook proc
    invoke UnhookWindowsHookEx,MhHook
    invoke UnhookWindowsHookEx,hHook
    ret
UnInstallHook endp

InstallHook proc hwnd:DWORD, ptor:DWORD, pInF:DWORD
    m2m hWnd, hwnd
    m2m ModifPtr,ptor
    m2m pIni, pInF
    mov i, 0 
    mov KFlag, 0
    mov Counter, 0
    mov TimeElapsedBefore, 0
    mov LimitShown,0
    invoke OpenProcess,PROCESS_VM_READ,NULL,hWnd
    push   eax
    invoke ReadProcessMemory,eax,pIni,addr IniFile,sizeof INIFILE,addr read
    pop    eax
    invoke CloseHandle,eax
    invoke SetWindowsHookEx,WH_KEYBOARD,addr MHookProc,hInstance,NULL
    mov MhHook, eax
    invoke SetWindowsHookEx,WH_CBT,addr HookProc,hInstance,NULL
    mov hHook, eax
    ret 
InstallHook endp

End DllEntry
