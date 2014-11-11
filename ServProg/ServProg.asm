; #########################################################################

      .586p
      .model flat, stdcall
      option casemap :none   ; case sensitive

; #########################################################################

      include \masm32\include\windows.inc
      include \masm32\include\user32.inc
      include \masm32\include\kernel32.inc
      include \masm32\include\advapi32.inc
      includelib \masm32\lib\user32.lib
      includelib \masm32\lib\kernel32.lib
      includelib \masm32\lib\advapi32.lib

; #########################################################################
; our user defined IO control codes

        ;=============
        ; Local macros
        ;=============
      m2m MACRO M1, M2
        push M2
        pop  M1
    ENDM
    
        szText MACRO Name, Text:VARARG
          LOCAL lbl
            jmp lbl
              Name db Text,0
            lbl:
          ENDM
          
freed MACRO aDest,alen
    pushad
    lea    edi, aDest
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
   
           
          HideMeKMD PROTO 
          
LOAD_READ_WHOLE                         EQU 1 
PRIV_READ_WHOLE                         EQU 2
LOAD_WRITE_STRING                       EQU 3
PRIV_WRITE_STRING                       EQU 4
LOAD_WRITE_ALL                          EQU 5
PRIV_WRITE_ALL                          EQU 6
KILL_SERV                               EQU 7
LOAD_SECONDARY                          EQU 8
WRITE_SECONDARY                         EQU 9
          
        ;=================
        ; Local prototypes
        ;=================
        WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD

    .data


        MutexName       db "ServProgMutexCheckIsPreviouslyRunned",0
        hInstance       dd 0
        
        hPipe           dd 0
        hFile           dd 0
        hThread         dd 0
        
        PipeName        db "\\.\pipe\GameWallPipe",0
        FilePath    	db 150 dup(0)
        PrivFileName	db "\pPriv32.txt",0
        LoadFileName	db "\pLoadEx32.txt",0
    SecFileName         db "\pSecExt32.txt",0
        hPriv           dd 0
        hLoad           dd 0
        hSec            dd 0
        read            dd 0
         aSize           dd 0
         ExitFlag dd 0
         hMutex         dd 0
sa                  SECURITY_ATTRIBUTES <0>
        MyBuf           db 1500 dup(0)

    .code
freed2 MACRO aDest,alen
    pushad
    mov    edi, aDest
    mov    al, 0
    mov    ecx, alen
    rep    stosb
    popad
ENDM


m2m MACRO M1, M2
  push M2
  pop  M1
ENDM

TRUSTEE STRUCT
    pMultipleTrustee            DWORD ?
    MultipleTrusteeOperation    DWORD ?
    TrusteeForm                 DWORD ?
    TrusteeType                 DWORD ?
    ptstrName                   DWORD ?
TRUSTEE ENDS

EXPLICIT_ACCESS STRUCT
    grfAccessPermissions DWORD ?
    grfAccessMode        DWORD ?
    grfInheritance       DWORD ?
    Trustee              TRUSTEE <>
EXPLICIT_ACCESS ENDS

SE_UNKNOWN_OBJECT_TYPE      equ 0
SE_FILE_OBJECT              equ 1
SE_SERVICE                  equ 2
SE_PRINTER                  equ 3
SE_REGISTRY_KEY             equ 4
SE_LMSHARE                  equ 5
SE_KERNEL_OBJECT            equ 6
SE_WINDOW_OBJECT            equ 7
SE_DS_OBJECT                equ 8
SE_DS_OBJECT_ALL            equ 9
SE_PROVIDER_DEFINED_OBJECT  equ 10
SE_WMIGUID_OBJECT           equ 11
SE_REGISTRY_WOW64_32KEY     equ 12


NOT_USED_ACCESS             equ 0
GRANT_ACCESS                equ 1
SET_ACCESS                  equ 2
DENY_ACCESS                 equ 3
REVOKE_ACCESS               equ 4
SET_AUDIT_SUCCESS           equ 5
SET_AUDIT_FAILURE           equ 6


SetPipeSecurity proc pSecurity:DWORD
    LOCAL pPipeSD:DWORD
    LOCAL saPipeSecurity:DWORD
    
    m2m saPipeSecurity,pSecurity
    
    ; security inits
    freed2 saPipeSecurity, sizeof SECURITY_ATTRIBUTES

    ; alloc & init SD
    invoke GlobalAlloc,GMEM_FIXED or GMEM_ZEROINIT,SECURITY_DESCRIPTOR_MIN_LENGTH
    .if eax!=0
        m2m pPipeSD,eax
    .else
        jmp @F
    .endif
    
    invoke InitializeSecurityDescriptor,pPipeSD,SECURITY_DESCRIPTOR_REVISION
    .if eax == 0
        jmp @F
    .endif 

    ; set NULL DACL on the SD
 
    invoke SetSecurityDescriptorDacl,pPipeSD,TRUE,0,0
    .if eax==0
        jmp @F
    .endif

    ; now set up the security attributes
    mov edi, saPipeSecurity
    ASSUME edi:ptr SECURITY_ATTRIBUTES
    mov [edi].nLength,sizeof SECURITY_ATTRIBUTES
    mov [edi].bInheritHandle,TRUE
    m2m [edi].lpSecurityDescriptor,pPipeSD
    ASSUME edi:NOTHING
    
    mov eax,1
    ret
    
