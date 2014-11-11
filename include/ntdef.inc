comment ^
Module Name:
    ntdef.inc

Abstract:
    Type definitions for the basic types.

Author:
    Four-F (four-f@mail.ru)   11-Oct-2002
^

TRUE		equ 1
FALSE		equ 0
NULL		equ 0

NTSTATUS	typedef		DWORD

CHAR		typedef		BYTE
PCHAR		typedef		PTR BYTE
UCHAR		typedef		BYTE
PUCHAR		typedef		PTR BYTE
WCHAR		typedef		WORD


CALLBACK	typedef		proto STDCALL
WINAPI		typedef		proto STDCALL
NTAPI		typedef		proto STDCALL

BOOL		typedef		DWORD
BOOLEAN		typedef		BYTE
PBOOLEAN	typedef		PTR BYTE
PBYTE		typedef		PTR BYTE
PWORD		typedef		PTR DORD
PDWORD		typedef		PTR DWORD
PVOID		typedef		PTR
PCVOID		typedef		PTR
PSTR		typedef		PTR BYTE
PCSTR		typedef		PTR BYTE
PWSTR		typedef		PTR WORD
PSZ			typedef		PTR BYTE
PCSZ		typedef		PTR BYTE
PDWORD_PTR	typedef		PTR DWORD

UINT		typedef		DWORD

LONG		typedef		SDWORD
PLONG		typedef		PTR SDWORD
ULONG		typedef		DWORD
PULONG		typedef		PTR DWORD

USHORT		typedef		WORD
USHORT		typedef		WORD

DOUBLE		typedef		QWORD

HANDLE		typedef		DWORD
HFILE		typedef		DWORD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Unicode strings are counted 16-bit character strings. If they are
; NULL terminated, Length does not include trailing NULL.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IFNDEF UNICODE_STRING
	UNICODE_STRING STRUCT
		woLength		WORD	?		; len of string in bytes (not chars)
		MaximumLength	WORD	?		; len of Buffer in bytes (not chars)
		Buffer			PWSTR	?		; pointer to string
	UNICODE_STRING ENDS
	PUNICODE_STRING	typedef	PTR UNICODE_STRING
ENDIF

; Counted String

_STRING STRUCT
	woLength		WORD	?
	MaximumLength	WORD	?
	Buffer			DWORD	?	; PCHAR
_STRING ENDS
PSTRING typedef PTR _STRING

LARGE_INTEGER UNION
  struct
    LowPart    DWORD ?
    HighPart   DWORD ?
  ends
  struct u
    LowPart    DWORD ?
    HighPart   DWORD ?
  ends
  QuadPart     QWORD ?
LARGE_INTEGER ENDS
PLARGE_INTEGER typedef PTR LARGE_INTEGER

ULARGE_INTEGER UNION
  struct
    LowPart    DWORD ?
    HighPart   DWORD ?
  ends
  struct u
    LowPart    DWORD ?
    HighPart   DWORD ?
  ends
  QuadPart     QWORD ?
ULARGE_INTEGER ENDS
PULARGE_INTEGER	typedef	PTR ULARGE_INTEGER

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Doubly linked list structure.  Can be used as either a list head, or
;  as link words.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

LIST_ENTRY STRUCT
	Flink	DWORD	?	; PTR LIST_ENTRY
	Blink	DWORD	?	; PTR LIST_ENTRY
LIST_ENTRY ENDS
PLIST_ENTRY typedef PTR LIST_ENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Singly linked list structure. Can be used as either a list head, or
;  as link words.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SINGLE_LIST_ENTRY STRUCT
	Next	DWORD	?	; PTR SINGLE_LIST_ENTRY
