;********************************************************************
;* RAW2DB v1.02 - Written by Lucifer48 [PC]	    	            *
;********************************************************************
.486p                           	;486 instruction set enable
.model flat, stdCALL
include monwin32.inc                    ;mes constantes

;*******************************************************************
;*  RESOURCE EQUATES                                               *
;*******************************************************************
DIALOG_MAIN	equ 4848
MY_ICON		equ 4849

IDC_GO		equ 3000
IDC_ABOUT	equ 3001
ID_OPTION1_X	equ 3002
ID_OPTION1_x	equ 3003
ID_OPTION1_d	equ 3004
ID_OPTION1_xc	equ 3005
ID_OPTION2_db	equ 3006
ID_OPTION2_dw	equ 3007
ID_OPTION2_dd	equ 3008
ID_OPTION2_dq	equ 3009
IDC_OPTION3	equ 3010
IDC_OPTION4	equ 3011
IDC_OPTION5	equ 3012
IDC_OPTION6	equ 3013

;*******************************************************************
;*  DATA                                                           *
;*******************************************************************
         .data
         
lu48class_hInst	dd 0                    ;hInst

;/* About box stuff */
mamsgbox	MSGBOXPARAMS <Size MSGBOXPARAMS, ?, ?, offset about_text, offset about_title, MB_OK+MB_USERICON, MY_ICON, 0, 0, LANG_NEUTRAL>
about_text	db "Thanks for using this little proggy ;)",0Ah,0Dh, "Last Compiled: ", ??date, " (", ??time, ").", 0
about_title	db "About...",0

;/* Default settings */
def_opt3	db "16",0
def_opt4	db ",",0

;/* Option Check - Errors */
check0	db "Raw2db v1.02", 0
check1	db "Please, enter a separator string.",0Ah,0Dh,"(default: ",22h,",",22h,")", 0
check2	db "Please, enter a positive number.",0Ah,0Dh,"(default: ",22h,"16",22h,")", 0
check3	db "Not enough memory (VirtualAlloc).", 0
check4	db "Can't create new file (CreateFileA).", 0
check5	db "Done, the ascii file has been written :)",0		;end job msgbox

;/* misc datas */
mask_decalage	dd 0		;oui,oui... on peut facilement trouver une relation avec "quantite"...
mask_X		dd (offset mask_wsprintf1), (offset mask_wsprintf5), (offset mask_wsprintf9), (offset mask_wsprintf13)
mask_x		dd (offset mask_wsprintf2), (offset mask_wsprintf6), (offset mask_wsprintf10), (offset mask_wsprintf14)
mask_d		dd (offset mask_wsprintf3), (offset mask_wsprintf7), (offset mask_wsprintf11), (offset mask_wsprintf15)
mask_xc		dd (offset mask_wsprintf4), (offset mask_wsprintf8), (offset mask_wsprintf12), (offset mask_wsprintf16)

mask_wsprintf1	db "%s%03Xh",0		;option1 (db)
mask_wsprintf2	db "%s%03xh",0
mask_wsprintf3	db "%s%03d",0
mask_wsprintf4	db "%s0x%02X",0
mask_wsprintf5	db "%s%05Xh",0		;option1 (dw)
mask_wsprintf6	db "%s%05xh",0
mask_wsprintf7	db "%s%05d",0
mask_wsprintf8	db "%s0x%04X",0
mask_wsprintf9	db "%s%09Xh",0		;option1 (dd)
mask_wsprintf10	db "%s%09xh",0
mask_wsprintf11	db "%s%010lu",0
mask_wsprintf12	db "%s0x%08X",0
mask_wsprintf13	db "%s%09X%08Xh",0	;option1 (dq)
mask_wsprintf14	db "%s%09x%08xh",0
mask_wsprintf15	db "%s%d[don't use it]%d",0	;problem with that..
mask_wsprintf16	db "%s0x%08X%08X",0

