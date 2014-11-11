.const
    m2m MACRO M1, M2
        push M2
        pop  M1
    ENDM
    
    RGB macro red, green, blue
    xor eax, eax
    mov ah, blue
    shl eax, 8
    mov ah, green
    mov al, red
    endm
    
    EnText MACRO text:req
		LOCAl cbyte
		cbyte=53h
		FORC pbyte, <&text>
			abyte=cbyte XOR '&pbyte'
			db abyte
		ENDM
	ENDM