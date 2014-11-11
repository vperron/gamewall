
.586p
.MODEL flat,stdcall	
option casemap:none	
ASSUME fs:NOTHING

include	\masm32\include\gdi32.inc
include	\masm32\include\kernel32.inc
include	\masm32\include\user32.inc
include	\masm32\include\advapi32.inc
include	\masm32\include\windows.inc	
include	\masm32\include\masm32.inc

include	\masm32\modules\macros.inc

include	data.inc

includelib \masm32\lib\gdi32.lib
includelib \masm32\lib\kernel32.lib	
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib	
includelib \masm32\lib\masm32.lib


include	\masm32\modules\filedlgs.asm
.code
InjectBegin:
nop
nop
nop
nop
MyReminderBox proc MsgHnd:DWORD
    mov eax, MsgHnd
    pushad
	jmp	@F
	TheText db "Ceci est une version d'évaluation du GameWall",10,13,"Acquérez la version complète pour 20 euros seulement !",0
	TheTitle db "Achetez le GameWall !",0
@@:	
    call __1
__1:
    pop esi
    sub esi, offset __1
    push 0
    lea ebx, [esi+offset TheTitle]
    push ebx
    lea ebx, [esi+offset TheText]
    push ebx
    push 0
	call eax
	popad
	ret
MyReminderBox endp
InjectEnd:

start:

    invoke GetModuleHandle,0	
    mov	  hInstance,eax	
	

    invoke GetFileName,NULL,addr	AppName,addr filter	
    cmp eax,0
    jnz coucou
    
    invoke GetFileName,NULL,addr	AppName,addr info	
    cmp eax,0
    jz  NonAuPatch
    invoke CreateFile,addr szFileName,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL
    mov FileHandle, eax
    invoke SetFilePointer,FileHandle,0,0,FILE_BEGIN
    invoke ReadFile,FileHandle,addr ReadBuf,ToRead,addr read,NULL
    invoke nrandom,99999999d
    bswap eax
    xor eax, 'FUCK'
    rol eax, 13
    bswap eax
    push eax
    rdtsc
    pop edx
    xor eax, edx
    and eax, 99999999d
    mov    VarKey, eax
    invoke wsprintf,addr newbuf,addr Format,eax
    invoke GetPathOnly,addr szFileName,addr szLicName
    invoke lstrcat,addr szLicName,addr newbuf
    invoke CreateFile,addr szLicName,GENERIC_READ or GENERIC_WRITE	,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
	mov	   hLic, eax
	invoke SetFilePointer,hLic,0,0,FILE_BEGIN
	;-------- Here encrypt the read	buffer
    lea    esi, ReadBuf	
    mov    edi, ToRead
    add    edi, esi	
	mov	   eax,	VarKey	
	mov	   ebx,	eax	
	xor	   ecx,	ecx	
	mov	   cl, bl
	shr	   ebx,	8
	add	   cl, bl
	shr	   ebx,	8
	add	   cl, bl
	shr	   ebx,	8
	add	   cl, bl

@@:	
	mov	edx, dword ptr [esi]
	rol	edx, cl	
	xor	edx, eax
	mov	dword ptr [esi], edx
	add	esi, 4
	cmp	esi, edi
	jnae @B	
		
    invoke WriteFile,hLic,addr ReadBuf,ToRead,addr read,NULL
    invoke CloseHandle,hLic	
    invoke CloseHandle,FileHandle
    invoke MessageBoxA,NULL,addr	Successful,addr	AppName,NULL
    jmp NonAuPatch
coucou: 	

    invoke CreateFile,addr szFileName,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL
    cmp eax,-1
    jnz	@F
        invoke MessageBoxA,NULL,addr ErrorOpening,addr AppName,NULL
        jmp NonAuPatch
@@: 	
    mov FileHandle, eax
	  
    invoke SetFilePointer,FileHandle,ToMove,NULL,FILE_BEGIN
    invoke ReadFile,FileHandle,addr ReadBuf,ToRead,addr read,NULL
	
    invoke GetPathOnly,addr szFileName,addr szLicName
    invoke lstrcat,addr szLicName,addr LicFile
    invoke CreateFile,addr szLicName,GENERIC_READ or GENERIC_WRITE	,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
	mov	   hLic, eax
	
	invoke GetPathOnly,addr szFileName,addr szLicName
    invoke lstrcat,addr szLicName,addr BlankFile
    invoke CreateFile,addr szLicName,GENERIC_READ or GENERIC_WRITE	,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
	mov	   hINFO, eax
	invoke SetFilePointer,hINFO,0,0,FILE_BEGIN
	invoke WriteFile,hINFO,addr ReadBuf,ToRead,addr read,NULL
    invoke CloseHandle,hINFO

;-------- Here encrypt the read	buffer
    lea    esi, ReadBuf	
    mov    edi, ToRead
    add    edi, esi	
	mov	   eax,	Key	
	mov	   ebx,	eax	
	xor	   ecx,	ecx	
	mov	   cl, bl
	shr	   ebx,	8
	add	   cl, bl
	shr	   ebx,	8
	add	   cl, bl
	shr	   ebx,	8
	add	   cl, bl

@@:	
	mov	edx, dword ptr [esi]
	rol	edx, cl	
	xor	edx, eax
	mov	dword ptr [esi], edx
	add	esi, 4
	cmp	esi, edi
	jnae @B	
		
    invoke WriteFile,hLic,addr ReadBuf,ToRead,addr read,NULL
    invoke CloseHandle,hLic	
	
    lea esi,	ReadBuf	
    mov ecx,	ToRead
@@:	
	mov	byte ptr [esi],	0
	inc	esi	
	dec	ecx	
	jnz	@B
    lea esi,	ReadBuf	
    lea edi,    InjectBegin
    mov ecx,	InjectEnd-InjectBegin
@@:
    mov al, byte ptr [edi]
    mov byte ptr [esi], al
    inc esi
    inc edi
    dec ecx
    jnz @B
	
    invoke SetFilePointer,FileHandle,ToMove,NULL,FILE_BEGIN
    invoke WriteFile,FileHandle,addr ReadBuf,ToRead,addr read,NULL
    invoke CloseHandle,FileHandle
    invoke MessageBoxA,NULL,addr	Successful,addr	AppName,NULL
NonAuPatch:	  
    invoke ExitProcess,eax

end	start