unite1		db "db ",0	;option2
unite2		db "dw ",0
unite3		db "dd ",0
unite4		db "dq ",0
quantite	dd 0		;contient 1=db, 2=dw, 4=dd, 8=dq
retourligne	db 0Dh,0Ah

;/* GetOpenFileName stuff */
filterstr	db "All Files (*.*)",0,"*.*",0,0
rep_courant	db ".",0
Open_LOAD_title	db "Load Script",0
Open_buffer	db Open_buffer_taille dup(0), 0
Open_buffer_taille	equ 255
OpenLOAD	OPENFILENAME <Size OPENFILENAME, ?, ?, offset filterstr, 0, 0, 0, offset Open_buffer, \
			      Open_buffer_taille, 0, 0, offset rep_courant, offset Open_LOAD_title, \
			      OFN_FILEMUSTEXIST+OFN_PATHMUSTEXIST+OFN_LONGNAMES+OFN_EXPLORER+OFN_HIDEREADONLY, \
			      0, 0, 0, 0, 0, 0>		;deux trucs à remplir: hwnd & hinstance
;/* File stuff */
lus		dd 0		;(utilisé par ReadFile) - non utilisé
hFichier	dd 0		;hwnd du fichier à traiter
tailleFichier	dd 0		;taille du fichier source
pmemFichier	dd 0		;pointeur de VirtualAlloc
hDestFichier	dd 0

outputextension db ".txt", 0	;default output extension

	.data?
;/* User preferences */
option1		dd ?		;pointeur vers mask
option2		dd ?
option3		dd ?		;nombre d'"item" par ligne.
option4		db 90h dup (?)	;séparateur
option5		db 90h dup (?)	;end of line
option5_size	dd ?		;strlen(option5)
option6		dd ?		;0 or 1 (little/big endian)

tempbuffer	db 100h dup(?)	;enough big to contain "option4".

;*******************************************************************
;*  CODE                                                           *
;*******************************************************************
         .code
main:                                   ;le code commence ici
	call	GetModuleHandle, NULL
	mov	[lu48class_hInst], eax
	
	call	DialogBoxParamA, lu48class_hInst, DIALOG_MAIN, 0, offset DialogWndProc, 0
	call	ExitProcess, NULL
    	call	InitCommonControls	;comctl32.dll

;***********************************************************************
;* MAIN DIALOG                                                         *
;***********************************************************************
DialogWndProc	proc	hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD

	cmp	wmsg, WM_INITDIALOG
	jz	wminitdialog
	cmp	wmsg, WM_COMMAND
	jz	wmcommand
	cmp	wmsg,WM_CLOSE
	jz	wmclose
      	mov     eax, FALSE       
	ret

;--------------
wminitdialog:
	call	SendDlgItemMessage, hwnd, IDC_OPTION4, WM_SETTEXT, 0, offset def_opt4
	call	SendDlgItemMessage, hwnd, IDC_OPTION3, WM_SETTEXT, 0, offset def_opt3
	call	CheckRadioButton, hwnd, ID_OPTION1_X, ID_OPTION1_d, ID_OPTION1_X
	call	CheckRadioButton, hwnd, ID_OPTION2_db, ID_OPTION2_dd, ID_OPTION2_db

	;termine de remplir la structure MSGBOXPARAMS
	push	hwnd
	pop	[mamsgbox].msg_hwndOwner
	push	lu48class_hInst
	pop	[mamsgbox].msg_hInstance

	;termine de remplir la structure OPENFILENAME
	push	hwnd
	pop	[OpenLOAD.hwndOwner]
	push	lu48class_hInst
	pop	[OpenLOAD.hInstance]

        mov	eax, TRUE
        ret
