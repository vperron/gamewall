
.code

MyGetProcAddress proc hImgBase:DWORD, pApiName:DWORD, Ordinal:DWORD
LOCAL nNamesCount:DWORD
LOCAL nFuncCount:DWORD
LOCAL AoN:DWORD
LOCAL AoNO:DWORD
LOCAL AoF:DWORD
LOCAL i:DWORD
LOCAL strSize:DWORD
        pushad
        cmp pApiName, 0
        jz ImportByOrdinal
        mov edi, pApiName
@@:
        mov al, byte ptr [edi]
        cmp al,0
        jz @F
        inc edi
        jmp @B
@@:
        sub edi,pApiName
        mov strSize,edi
ImportByOrdinal:
        mov esi, hImgBase
        mov eax, dword ptr [esi+3Ch]
        mov hImgBase, esi
        add esi, eax
        assume esi:ptr IMAGE_NT_HEADERS
        mov eax, [esi].OptionalHeader.DataDirectory.VirtualAddress
        assume esi:nothing
        mov esi, hImgBase
        add esi, eax
        assume esi:ptr IMAGE_EXPORT_DIRECTORY
        mov eax, [esi].NumberOfNames
        mov nNamesCount, eax
        mov eax, [esi].NumberOfFunctions
        mov nFuncCount, eax
        mov eax, [esi].AddressOfNames
        mov ebx, hImgBase
        add eax, ebx
        mov AoN, eax
        mov eax, [esi].AddressOfNameOrdinals
        add eax, ebx
        mov AoNO, eax
        mov eax, [esi].AddressOfFunctions
        add eax, ebx
        mov AoF, eax
        cmp pApiName,0
        jnz ProcessWithNames
        mov eax, Ordinal
        mov edi, AoF
        mov eax, dword ptr [edi+eax*4]
        add eax, hImgBase
        mov i, eax
        popad
        mov eax, i
        ret 0Ch        
        
ProcessWithNames:
        assume esi:nothing
        xor ecx, ecx
        mov edx, nNamesCount
        mov i, ecx
SearchAnotherName:
        mov ecx, i
        mov eax, AoN
        mov esi, dword ptr [eax+ecx*4]
        add esi, ebx
        mov edi, pApiName
        mov ecx, strSize
        repe cmpsb
        jz FoundTheApi
        inc i
        dec edx
        jz FatalError
        jmp SearchAnotherName
FoundTheApi:
        mov ecx, i
        mov eax, AoNO
        xor edx, edx
        mov dx, word ptr [eax+ecx*2] 
        mov eax, AoF
        mov ecx, edx
        mov edx, dword ptr [eax+ecx*4]
        add edx, ebx
        mov i, edx
        popad
        mov eax, i
        ret 0Ch
FatalError:
        popad
        xor eax, eax
        ret 0Ch
MyGetProcAddress endp
