include \masm32\include\kernel32.inc
includelib \masm32\lib\kernel32.lib

.const
ExeName2PID PROTO :DWORD,:DWORD,:DWORD
ErrorNotFound	db "Sorry but the process could not be found !",0

.data
hSnSh		dd 0

.code
;------------------------------------------------ Tryes to find the process whose name is designed in the "ExeName" string    
ExeName2PID proc pExeName:DWORD, pPE32:DWORD,Alert:DWORD

FindProcess:    
		pushad
		mov esi, pPE32
		ASSUME esi:ptr PROCESSENTRY32
                invoke CreateToolhelp32Snapshot,TH32CS_SNAPPROCESS,0
                mov hSnSh,eax
                mov [esi].dwSize, sizeof PROCESSENTRY32
                invoke Process32First,eax,esi
                lea edi, dword ptr [esi].szExeFile
                invoke lstrcmpi,pExeName,edi
                cmp eax, 0
                jz ProcFound
re_Search:
		invoke Process32Next,hSnSh,esi
		cmp eax, FALSE
		jz NotFound
		lea edi, dword ptr [esi].szExeFile
		invoke lstrcmpi,pExeName,edi
                cmp eax, 0
                jnz re_Search	
ProcFound:
		mov eax, [esi].th32ProcessID
		mov hSnSh, eax
		popad
		mov eax, hSnSh
		jmp endExeName2PID
NotFound:
        cmp Alert, 0
        jz NoMessage
		invoke MessageBoxA,NULL,addr ErrorNotFound,NULL,NULL
NoMessage:
		popad
		xor eax, eax
endExeName2PID:
		ret 08
ExeName2PID endp