;--------------
wmcommand:
	mov	eax,wparam
	cmp	ax, 2		;IDCANCEL (user press ESC)
	jz	wmclose
	cmp	ax, IDC_ABOUT
	jz	aboutmsg
	cmp	ax, IDC_GO
	jz	gocheck

	mov	eax, TRUE
	ret                     
	;--------------
	aboutmsg:
	call	MessageBoxIndirectA, offset mamsgbox
	mov	eax, TRUE
	ret 
	;--------------
	gocheck:
	call	SendDlgItemMessage, hwnd, IDC_OPTION4, WM_GETTEXTLENGTH, 0, 0
	test	eax, eax
	jnz	nextcheck01
	call	MessageBoxA, hwnd, offset check1, offset check0, MB_ICONERROR
	jmp	endcheck
nextcheck01:
	call	GetDlgItemInt, hwnd, IDC_OPTION3, NULL, FALSE
	test	eax, eax
	jnz	nextcheck02
	call	MessageBoxA, hwnd, offset check2, offset check0, MB_ICONERROR
	jmp	endcheck	
nextcheck02:
	mov	option3, eax
	call	SendDlgItemMessage, hwnd, IDC_OPTION4, WM_GETTEXT, 100h, offset option4
	call	SendDlgItemMessage, hwnd, IDC_OPTION5, WM_GETTEXT, 100h, offset option5
	mov	dword ptr [option5+eax], 0A0Dh
	inc	eax
	inc	eax
	mov	option5_size, eax
;
; db dw dd ?
;
	call	IsDlgButtonChecked, hwnd, ID_OPTION2_db
	test	eax, eax
	jz	nextcheck2
	mov	option2, offset unite1
	mov	mask_decalage, 0
	mov	quantite, 1
	jmp	nextcheck1
nextcheck2:
	call	IsDlgButtonChecked, hwnd, ID_OPTION2_dw
	test	eax, eax
	jz	nextcheck3
	mov	option2, offset unite2
	mov	mask_decalage, 1
	mov	quantite, 2
	jmp	nextcheck1
nextcheck3:
	call	IsDlgButtonChecked, hwnd, ID_OPTION2_dd
	test	eax, eax
	jz	nextcheck4
	mov	option2, offset unite3
	mov	mask_decalage, 2
	mov	quantite, 4
	jmp	nextcheck1
nextcheck4:
	mov	mask_decalage, 3
	mov	quantite, 8
	mov	option2, offset unite4
nextcheck1:
;
; %X %x %d 0x%X ?
;
	call	IsDlgButtonChecked, hwnd, ID_OPTION1_X
	test	eax, eax
	jz	nextcheck6
	mov	eax, mask_decalage
	mov	eax, dword ptr [4*eax+mask_X]
	mov	option1, eax
	jmp	nextcheck5
nextcheck6:
	call	IsDlgButtonChecked, hwnd, ID_OPTION1_x
	test	eax, eax
	jz	nextcheck7
	mov	eax, mask_decalage
	mov	eax, dword ptr [4*eax+mask_x]
	mov	option1, eax
	jmp	nextcheck5
nextcheck7:
	call	IsDlgButtonChecked, hwnd, ID_OPTION1_d
	test	eax, eax
	jz	nextcheck8
	mov	eax, mask_decalage
	mov	eax, dword ptr [4*eax+mask_d]
	mov	option1, eax
	jmp	nextcheck5
nextcheck8:
	mov	eax, mask_decalage
	mov	eax, dword ptr [4*eax+mask_xc]
	mov	option1, eax
nextcheck5:
;
; Byte order : little or big endian ?
;
	call	IsDlgButtonChecked, hwnd, IDC_OPTION6
	mov	option6, eax
;
	call	cestbonmaintenant		;let's go ;)
endcheck:
	mov	eax, TRUE
	ret 
;--------------
wmclose:
	call	EndDialog, hwnd, NULL
        mov	eax, TRUE
        ret

DialogWndProc	endp

