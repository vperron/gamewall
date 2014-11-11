
.486p
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


.code

start:

    invoke GetModuleHandle,0	
    mov	  hInstance,eax	
	 
    invoke CreateFile,addr szFileName,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ	or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL
    mov    hFile, eax
    invoke SetFilePointer,hFile,0,0,FILE_BEGIN
    invoke ReadFile,hFile,addr ReadBuf,sizeof ReadBuf,addr read,NULL
	
    invoke CreateFile,addr szCrypt1,GENERIC_READ or GENERIC_WRITE	,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
	mov	   hCrypt1, eax	
    invoke CreateFile,addr szCrypt2,GENERIC_READ or GENERIC_WRITE	,FILE_SHARE_READ,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL
	mov	   hCrypt2, eax	  
   

	invoke SetFilePointer,hCrypt1,0,0,FILE_BEGIN
	invoke SetFilePointer,hCrypt2,0,0,FILE_BEGIN


    lea    esi, ReadBuf	
    mov    ecx, sizeof ReadBuf
@@:
    xor    byte ptr [esi], 13h
    add    byte ptr [esi], 13h
    not    byte ptr [esi]
    inc    esi
    dec    ecx
    jnz    @B
		
    lea    esi, ReadBuf
    invoke WriteFile,hCrypt1,esi,16Bh,addr read,NULL
    add    esi, 16Bh
    mov    ecx, sizeof ReadBuf-16Bh
    invoke WriteFile,hCrypt2,esi,ecx,addr read,NULL
    invoke CloseHandle,hFile
    invoke CloseHandle,hCrypt1
    invoke CloseHandle,hCrypt2
	
    invoke ExitProcess,eax

end	start
