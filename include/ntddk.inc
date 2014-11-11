comment ^
Module Name:
    ntddk.inc

Abstract:
    This module defines the NT types, constants, and functions that are
    exposed to device drivers.

Author:
    Four-F (four-f@mail.ru)
Last update:
	15-Jan-2003

IMPORTANT:
    Some fields name have been changed because of collision with masm reserved words
^

include ntdef.inc

PSECURITY_DESCRIPTOR	typedef	PTR

KSPIN_LOCK				typedef	DWORD
KPROCESSOR_MODE			typedef	BYTE

KAFFINITY				typedef	DWORD
IFNDEF PDWORD
	PDWORD				typedef PTR DWORD
ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; I/O system definitions.
;
; Define a Memory Descriptor List (MDL)
;
; An MDL describes pages in a virtual buffer in terms of physical pages.  The
; pages associated with the buffer are described in an array that is allocated
; just after the MDL header structure itself.  In a future compiler this will
; be placed at:
;
;      ULONG Pages[];
;
; Until this declaration is permitted, however, one simply calculates the
; base of the array by adding one to the base MDL pointer:
;
;      Pages = (PULONG) (Mdl + 1);
;
; Notice that while in the context of the subject thread, the base virtual
; address of a buffer mapped by an MDL may be referenced using the following:
;
;      Mdl->StartVa | Mdl->ByteOffset
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MDL STRUCT			; sizeof= 1Ch
	Next			PVOID	?	; PTR MDL
	woSize			WORD	?	; 04h original name Size
	MdlFlags		WORD	?	; 06h
	Process			PVOID	?	; 08h PTR EPROCESS
	MappedSystemVa	PVOID	?	; 0Ch
	StartVa			PVOID	?	; 10h
	ByteCount		DWORD	?	; 14h
	ByteOffset		DWORD	?	; 18h
MDL ENDS
PMDL typedef PTR MDL

MDL_MAPPED_TO_SYSTEM_VA     equ 0001h
MDL_PAGES_LOCKED            equ 0002h
MDL_SOURCE_IS_NONPAGED_POOL equ 0004h
MDL_ALLOCATED_FIXED_SIZE    equ 0008h
MDL_PARTIAL                 equ 0010h
MDL_PARTIAL_HAS_BEEN_MAPPED equ 0020h
MDL_IO_PAGE_READ            equ 0040h
MDL_WRITE_OPERATION         equ 0080h
MDL_PARENT_MAPPED_SYSTEM_VA equ 0100h
MDL_LOCK_HELD               equ 0200h
MDL_PHYSICAL_VIEW           equ 0400h
MDL_IO_SPACE                equ 0800h
MDL_NETWORK_HEADER          equ 1000h
MDL_MAPPING_CAN_FAIL        equ 2000h
MDL_ALLOCATED_MUST_SUCCEED  equ 4000h

MDL_MAPPING_FLAGS equ MDL_MAPPED_TO_SYSTEM_VA or MDL_PAGES_LOCKED or MDL_SOURCE_IS_NONPAGED_POOL or MDL_PARTIAL_HAS_BEEN_MAPPED or MDL_PARENT_MAPPED_SYSTEM_VA or MDL_LOCK_HELD or MDL_SYSTEM_VA or MDL_IO_SPACE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  The following are masks for the predefined standard access types
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DELETE                           equ 00010000h
READ_CONTROL                     equ 00020000h
WRITE_DAC                        equ 00040000h
WRITE_OWNER                      equ 00080000h
SYNCHRONIZE                      equ 00100000h

STANDARD_RIGHTS_REQUIRED         equ 000F0000h

STANDARD_RIGHTS_READ             equ READ_CONTROL
STANDARD_RIGHTS_WRITE            equ READ_CONTROL
STANDARD_RIGHTS_EXECUTE          equ READ_CONTROL

STANDARD_RIGHTS_ALL              equ 001F0000h

SPECIFIC_RIGHTS_ALL              equ 0000FFFFh

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; AccessSystemAcl access type
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ACCESS_SYSTEM_SECURITY           equ 01000000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; MaximumAllowed access type
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MAXIMUM_ALLOWED                  equ 02000000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  These are the generic rights.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GENERIC_READ                     equ 80000000h
GENERIC_WRITE                    equ 40000000h
GENERIC_EXECUTE                  equ 20000000h
GENERIC_ALL                      equ 10000000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Define the generic mapping array.  This is used to denote the
;  mapping of each generic access right to a specific access mask.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GENERIC_MAPPING STRUCT	; sizeof = 10h
	GenericRead		DWORD	?
	GenericWrite	DWORD	?	; 04h
	GenericExecute	DWORD	?	; 08h
	GenericAll		DWORD	?	; 0Ch
GENERIC_MAPPING ENDS
PGENERIC_MAPPING typedef PTR GENERIC_MAPPING

; Priority increment definitions.  The comment for each definition gives
; the names of the system services that use the definition when satisfying
; a wait.

; Priority increment used when satisfying a wait on an executive event
; (NtPulseEvent and NtSetEvent)

EVENT_INCREMENT			equ 1

; Priority increment when no I/O has been done.  This is used by device
; and file system drivers when completing an IRP (IoCompleteRequest).

IO_NO_INCREMENT			equ 0

; Priority increment for completing CD-ROM I/O.  This is used by CD-ROM device
; and file system drivers when completing an IRP (IoCompleteRequest)

IO_CD_ROM_INCREMENT		equ 1

; Priority increment for completing disk I/O.  This is used by disk device
; and file system drivers when completing an IRP (IoCompleteRequest)

IO_DISK_INCREMENT		equ 1

; Priority increment for completing keyboard I/O.  This is used by keyboard
; device drivers when completing an IRP (IoCompleteRequest)

IO_KEYBOARD_INCREMENT	equ 6

; Priority increment for completing mailslot I/O.  This is used by the mail-
; slot file system driver when completing an IRP (IoCompleteRequest).

IO_MAILSLOT_INCREMENT	equ 2

; Priority increment for completing mouse I/O.  This is used by mouse device
; drivers when completing an IRP (IoCompleteRequest)

IO_MOUSE_INCREMENT		equ 6

; Priority increment for completing named pipe I/O.  This is used by the
; named pipe file system driver when completing an IRP (IoCompleteRequest).

IO_NAMED_PIPE_INCREMENT	equ 2

; Priority increment for completing network I/O.  This is used by network
; device and network file system drivers when completing an IRP
; (IoCompleteRequest).

IO_NETWORK_INCREMENT	equ 2

; Priority increment for completing parallel I/O.  This is used by parallel
; device drivers when completing an IRP (IoCompleteRequest)

IO_PARALLEL_INCREMENT	equ 1

; Priority increment for completing serial I/O.  This is used by serial device
; drivers when completing an IRP (IoCompleteRequest)

IO_SERIAL_INCREMENT		equ 2

; Priority increment for completing sound I/O.  This is used by sound device
; drivers when completing an IRP (IoCompleteRequest)

IO_SOUND_INCREMENT		equ 8

; Priority increment for completing video I/O.  This is used by video device
; drivers when completing an IRP (IoCompleteRequest)

IO_VIDEO_INCREMENT		equ 1

; Priority increment used when satisfying a wait on an executive semaphore
; (NtReleaseSemaphore)

SEMAPHORE_INCREMENT		equ 1

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  Time conversion routines
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

TIME_FIELDS STRUCT		; sizeof = 10h
	Year			WORD	?	; 0Eh range [1601...]
	Month			WORD	?	; 0Eh range [1..12]
	Day				WORD	?	; 0Eh range [1..31]
	Hour			WORD	?	; 0Eh range [0..23]
	Minute			WORD	?	; 0Eh range [0..59]
	Second			WORD	?	; 0Eh range [0..59]
	Milliseconds	WORD	?	; 0Eh range [0..999]
	Weekday			WORD	?	; 0Eh range [0..6] == [Sunday..Saturday]
TIME_FIELDS ENDS
PTIME_FIELDS typedef PTR TIME_FIELDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Asynchronous Procedure Call (APC) object
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KAPC STRUCT			; sizeof = 30h
	fwType			WORD		IO_TYPE_APC	; 12h
	cbSize			WORD		?			; cb
	Spare0			DWORD		?
	Thread			PVOID		?			; PTR KTHREAD
	ApcListEntry	LIST_ENTRY	<>
	KernelRoutine	PVOID		?			; KiSuspendNop
	RundownRoutine	PVOID		?
	NormalRoutine	PVOID		?			; KiSuspendThread
	NormalContext	PVOID		?

	; N.B. The following two members MUST be together.

	SystemArgument1	PVOID		?
	SystemArgument2	PVOID		?
	ApcStateIndex	CHAR		?
	ApcMode			KPROCESSOR_MODE	?
	Inserted		BOOLEAN		?
					db			?			; padding
KAPC ENDS
PKAPC typedef PTR KAPC

; Doubly linked list structure.  Can be used as either a list head, or
; as link words.
comment ^
LIST_ENTRY STRUCT
	Flink	PVOID	?	; PLIST_ENTRY
	Blink	PVOID	?	; PLIST_ENTRY
LIST_ENTRY ENDS
^
PLIST_ENTRY	typedef PTR LIST_ENTRY

; Define the format of a completion message.

IO_COMPLETION_CONTEXT STRUCT		; sizeof = 8h
	Port	PVOID	?
	Key		PVOID	?
IO_COMPLETION_CONTEXT ENDS
PIO_COMPLETION_CONTEXT typedef PTR IO_COMPLETION_CONTEXT

; The following structure is pointed to by the SectionObject pointer field
; of a file object, and is allocated by the various NT file systems.

SECTION_OBJECT_POINTERS STRUCT		; sizeof = 0Ch
	DataSectionObject	PVOID	?
	SharedCacheMap		PVOID	?
	ImageSectionObject	PVOID	?
SECTION_OBJECT_POINTERS ENDS
PSECTION_OBJECT_POINTERS typedef PTR SECTION_OBJECT_POINTERS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Common dispatcher object header
; 
; N.B. The size field contains the number of dwords in the structure.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DISPATCHER_HEADER STRUCT		; sizeof = 10h
	byType			BYTE		?	; DISP_TYPE_* (original name Type)
	Absolute		BYTE		?
	cbSize			BYTE		?	; cb
	Inserted		BYTE		?
	SignalState		DWORD		?
	WaitListHead	LIST_ENTRY	<>
DISPATCHER_HEADER ENDS

KWAIT_BLOCK STRUCT		; sizeof = 18h
	WaitListEntry	LIST_ENTRY	<>
	Thread			PVOID		?	; PTR KTHREAD
	Object			PVOID		?
	NextWaitBlock	PVOID		?	; PTR KWAIT_BLOCK
	WaitKey			WORD		?
	WaitType		WORD		?
KWAIT_BLOCK ENDS
PKWAIT_BLOCK typedef PTR KWAIT_BLOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Event object
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KEVENT STRUCT	; sizeof = 10h
	Header	DISPATCHER_HEADER	<>
KEVENT ENDS
PKEVENT typedef PTR KEVENT

KDEVICE_QUEUE STRUCT	; sizeof = 14h
	fwType			WORD		IO_TYPE_DEVICE_QUEUE; 14h
	cbSize			WORD		?					; cb
	DeviceListHead	LIST_ENTRY	<>
	ksLock			KSPIN_LOCK	?					; Lock is masm reserved symbol
	Busy			BOOLEAN		?
					db			3 dup(?)			; padding
KDEVICE_QUEUE ENDS
PKDEVICE_QUEUE	typedef PTR KDEVICE_QUEUE