;*******************************************************************
;* Important proc						   *
;*******************************************************************
readitem	 proc
	.if quantite==1
	movzx	eax, byte ptr [edi]
	.elseif quantite==2
	movzx	eax, word ptr [edi]
	  .if (option6!=0)
	   rol	ax, 8
	  .endif
	.elseif quantite==4
	mov	eax, dword ptr [edi]
	  .if (option6!=0)
	   bswap eax
	  .endif
	.else
	mov	eax, dword ptr [edi]
	mov	ecx, dword ptr [edi+4]
	  .if (option6!=0)
	   bswap eax
	   bswap ecx
	   xchg	eax, ecx
	  .endif
	.endif
	ret
readitem	 endp
;*******************************************************************
cestbonmaintenant	proc
			uses	edi, esi, ebx
;
; Select the file to convert
;
	mov	byte ptr [Open_buffer], 0	;efface le nom précédent
	call	GetOpenFileName, offset OpenLOAD
	cmp	eax, 1
	jnz	pasouvert
	call	CreateFileA, offset Open_buffer, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
	;le fichier existe forcement !

	mov	hFichier, eax
	call	GetFileSize, eax, NULL
	mov	tailleFichier, eax
	add	eax, 7			;pad pour avoir un multiple de 8 (pour que DQ ne déborde pas).
	call	VirtualAlloc, NULL, eax, MEM_COMMIT, PAGE_READWRITE
	test	eax, eax
	jz	memoryerror
	mov	pmemFichier, eax
	call	ReadFile, hFichier, eax, tailleFichier, offset lus, NULL
	call	CloseHandle, hFichier

	;creation du fichier de destination
	movzx	eax, [OpenLOAD].nFileExtension
	test	eax, eax
	jz	noext
	mov	byte ptr [Open_buffer+eax-1], 0
noext:
	call	lstrcat, offset Open_buffer, offset outputextension

	call	CreateFileA, offset Open_buffer, GENERIC_WRITE+GENERIC_READ, 0, 0, \
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
	cmp	eax, -1
	jz	creationerror
	mov	hDestFichier, eax
;***
; "Algo"
;***
	mov	esi, tailleFichier
	mov	edi, pmemFichier
	call	WriteFile, hDestFichier, offset retourligne, 2, offset lus, 0
	jmp	premfois
boucle2:
	call	WriteFile, hDestFichier, offset option5, option5_size, offset lus, 0	;end of line
premfois:
	xor	ebx, ebx
	call	readitem		;result eax (and ecx if quantite==8)
	
	.if (quantite!=8)
	 call	_wsprintfA, offset tempbuffer, dword ptr [option1], dword ptr [option2], eax
	 add	esp, 10h
	.else
	 call	_wsprintfA, offset tempbuffer, dword ptr [option1], dword ptr [option2], ecx, eax
	 add	esp, 14h
	.endif

	call	WriteFile, hDestFichier, offset tempbuffer, eax, offset lus, 0
boucle1:
	add	edi, quantite
	sub	esi, quantite
	jbe	bouclefin
	inc	ebx
	cmp	ebx, option3
	jz	boucle2

	call	readitem		;result eax
	call	_wsprintfA, offset tempbuffer, dword ptr [option1], offset option4, eax
	add	esp, 10h
	call	WriteFile, hDestFichier, offset tempbuffer, eax, offset lus, 0
	jmp	boucle1
bouclefin:
	call	WriteFile, hDestFichier, offset retourligne, 2, offset lus, 0
;****
; fin :)
;***
	call	CloseHandle, hDestFichier
	call	VirtualFree, pmemFichier, NULL, MEM_RELEASE
	call	MessageBoxA, hwnd, offset check5, offset check0, MB_ICONINFORMATION
pasouvert:
	xor	eax,eax
	ret
memoryerror:	
	call	MessageBoxA, hwnd, offset check3, offset check0, MB_OK
	mov	eax, -1
	ret
creationerror:
	call	VirtualFree, pmemFichier, NULL, MEM_RELEASE
	call	MessageBoxA, hwnd, offset check4, offset check0, MB_OK
	mov	eax, -1
	ret
cestbonmaintenant	endp
;*******************************************************************

	end main			;end of code