SINGLE_LIST_ENTRY ENDS
PSINGLE_LIST_ENTRY typedef PTR SINGLE_LIST_ENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Valid values for the Attributes field
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJ_INHERIT             equ 00000002h
OBJ_PERMANENT           equ 00000010h
OBJ_EXCLUSIVE           equ 00000020h
OBJ_CASE_INSENSITIVE    equ 00000040h
OBJ_OPENIF              equ 00000080h
OBJ_OPENLINK            equ 00000100h
OBJ_KERNEL_HANDLE       equ 00000200h
OBJ_VALID_ATTRIBUTES    equ 000003F2h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Object Attributes structure
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJECT_ATTRIBUTES STRUCT		; sizeof = 18h
	dwLength					DWORD			? ; original name Length
	RootDirectory				HANDLE			?
	ObjectName					PUNICODE_STRING	?
	Attributes					DWORD			?
	SecurityDescriptor			PVOID			? ; Points to type SECURITY_DESCRIPTOR
	SecurityQualityOfService	PVOID			? ; Points to type SECURITY_QUALITY_OF_SERVICE
OBJECT_ATTRIBUTES ENDS
POBJECT_ATTRIBUTES typedef OBJECT_ATTRIBUTES

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        
$IsAddr2 MACRO Operand:REQ
	local a
	a = 0

	IF @SizeStr(<Operand>) GT 5		
		IFIDNI <addr >, @SubStr(<Operand>, 1 , 5)
			a = 1
		ENDIF
	ENDIF

	IF a
        EXITM <-1>
    ELSE
        EXITM <0>
    ENDIF
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

InitializeObjectAttributes MACRO p:REQ, n:REQ, a:REQ, r:REQ, s:REQ

;; Be very carefull with this macro !!!
;; It can contain some hiden bugs !!!
;; In ambiguous cases fill OBJECT_ATTRIBUTES structure manually

	local adr, reax, reax
	reax = 0
	recx = 0

	IF (OPATTR (p)) AND 00010000y
		;; is a register value
		IFDIFI <p>, <ecx>		;; not ecx
			mov ecx, p
		ENDIF
	ELSE
		IF $IsAddr2(p)
			adr SUBSTR <p>, 6
			IF $IsStack(adr)			;; is relative to SS
				lea ecx, adr
				recx = 1				;; no more eax
			ELSE
				mov ecx, offset adr
			ENDIF
		ENDIF
	ENDIF

	PUSHCONTEXT ASSUMES
	assume ecx:ptr OBJECT_ATTRIBUTES
	mov [ecx].dwLength, sizeof OBJECT_ATTRIBUTES

	IF (OPATTR (r)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push r
		pop [ecx].RootDirectory
	ELSE
		IF r EQ 0
			and [ecx].RootDirectory, 0	;; NULL
		ELSE
			mov [ecx].RootDirectory, r
		ENDIF
	ENDIF

	IF (OPATTR (a)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push a
		pop [ecx].Attributes
	ELSE
		IF r EQ 0
			and [ecx].Attributes, 0
		ELSE
			mov [ecx].Attributes, r
		ENDIF
	ENDIF

	IF (OPATTR (n)) AND 00010000y
		;; is a register value
		;IFDIFI <n>, <eax>		;; not eax
			mov [ecx].ObjectName, n
		;ENDIF
	ELSE
		IF $IsAddr2(n)
			adr SUBSTR <n>, 6
			IF $IsStack(adr)			;; is relative to SS
				lea eax, adr
				mov [ecx].ObjectName, eax
				reax = 1				;; no more eax
			ELSE
				mov [ecx].ObjectName, offset adr
			ENDIF

		ELSEIF (OPATTR (n)) AND 00000010y
			;; is a memory variable or has a relocatable data label
			push n
			pop [ecx].ObjectName
		ENDIF
	ENDIF

	IF (OPATTR (s)) AND 00000010y
		;; is a memory variable or has a relocatable data label
		push s
		pop [ecx].SecurityDescriptor
	ELSE
		IF r EQ 0
			and [ecx].SecurityDescriptor, 0	;; NULL
		ELSE
			mov [ecx].SecurityDescriptor, r
		ENDIF
	ENDIF

	and [ecx].SecurityQualityOfService, 0	;; NULL
	assume ecx:nothing
	POPCONTEXT ASSUMES
ENDM