KDEVICE_QUEUE_ENTRY STRUCT	; sizeof = 10h
	DeviceListEntry	LIST_ENTRY	<>
	SortKey		DWORD		?
	Inserted	BOOLEAN		?
				db			3 dup(?)			; padding
KDEVICE_QUEUE_ENTRY ENDS
PKDEVICE_QUEUE_ENTRY typedef PTR KDEVICE_QUEUE_ENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define share access rights to files and directories
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FILE_SHARE_READ                 equ 1  ; winnt
FILE_SHARE_WRITE                equ 2  ; winnt
FILE_SHARE_DELETE               equ 4  ; winnt
FILE_SHARE_VALID_FLAGS          equ 7

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the file attributes values
;
; Note:  0x00000008 is reserved for use for the old DOS VOLID (volume ID)
;         and is therefore not considered valid in NT.
;
; Note:  0x00000010 is reserved for use for the old DOS SUBDIRECTORY flag
;         and is therefore not considered valid in NT.  This flag has
;         been disassociated with file attributes since the other flags are
;         protected with READ_ and WRITE_ATTRIBUTES access to the file.
;
; Note:  Note also that the order of these flags is set to allow both the
;         FAT and the Pinball File Systems to directly set the attributes
;         flags in attributes words without having to pick each flag out
;         individually.  The order of these flags should not be changed!
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FILE_ATTRIBUTE_READONLY             equ 00000001h  ; winnt
FILE_ATTRIBUTE_HIDDEN               equ 00000002h  ; winnt
FILE_ATTRIBUTE_SYSTEM               equ 00000004h  ; winnt

FILE_ATTRIBUTE_DIRECTORY            equ 00000010h  ; winnt
FILE_ATTRIBUTE_ARCHIVE              equ 00000020h  ; winnt
FILE_ATTRIBUTE_DEVICE               equ 00000040h  ; winnt
FILE_ATTRIBUTE_NORMAL               equ 00000080h  ; winnt

FILE_ATTRIBUTE_TEMPORARY            equ 00000100h  ; winnt
FILE_ATTRIBUTE_SPARSE_FILE          equ 00000200h  ; winnt
FILE_ATTRIBUTE_REPARSE_POINT        equ 00000400h  ; winnt
FILE_ATTRIBUTE_COMPRESSED           equ 00000800h  ; winnt

FILE_ATTRIBUTE_OFFLINE              equ 00001000h  ; winnt
FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  equ 00002000h  ; winnt

; defined in windows.inc as:
;     FILE_ATTRIBUTE_ENCRYPTED equ 00000040h

;FILE_ATTRIBUTE_ENCRYPTED            equ 00004000h  ; winnt

; This definition is old and will disappear shortly

FILE_ATTRIBUTE_CONTENT_INDEXED  equ FILE_ATTRIBUTE_NOT_CONTENT_INDEXED

FILE_ATTRIBUTE_VALID_FLAGS      equ 00007fb7h
FILE_ATTRIBUTE_VALID_SET_FLAGS  equ 000031a7h

; Define the create disposition values

FILE_SUPERSEDE                  equ 0
FILE_OPEN                       equ 1
FILE_CREATE                     equ 2
FILE_OPEN_IF                    equ 3
FILE_OVERWRITE                  equ 4
FILE_OVERWRITE_IF               equ 5
FILE_MAXIMUM_DISPOSITION        equ 5

; Define the create/open option flags

FILE_DIRECTORY_FILE                     equ 00000001h
FILE_WRITE_THROUGH                      equ 00000002h
FILE_SEQUENTIAL_ONLY                    equ 00000004h
FILE_NO_INTERMEDIATE_BUFFERING          equ 00000008h

FILE_SYNCHRONOUS_IO_ALERT               equ 00000010h
FILE_SYNCHRONOUS_IO_NONALERT            equ 00000020h
FILE_NON_DIRECTORY_FILE                 equ 00000040h
FILE_CREATE_TREE_CONNECTION             equ 00000080h

FILE_COMPLETE_IF_OPLOCKED               equ 00000100h
FILE_NO_EA_KNOWLEDGE                    equ 00000200h
FILE_OPEN_FOR_RECOVERY                  equ 00000400h
FILE_RANDOM_ACCESS                      equ 00000800h

FILE_DELETE_ON_CLOSE                    equ 00001000h
FILE_OPEN_BY_FILE_ID                    equ 00002000h
FILE_OPEN_FOR_BACKUP_INTENT             equ 00004000h
FILE_NO_COMPRESSION                     equ 00008000h

FILE_RESERVE_OPFILTER                   equ 00100000h
FILE_OPEN_REPARSE_POINT                 equ 00200000h
FILE_OPEN_NO_RECALL                     equ 00400000h
FILE_OPEN_FOR_FREE_SPACE_QUERY          equ 00800000h

FILE_COPY_STRUCTURED_STORAGE            equ 00000041h
FILE_STRUCTURED_STORAGE                 equ 00000441h

FILE_VALID_OPTION_FLAGS                 equ 00ffffffh
FILE_VALID_PIPE_OPTION_FLAGS            equ 00000032h
FILE_VALID_MAILSLOT_OPTION_FLAGS        equ 00000032h
FILE_VALID_SET_FLAGS                    equ 00000036h

; Define the I/O status information return values for NtCreateFile/NtOpenFile

FILE_SUPERSEDED                 equ 0
FILE_OPENED                     equ 1
FILE_CREATED                    equ 2
FILE_OVERWRITTEN                equ 3
FILE_EXISTS                     equ 4
FILE_DOES_NOT_EXIST             equ 5

; Define special ByteOffset parameters for read and write operations

FILE_WRITE_TO_END_OF_FILE       equ ffffffffh
FILE_USE_FILE_POINTER_POSITION  equ fffffffeh

; Define alignment requirement values

FILE_BYTE_ALIGNMENT             equ 00000000h
FILE_WORD_ALIGNMENT             equ 00000001h
FILE_LONG_ALIGNMENT             equ 00000003h
FILE_QUAD_ALIGNMENT             equ 00000007h
FILE_OCTA_ALIGNMENT             equ 0000000fh
FILE_32_BYTE_ALIGNMENT          equ 0000001fh
FILE_64_BYTE_ALIGNMENT          equ 0000003fh
FILE_128_BYTE_ALIGNMENT         equ 0000007fh
FILE_256_BYTE_ALIGNMENT         equ 000000ffh
FILE_512_BYTE_ALIGNMENT         equ 000001ffh

; Define the maximum length of a filename string

MAXIMUM_FILENAME_LENGTH         equ 256

; Define the various device characteristics flags

FILE_REMOVABLE_MEDIA            equ 00000001h
FILE_READ_ONLY_DEVICE           equ 00000002h
FILE_FLOPPY_DISKETTE            equ 00000004h
FILE_WRITE_ONCE_MEDIA           equ 00000008h
FILE_REMOTE_DEVICE              equ 00000010h
FILE_DEVICE_IS_MOUNTED          equ 00000020h
FILE_VIRTUAL_VOLUME             equ 00000040h
FILE_AUTOGENERATED_DEVICE_NAME  equ 00000080h
FILE_DEVICE_SECURE_OPEN         equ 00000100h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; flags specified here will be propagated up and down a device stack
; after FDO and all filter devices are added, but before the device
; stack is started
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FILE_CHARACTERISTICS_PROPAGATED equ FILE_REMOVABLE_MEDIA or FILE_READ_ONLY_DEVICE or FILE_FLOPPY_DISKETTE or FILE_WRITE_ONCE_MEDIA or FILE_DEVICE_SECURE_OPEN

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the base asynchronous I/O argument types
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IO_STATUS_BLOCK STRUCT		; sizeof = 8
	Status			NTSTATUS	?
	Information		DWORD		?