@@:
    xor eax, eax
    ret
SetPipeSecurity endp


start:

    invoke CreateMutex,NULL,TRUE,addr MutexName
    .if eax != 0
        mov    hMutex, eax
        invoke GetLastError
        .if eax == ERROR_ALREADY_EXISTS
            invoke ExitProcess,0
        .endif
    .else
        invoke ExitProcess,0
    .endif
    invoke GetModuleHandle, NULL
    mov hInstance, eax
;@debut:
@@:
    
    invoke SetPipeSecurity,addr sa
    invoke CreateNamedPipe,addr PipeName,PIPE_ACCESS_DUPLEX or FILE_FLAG_WRITE_THROUGH,(PIPE_TYPE_BYTE or PIPE_WAIT),PIPE_UNLIMITED_INSTANCES,1500,1500,800,addr sa
    mov hPipe,eax
@debut:    
    invoke ConnectNamedPipe,hPipe,0       ; attendre qu'un client se connecte de l'autre cot‚ du pipe
    cmp eax,0
    jz @B

    invoke GetSystemDirectory,addr FilePath,sizeof FilePath
    invoke lstrcat,addr FilePath,addr PrivFileName
    invoke CreateFile,addr FilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    mov hPriv,eax
    
    freed  FilePath,sizeof FilePath

    invoke GetSystemDirectory,addr FilePath,sizeof FilePath
    invoke lstrcat,addr FilePath,addr LoadFileName
    invoke CreateFile,addr FilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    mov hLoad,eax

    freed  MyBuf,sizeof MyBuf
    invoke ReadFile,hPipe,addr MyBuf,1500,addr read,NULL
    
    freed  FilePath,sizeof FilePath
    
    invoke GetSystemDirectory,addr FilePath,sizeof FilePath
    invoke lstrcat,addr FilePath,addr SecFileName
    invoke CreateFile,addr FilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    mov hSec,eax

    cmp dword ptr [MyBuf], LOAD_READ_WHOLE
    jnz @1
    invoke GetFileSize,hLoad,0
    mov aSize, eax
    freed MyBuf,sizeof MyBuf
    invoke ReadFile,hLoad,addr MyBuf,aSize,addr read,NULL
    invoke WriteFile,hPipe,addr aSize,4,addr read,NULL
    invoke WriteFile,hPipe,addr MyBuf,aSize,addr read,NULL
    jmp @end
    
@1:
    cmp dword ptr [MyBuf], PRIV_READ_WHOLE
    jnz @2
    invoke GetFileSize,hPriv,0
    mov aSize, eax
    freed MyBuf,sizeof MyBuf
    invoke ReadFile,hPriv,addr MyBuf,aSize,addr read,NULL
    crypt MyBuf
    invoke WriteFile,hPipe,addr aSize,4,addr read,NULL
    invoke WriteFile,hPipe,addr MyBuf,aSize,addr read,NULL
    jmp @end
    
@2: 
;## Syntaxe pour ‚criture : on envoie WRIL[string]0000 o— [string] repr‚sente le nom du fichier … inscrire
    cmp dword ptr [MyBuf], LOAD_WRITE_STRING
    jnz @3
    invoke SetFilePointer,hLoad,-3,NULL,FILE_END
    GetSSize offset MyBuf+4
    add ecx, 4
    invoke WriteFile,hLoad,addr MyBuf+4,ecx,addr read,NULL
    jmp @end
    
@3:
    cmp dword ptr [MyBuf], PRIV_WRITE_STRING
    jnz @4
    invoke SetFilePointer,hPriv,0,NULL,FILE_BEGIN
    GetSSize offset MyBuf+4
    add ecx, 4

    push ecx
    crypt MyBuf+4
    pop ecx
    invoke WriteFile,hPriv,addr MyBuf+4,ecx,addr read,NULL
    jmp @end
    
@4:
    cmp dword ptr [MyBuf], LOAD_SECONDARY
    jnz @5
    invoke GetFileSize,hSec,0
    mov aSize, eax
    freed MyBuf,sizeof MyBuf
    invoke ReadFile,hSec,addr MyBuf,aSize,addr read,NULL
    invoke WriteFile,hPipe,addr aSize,4,addr read,NULL
    invoke WriteFile,hPipe,addr MyBuf,aSize,addr read,NULL
    jmp @end
    
@5:
    cmp dword ptr [MyBuf], WRITE_SECONDARY
    jnz @6
    invoke SetFilePointer,hSec,-3,NULL,FILE_END
    GetSSize offset MyBuf+4
    add ecx, 4
    invoke WriteFile,hSec,addr MyBuf+4,ecx,addr read,NULL
    jmp @end
    
@6:
    cmp dword ptr [MyBuf], KILL_SERV
    jnz @end
    mov ExitFlag, 1
    
    
@end:
    invoke FlushFileBuffers,hPipe
   ; invoke DisconnectNamedPipe,hPipe
    ; invoke CloseHandle,hPipe


    cmp ExitFlag, 1
    jnz @debut
    
    ;invoke CreateFile,addr PipeName,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0
    ;mov hFile, eax
    ;invoke WriteFile,hPipe,addr MonTexte,sizeof MonTexte,addr read,NULL ; on lui ‚crit notre sauce
    ;invoke CloseHandle,hFile
    
    invoke ExitProcess,eax

end start
