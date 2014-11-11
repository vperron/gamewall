; structure present at ntoskrnl!KeServiceDescriptorTable and at 'KeServiceDescriptorTableShadow'
;
SSDT struct
	pSSAT              DWORD   ?      ; System Service Address Table   ( LPVOID[] )
	Obsolete           DWORD   ?      ; or maybe: API ID base
	dwAPICount         DWORD   ?
	pSSPT              DWORD   ?      ; System Service Parameter Table ( BYTE[] )
SSDT ends

SYSTEM_THREAD_INFORMATION STRUCT  ;size THREAD_INFO_SIZE
 KernelTime       QWORD ?           ;00
 UserTime         QWORD ?           ;08
 CreateTime       QWORD ?           ;10
 TickCount        DWORD ?           ;18
 StartAddress     DWORD ?           ;1C
 ClientId         CLIENT_ID <>      ;20
 DynamicPriority  DWORD ?           ;28
 BasePriority     DWORD ?           ;2C
 ContextSwitches  DWORD ?           ;30
 State            DWORD ?           ;34
 WaitReason       DWORD ?           ;38
 Reserved3C       DWORD ?           ;3C
SYSTEM_THREAD_INFORMATION ENDS



NT4_SYSTEM_PROCESS_INFORMATION STRUCT
 SizeOfBlock                DWORD ?           ;00 NULL for info end
 ThreadCount                DWORD ?           ;04 number of threads in this block
 Reserved08                 DWORD 06H DUP (?) ;08
 CreateTime                 QWORD ?           ;20
 UserTime                   QWORD ?           ;28
 KernelTime                 QWORD ?           ;30
 ProcessName                UNICODE_STRING <> ;38
 Priority                   DWORD ?           ;40
 ProcessId                  DWORD ?           ;44
 ParentProcessId            DWORD ?           ;48
 HandleCount                DWORD ?           ;4C
 SessionId                  DWORD ?           ;50 ??
 Reserved54                 DWORD ?           ;54
 PeakVirtualSize            DWORD ?           ;58
 VirtualSize                DWORD ?           ;5C
 PageFaultCount             DWORD ?           ;60
 PeakWorkingSetSize         DWORD ?           ;64
 WorkingSetSize             DWORD ?           ;68
 QuotaPeakPagedPoolUsage    DWORD ?           ;6C
 QuotaPagedPoolUsage        DWORD ?           ;70
 QuotaPeakNonPagedPoolUsage DWORD ?           ;74
 QuotaNonPagedPoolUsage     DWORD ?           ;78
 PagefileUsage              DWORD ?           ;7C
 PeakPagefileUsage          DWORD ?           ;80
 PrivatePages               DWORD ?           ;84
 ;follows array of SYSTEM_THREAD_INFORMATIONs
 ThreadInformation SYSTEM_THREAD_INFORMATION <> ;88
NT4_SYSTEM_PROCESS_INFORMATION ENDS

NT5_SYSTEM_PROCESS_INFORMATION STRUCT
 SizeOfBlock                DWORD ?           ;00 NULL for info end
 ThreadCount                DWORD ?           ;04 number of threads in this block
 Reserved08                 DWORD 06H DUP (?) ;08
 CreateTime                 QWORD ?           ;20
 UserTime                   QWORD ?           ;28
 KernelTime                 QWORD ?           ;30
 ProcessName                UNICODE_STRING <> ;38
 Priority                   DWORD ?           ;40
 ProcessId                  DWORD ?           ;44
 ParentProcessId            DWORD ?           ;48
 HandleCount                DWORD ?           ;4C
 SessionId                  DWORD ?           ;50 ??
 Reserved54                 DWORD ?           ;54
 PeakVirtualSize            DWORD ?           ;58
 VirtualSize                DWORD ?           ;5C
 PageFaultCount             DWORD ?           ;60
 PeakWorkingSetSize         DWORD ?           ;64
 WorkingSetSize             DWORD ?           ;68
 QuotaPeakPagedPoolUsage    DWORD ?           ;6C
 QuotaPagedPoolUsage        DWORD ?           ;70
 QuotaPeakNonPagedPoolUsage DWORD ?           ;74
 QuotaNonPagedPoolUsage     DWORD ?           ;78
 PagefileUsage              DWORD ?           ;7C
 PeakPagefileUsage          DWORD ?           ;80
 PrivatePages               DWORD ?           ;84
 ReadOperationCount         QWORD ?           ;88
 WriteOperationCount        QWORD ?           ;90
 OtherOperationCount        QWORD ?           ;98
 ReadTransferCount          QWORD ?           ;A0
 WriteTransferCount         QWORD ?           ;A8
 OtherTransferCount         QWORD ?           ;B0
 ;follows array of SYSTEM_THREAD_INFORMATIONs
 ThreadInformation SYSTEM_THREAD_INFORMATION <> ;B8
NT5_SYSTEM_PROCESS_INFORMATION ENDS

IFDEF NT4
     SYSTEM_PROCESS_INFORMATION TEXTEQU <NT4_SYSTEM_PROCESS_INFORMATION>
ELSE
     SYSTEM_PROCESS_INFORMATION TEXTEQU <NT5_SYSTEM_PROCESS_INFORMATION>
ENDIF
;
; structure being built by the PUSHAD instruction on the stack
;
PUSHA_STRUCT struct 1
	_EDI               DWORD ?
	_ESI               DWORD ?
	_EBP               DWORD ?
	_ESP               DWORD ?
	_EBX               DWORD ?
	_EDX               DWORD ?
	_ECX               DWORD ?
	_EAX               DWORD ?
PUSHA_STRUCT ends


;------------CONST-----------------------------------------------------------------------
.const
CCOUNTED_UNICODE_STRING	            "\\Device\\GWallDev",       szDevPath,      4
CCOUNTED_UNICODE_STRING	            "\\??\\GWallDev",           szSymPathNT,    4
CCOUNTED_UNICODE_STRING	            "\\DosDevices\\GWallDev",   szSymPath,      4
;TEXTW szDevPath,                   <\Device\INVISIBILITY/0>
;TEXTW szSymPath,                   <\DosDevices\INVISIBILITY/0>

;------------DATA------------------------------------------------------------------------
.data
TablePresent                        dd 0
PIDCount                            dd -1                       ; nombre de PIDS à vérifier
pPIDTable                           dd -1                       ; pointeur vers la table de words contenant les pIDS
IsHooked                            dd 0
pDevObj                             PDEVICE_OBJECT 0
pNQSI                               dd -1                       ; ptr 2 ntdll!NtQuerySystemInformation
;pEW                                dd -1                       ; ptr 2 user32!EnumWindows
dwTargetPID                         dd -1
dwNQSI_NT_ID                        dd -1
pOldNtOsNQSI                        dd  0