IO_STATUS_BLOCK ENDS
PIO_STATUS_BLOCK typedef PTR IO_STATUS_BLOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the file information class values
;
; WARNING:  The order of the following values are assumed by the I/O system.
;           Any changes made here should be reflected there as well.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _FILE_INFORMATION_CLASS {
    FileDirectoryInformation      equ 1
    FileFullDirectoryInformation  equ 2
    FileBothDirectoryInformation  equ 3
    FileBasicInformation          equ 4  ; wdm
    FileStandardInformation       equ 5  ; wdm
    FileInternalInformation       equ 6
    FileEaInformation             equ 7
    FileAccessInformation         equ 8
    FileNameInformation           equ 9
    FileRenameInformation         equ 10
    FileLinkInformation           equ 11
    FileNamesInformation          equ 12
    FileDispositionInformation    equ 13
    FilePositionInformation       equ 14 ; wdm
    FileFullEaInformation         equ 15
    FileModeInformation           equ 16
    FileAlignmentInformation      equ 17
    FileAllInformation            equ 18
    FileAllocationInformation     equ 19
    FileEndOfFileInformation      equ 20 ; wdm
    FileAlternateNameInformation  equ 21
    FileStreamInformation         equ 22
    FilePipeInformation           equ 23
    FilePipeLocalInformation      equ 24
    FilePipeRemoteInformation     equ 25
    FileMailslotQueryInformation  equ 26
    FileMailslotSetInformation    equ 27
    FileCompressionInformation    equ 28
    FileObjectIdInformation       equ 29
    FileCompletionInformation     equ 30
    FileMoveClusterInformation    equ 31
    FileQuotaInformation          equ 32
    FileReparsePointInformation   equ 33
    FileNetworkOpenInformation    equ 34
    FileAttributeTagInformation   equ 35
    FileTrackingInformation       equ 36
    FileMaximumInformation        equ 37
comment ^
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the various structures which are returned on query operations
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

typedef struct _FILE_BASIC_INFORMATION {                    
    LARGE_INTEGER CreationTime;                             
    LARGE_INTEGER LastAccessTime;                           
    LARGE_INTEGER LastWriteTime;                            
    LARGE_INTEGER ChangeTime;                               
    ULONG FileAttributes;                                   
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;         
                                                            
typedef struct _FILE_STANDARD_INFORMATION {                 
    LARGE_INTEGER AllocationSize;                           
    LARGE_INTEGER EndOfFile;                                
    ULONG NumberOfLinks;                                    
    BOOLEAN DeletePending;                                  
    BOOLEAN Directory;                                      
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;   
                                                            
typedef struct _FILE_POSITION_INFORMATION {                 
    LARGE_INTEGER CurrentByteOffset;                        
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;   
                                                            
typedef struct _FILE_ALIGNMENT_INFORMATION {                
    ULONG AlignmentRequirement;                             
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION; 
                                                            
typedef struct _FILE_NAME_INFORMATION {                     
    ULONG FileNameLength;                                   
    WCHAR FileName[1];                                      
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;           

typedef struct _FILE_NETWORK_OPEN_INFORMATION {                 
    LARGE_INTEGER CreationTime;                                 
    LARGE_INTEGER LastAccessTime;                               
    LARGE_INTEGER LastWriteTime;                                
    LARGE_INTEGER ChangeTime;                                   
    LARGE_INTEGER AllocationSize;                               
    LARGE_INTEGER EndOfFile;                                    
    ULONG FileAttributes;                                       
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;   
^
FILE_ATTRIBUTE_TAG_INFORMATION STRUCT		; sizeof=8
	FileAttributes	DWORD	?
	ReparseTag		DWORD	?
FILE_ATTRIBUTE_TAG_INFORMATION ENDS
PFILE_ATTRIBUTE_TAG_INFORMATION typedef PTR FILE_ATTRIBUTE_TAG_INFORMATION

FILE_DISPOSITION_INFORMATION STRUCT			; sizeof=1  ;-)
	DeleteFile		BOOLEAN	?
FILE_DISPOSITION_INFORMATION ENDS
PFILE_DISPOSITION_INFORMATION typedef PTR FILE_DISPOSITION_INFORMATION

FILE_END_OF_FILE_INFORMATION STRUCT			; sizeof=8
	EndOfFile		LARGE_INTEGER	<>
FILE_END_OF_FILE_INFORMATION ENDS
PFILE_END_OF_FILE_INFORMATION typedef PTR FILE_END_OF_FILE_INFORMATION

FILE_FULL_EA_INFORMATION STRUCT		; sizeof = 0Ch
	NextEntryOffset		DWORD	?	; 00h
	qwFlags				BYTE	?	; 04h
	EaNameLength		BYTE	?	; 05h
	EaValueLength		WORD	?	; 06h
	EaName				BYTE	?	; 08h
						db 3 dup(?)
FILE_FULL_EA_INFORMATION ENDS
PFILE_FULL_EA_INFORMATION typedef PTR FILE_FULL_EA_INFORMATION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the file system information class values
;
; WARNING:  The order of the following values are assumed by the I/O system.
;           Any changes made here should be reflected there as well.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _FSINFOCLASS {
    FileFsVolumeInformation      equ 1
    FileFsLabelInformation       equ 2
    FileFsSizeInformation        equ 3
    FileFsDeviceInformation      equ 4
    FileFsAttributeInformation   equ 5
    FileFsControlInformation     equ 6
    FileFsFullSizeInformation    equ 7
    FileFsObjectIdInformation    equ 8
    FileFsMaximumInformation     equ 9

FILE_FS_DEVICE_INFORMATION STRUCT		; sizeof = 8
	DeviceType			DWORD	?	; 00h DEVICE_TYPE
	Characteristics		DWORD	?	; 04h
FILE_FS_DEVICE_INFORMATION ENDS
PFILE_FS_DEVICE_INFORMATION typedef PTR FILE_FS_DEVICE_INFORMATION                                                                

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define segement buffer structure for scatter/gather read/write.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef union _FILE_SEGMENT_ELEMENT {
;    PVOID64 Buffer;
;    ULONGLONG Alignment;
;}FILE_SEGMENT_ELEMENT, *PFILE_SEGMENT_ELEMENT;

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define File Object (FO) flags
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

FO_FILE_OPEN                    equ 00000001h
FO_SYNCHRONOUS_IO               equ 00000002h
FO_ALERTABLE_IO                 equ 00000004h
FO_NO_INTERMEDIATE_BUFFERING    equ 00000008h
FO_WRITE_THROUGH                equ 00000010h
FO_SEQUENTIAL_ONLY              equ 00000020h
FO_CACHE_SUPPORTED              equ 00000040h
FO_NAMED_PIPE                   equ 00000080h
FO_STREAM_FILE                  equ 00000100h
FO_MAILSLOT                     equ 00000200h
FO_GENERATE_AUDIT_ON_CLOSE      equ 00000400h
FO_DIRECT_DEVICE_OPEN           equ 00000800h
FO_FILE_MODIFIED                equ 00001000h
FO_FILE_SIZE_CHANGED            equ 00002000h
FO_CLEANUP_COMPLETE             equ 00004000h
FO_TEMPORARY_FILE               equ 00008000h
FO_DELETE_ON_CLOSE              equ 00010000h
FO_OPENED_CASE_SENSITIVE        equ 00020000h
FO_HANDLE_CREATED               equ 00040000h
FO_FILE_FAST_IO_READ            equ 00080000h
FO_RANDOM_ACCESS                equ 00100000h
FO_FILE_OPEN_CANCELLED          equ 00200000h
FO_VOLUME_OPEN                  equ 00400000h

FILE_OBJECT STRUCT		; sizeof = 70h
	fwType				WORD				IO_TYPE_FILE	; 5
	cbSize				WORD				?				; cb
	DeviceObject		PVOID				?				; 04h PTR DEVICE_OBJECT
	Vpb					PVOID				?				; 08h PVPB
	FsContext			PVOID				?				; 0Ch
	FsContext2			PVOID				?				; 10h
	SectionObjectPointer	PSECTION_OBJECT_POINTERS ?		; 14h
	PrivateCacheMap		PVOID				?				; 18h
	FinalStatus			NTSTATUS			?				; 1Ch
	RelatedFileObject	PVOID				?				; 20h PTR FILE_OBJECT
	LockOperation		BOOLEAN				?				; 24h
	DeletePending		BOOLEAN				?				; 25h
	ReadAccess			BOOLEAN				?				; 26h
	WriteAccess			BOOLEAN				?				; 27h
	DeleteAccess		BOOLEAN				?				; 28h
	SharedRead			BOOLEAN				?				; 29h
	SharedWrite			BOOLEAN				?				; 2Ah
	SharedDelete		BOOLEAN				?				; 2Bh
	Flags				DWORD				?				; 2Ch FO_*
	FileName			UNICODE_STRING		<>				; 30h
	CurrentByteOffset	LARGE_INTEGER		<>				; 38h
	Waiters				DWORD				?				; 40h
	Busy				DWORD				?				; 44h
	LastLock			PVOID				?				; 48h
	kevLock				KEVENT				<>				; 4Ch Lock is masm reserved symbol
	Event				KEVENT				<>				; 5Ch
	CompletionContext	PIO_COMPLETION_CONTEXT	?			; 6Ch
FILE_OBJECT ENDS
PFILE_OBJECT typedef PTR FILE_OBJECT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Registry Specific Access Rights.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KEY_QUERY_VALUE         equ 0001h
KEY_SET_VALUE           equ 0002h
KEY_CREATE_SUB_KEY      equ 0004h
KEY_ENUMERATE_SUB_KEYS  equ 0008h
KEY_NOTIFY              equ 0010h
KEY_CREATE_LINK         equ 0020h

KEY_READ	equ (STANDARD_RIGHTS_READ or KEY_QUERY_VALUE or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY) AND NOT SYNCHRONIZE
KEY_WRITE	equ (STANDARD_RIGHTS_WRITE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY) AND NOT SYNCHRONIZE
KEY_EXECUTE	equ KEY_READ AND NOT SYNCHRONIZE
KEY_ALL_ACCESS	equ (STANDARD_RIGHTS_ALL or KEY_QUERY_VALUE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY or KEY_CREATE_LINK) AND NOT SYNCHRONIZE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Open/Create Options
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

REG_OPTION_RESERVED         equ 00000000   ; Parameter is reserved
REG_OPTION_NON_VOLATILE     equ 00000000   ; Key is preserved when system is rebooted
REG_OPTION_VOLATILE         equ 00000001   ; Key is not preserved when system is rebooted
REG_OPTION_CREATE_LINK      equ 00000002   ; Created key is a symbolic link
REG_OPTION_BACKUP_RESTORE   equ 00000004   ; open for backup or restore special access rules privilege required
REG_OPTION_OPEN_LINK        equ 00000008   ; Open symbolic link
REG_LEGAL_OPTION			equ REG_OPTION_RESERVED or REG_OPTION_NON_VOLATILE or REG_OPTION_VOLATILE or REG_OPTION_CREATE_LINK or REG_OPTION_BACKUP_RESTORE or REG_OPTION_OPEN_LINK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Key creation/open disposition
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

REG_CREATED_NEW_KEY         equ 00000001   ; New Registry Key created
REG_OPENED_EXISTING_KEY     equ 00000002   ; Existing Key opened

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Key restore flags
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

REG_WHOLE_HIVE_VOLATILE     equ 00000001   ; Restore whole hive volatile
REG_REFRESH_HIVE            equ 00000002   ; Unwind changes to last flush
REG_NO_LAZY_FLUSH           equ 00000004   ; Never lazy flush this hive
REG_FORCE_RESTORE           equ 00000008   ; Force the restore process even when we have open handles on subkeys

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Key query structures
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KEY_VALUE_BASIC_INFORMATION STRUCT	; sizeof = 10h
	TitleIndex	DWORD	?	; 00h
	dwType		DWORD	?	; 04h (original field name Type)
	NameLength	DWORD	?	; 08h
	woName		WORD	?	; 0Ch Variable size (original field name Name)
				WORD	?	; padding
KEY_VALUE_BASIC_INFORMATION ENDS
PKEY_VALUE_BASIC_INFORMATION typedef PTR KEY_VALUE_BASIC_INFORMATION

KEY_VALUE_FULL_INFORMATION STRUCT	; sizeof = 18h
	TitleIndex	DWORD	?	; 00h
	dwType		DWORD	?	; 04h (original field name Type)
	DataOffset	DWORD	?	; 08h
	DataLength	DWORD	?	; 0Ch
	NameLength	DWORD	?	; 10h
	woName		WORD	?	; 14h Variable size  (original field name Name)
				WORD	?	; 16h padding
	;Data[1];            // Variable size data not declared
KEY_VALUE_FULL_INFORMATION ENDS
PKEY_VALUE_FULL_INFORMATION typedef PTR KEY_VALUE_FULL_INFORMATION

KEY_VALUE_PARTIAL_INFORMATION STRUCT	; sizeof = 10h
	TitleIndex	DWORD	?
	dwType		DWORD	?	; original field name Type
	DataLength	DWORD	?
	Data		BYTE	?	; Variable size
				db 3 dup(?)	; padding
KEY_VALUE_PARTIAL_INFORMATION ENDS
PKEY_VALUE_PARTIAL_INFORMATION typedef PTR KEY_VALUE_PARTIAL_INFORMATION

KEY_VALUE_PARTIAL_INFORMATION_ALIGN64 STRUCT	; sizeof = 0Ch
	dwType		DWORD	?	; original field name Type
	DataLength	DWORD	?
	Data		BYTE	?	; Variable size
				db 3 dup(?)	; padding
KEY_VALUE_PARTIAL_INFORMATION_ALIGN64 ENDS
PKEY_VALUE_PARTIAL_INFORMATION_ALIGN64 typedef PTR KEY_VALUE_PARTIAL_INFORMATION_ALIGN64

KEY_VALUE_ENTRY STRUCT	; sizeof = 10h
	ValueName	PVOID	?	; PTR UNICODE_STRING
	DataLength	DWORD	?
	DataOffset	DWORD	?
	dwType		DWORD	?	; original field name Type
KEY_VALUE_ENTRY ENDS
PKEY_VALUE_ENTRY typedef PTR KEY_VALUE_ENTRY

;typedef enum _KEY_VALUE_INFORMATION_CLASS {
KeyValueBasicInformation			equ 0
KeyValueFullInformation				equ 1
KeyValuePartialInformation			equ 2
KeyValueFullInformationAlign64		equ 3
KeyValuePartialInformationAlign64	equ 4

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Pool Allocation routines (in pool.c)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _POOL_TYPE {
    NonPagedPool							equ 0
    PagedPool								equ 1
    NonPagedPoolMustSucceed					equ 2
    DontUseThisType							equ 3
    NonPagedPoolCacheAligned				equ 4
    PagedPoolCacheAligned					equ 5
    NonPagedPoolCacheAlignedMustS			equ 6
    MaxPoolType								equ 7

	; Note these per session types are carefully chosen so that the appropriate
	; masking still applies as well as MaxPoolType above.

    NonPagedPoolSession						equ 32
    PagedPoolSession						equ 33
    NonPagedPoolMustSucceedSession			equ 34
    DontUseThisTypeSession					equ 35
    NonPagedPoolCacheAlignedSession			equ 36
    PagedPoolCacheAlignedSession			equ 37
    NonPagedPoolCacheAlignedMustSSession	equ 38

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; _EX_POOL_PRIORITY_ provides a method for the system to handle requests
; intelligently in low resource conditions.
;
; LowPoolPriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is low on resources.  An example of
; this could be for a non-critical network connection where the driver can
; handle the failure case when system resources are close to being depleted.
;
; NormalPoolPriority should be used when it is acceptable to the driver for the
; mapping request to fail if the system is very low on resources.  An example
; of this could be for a non-critical local filesystem request.
;
; HighPoolPriority should be used when it is unacceptable to the driver for the
; mapping request to fail unless the system is completely out of resources.
; An example of this would be the paging file path in a driver.
;
; SpecialPool can be specified to bound the allocation at a page end (or
; beginning).  This should only be done on systems being debugged as the
; memory cost is expensive.
;
; N.B.  These values are very carefully chosen so that the pool allocation
;       code can quickly crack the priority request.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _EX_POOL_PRIORITY {
    LowPoolPriority							equ 0
    LowPoolPrioritySpecialPoolOverrun		equ 8
    LowPoolPrioritySpecialPoolUnderrun		equ 9
    NormalPoolPriority						equ 16
    NormalPoolPrioritySpecialPoolOverrun	equ 24
    NormalPoolPrioritySpecialPoolUnderrun	equ 25
    HighPoolPriority						equ 32
    HighPoolPrioritySpecialPoolOverrun		equ 40
    HighPoolPrioritySpecialPoolUnderrun		equ 41

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define I/O Request Packet (IRP) flags
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IRP_NOCACHE                     equ 00000001h
IRP_PAGING_IO                   equ 00000002h
IRP_MOUNT_COMPLETION            equ 00000002h
IRP_SYNCHRONOUS_API             equ 00000004h
IRP_ASSOCIATED_IRP              equ 00000008h
IRP_BUFFERED_IO                 equ 00000010h
IRP_DEALLOCATE_BUFFER           equ 00000020h
IRP_INPUT_OPERATION             equ 00000040h
IRP_SYNCHRONOUS_PAGING_IO       equ 00000040h
IRP_CREATE_OPERATION            equ 00000080h
IRP_READ_OPERATION              equ 00000100h
IRP_WRITE_OPERATION             equ 00000200h
IRP_CLOSE_OPERATION             equ 00000400h

IRP_DEFER_IO_COMPLETION         equ 00000800h
IRP_OB_QUERY_NAME               equ 00001000h
IRP_HOLD_DEVICE_QUEUE           equ 00002000h
IRP_RETRY_IO_COMPLETION         equ 00004000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define I/O request packet (IRP) alternate flags for allocation control.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IRP_QUOTA_CHARGED               equ 01h
IRP_ALLOCATED_MUST_SUCCEED      equ 02h
IRP_ALLOCATED_FIXED_SIZE        equ 04h
IRP_LOOKASIDE_ALLOCATION        equ 08h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; I/O Request Packet (IRP) definition
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

_IRP STRUCT			; sizeof = 70h
	fwType			WORD	?
	cbSize			WORD	?	; 02h

	; Define the common fields used to control the IRP.

	; Define a pointer to the Memory Descriptor List (MDL) for this I/O
	; request.  This field is only used if the I/O is "direct I/O".

	MdlAddress		PVOID	?		; 04h PMDL

	; Flags word - used to remember various flags.

	Flags			DWORD	?		; 08h

	; The following union is used for one of three purposes:
	;
	; 1. This IRP is an associated IRP.  The field is a pointer to a master IRP.
	;
	; 2. This is the master IRP.  The field is the count of the number of
	; IRPs which must complete (associated IRPs) before the master can
	; complete.
	;
	; 3. This operation is being buffered and the field is the address of
	; the system space buffer.

	UNION AssociatedIrp
		MasterIrp		PVOID	?	; 0Ch PIRP
		IrpCount		DWORD	?	; 0Ch 
		SystemBuffer	PVOID	?	; 0Ch 
	ENDS	; AssociatedIrp

	; Thread list entry - allows queueing the IRP to the thread pending I/O
	; request packet list.

	ThreadListEntry	LIST_ENTRY	<>	; 10h

	; I/O status - final status of operation.

	IoStatus		IO_STATUS_BLOCK	<>	; 18h

	; Requestor mode - mode of the original requestor of this operation.

	RequestorMode	BYTE	?	; 20h KPROCESSOR_MODE

	; Pending returned - TRUE if pending was initially returned as the
	; status for this packet.

	PendingReturned	BYTE	?	; 21h

	; Stack state information.

	StackCount		BYTE	?	; 22h
	CurrentLocation	BYTE	?	; 23h

	; Cancel - packet has been canceled.

	Cancel			BYTE	?	; 24h

	; Cancel Irql - Irql at which the cancel spinlock was acquired.

	CancelIrql		BYTE	?	; 25h

	; ApcEnvironment - Used to save the APC environment at the time that the
	; packet was initialized.

	ApcEnvironment	BYTE	?	; 26h

	; Allocation control flags.

	AllocationFlags	BYTE	?	27h

	; User parameters.

	UserIosb		PIO_STATUS_BLOCK	?	; 28h
	UserEvent		PKEVENT				?	; 2Ch
	UNION Overlay							; 30h
		STRUCT AsynchronousParameters		; 30h
			UserApcRoutine	PVOID	?	; 30h PIO_APC_ROUTINE
			UserApcContext	PVOID	?	; 34h
		ENDS	; AsynchronousParameters
		AllocationSize		LARGE_INTEGER	<>	; 30h
	ENDS	; Overlay

	; CancelRoutine - Used to contain the address of a cancel routine supplied
	; by a device driver when the IRP is in a cancelable state.

	CancelRoutine	PVOID		?		; 38h PDRIVER_CANCEL

	; Note that the UserBuffer parameter is outside of the stack so that I/O
	; completion can copy data back into the user's address space without
	; having to know exactly which service was being invoked.  The length
	; of the copy is stored in the second half of the I/O status block. If
	; the UserBuffer field is NULL, then no copy is performed.

	UserBuffer		PVOID		?	; 3Ch

	; Kernel structures
	;
	; The following section contains kernel structures which the IRP needs
	; in order to place various work information in kernel controller system
	; queues.  Because the size and alignment cannot be controlled, they are
	; placed here at the end so they just hang off and do not affect the
	; alignment of other fields in the IRP.

	UNION Tail
		STRUCT Overlay
			UNION
				; DeviceQueueEntry - The device queue entry field is used to
				; queue the IRP to the device driver device queue.

				DeviceQueueEntry	KDEVICE_QUEUE_ENTRY	<>

				STRUCT
					; The following are available to the driver to use in
					; whatever manner is desired, while the driver owns the
					; packet.
					DriverContext	PVOID	4 dup(?)
				ENDS

			ENDS

			; Thread - pointer to caller's Thread Control Block.

			Thread				PVOID	?			; PETHREAD

			; Auxiliary buffer - pointer to any auxiliary buffer that is
			; required to pass information to a driver that is not contained
			; in a normal buffer.

			AuxiliaryBuffer		PCHAR	?

			; The following unnamed structure must be exactly identical
			; to the unnamed structure used in the minipacket header used
			; for completion queue entries.

			STRUCT

				; List entry - used to queue the packet to completion queue, among
				; others.

				ListEntry		LIST_ENTRY	<>

				UNION

					; Current stack location - contains a pointer to the current
					; IO_STACK_LOCATION structure in the IRP stack.  This field
					; should never be directly accessed by drivers.  They should
					; use the standard functions.
					CurrentStackLocation	PVOID	?	; PTR IO_STACK_LOCATION

					; Minipacket type.

					PacketType				DWORD	?
				ENDS
			ENDS

			; Original file object - pointer to the original file object
			; that was used to open the file.  This field is owned by the
			; I/O system and should not be used by any other drivers.

			OriginalFileObject	PFILE_OBJECT	?

		ENDS	; Overlay

		; APC - This APC control block is used for the special kernel APC as
		; well as for the caller's APC, if one was specified in the original
		; argument list.  If so, then the APC is reused for the normal APC for
		; whatever mode the caller was in and the "special" routine that is
		; invoked before the APC gets control simply deallocates the IRP.

		Apc		KAPC	<>

		; CompletionKey - This is the key that is used to distinguish
		; individual I/O operations initiated on a single file handle.

		CompletionKey	PVOID	?

	ENDS	;Tail

_IRP ENDS
PIRP typedef PTR _IRP

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Volume Parameter Block (VPB)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MAXIMUM_VOLUME_LABEL_LENGTH	equ (32 * sizeof(WCHAR))	; 32 characters

VPB STRUCT				; sizeof = 58h
	fwType				WORD			IO_TYPE_VPB	; 0Ah
	cbSize				WORD			?			; cb
	Flags				WORD			?
	VolumeLabelLength	WORD			?			; in bytes without terminat. zero
	DeviceObject		PVOID			?			; PDEVICE_OBJECT
	RealDevice			PVOID			?			; PDEVICE_OBJECT
	SerialNumber		DWORD			?
	ReferenceCount		DWORD			?
	VolumeLabel			WORD			(MAXIMUM_VOLUME_LABEL_LENGTH / (sizeof WCHAR)) dup(?)
VPB ENDS
PVPB typedef PTR VPB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the various device type values.  Note that values used by Microsoft
; Corporation are in the range 0-32767, and 32768-65535 are reserved for use
; by customers.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DEVICE_TYPE	typedef	DWORD

FILE_DEVICE_BEEP                equ 01
FILE_DEVICE_CD_ROM              equ 02
FILE_DEVICE_CD_ROM_FILE_SYSTEM  equ 03
FILE_DEVICE_CONTROLLER          equ 04
FILE_DEVICE_DATALINK            equ 05
FILE_DEVICE_DFS                 equ 06
FILE_DEVICE_DISK                equ 07
FILE_DEVICE_DISK_FILE_SYSTEM    equ 08
FILE_DEVICE_FILE_SYSTEM         equ 09
FILE_DEVICE_INPORT_PORT         equ 0ah
FILE_DEVICE_KEYBOARD            equ 0bh
FILE_DEVICE_MAILSLOT            equ 0ch
FILE_DEVICE_MIDI_IN             equ 0dh
FILE_DEVICE_MIDI_OUT            equ 0eh
FILE_DEVICE_MOUSE               equ 0fh
FILE_DEVICE_MULTI_UNC_PROVIDER  equ 10h
FILE_DEVICE_NAMED_PIPE          equ 11h
FILE_DEVICE_NETWORK             equ 12h
FILE_DEVICE_NETWORK_BROWSER     equ 13h
FILE_DEVICE_NETWORK_FILE_SYSTEM equ 14h
FILE_DEVICE_NULL                equ 15h
FILE_DEVICE_PARALLEL_PORT       equ 16h
FILE_DEVICE_PHYSICAL_NETCARD    equ 17h
FILE_DEVICE_PRINTER             equ 18h
FILE_DEVICE_SCANNER             equ 19h
FILE_DEVICE_SERIAL_MOUSE_PORT   equ 1ah
FILE_DEVICE_SERIAL_PORT         equ 1bh
FILE_DEVICE_SCREEN              equ 1ch
FILE_DEVICE_SOUND               equ 1dh
FILE_DEVICE_STREAMS             equ 1eh
FILE_DEVICE_TAPE                equ 1fh
FILE_DEVICE_TAPE_FILE_SYSTEM    equ 20h
FILE_DEVICE_TRANSPORT           equ 21h
FILE_DEVICE_UNKNOWN             equ 22h
FILE_DEVICE_VIDEO               equ 23h
FILE_DEVICE_VIRTUAL_DISK        equ 24h
FILE_DEVICE_WAVE_IN             equ 25h
FILE_DEVICE_WAVE_OUT            equ 26h
FILE_DEVICE_8042_PORT           equ 27h
FILE_DEVICE_NETWORK_REDIRECTOR  equ 28h
FILE_DEVICE_BATTERY             equ 29h
FILE_DEVICE_BUS_EXTENDER        equ 2ah
FILE_DEVICE_MODEM               equ 2bh
FILE_DEVICE_VDM                 equ 2ch
FILE_DEVICE_MASS_STORAGE        equ 2dh
FILE_DEVICE_SMB                 equ 2eh
FILE_DEVICE_KS                  equ 2fh
FILE_DEVICE_CHANGER             equ 30h
FILE_DEVICE_SMARTCARD           equ 31h
FILE_DEVICE_ACPI                equ 32h
FILE_DEVICE_DVD                 equ 33h
FILE_DEVICE_FULLSCREEN_VIDEO    equ 34h
FILE_DEVICE_DFS_FILE_SYSTEM     equ 35h
FILE_DEVICE_DFS_VOLUME          equ 36h
FILE_DEVICE_SERENUM             equ 37h
FILE_DEVICE_TERMSRV             equ 38h
FILE_DEVICE_KSEC                equ 39h

; Macro definition for defining IOCTL and FSCTL function control codes.  Note
; that function codes 0-2047 are reserved for Microsoft Corporation, and
; 2048-4095 are reserved for customers.

CTL_CODE MACRO DeviceType:=<0>, Function:=<0>, Method:=<0>, Access:=<0>
	EXITM %(((DeviceType) SHL 16) OR ((Access) SHL 14) OR ((Function) SHL 2) OR (Method))
ENDM

; Macro to extract device type out of the device io control code

DEVICE_TYPE_FROM_CTL_CODE MACRO ctrlCode:REQ
	EXITM %((ctrlCode AND 0ffff0000h) SHR 16)
ENDM

; Define the method codes for how buffers are passed for I/O and FS controls

METHOD_BUFFERED                 equ 0
METHOD_IN_DIRECT                equ 1
METHOD_OUT_DIRECT               equ 2
METHOD_NEITHER                  equ 3

; Define the access check value for any access
;
; The FILE_READ_ACCESS and FILE_WRITE_ACCESS constants are also defined in
; ntioapi.h as FILE_READ_DATA and FILE_WRITE_DATA. The values for these
; constants *MUST* always be in sync.
;
; FILE_SPECIAL_ACCESS is checked by the NT I/O system the same as FILE_ANY_ACCESS.
; The file systems, however, may add additional access checks for I/O and FS controls
; that use this value.

FILE_ANY_ACCESS			equ 0
FILE_SPECIAL_ACCESS		equ FILE_ANY_ACCESS
FILE_READ_ACCESS		equ 1	; file & pipe
FILE_WRITE_ACCESS		equ 2	; file & pipe

; begin_winnt

; Define access rights to files and directories

; The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
; devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
; constants *MUST* always be in sync.
; The values are redefined in devioctl.h because they must be available to
; both DOS and NT.

FILE_READ_DATA				equ 0001	; file & pipe
FILE_LIST_DIRECTORY			equ 0001	; directory

FILE_WRITE_DATA				equ 0002	; file & pipe
FILE_ADD_FILE				equ 0002	; directory

FILE_APPEND_DATA			equ 0004	; file
FILE_ADD_SUBDIRECTORY		equ 0004	; directory
FILE_CREATE_PIPE_INSTANCE	equ 0004	; named pipe

FILE_READ_EA				equ 0008	; file & directory

FILE_WRITE_EA				equ 0010h	; file & directory

FILE_EXECUTE				equ 0020h	; file
FILE_TRAVERSE				equ 0020h	; directory

FILE_DELETE_CHILD			equ 0040h	; directory

FILE_READ_ATTRIBUTES		equ 0080h	; all

FILE_WRITE_ATTRIBUTES		equ 0100h	; all

FILE_ALL_ACCESS				equ STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 1FFh

FILE_GENERIC_READ equ STANDARD_RIGHTS_READ or FILE_READ_DATA or FILE_READ_ATTRIBUTES or FILE_READ_EA or SYNCHRONIZE

FILE_GENERIC_WRITE equ STANDARD_RIGHTS_WRITE or FILE_WRITE_DATA or FILE_WRITE_ATTRIBUTES or FILE_WRITE_EA or FILE_APPEND_DATA or SYNCHRONIZE

FILE_GENERIC_EXECUTE equ STANDARD_RIGHTS_EXECUTE or FILE_READ_ATTRIBUTES or FILE_EXECUTE or SYNCHRONIZE

; end_winnt

; Define share access rights to files and directories

FILE_SHARE_READ                 equ 1	; winnt
FILE_SHARE_WRITE                equ 2	; winnt
FILE_SHARE_DELETE               equ 4	; winnt
FILE_SHARE_VALID_FLAGS          equ 7

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define Wait Context Block (WCB)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

WAIT_CONTEXT_BLOCK STRUCT	; sizeof = 28h
	WaitQueueEntry			KDEVICE_QUEUE_ENTRY	<>
	DeviceRoutine			PVOID				?	; PDRIVER_CONTROL
	DeviceContext			PVOID				?
	NumberOfMapRegisters	DWORD				?
	DeviceObject			PVOID				?
	CurrentIrp				PVOID				?
	BufferChainingDpc		PVOID				?	; PKDPC
WAIT_CONTEXT_BLOCK ENDS
PWAIT_CONTEXT_BLOCK typedef PTR WAIT_CONTEXT_BLOCK

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KDPC STRUCT			; sizeof = 20h
	woType			WORD		IO_TYPE_DPC	; 13h (original name Type)
	Number			BYTE		?
	Importance		BYTE		?
	DpcListEntry	LIST_ENTRY	<>
	DeferredRoutine	PVOID		?
	DeferredContext	PVOID		?
	SystemArgument1	PVOID		?
	SystemArgument2	PVOID		?
	pdwLock			PDWORD		?			; (original Lock)
KDPC ENDS
PKDPC typedef PTR KDPC

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Device Object structure definition
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DEVICE_OBJECT STRUCT	; sizeof = 0B8h
	fwType				WORD			IO_TYPE_DEVICE	; 3
	cbSize				WORD			?	; 02h
	ReferenceCount		DWORD			?	; 04h
	DriverObject		PVOID			?	; 08h PDRIVER_OBJECT
	NextDevice			PVOID			?	; 0Ch PDEVICE_OBJECT
	AttachedDevice		PVOID			?	; 10h PDEVICE_OBJECT
	CurrentIrp			PIRP			?	; 14h PTR _IRP
	Timer				PVOID			?	; 18h PIO_TIMER
	Flags				DWORD			?	; 1Ch DO_*
	Characteristics		DWORD			?	; 20h FILE_*
	Vpb					PVPB			?	; 24h PTR VPB
	DeviceExtension		PVOID			?	; 28h 
	DeviceType			DEVICE_TYPE		?	; 2Ch
	StackSize			CHAR			?	; 30h
						db		3 dup(?)	; padding
	UNION Queue
		ListEntry		LIST_ENTRY		<>	; 34h
		Wcb				WAIT_CONTEXT_BLOCK <>	; 34h
	ENDS ; Queue
	AlignmentRequirement	DWORD		?	; 5Ch
	DeviceQueue			KDEVICE_QUEUE	<>	; 60h
	Dpc					KDPC			<>	; 74h

	; The following field is for exclusive use by the filesystem to keep
	; track of the number of Fsp threads currently using the device

	ActiveThreadCount	DWORD			?	; 94h
	SecurityDescriptor	PSECURITY_DESCRIPTOR ?	; 98h
	DeviceLock			KEVENT			<>	; 9Ch

	SectorSize			WORD			?	; 0ACh
	Spare1				WORD			?	; 0AEh

	DeviceObjectExtension	PVOID		?	; 0B0h PDEVOBJ_EXTENSION
	Reserved				PVOID		?	; 0B4h
DEVICE_OBJECT ENDS
PDEVICE_OBJECT typedef PTR DEVICE_OBJECT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


DEVOBJ_EXTENSION STRUCT		; sizeof = 8h
	woType			WORD			IO_TYPE_DEVICE_OBJECT_EXTENSION	; 00h (original name Type)
	cbSize			WORD			?						; 02h (original name Size)
	; Public part of the DeviceObjectExtension structure
	DeviceObject	PDEVICE_OBJECT	?						; 04h owning device object

; kd displays this additional fields (sizeof=24h))
;+08 uint32   PowerFlags
;+0c  *Dope
;+10 uint32   ExtensionFlags
;+14 void     *DeviceNode
;+18 struct   _DEVICE_OBJECT *AttachedTo
;+1c struct   _LIST_ENTRY FileObjectList
;+1c    struct   _LIST_ENTRY *Flink
;+20    struct   _LIST_ENTRY *Blink

DEVOBJ_EXTENSION ENDS
PDEVOBJ_EXTENSION typedef PTR DEVOBJ_EXTENSION

; Define I/O system data structure type codes.  Each major data structure in
; the I/O system has a type code  The type field in each structure is at the
; same offset.  The following values can be used to determine which type of
; data structure a pointer refers to.

IO_TYPE_ADAPTER					equ 1
IO_TYPE_CONTROLLER				equ 2
IO_TYPE_DEVICE					equ 3
IO_TYPE_DRIVER					equ 4
IO_TYPE_FILE					equ 5
IO_TYPE_IRP						equ 6
IO_TYPE_MASTER_ADAPTER			equ 7
IO_TYPE_OPEN_PACKET				equ 8
IO_TYPE_TIMER					equ 9
IO_TYPE_VPB						equ 0Ah
IO_TYPE_ERROR_LOG				equ 0Bh
IO_TYPE_ERROR_MESSAGE			equ 0Ch
IO_TYPE_DEVICE_OBJECT_EXTENSION	equ 0Dh

IO_TYPE_APC						equ 12h
IO_TYPE_DPC						equ 13h
IO_TYPE_DEVICE_QUEUE			equ 14h
IO_TYPE_EVENT_PAIR				equ 15h
IO_TYPE_INTERRUPT				equ 16h
IO_TYPE_PROFILE					equ 17h


; Define the major function codes for IRPs.

IRP_MJ_CREATE					equ 0
IRP_MJ_CREATE_NAMED_PIPE		equ 1
IRP_MJ_CLOSE					equ 2
IRP_MJ_READ						equ 3
IRP_MJ_WRITE					equ 4
IRP_MJ_QUERY_INFORMATION		equ 5
IRP_MJ_SET_INFORMATION			equ 6
IRP_MJ_QUERY_EA					equ 7
IRP_MJ_SET_EA					equ 8
IRP_MJ_FLUSH_BUFFERS			equ 9
IRP_MJ_QUERY_VOLUME_INFORMATION	equ 0Ah
IRP_MJ_SET_VOLUME_INFORMATION	equ 0Bh
IRP_MJ_DIRECTORY_CONTROL		equ 0Ch
IRP_MJ_FILE_SYSTEM_CONTROL		equ 0Dh
IRP_MJ_DEVICE_CONTROL			equ 0Eh
IRP_MJ_INTERNAL_DEVICE_CONTROL	equ 0Fh
IRP_MJ_SHUTDOWN					equ 10h
IRP_MJ_LOCK_CONTROL				equ 11h
IRP_MJ_CLEANUP					equ 12h
IRP_MJ_CREATE_MAILSLOT			equ 13h
IRP_MJ_QUERY_SECURITY			equ 14h
IRP_MJ_SET_SECURITY				equ 15h
IRP_MJ_POWER					equ 16h
IRP_MJ_SYSTEM_CONTROL			equ 17h
IRP_MJ_DEVICE_CHANGE			equ 18h
IRP_MJ_QUERY_QUOTA				equ 19h
IRP_MJ_SET_QUOTA				equ 1Ah
IRP_MJ_PNP						equ 1Bh
IRP_MJ_PNP_POWER				equ IRP_MJ_PNP	; Obsolete....
IRP_MJ_MAXIMUM_FUNCTION			equ 1Bh

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DRIVER_EXTENSION STRUCT		; sizeof = 14h

	; Back pointer to Driver Object

	DriverObject	PVOID			?	; PDRIVER_OBJECT

	; The AddDevice entry point is called by the Plug & Play manager
	; to inform the driver when a new device instance arrives that this
	; driver must control.

	AddDevice		PVOID			?	; 04h PDRIVER_ADD_DEVICE

	; The count field is used to count the number of times the driver has
	; had its registered reinitialization routine invoked.

	Count			DWORD			?	; 08h

	; The service name field is used by the pnp manager to determine
	; where the driver related info is stored in the registry.

	ServiceKeyName	UNICODE_STRING	<>	; 0Ch

	; Note: any new shared fields get added here.

; kd displays additional field
;+14 struct   _IO_CLIENT_EXTENSION *ClientDriverExtension

DRIVER_EXTENSION ENDS
PDRIVER_EXTENSION typedef PTR DRIVER_EXTENSION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DRIVER_OBJECT STRUCT	; sizeof= 0A8h
	fwType			WORD			IO_TYPE_DRIVER	; 4
	cbSize			WORD			?			; cb

	; The following links all of the devices created by a single driver
	; together on a list, and the Flags word provides an extensible flag
	; location for driver objects.

	DeviceObject	PDEVICE_OBJECT	?
	Flags			DWORD			?

	; The following section describes where the driver is loaded.  The count
	; field is used to count the number of times the driver has had its
	; registered reinitialization routine invoked.

	DriverStart		PVOID			?
	DriverSize		DWORD			?
	DriverSection	PVOID			?
	DriverExtension	PDRIVER_EXTENSION	?

	; The driver name field is used by the error log thread
	; determine the name of the driver that an I/O request is/was bound.

	DriverName		UNICODE_STRING	<>

	; The following section is for registry support.  Thise is a pointer
	; to the path to the hardware information in the registry

	HardwareDatabase	PUNICODE_STRING	?

	; The following section contains the optional pointer to an array of
	; alternate entry points to a driver for "fast I/O" support.  Fast I/O
	; is performed by invoking the driver routine directly with separate
	; parameters, rather than using the standard IRP call mechanism.  Note
	; that these functions may only be used for synchronous I/O, and when
	; the file is cached.

	FastIoDispatch	PVOID			?

	; The following section describes the entry points to this particular
	; driver.  Note that the major function dispatch table must be the last
	; field in the object so that it remains extensible.

	DriverInit		PVOID			?
	DriverStartIo	PVOID			?
	DriverUnload	PVOID			?
	MajorFunction	PVOID			(IRP_MJ_MAXIMUM_FUNCTION + 1) dup(?)

DRIVER_OBJECT ENDS
PDRIVER_OBJECT typedef PTR DRIVER_OBJECT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;  BitMap routines.  The following structure, routines, and macros are
;  for manipulating bitmaps.  The user is responsible for allocating a bitmap
;  structure (which is really a header) and a buffer (which must be longword
;  aligned and multiple longwords in size).

RTL_BITMAP STRUCT	; sizeof = 8
	SizeOfBitMap	DWORD	?	; Number of bits in bit map
	Buffer			PVOID	?	; Pointer to the bit map itself
RTL_BITMAP ENDS
PRTL_BITMAP typedef PTR RTL_BITMAP

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IO_STACK_LOCATION STRUCT	; sizeof = 24h
	MajorFunction		BYTE	?	; 00h
	MinorFunction		BYTE	?	; 01h
	Flags				BYTE	?	; 02h
	Control				BYTE	?	; 03h

	; The following user parameters are based on the service that is being
	; invoked.  Drivers and file systems can determine which set to use based
	; on the above major and minor function codes.

	union Parameters				; 04h

		; System service parameters for:  NtCreateFile

		struct Create				; 04h
			SecurityContext		PVOID	?	; 04h PTR IO_SECURITY_CONTEXT
			Options				DWORD	?	; 08h
			; if _WIN64
			;	POINTER_ALIGNMENT DECLSPEC_ALIGN(8)
			; endif
			FileAttributes		WORD	?	; 0Ch POINTER_ALIGNMENT
			ShareAccess			WORD	?	; 0Eh
			EaLength			DWORD	?	; 10h POINTER_ALIGNMENT
		ends	; Create

		; System service parameters for:  NtReadFile

		struct Read					; 04h
			dwLength			DWORD	?	; 04h
			Key					DWORD	?	; 08h POINTER_ALIGNMENT
			ByteOffset			LARGE_INTEGER	<>	; 0Ch
		ends	; Read

		; System service parameters for:  NtWriteFile

		struct Write				; 04h
			dwLength			DWORD	?	; 04h
			Key					DWORD	?	; 08h POINTER_ALIGNMENT
			ByteOffset			LARGE_INTEGER	<>	; 0Ch
		ends	; Write

		; System service parameters for:  NtQueryInformationFile

		struct QueryFile
			dwLength				DWORD	?	; 04h original field name Length
			FileInformationClass	DWORD	?	; 08h POINTER_ALIGNMENT
		ends	; QueryFile

comment ^
        //
        // System service parameters for:  NtSetInformationFile
        //

        struct {
            ULONG Length;
            FILE_INFORMATION_CLASS POINTER_ALIGNMENT FileInformationClass;
            PFILE_OBJECT FileObject;
            union {
                struct {
                    BOOLEAN ReplaceIfExists;
                    BOOLEAN AdvanceOnly;
                };
                ULONG ClusterCount;
                HANDLE DeleteHandle;
            };
        } SetFile;


        //
        // System service parameters for:  NtQueryVolumeInformationFile
        //

        struct {
            ULONG Length;
            FS_INFORMATION_CLASS POINTER_ALIGNMENT FsInformationClass;
        } QueryVolume;

^
		; System service parameters for:  NtFlushBuffersFile
		;
		; No extra user-supplied parameters.

		; System service parameters for:  NtDeviceIoControlFile
		; 
		; Note that the user's output buffer is stored in the UserBuffer field
		; and the user's input buffer is stored in the SystemBuffer field.

		struct DeviceIoControl		; 04h
			OutputBufferLength		DWORD	?	; 04h
			InputBufferLength		DWORD	?	; 08h POINTER_ALIGNMENT
			IoControlCode			DWORD	?	; 0Ch POINTER_ALIGNMENT
			Type3InputBuffer		PVOID	?	; 10h
		ends	; DeviceIoControl
comment ^
// end_wdm
        //
        // System service parameters for:  NtQuerySecurityObject
        //

        struct {
            SECURITY_INFORMATION SecurityInformation;
            ULONG POINTER_ALIGNMENT Length;
        } QuerySecurity;

        //
        // System service parameters for:  NtSetSecurityObject
        //

        struct {
            SECURITY_INFORMATION SecurityInformation;
            PSECURITY_DESCRIPTOR SecurityDescriptor;
        } SetSecurity;

// begin_wdm
        //
        // Non-system service parameters.
        //
        // Parameters for MountVolume
        //

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } MountVolume;

        //
        // Parameters for VerifyVolume
        //

        struct {
            PVPB Vpb;
            PDEVICE_OBJECT DeviceObject;
        } VerifyVolume;

        //
        // Parameters for Scsi with internal device contorl.
        //

        struct {
            struct _SCSI_REQUEST_BLOCK *Srb;
        } Scsi;


        //
        // Parameters for IRP_MN_QUERY_DEVICE_RELATIONS
        //

        struct {
            DEVICE_RELATION_TYPE Type;
        } QueryDeviceRelations;

        //
        // Parameters for IRP_MN_QUERY_INTERFACE
        //

        struct {
            CONST GUID *InterfaceType;
            USHORT Size;
            USHORT Version;
            PINTERFACE Interface;
            PVOID InterfaceSpecificData;
        } QueryInterface;

// end_ntifs

        //
        // Parameters for IRP_MN_QUERY_CAPABILITIES
        //

        struct {
            PDEVICE_CAPABILITIES Capabilities;
        } DeviceCapabilities;

        //
        // Parameters for IRP_MN_FILTER_RESOURCE_REQUIREMENTS
        //

        struct {
            PIO_RESOURCE_REQUIREMENTS_LIST IoResourceRequirementList;
        } FilterResourceRequirements;
^
		; Parameters for IRP_MN_READ_CONFIG and IRP_MN_WRITE_CONFIG

		struct ReadWriteConfig			; 04h
			WhichSpace		DWORD	?	; 04h
			Buffer			PVOID	?	; 08h
			dwOffset		DWORD	?	; 0Ch
			wdLength		DWORD	?	; 10h POINTER_ALIGNMENT (original field name Length)
		ends

		; Parameters for IRP_MN_SET_LOCK

		struct SetLock					; 04h
			bLock			BOOLEAN	?	; 04h (original name Lock)
							db 3 dup(?)	; padding ???
		ends
comment ^
        //
        // Parameters for IRP_MN_QUERY_ID
        //

        struct {
            BUS_QUERY_ID_TYPE IdType;
        } QueryId;

        //
        // Parameters for IRP_MN_QUERY_DEVICE_TEXT
        //

        struct {
            DEVICE_TEXT_TYPE DeviceTextType;
            LCID POINTER_ALIGNMENT LocaleId;
        } QueryDeviceText;

        //
        // Parameters for IRP_MN_DEVICE_USAGE_NOTIFICATION
        //

        struct {
            BOOLEAN InPath;
            BOOLEAN Reserved[3];
            DEVICE_USAGE_NOTIFICATION_TYPE POINTER_ALIGNMENT Type;
        } UsageNotification;

        //
        // Parameters for IRP_MN_WAIT_WAKE
        //

        struct {
            SYSTEM_POWER_STATE PowerState;
        } WaitWake;

        //
        // Parameter for IRP_MN_POWER_SEQUENCE
        //

        struct {
            PPOWER_SEQUENCE PowerSequence;
        } PowerSequence;

        //
        // Parameters for IRP_MN_SET_POWER and IRP_MN_QUERY_POWER
        //

        struct {
            ULONG SystemContext;
            POWER_STATE_TYPE POINTER_ALIGNMENT Type;
            POWER_STATE POINTER_ALIGNMENT State;
            POWER_ACTION POINTER_ALIGNMENT ShutdownType;
        } Power;

        //
        // Parameters for StartDevice
        //

        struct {
            PCM_RESOURCE_LIST AllocatedResources;
            PCM_RESOURCE_LIST AllocatedResourcesTranslated;
        } StartDevice;

// begin_ntifs
        //
        // Parameters for Cleanup
        //
        // No extra parameters supplied
        //

        //
        // WMI Irps
        //

        struct {
            ULONG_PTR ProviderId;
            PVOID DataPath;
            ULONG BufferSize;
            PVOID Buffer;
        } WMI;
^
		; Others - driver-specific

		struct Others
			Argument1	PVOID	?
			Argument2	PVOID	?
			Argument3	PVOID	?
			Argument4	PVOID	?
		ends

	ends	; Parameters

	; Save a pointer to this device driver's device object for this request
	; so it can be passed to the completion routine if needed.

	DeviceObject		PDEVICE_OBJECT	?

	; The following location contains a pointer to the file object for this

	FileObject			PFILE_OBJECT	?

	; The following routine is invoked depending on the flags in the above
	; flags field.

	CompletionRoutine	PVOID	?	; PIO_COMPLETION_ROUTINE

	; The following is used to store the address of the context parameter
	; that should be passed to the CompletionRoutine.

	Context				PVOID	?

IO_STACK_LOCATION ENDS
PIO_STACK_LOCATION typedef PTR IO_STACK_LOCATION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; PIO_STACK_LOCATION IoGetCurrentIrpStackLocation( IN PIRP Irp )
;
; Macro Description:
;
;     This macro is invoked to return a pointer to the current stack location
;     in an I/O Request Packet (IRP).
;
; Arguments:
;
;     Irp - Pointer to the I/O Request Packet.
;
; Return Value:
;
;     The function value is a pointer to the current stack location in the
;     packet.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

IoGetCurrentIrpStackLocation MACRO pIrp:REQ
	IFDIFI <pIrp>, <eax>	;; don't move eax onto itself
		mov eax, pIrp
	ENDIF
	mov eax, (_IRP PTR [eax]).Tail.Overlay.CurrentStackLocation
ENDM

;typedef enum _MODE {
KernelMode	equ 0
UserMode	equ 1
MaximumMode	equ 2
;} MODE;


; Routines to support fast mutexes.

FAST_MUTEX STRUCT	; sizeof = 20h
	Count		SDWORD	?
	Owner		PVOID	?	; PTR KTHREAD
	Contention	DWORD	?
	Event		KEVENT	<>
	OldIrql		DWORD	?
FAST_MUTEX ENDS
PFAST_MUTEX typedef PTR FAST_MUTEX

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; ClientId
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CLIENT_ID STRUCT	; sizeof = 8 
	UniqueProcess	HANDLE	?
	UniqueThread	HANDLE	?
CLIENT_ID ENDS
PCLIENT_ID typedef PTR CLIENT_ID

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Thread Environment Block (and portable part of Thread Information Block)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;  NT_TIB - Thread Information Block - Portable part.
;
;      This is the subsystem portable part of the Thread Information Block.
;      It appears as the first part of the TEB for all threads which have
;      a user mode component.

NT_TIB STRUCT				; sizeof = 1Ch
	ExceptionList			PVOID	?	; PTR EXCEPTION_REGISTRATION_RECORD
	StackBase				PVOID	?	; 04h
	StackLimit				PVOID	?	; 08h
	SubSystemTib			PVOID	?	; 0Ch
	union
		FiberData			PVOID	?	; 10h
		Version				DWORD	?	; 10h
	ends
	ArbitraryUserPointer	PVOID	?	; 14h
	Self					PVOID	?	; 18h PTR NT_TIB
NT_TIB ENDS
PNT_TIB typedef PTR NT_TIB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Process Information Classes
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; typedef enum _PROCESSINFOCLASS

    ProcessBasicInformation			equ 0
    ProcessQuotaLimits				equ 1
    ProcessIoCounters				equ 2
    ProcessVmCounters				equ 3
    ProcessTimes					equ 4
    ProcessBasePriority				equ 5
    ProcessRaisePriority			equ 6
    ProcessDebugPort				equ 7
    ProcessExceptionPort			equ 8
    ProcessAccessToken				equ 9
    ProcessLdtInformation			equ 10
    ProcessLdtSize					equ 11
    ProcessDefaultHardErrorMode		equ 12
    ProcessIoPortHandlers			equ 13 ; Note: this is kernel mode only
    ProcessPooledUsageAndLimits		equ 14
    ProcessWorkingSetWatch			equ 15
    ProcessUserModeIOPL				equ 16
    ProcessEnableAlignmentFaultFixup equ 17
    ProcessPriorityClass			equ 18
    ProcessWx86Information			equ 19
    ProcessHandleCount				equ 20
    ProcessAffinityMask				equ 21
    ProcessPriorityBoost			equ 22
    ProcessDeviceMap				equ 23
    ProcessSessionInformation		equ 24
    ProcessForegroundInformation	equ 25
    ProcessWow64Information			equ 26
    MaxProcessInfoClass				equ 27

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Thread Information Classes
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; typedef enum _THREADINFOCLASS

    ThreadBasicInformation			equ 0
    ThreadTimes						equ 1
    ThreadPriority					equ 2
    ThreadBasePriority				equ 3
    ThreadAffinityMask				equ 4
    ThreadImpersonationToken		equ 5
    ThreadDescriptorTableEntry		equ 6
    ThreadEnableAlignmentFaultFixup	equ 7
    ThreadEventPair_Reusable		equ 8
    ThreadQuerySetWin32StartAddress	equ 9
    ThreadZeroTlsCell				equ 10
    ThreadPerformanceCount			equ 11
    ThreadAmILastThread				equ 12
    ThreadIdealProcessor			equ 13
    ThreadPriorityBoost				equ 14
    ThreadSetTlsArrayAddress		equ 15
    ThreadIsIoPending				equ 16
    ThreadHideFromDebugger			equ 17
    MaxThreadInfoClass				equ 18

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Process Information Structures
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; PageFaultHistory Information
;  NtQueryInformationProcess using ProcessWorkingSetWatch

PROCESS_WS_WATCH_INFORMATION STRUCT	; sizeof = 8
	FaultingPc	PVOID	?
	FaultingVa	PVOID	?
PROCESS_WS_WATCH_INFORMATION ENDS
PPROCESS_WS_WATCH_INFORMATION typedef PTR PROCESS_WS_WATCH_INFORMATION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Basic Process Information
;  NtQueryInformationProcess using ProcessBasicInfo
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PROCESS_BASIC_INFORMATION STRUCT	; sizeof = 18h
	ExitStatus				NTSTATUS	?
	PebBaseAddress			PVOID		? ; PPEB
	AffinityMask			DWORD		?
	BasePriority			DWORD		? ; KPRIORITY
	UniqueProcessId			DWORD		?
	InheritedFromUniqueProcessId	DWORD		?
PROCESS_BASIC_INFORMATION ENDS
PPROCESS_BASIC_INFORMATION typedef PTR PROCESS_BASIC_INFORMATION

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Process I/O Counters
;  NtQueryInformationProcess using ProcessIoCounters
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; begin_winnt
IO_COUNTERS STRUCT		; sizeof = 30h
	ReadOperationCount	QWORD	?
	WriteOperationCount	QWORD	?	; 08h
	OtherOperationCount	QWORD	?	; 10h
	ReadTransferCount	QWORD	?	; 18h
	WriteTransferCount	QWORD	?	; 20h
	OtherTransferCount	QWORD	?	; 28h
IO_COUNTERS ENDS
PIO_COUNTERS typedef PTR IO_COUNTERS
; end_winnt

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Processor Control Block (PRCB)
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PRCB_MINOR_VERSION		equ 1
PRCB_MAJOR_VERSION		equ 1
PRCB_BUILD_DEBUG		equ 1
PRCB_BUILD_UNIPROCESSOR	equ 2

; base address 0FFDFF120h

KPRCB STRUCT		; sizeof = 1Ch

; Major and minor version numbers of the PCR.

	MinorVersion	WORD	?
	MajorVersion	WORD	?

; Start of the architecturally defined section of the PRCB. This section
; may be directly addressed by vendor/platform specific HAL code and will
; not change from version to version of NT.

	CurrentThread	PVOID		? ; 04h PTR KTHREAD
	NextThread		PVOID		? ; 08h PTR KTHREAD
	IdleThread		PVOID		? ; 0Ch PTR KTHREAD
	Number			CHAR 		? ; 10h
	Reserved		CHAR 		? ; 11h
	BuildType		WORD		? ; 12h
	SetMember		KAFFINITY	? ; 14h

	RestartBlock	PVOID		? ; 18h PTR RESTART_BLOCK

KPRCB ENDS
PKPRCB typedef PTR KPRCB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Processor Control Region Structure Definition
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PCR_MINOR_VERSION	equ 1
PCR_MAJOR_VERSION	equ 1

; base address 0FFDFF000h

KPCR STRUCT			; sizeof = 54h

; Start of the architecturally defined section of the PCR. This section
; may be directly addressed by vendor/platform specific HAL code and will
; not change from version to version of NT.

	NtTib			NT_TIB	<>
	SelfPcr			PVOID		? ; 1Ch PTR KPCR flat address of this PCR
	Prcb			PKPRCB		? ; 20h pointer to Prcb
	Irql			BYTE		? ; 24h KIRQL
					db	3 dup(?)	; padding
	IRR				DWORD		? ; 28h
	IrrActive		DWORD		? ; 2Ch
	IDR				DWORD		? ; 30h
	Reserved2		DWORD		?

	IDT				PVOID		? ; 38h PTR KIDTENTRY
	GDT				PVOID		? ; 3Ch PTR KGDTENTRY
	TSS				PVOID		? ; 40h PTR KTSS
	MajorVersion	WORD		? ; 44h
	MinorVersion	WORD		? ; 46h
	SetMember		KAFFINITY	? ; 48h
	StallScaleFactor DWORD		? ; 4Ch
	DebugActive		BYTE		? ; 50h
	Number			BYTE		? ; 51h
					db 	2 dup(?)	; padding
KPCR ENDS
PKPCR typedef PTR KPCR

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Product types
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt		equ 1
    NtProductLanManNt	equ 2
    NtProductServer		equ 3

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; the bit mask, SharedUserData->SuiteMask, is a ULONG
; so there can be a maximum of 32 entries
; in this enum.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _SUITE_TYPE {
    SmallBusiness			equ 0
    Enterprise				equ 1
    BackOffice				equ 2
    CommunicationServer		equ 3
    TerminalServer			equ 4
    SmallBusinessRestricted	equ 5
    EmbeddedNT				equ 6
    DataCenter				equ 7
    SingleUserTS			equ 8
    Personal				equ 9
    ServerAppliance			equ 10
    MaxSuiteType			equ 11

VER_SERVER_NT                       equ 80000000
VER_WORKSTATION_NT                  equ 40000000
VER_SUITE_SMALLBUSINESS             equ 00000001
VER_SUITE_ENTERPRISE                equ 00000002
VER_SUITE_BACKOFFICE                equ 00000004
VER_SUITE_COMMUNICATIONS            equ 00000008
VER_SUITE_TERMINAL                  equ 00000010
VER_SUITE_SMALLBUSINESS_RESTRICTED  equ 00000020
VER_SUITE_EMBEDDEDNT                equ 00000040
VER_SUITE_DATACENTER                equ 00000080
VER_SUITE_SINGLEUSERTS              equ 00000100
VER_SUITE_PERSONAL                  equ 00000200
VER_SUITE_SERVERAPPLIANCE           equ 00000400

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define system time structure.

KSYSTEM_TIME STRUCT		; sizeof = 0Ch
	LowPart		DWORD	?
	High1Time	SDWORD	?
	High2Time	SDWORD	?
KSYSTEM_TIME ENDS
PKSYSTEM_TIME typedef PTR KSYSTEM_TIME

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Defined processor features
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PF_FLOATING_POINT_PRECISION_ERRATA  equ 0
PF_FLOATING_POINT_EMULATED          equ 1
PF_COMPARE_EXCHANGE_DOUBLE          equ 2
PF_MMX_INSTRUCTIONS_AVAILABLE       equ 3
PF_PPC_MOVEMEM_64BIT_OK             equ 4
PF_ALPHA_BYTE_INSTRUCTIONS          equ 5
PF_XMMI_INSTRUCTIONS_AVAILABLE      equ 6
PF_3DNOW_INSTRUCTIONS_AVAILABLE     equ 7
PF_RDTSC_INSTRUCTION_AVAILABLE      equ 8
PF_PAE_ENABLED                      equ 9

;typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE {
	StandardDesign	equ 0	; None == 0 == standard design
	NEC98x86		equ 1	; NEC PC98xx series on X86
	EndAlternatives	equ 2	; past end of known alternatives

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KI_USER_SHARED_DATA	equ 0ffdf0000h
SharedUserData		equ KI_USER_SHARED_DATA

PROCESSOR_FEATURE_MAX	equ 64

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Semaphore object
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KSEMAPHORE STRUCT	; sizeof = 14h
	Header	DISPATCHER_HEADER	<>
	Limit	SDWORD	?
KSEMAPHORE ENDS
PKSEMAPHORE typedef PTR KSEMAPHORE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Timer object
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KTIMER STRUCT		; sizeof = 28h
	Header			DISPATCHER_HEADER	<>
	DueTime			ULARGE_INTEGER		<>	; 10h
	TimerListEntry	LIST_ENTRY			<>	; 18h
	Dpc				PKDPC				?
	Period			SDWORD				?	; 24h
KTIMER ENDS
PKTIMER typedef PTR KTIMER


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Exception flag definitions.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EXCEPTION_NONCONTINUABLE 			equ 1	; Noncontinuable exception

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define maximum number of exception parameters.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EXCEPTION_MAXIMUM_PARAMETERS		equ 15	; maximum number of exception parameters

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Exception record definition.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EXCEPTION_RECORD STRUCT
	ExceptionCode			NTSTATUS	?
	ExceptionFlags			DWORD		?
	ExceptionRecord			PVOID		?	; PTR EXCEPTION_RECORD
	ExceptionAddress		PVOID		?
	NumberParameters		DWORD		?
	ExceptionInformation	DWORD 		EXCEPTION_MAXIMUM_PARAMETERS dup(?)
EXCEPTION_RECORD ENDS
PEXCEPTION_RECORD typedef PTR EXCEPTION_RECORD

EXCEPTION_RECORD32 STRUCT
	ExceptionCode			NTSTATUS	?
	ExceptionFlags			DWORD	?
	ExceptionRecord			DWORD	?
	ExceptionAddress		DWORD	?
	NumberParameters		DWORD	?
	ExceptionInformation	DWORD EXCEPTION_MAXIMUM_PARAMETERS dup(?)
EXCEPTION_RECORD32 ENDS
PEXCEPTION_RECORD32 typedef PTR EXCEPTION_RECORD32

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Typedef for pointer returned by exception_info()
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EXCEPTION_POINTERS STRUCT
	ExceptionRecord	PVOID	?	; PTR EXCEPTION_RECORD
	ContextRecord	PVOID	?	; PTR CONTEXT
EXCEPTION_POINTERS ENDS
PEXCEPTION_POINTERS typedef PTR EXCEPTION_POINTERS

SIZE_OF_80387_REGISTERS			equ 80
MAXIMUM_SUPPORTED_EXTENSION		equ 512

FLOATING_SAVE_AREA STRUCT
	ControlWord		DWORD	?
	StatusWord		DWORD	?
	TagWord			DWORD	?
	ErrorOffset		DWORD	?
	ErrorSelector	DWORD	?
	DataOffset		DWORD	?
	DataSelector	DWORD	?
	RegisterArea	BYTE SIZE_OF_80387_REGISTERS dup(?)
	Cr0NpxState		DWORD	?
FLOATING_SAVE_AREA ENDS
PFLOATING_SAVE_AREA typedef PTR FLOATING_SAVE_AREA

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Context Frame
;
;  This frame has a several purposes: 1) it is used as an argument to
;  NtContinue, 2) is is used to constuct a call frame for APC delivery,
;  and 3) it is used in the user level thread creation routines.
;
;  The layout of the record conforms to a standard call frame.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CONTEXT STRUCT

	; The flags values within this flag control the contents of
	; a CONTEXT record.
	;
	; If the context record is used as an input parameter, then
	; for each portion of the context record controlled by a flag
	; whose value is set, it is assumed that that portion of the
	; context record contains valid context. If the context record
	; is being used to modify a threads context, then only that
	; portion of the threads context will be modified.
	;
	; If the context record is used as an IN OUT parameter to capture
	; the context of a thread, then only those portions of the thread's
	; context corresponding to set flags will be returned.
	;
	; The context record is never used as an OUT only parameter.

	ContextFlags	DWORD	?

	; This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
	; set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
	; included in CONTEXT_FULL.

	regDr0			DWORD	?
	regDr1			DWORD	?
	regDr2			DWORD	?
	regDr3			DWORD	?
	regDr6			DWORD	?
	regDr7			DWORD	?

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_FLOATING_POINT.

	FloatSave		FLOATING_SAVE_AREA <>

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_SEGMENTS.

	regSegGs		DWORD	?
	regSegFs		DWORD	?
	regSegEs		DWORD	?
	regSegDs		DWORD	?

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_INTEGER.

	regEdi			DWORD	?
	regEsi			DWORD	?
	regEbx			DWORD	?
	regEdx			DWORD	?
	regEcx			DWORD	?
	regEax			DWORD	?

	; This section is specified/returned if the
	; ContextFlags word contians the flag CONTEXT_CONTROL.

	regEbp			DWORD	?
	regEip			DWORD	?
	regSegCs		DWORD	?	; MUST BE SANITIZED
	regEFlags		DWORD	?	; MUST BE SANITIZED
	regEsp			DWORD	?
	regSegSs		DWORD	?

	; This section is specified/returned if the ContextFlags word
	; contains the flag CONTEXT_EXTENDED_REGISTERS.
	; The format and contexts are processor specific

	ExtendedRegisters	BYTE	MAXIMUM_SUPPORTED_EXTENSION dup(?)

CONTEXT ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; i386 Specific portions of mm component
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the page size for the Intel 386 as 4096 (0x1000).
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PAGE_SIZE	equ 1000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the number of trailing zeroes in a page aligned virtual address.
; This is used as the shift count when shifting virtual addresses to
; virtual page numbers.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PAGE_SHIFT	equ 12

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the number of bits to shift to right justify the Page Directory Index
; field of a PTE.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PDI_SHIFT_X86		equ 22
PDI_SHIFT_X86PAE	equ 21
;#define PPI_SHIFT 30


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the number of bits to shift to right justify the Page Table Index
; field of a PTE.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PTI_SHIFT	equ 12

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Define the highest user address and user probe address.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; The lowest user address reserves the low 64k.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MM_LOWEST_USER_ADDRESS	equ 10000h

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; The lowest address for system space.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;#if !defined (_X86PAE_)
;MM_LOWEST_SYSTEM_ADDRESS	equ C0800000h
;#else
;#define MM_LOWEST_SYSTEM_ADDRESS (PVOID)0xC0C00000
;#endif

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Result type definition for i386.  (Machine specific enumerate type
; which is return type for portable exinterlockedincrement/decrement
; procedures.)  In general, you should use the enumerated type defined
; in ex.h instead of directly referencing these constants.
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; Flags loaded into AH by LAHF instruction

EFLAG_SIGN		equ 8000h
EFLAG_ZERO		equ 4000h
EFLAG_SELECT	equ EFLAG_SIGN or EFLAG_ZERO


;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Interrupt Request Level definitions
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PASSIVE_LEVEL		equ 0			; Passive release level
LOW_LEVEL			equ 0			; Lowest interrupt level
APC_LEVEL			equ 1			; APC interrupt level
DISPATCH_LEVEL		equ 2			; Dispatcher level

PROFILE_LEVEL		equ 27			; timer used for profiling.
CLOCK1_LEVEL		equ 28			; Interval clock 1 level - Not used on x86
CLOCK2_LEVEL		equ 28			; Interval clock 2 level
IPI_LEVEL			equ 29			; Interprocessor interrupt level
POWER_LEVEL			equ 30			; Power failure level
HIGH_LEVEL			equ 31			; Highest interrupt level
SYNCH_LEVEL			equ IPI_LEVEL-1	; synchronization level

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; These two macros are not a part of original ntddk.h file of course
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IFNDEF @ArgRev
@ArgRev MACRO arglist
    LOCAL txt, arg
    txt TEXTEQU <>
%   FOR arg, <arglist>
        txt CATSTR <arg>, <!,>, txt
    ENDM
    txt SUBSTR  txt, 1, @SizeStr(%txt) - 1
    txt CATSTR  <!<>, txt, <!>>
    EXITM txt
ENDM
ENDIF
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

fastcall MACRO api:REQ, p1, p2, px:VARARG

; Added for supporting fastcall calling convention
;
; Usage Example:
;   	fastcall IofCompleteRequest, pIrp, IO_NO_INCREMENT
;
; addr is not supported
; offset is not supported for first and second param
; May be next time

local arg, line, recx

	recx = 0		

	IFNB <px>
		% FOR arg, @ArgRev( <px> )
			push arg
		ENDM
	ENDIF

	IFNB <p1>
		IF (OPATTR (p1)) AND 00010000y	;; Is a register value
			IFDIFI <p1>, <ecx>			;; do not move ecx onto itself
				mov ecx, p1
				recx = 1				;; no more ecx
			ENDIF
		ELSEIF (OPATTR (p1)) AND 00000100y	;; Is an immediate value
			IF p1 EQ 0
				xor ecx, ecx
			ELSEIF p1 EQ 1
				xor ecx, ecx
				inc ecx
			ELSEIF p1 EQ -1
				or ecx, -1
				;;xor ecx, ecx
				;;dec ecx
			ELSE
				mov ecx, p1
				recx = 1				;; no more ecx
			ENDIF
		ELSE
			mov ecx, p1
			recx = 1					;; no more ecx
		ENDIF

		IFNB <p2>
			IF (OPATTR (p2)) AND 00010000y	;; Is a register value
				IFIDNI <p2>, <ecx>
					IF recx							;; if ecx was used report error
						line TEXTEQU %@Line
						% ECHO @FileCur(line) : ERROR! ECX register value overwritten by fastcall macro.
						.ERR
					ENDIF
				ENDIF

				IFDIFI <p2>, <edx>		;; do not move edx onto itself
					mov edx, p2
				ENDIF
			ELSEIF (OPATTR (p2)) AND 00000100y	;; Is an immediate value
				IF p2 EQ 0
					xor edx, edx
				ELSEIF p2 EQ 1
					xor edx, edx
					inc edx
				ELSEIF p2 EQ -1
					or edx, -1
					;;xor edx, edx
					;;dec edx
				ELSE
					mov edx, p2
				ENDIF
			ELSE
				mov edx, p2
			ENDIF
		ENDIF
	ENDIF
	call api
ENDM

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; This helper macro is also not a part of original ntddk.h of course
; It helps me much never forget what i have to improve in source code of driver
;
; Usage:
;    Fix
; or
;    Fix <The system always crushes here>
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
IFNDEF Fix
Fix MACRO txt:=<Fix this later!!!!>
	local pos, spos

	pos = 0
	spos = 0

	% FORC fix_chr, @FileCur		;; Don't display full path. Easier to read.
		pos = pos + 1
		IF "&fix_chr" EQ 5Ch		;; "/"
			spos = pos
		ENDIF
	ENDM
	% ECHO @CatStr(<Fix: >, @SubStr(%@FileCur, spos+1,), <(%@Line) - txt>)
ENDM
ENDIF