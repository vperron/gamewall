.code
OpenPipe proc
    

@@:
    freed ServBuf,sizeof ServBuf
    invoke lstrcat,addr ServBuf,addr IniFile.ServCompName
    invoke lstrcat,addr ServBuf,addr ClientPipeEnd
    invoke CreateFile,addr ServBuf,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0
    .if eax == -1
        invoke GetLastError
        .if eax == ERROR_PIPE_BUSY
            invoke WaitNamedPipe,addr ServBuf,NMPWAIT_USE_DEFAULT_WAIT
            cmp eax, 0
            jz @errorACF
            jmp @B
        .elseif eax == ERROR_FILE_NOT_FOUND
            jmp @errorACF
        .else
            jmp @B
        .endif
    .endif
    ret
@errorACF:
    xor eax, eax
    ret
OpenPipe endp



AccessConfigFiles proc Param:DWORD,pBuf:DWORD,OptSize:DWORD
LOCAL hPipe:DWORD
LOCAL aSize:DWORD

    cmp IniFile.IsNetwork, 1
    jnz @normal
    cmp IniFile.IsServer, 1
    jz @normal

   ; lancer openpipe dans un thread...
    invoke OpenPipe

    .if eax == 0
        jmp @errorACF
    .else
        mov hPipe, eax
    .endif
    
    mov ecx, Param
    .if (ecx==LOAD_READ_WHOLE) || (ecx==PRIV_READ_WHOLE) || (ecx==LOAD_SECONDARY)
    
    freed  CmdBuf,sizeof CmdBuf
    m2m dword ptr [CmdBuf],ecx
    invoke WriteFile,hPipe,addr CmdBuf,4,addr read,0
    freed2 pBuf, OptSize
    invoke ReadFile,hPipe,addr aSize,4,addr read,NULL 
    .if aSize != 0
        invoke ReadFile,hPipe,pBuf,aSize,addr read,NULL
    .endif
    invoke CloseHandle,hPipe 
    
    
    m2m ecx, dword ptr [CmdBuf]
    .if ecx == 2
        add ecx, 2
        jmp @after
    .endif
    
    jmp    @endACF2
    
    .else


    freed  CmdBuf,sizeof CmdBuf
    m2m    dword ptr [CmdBuf],ecx
    GetSSize pBuf
    add ecx,4
    push ecx
    mcopy pBuf, offset CmdBuf+4,ecx
    pop ecx
    invoke WriteFile,hPipe,addr CmdBuf,ecx,addr read,0
    jmp    @endACF2
    
    .endif
    
@normal:
    mov ecx, Param
@after:
    cmp ecx, LOAD_READ_WHOLE
    jnz @0
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr LaunchFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push eax
    invoke ReadFile,eax,pBuf,OptSize,addr read,NULL
    pop eax
    invoke CloseHandle, eax
    jmp @endACF2
    
@0:
    cmp ecx, LOAD_SECONDARY
    jnz @1
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr SecFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push eax
    invoke ReadFile,eax,pBuf,OptSize,addr read,NULL
    pop eax
    invoke CloseHandle, eax
    jmp @endACF2

@1:
    cmp ecx, PRIV_READ_WHOLE
    jnz @2
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr PrivFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push eax
    invoke ReadFile,eax,pBuf,OptSize,addr read,NULL
    pop eax
    invoke CloseHandle, eax
    crypt2 pBuf
    ;mov byte ptr [edi], 53h
@2:
    cmp ecx, LOAD_WRITE_STRING
    jnz @3
        GetSSize pBuf
    add ecx,4
    mov aSize, ecx
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr LaunchFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push   eax
    invoke SetFilePointer,eax,-3,NULL,FILE_END
    pop    eax
    push   eax

    invoke WriteFile,eax,pBuf,aSize,addr read,NULL
    pop    eax
    invoke CloseHandle, eax
    jmp @endACF2
@3:
    cmp ecx, PRIV_WRITE_STRING
    jnz @4
    GetSSize pBuf
    add ecx,4
    mov aSize, ecx
        crypt2 pBuf
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr PrivFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push   eax
    ;invoke SetFilePointer,eax,-3,NULL,FILE_END
    invoke SetFilePointer,eax,0,0,FILE_BEGIN
    pop    eax
    push   eax

    invoke WriteFile,eax,pBuf,aSize,addr read,NULL
    pop    eax
    invoke CloseHandle, eax
    crypt2 pBuf
    
@4:
    cmp ecx, WRITE_SECONDARY
    jnz @endACF2
        GetSSize pBuf
    add ecx,4
    mov aSize, ecx
    freed LaunchFilePath,sizeof LaunchFilePath
    invoke GetSystemDirectory,addr LaunchFilePath,sizeof LaunchFilePath
    invoke lstrcat,addr LaunchFilePath,addr SecFileName
    invoke CreateFile,addr LaunchFilePath,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE or FILE_ATTRIBUTE_HIDDEN,NULL
    push   eax
    invoke SetFilePointer,eax,0,NULL,FILE_BEGIN
    pop    eax
    push   eax

    invoke WriteFile,eax,pBuf,aSize,addr read,NULL
    pop    eax
    invoke CloseHandle, eax
    jmp @endACF2
        
@endACF2:
    mov eax, 1
    ret
    
@errorACF:
    xor eax, eax
    ret
AccessConfigFiles endp


