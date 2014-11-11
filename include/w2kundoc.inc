comment ^

Module Name:
    w2kundoc.inc

Abstract:
    This module defines some undocumented W2000 structures and constants.

Author:
    Four-F (four-f@mail.ru)

Last Update:
	13-Jan-2003

Comments:
	Hand made -> Bugs are very possible ;(
	Your bug report is very welcome.
^

IFNDEF KAFFINITY
	include ntddk.inc
ENDIF

;IFNDEF KPROCESSOR_MODE
;	KPROCESSOR_MODE typedef BYTE
;ENDIF

;IFNDEF ULARGE_INTEGER
;	include ntdef.inc
;ENDIF

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KAPC_STATE STRUCT		; sizeof = 18h
	ApcListHead			LIST_ENTRY 2 dup(<?>)
	Process				PVOID	?	; 10h PTR KPROCESS
	KernelApcInProgress	BYTE	?	; 14h
	KernelApcPending	BYTE	?	; 15h
	UserApcPending		BYTE	?	; 16h
	db ?
KAPC_STATE ENDS
PKAPC_STATE typedef PTR KAPC_STATE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KGDTENTRY STRUCT		; sizeof = 8
	LimitLow		WORD	?
	BaseLow			WORD	?
	union dwHighWord		; original HighWord
		struct Bytes
			BaseMid	BYTE	?
			Flags1	BYTE	?
			Flags2	BYTE	?
			BaseHi	BYTE	?
		ends
		; Damn! ...record field names must be unique...
		; kd displays it as __unnamed11, so i prepend each field name with 'u11'
		Bits RECORD \
			u11BaseHi:8,		; bits24-31 BaseHi
			u11Granularity:1,	; bits23-23 Granularity
			u11Default_Big:1,	; bits22-22 Default_Big
			u11Reserved_0:1,	; bits21-21 Reserved_0
			u11Sys:1,			; bits20-20 Sys
			u11LimitHi:4,		; bits16-19 LimitHi
			u11Pres:1,			; bits15-15 Pres
			u11Dpl:2,			; bits13-14 Dpl
			u11Type:5,			; bits8-12  Type
			u11BaseMid:8		; bits0-7   BaseMid
	ends ; HighWord
KGDTENTRY ENDS
PKGDTENTRY typedef PTR KGDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KIDTENTRY STRUCT		; sizeof = 8
	woOffset		WORD	?	; original name Offset
	Selector		WORD	?
	Access			WORD	?
	ExtendedOffset	WORD	?
KIDTENTRY ENDS
PKIDTENTRY typedef PTR KIDTENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PS_IMPERSONATION_INFORMATION STRUCT			; sizeof = 0Ch
	Token				PVOID	?
	CopyOnOpen			BYTE	?	; 4h
	EffectiveOnly		BYTE	?	; 5h
						db 2 dup(?)	; padding
	ImpersonationLevel	UINT	?	; 8h
PS_IMPERSONATION_INFORMATION ENDS
PPS_IMPERSONATION_INFORMATION typedef PTR PS_IMPERSONATION_INFORMATION

WOW64_PROCESS STRUCT
	Wow64	PVOID	?
WOW64_PROCESS ENDS
PWOW64_PROCESS typedef PTR WOW64_PROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

MMSUPPORT_FLAGS RECORD \
	Filler:25,			; bits7-31 Filler
	WriteWatch:1,		; bits6-6 WriteWatch
	WorkingSetHard:1,	; bits5-5 WorkingSetHard
	TrimHard:1,			; bits4-4 TrimHard
	SessionLeader:1,	; bits3-3 SessionLeader
	ProcessInSession:1,	; bits2-2 ProcessInSession
	BeingTrimmed:1,		; bits1-1 BeingTrimmed
	SessionSpace:1		; bits0-0 SessionSpace

MMSUPPORT STRUCT		; sizeof = 48h
;db 48h dup(?)
	LastTrimTime				LARGE_INTEGER	<>
	LastTrimFaultCount			DWORD		?	; 08h
	PageFaultCount				DWORD		?	; 0Ch
	PeakWorkingSetSize			DWORD		?	; 10h
	WorkingSetSize				DWORD		?	; 14h
	MinimumWorkingSetSize		DWORD		?	; 18h
	MaximumWorkingSetSize		DWORD		?	; 1Ch
	VmWorkingSetList			PVOID		?	; 20h
	WorkingSetExpansionLinks	LIST_ENTRY	<>	; 24h
	AllowWorkingSetAdjustment	BOOLEAN		?	; 2Ch
	AddressSpaceBeingDeleted	BOOLEAN		?	; 2Dh
	ForegroundSwitchCount		BYTE		?	; 2Eh
	MemoryPriority				BYTE		?	; 2Fh
	union										; 30h
		LongFlags				DWORD		?
		Flags					MMSUPPORT_FLAGS	<>
	ends
	Claim						DWORD		?	; 34h
	NextEstimationSlot			DWORD		?	; 38h
	NextAgingSlot				DWORD		?	; 3Ch
	EstimatedAvailable			DWORD		?	; 40h
	GrowthSinceLastEstimate		DWORD		?	; 44h
MMSUPPORT ENDS
PMMSUPPORT typedef PTR MMSUPPORT

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OWNER_ENTRY STRUCT		; sizeof = 8
	OwnerThread		DWORD	?
	union
		OwnerCount	SDWORD	?
		TableSize		DWORD	?
	ends
OWNER_ENTRY ENDS
POWNER_ENTRY typedef PTR OWNER_ENTRY

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ERESOURCE STRUCT			; sizeof = 38h
	SystemResourcesList			LIST_ENTRY	<>
	OwnerTable					PVOID		?	; 08h PTR OWNER_ENTRY
	ActiveCount					WORD		?	; 0Ch
	Flag						WORD		?	; 0Eh
	SharedWaiters				PVOID		?	; 10h PTR KSEMAPHORE
	ExclusiveWaiters			PVOID		?	; 14h PTR KEVENT
	OwnerThreads				OWNER_ENTRY	2 dup(<>)	; 18h
	ContentionCount				DWORD		?	; 28h
	NumberOfSharedWaiters		WORD		?	; 2Ch
	NumberOfExclusiveWaiters	WORD		?	; 2Eh
	union
		Address					PVOID		?	; 30h
		CreatorBackTraceIndex	DWORD		?	; 30
	ends
	SpinLock					DWORD		?	; 34h
ERESOURCE ENDS
PERESOURCE typedef PTR ERESOURCE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

; page directory entry (4-mB page)
; I've prefixed all fields with "pde4m"
HARDWARE_PDE4M_X86 RECORD \
	pde4mPageFrameNumber:10,	; bits22-31 PageFrameNumber
	pde4m_reserved:13,			; bits9-21 reserved
	pde4mGlobal:1,				; bit8 Global
	pde4mLargePage:1,			; bit7 LargePage
	pde4mDirty:1,				; bit6 Dirty
	pde4mAccessed:1,			; bit5 Accessed
	pde4mCacheDisable:1,		; bit4 CacheDisable
	pde4mWriteThrough:1,		; bit3 WriteThrough
	pde4mOwner:1, 				; bit2 Owner
	pde4mWrite:1,				; bit1 Write
	pde4mValid:1				; bit0 Valid

; page directory entry (4-kB page)
; I've prefixed all fields with "pde4k"
HARDWARE_PDE4K_X86 RECORD \
	pde4kPageFrameNumber:20,; bits12-31
	pde4k_reserved:3,		; bit9-11
	pde4kGlobal:1,			; bit8
	pde4kLargePage:1,		; bit7
	pde4kDirty:1,			; bit6 Reserved	???
	pde4kAccessed:1,		; bit5
	pde4kCacheDisable:1,	; bit4
	pde4kWriteThrough:1,	; bit3
	pde4kOwner:1,			; bit2
	pde4kWrite:1,			; bit1
	pde4kValid:1			; bit0

; page table entry
; I've prefixed all fields with "pte"
HARDWARE_PTE_X86 RECORD \
	ptePageFrameNumber:20,	; bits12-31 PageFrameNumber
	pte_reserved:1,			; bit11 reserved
	ptePrototype:1,			; bit10 Prototype
	pteCopyOnWrite:1,		; bit9 CopyOnWrite
	pteGlobal:1,			; bit8 Global
	pteLargePage:1,			; bit7 LargePage
	pteDirty:1,				; bit6 Dirty
	pteAccessed:1,			; bit5 Accessed
	pteCacheDisable:1,		; bit4 CacheDisable
	pteWriteThrough:1,		; bit3 WriteThrough
	pteOwner:1, 			; bit2 Owner
	pteWrite:1,				; bit1 Write
	pteValid:1				; bit0 Valid

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;HANDLE_TABLE_ENTRY STRUCT		; sizeof = 8
;	Object					PVOID	?
;	ObAttributes			DWORD	?
;	GrantedAccess			DWORD	?
;	GrantedAccessIndex		DWORD	?
;	CreatorBackTraceIndex	DWORD	?
;	NextFreeTableEntry		DWORD	?


HANDLE_TABLE STRUCT			; sizeof = 6Ch
	Flags					DWORD		?
	HandleCount				SDWORD		?	; 04h
	Table					PVOID		?	; 08h PTR PTR PTR HANDLE_TABLE_ENTRY
	QuotaProcess			PVOID		?	; 0Ch PTR EPROCESS
	UniqueProcessId			PVOID		?	; 10h
	FirstFreeTableEntry		SDWORD		?	; 14h
	NextIndexNeedingPool	SDWORD		?	; 18h
	HandleTableLock			ERESOURCE	<>	; 1Ch
	HandleTableList			LIST_ENTRY	<>	; 54h
	HandleContentionEvent	KEVENT		<>	; 5Ch
HANDLE_TABLE ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OBJECT_NAME_INFORMATION STRUCT		; sizeof = 8
	usName	UNICODE_STRING	<>		; original name Name
OBJECT_NAME_INFORMATION ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PAGEFAULT_HISTORY STRUCT		; sizeof = 18h
	CurrentIndex	DWORD	?
	MaxIndex		DWORD	?	; 04h
	SpinLock		DWORD	?	; 08h
	Reserved		PVOID	?	; 0Ch
	WatchInfo		PROCESS_WS_WATCH_INFORMATION	<>	; 10h
PAGEFAULT_HISTORY ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS_QUOTA_BLOCK STRUCT			; sizeof = 2Ch
	QuotaLock				DWORD		?
	ReferenceCount			DWORD		?	; 04h
	QuotaPeakPoolUsage		DWORD 2 dup(?)	; 08h
	QuotaPoolUsage			DWORD 2 dup(?)	; 10h
	QuotaPoolLimit			DWORD 2 dup(?)	; 18h
	PeakPagefileUsage		DWORD		?	; 20h
	PagefileUsage			DWORD		?	; 24h
	PagefileLimit			DWORD		?	; 28h
EPROCESS_QUOTA_BLOCK ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EJOB STRUCT						; sizeof = 170h
	Event						KEVENT			<>
	JobLinks					LIST_ENTRY		<>	; 010h
	ProcessListHead				LIST_ENTRY		<>	; 018h
	JobLock						ERESOURCE		<>	; 020h
	TotalUserTime				LARGE_INTEGER	<>	; 058h
	TotalKernelTime				LARGE_INTEGER	<>	; 060h
	ThisPeriodTotalUserTime		LARGE_INTEGER	<>	; 068h
	ThisPeriodTotalKernelTime	LARGE_INTEGER	<>	; 070h
	TotalPageFaultCount			DWORD			?	; 078h
	TotalProcesses				DWORD			?	; 07Ch
	ActiveProcesses				DWORD			?	; 080h
	TotalTerminatedProcesses	DWORD			?	; 084h
	PerProcessUserTimeLimit		LARGE_INTEGER	<>	; 088h
	PerJobUserTimeLimit			LARGE_INTEGER	<>	; 090h
	LimitFlags					DWORD			?	; 098h
	MinimumWorkingSetSize		DWORD			?	; 09Ch
	MaximumWorkingSetSize		DWORD			?	; 0A0h
	ActiveProcessLimit			DWORD			?	; 0A4h
	Affinity					DWORD			?	; 0A8h
	PriorityClass				BYTE			?	; 0ACh
								db 	3 dup(?)		; padding
	UIRestrictionsClass			DWORD			?	; 0B0h
	SecurityLimitFlags			DWORD			?	; 0B4h
	Token						PVOID			?	; 0B8h
	Filter						PVOID			?	; 0BCh PTR PS_JOB_TOKEN_FILTER
	EndOfJobTimeAction			DWORD			?	; 0C0h
	CompletionPort				PVOID			?	; 0C4h
	CompletionKey				PVOID			?	; 0C8h
	SessionId					DWORD			?	; 0CCh
	SchedulingClass				DWORD			?	; 0D0h
								dd				?	; padding
	ReadOperationCount			QWORD			?	; 0D8h
	WriteOperationCount			QWORD			?	; 0E0h
	OtherOperationCount			QWORD			?	; 0E8h
	ReadTransferCount			QWORD			?	; 0F0h
	WriteTransferCount			QWORD			?	; 0F8h
	OtherTransferCount			QWORD			?	; 100h
	IoInfo						IO_COUNTERS		<>	; 108h
	ProcessMemoryLimit			DWORD			?	; 138h
	JobMemoryLimit				DWORD			?	; 13Ch
	PeakProcessMemoryUsed		DWORD			?	; 140h
	PeakJobMemoryUsed			DWORD			?	; 144h
	CurrentJobMemoryUsed		DWORD			?	; 148h
	MemoryLimitsLock			FAST_MUTEX		<>	; 14Ch
								DWORD			?	; padding ???
EJOB ENDS
PEJOB typedef PTR EJOB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of EPROCESS structure (first member)
; also known as Process Control Block, PCB

KPROCESS STRUCT			; sizeof = 06Ch
	Header				DISPATCHER_HEADER	<> ; DO_TYPE_PROCESS (0x1B)
	ProfileListHead		LIST_ENTRY			<>
	DirectoryTableBase	DWORD				?	; 018h
	PageTableBase		DWORD				?	; 01Ch
	LdtDescriptor		KGDTENTRY			<>	; 020h
	Int21Descriptor		KIDTENTRY			<>	; 028h
	IopmOffset			WORD				?	; 030h
	Iopl				BYTE				?	; 032h
	VdmFlag				BOOLEAN				?	; 033h
	ActiveProcessors	DWORD				?	; 034h
	KernelTime			DWORD				?	; ticks
	UserTime			DWORD				?	; ticks
	ReadyListHead		LIST_ENTRY			<>	; 040h
	SwapListEntry		LIST_ENTRY			<>
	ThreadListHead		LIST_ENTRY			<>	; KTHREAD.ThreadListEntry
	ProcessLock			PVOID				?	; 058h
	Affinity			KAFFINITY			?	; 05Ch
	StackCount			WORD				?	; 060h
	BasePriority		BYTE				?	; 062h
	ThreadQuantum		BYTE				?	; 063h
	AutoAlignment		BOOLEAN				?	; 064h
	State				BYTE				?	; 065h
	ThreadSeed			BYTE				?	; 066h
	DisableBoost		BOOLEAN				?	; 067h
	PowerState			BYTE				?	; 068h
	DisableQuantum		BOOLEAN				?	; 069h
;	IdealNode			BYTE				?	; ???
	Spare				BYTE		  2 dup(?)	; 06Ah
KPROCESS ENDS
PKPROCESS typedef PTR KPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

EPROCESS STRUCT						; sizeof = 288h
	Pcb								KPROCESS		<>
	ExitStatus						DWORD			?	; 06Ch
	LockEvent						KEVENT			<>	; 070h
	LockCount						DWORD			?	; 080h
									DWORD			?	; 084h ???
	CreateTime						LARGE_INTEGER	<>	; 088h
	ExitTime						LARGE_INTEGER	<>	; 090h
	LockOwner						PVOID			?	; 098h PTR KTHREAD
	UniqueProcessId					DWORD			?	; 09Ch
	ActiveProcessLinks				LIST_ENTRY		<>	; 0A0h
	QuotaPeakPoolUsage				DWORD	2 dup(?)	; 0A8h  NP, P
	QuotaPoolUsage					DWORD	2 dup(?)	; 0B0h  NP, P
	PagefileUsage					DWORD			?	; 0B8h
	CommitCharge					DWORD			?	; 0BCh
	PeakPagefileUsage				DWORD			?	; 0C0h
	PeakVirtualSize					DWORD			?	; 0C4h
	VirtualSize						DWORD			?	; 0C8h
	Vm								MMSUPPORT		<>	; 0D0h
	SessionProcessLinks				LIST_ENTRY		<>	; 118h
	DebugPort						PVOID			?	; 120h
	ExceptionPort					PVOID			?	; 124h
	ObjectTable						PVOID			?	; 128h PTR HANDLE_TABLE
	Token							PVOID			?	; 12Ch
	WorkingSetLock					FAST_MUTEX		<>	; 130h
	OldIrql							DWORD			?	; 14Ch
	WorkingSetPage					DWORD			?	; 150h
	ProcessOutswapEnabled			BYTE			?	; 154h
	ProcessOutswapped				BYTE			?	; 155h
	AddressSpaceInitialized			BYTE			?	; 156h
	AddressSpaceDeleted				BYTE			?	; 157h
	AddressCreationLock				FAST_MUTEX		<>	; 158h
	HyperSpaceLock					DWORD			?	; 178h
	ForkInProgress					PVOID			?	; 17Ch PTR ETHREAD
	VmOperation						WORD			?	; 180h
	ForkWasSuccessful				BYTE			?	; 182h
	MmAgressiveWsTrimMask			BYTE			?	; 183h
	VmOperationEvent				PVOID			?	; 184h PTR KEVENT
	PaeTop							PVOID			?	; 188h
	LastFaultCount					DWORD			?	; 18Ch
	ModifiedPageCount				DWORD			?	; 190h
	VadRoot							PVOID			?	; 194h
	VadHint							PVOID			?	; 198h
	CloneRoot						PVOID			?	; 19Ch
	NumberOfPrivatePages			DWORD			?	; 1A0h
	NumberOfLockedPages				DWORD			?	; 1A4h
	NextPageColor					WORD			?	; 1A8h
	ExitProcessCalled				BYTE			?	; 1AAh
	CreateProcessReported			BYTE			?	; 1ABh
	SectionHandle					PVOID			?	; 1ACh
	Peb								PVOID			?	; 1B0h PTR PEB
	SectionBaseAddress				PVOID			?	; 1B4h
	QuotaBlock						PVOID			?	; 1B8h PTR EPROCESS_QUOTA_BLOCK
	LastThreadExitStatus			DWORD			?	; 1BCh
	WorkingSetWatch					PVOID			?	; 1C0h PTR PAGEFAULT_HISTORY
	Win32WindowStation				PVOID			?	; 1C4h
	InheritedFromUniqueProcessId	PVOID			?	; 1C8h
	GrantedAccess					DWORD			?	; 1CCh
	DefaultHardErrorProcessing		DWORD			?	; 1D0h
	LdtInformation					PVOID			?	; 1D4h
	VadFreeHint						PVOID			?	; 1D8h
	VdmObjects						PVOID			?	; 1DCh
	DeviceMap						PVOID			?	; 1E0h
	SessionId						DWORD			?	; 1E4h
	PhysicalVadList					LIST_ENTRY		<>	; 1E8h
	union
		PageDirectoryPte			HARDWARE_PTE_X86	<>	; 1F0h
		Filler						QWORD			?	; 1F0h
	ends
	PaePageDirectoryPage			DWORD			?	; 1F8h
	ImageFileName					BYTE	16 dup(?)	; 1FCh
	VmTrimFaultValue				DWORD			?	; 20Ch
	SetTimerResolution				BYTE			?	; 210h
	PriorityClass					BYTE			?	; 211h
	union
		struct
			SubSystemMinorVersion	BYTE			?	; 212h
			SubSystemMajorVersion	BYTE			?	; 213h
		ends
		SubSystemVersion			WORD			?	; 212h
	ends
	Win32Process					PVOID			?	; 214h
	Job								PVOID			?	; 218h PTR EJOB
	JobStatus						DWORD			?	; 21Ch
	JobLinks						LIST_ENTRY		<>	; 220h
	LockedPagesList					PVOID			?	; 228h
	SecurityPort					PVOID			?	; 22Ch
	Wow64Process					PVOID			?	; 230h PTR WOW64_PROCESS
									DWORD			?	; 234h ???
	ReadOperationCount				LARGE_INTEGER	<>	; 238h
	WriteOperationCount				LARGE_INTEGER	<>	; 240h
	OtherOperationCount				LARGE_INTEGER	<>	; 248h
	ReadTransferCount				LARGE_INTEGER	<>	; 250h
	WriteTransferCount				LARGE_INTEGER	<>	; 258h
	OtherTransferCount				LARGE_INTEGER	<>	; 260h
	CommitChargeLimit				DWORD			?	; 268h
	CommitChargePeak				DWORD			?	; 26Ch
	ThreadListHead					LIST_ENTRY		<>	; 270h
	VadPhysicalPagesBitMap			RTL_BITMAP		<>	; 278h
	VadPhysicalPages				DWORD			?	; 27Ch
	AweLock							DWORD			?	; 280h
EPROCESS ENDS
PEPROCESS typedef PTR EPROCESS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; a part of ETHREAD structure (first member)
; also known as Thread Control Block, TCB

KTHREAD STRUCT				; sizeof = 1B0h
	Header					DISPATCHER_HEADER	<>	; DO_TYPE_THREAD (0x6C)
	MutantListHead			LIST_ENTRY			<>	; 010h
	InitialStack			PVOID				?	; 018h
	StackLimit				PVOID				?	; 01Ch
	Teb						PVOID				?	; PTR TEB
	TlsArray				PVOID				?	; 024h
	KernelStack				PVOID				?	; 028h
	DebugActive				BOOLEAN				?	; 02Ch
	State					BYTE				?	; THREAD_STATE_*
	Alerted					BOOLEAN		2 dup(?)
	Iopl					BYTE				?	; 030h
	NpxState				BYTE				?
	Saturation				BYTE				?
	Priority				BYTE				?
	ApcState				KAPC_STATE			<>	; 034h
	ContextSwitches			DWORD				?	; 04Ch
	WaitStatus				DWORD				?	; 050h
	WaitIrql				BYTE				?	; 054h
	WaitMode				BYTE				?
	WaitNext				BYTE				?	; 056h
	WaitReason				BYTE				?
	WaitBlockList			PVOID				?	; 058h PTR KWAIT_BLOCK
	WaitListEntry			LIST_ENTRY			<>	; 05Ch
	WaitTime				DWORD				?	; 064h
	BasePriority			BYTE				?	; 068h
	DecrementCount			BYTE				?
	PriorityDecrement		BYTE				?
	Quantum					BYTE				?
	WaitBlock				KWAIT_BLOCK	4 dup(<>)	; 06Ch
	LegoData				DWORD				?	; 0CCh
	KernelApcDisable		DWORD				? 	; 0D0h
	UserAffinity			KAFFINITY			?	; 0D4h
	SystemAffinityActive	BOOLEAN				?	; 0d8H
	PowerState				BYTE				?
	NpxIrql					BYTE				?
	Pad						BYTE				?
	ServiceTable			PVOID				?	; 0DCh PTR SERVICE_DESCRIPTOR_TABLE
	Queue					PVOID				?	; 0E0h PTR KQUEUE
	ApcQueueLock			UINT				?	; 0E4h
	Timer					KTIMER				<>	; 0E8h
	QueueListEntry			LIST_ENTRY			<>	; 110h
	Affinity				KAFFINITY			?	; 118h
	Preempted				BOOLEAN				?	; 11Ch
	ProcessReadyQueue		BOOLEAN				?
	KernelStackResident		BOOLEAN				?
	NextProcessor			BYTE				?
	CallbackStack			PVOID				?	; 120h
	Win32Thread				PVOID				?	; 124h PTR WIN32_THREAD ???
	TrapFrame				PVOID				?
	ApcStatePointer			PVOID		2 dup(?)	; 12Ch PTR KAPC_STATE
	PreviousMode			KPROCESSOR_MODE		?	; 134h
	EnableStackSwap			BOOLEAN				?	; 135h
	LargeStack				BOOLEAN				?	; 136h
	ResourceIndex			BYTE				?	; 137h
	KernelTime				DWORD				?	; 138h ticks
	UserTime				DWORD				?	; 13Ch ticks
	SavedApcState			KAPC_STATE			<>	; 140h
	Alertable				BOOLEAN				?	; 158h
	ApcStateIndex			BYTE				?	; 159h
	ApcQueueable			BOOLEAN				?	; 15Ah
	AutoAlignment			BOOLEAN				?	; 15Bh
	StackBase				PVOID				?	; 15Ch
	SuspendApc				KAPC				<>	; 160h
	SuspendSemaphore		KSEMAPHORE			<>	; 190h
	ThreadListEntry			LIST_ENTRY			<>	; 1A4h see KPROCESS
	FreezeCount				BYTE				?	; 1ACh
	SuspendCount			BYTE				?	; 1ADh
	IdealProcessor			BYTE				?	; 1AEh
	DisableBoost			BOOLEAN				?	; 1AFh
KTHREAD ENDS
PKTHREAD typedef PTR KTHREAD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ETHREAD STRUCT				; sizeof = 248h
	Tcb						KTHREAD			<>
	CreateTime				LARGE_INTEGER	<>	; 1B0h
	; 1b0 bits0-1 NestedFaultCount
	; 1b0 bits2-2 ApcNeeded
	union
		ExitTime				LARGE_INTEGER	<>	; 1B8h
		LpcReplyChain			LIST_ENTRY		<>	; 1B8h
	ends
	union
		ExitStatus				DWORD		?	; 1C0h
		OfsChain				PVOID		?	; 1C0h
	ends
	PostBlockList				LIST_ENTRY	<>	; 1C4h
	TerminationPortList			LIST_ENTRY	<>	; 1CCh
	ActiveTimerListLock			UINT		?	; 1D4h
	ActiveTimerListHead			LIST_ENTRY	<>	; 1D8h
	Cid							CLIENT_ID	<>	; 1E0h
	LpcReplySemaphore			KSEMAPHORE	<>	; 1E8h
	LpcReplyMessage				PVOID		?	; 1FCh
	LpcReplyMessageId			UINT		?	; 200h
	PerformanceCountLow			UINT		?	; 204h
	ImpersonationInfo			PVOID		?	; 208h PTR PS_IMPERSONATION_INFORMATION
	IrpList						LIST_ENTRY	<>	; 20Ch
	TopLevelIrp					UINT		?	; 214h
	DeviceToVerify				PVOID		?	; 218h PTR DEVICE_OBJECT
	ReadClusterSize				UINT		?	; 21Ch
	ForwardClusterOnly			BYTE		?	; 220h
	DisablePageFaultClustering	BYTE		?	; 221h
	DeadThread					BYTE		?	; 222h
	HideFromDebugger			BYTE		?	; 223h
	HasTerminated				UINT		?	; 224h
	GrantedAccess				UINT		?	; 228h
	ThreadsProcess				PVOID		?	; 22Ch PTR EPROCESS
	StartAddress				PVOID		?	; 230h
	union
		Win32StartAddress		PVOID		?	; 234h
		LpcReceivedMessageId	UINT		?	; 234h
	ends
	LpcExitThreadCalled			BYTE		?	; 238h
	HardErrorsAreDisabled		BYTE		?	; 239h
	LpcReceivedMsgIdValid		BYTE		?	; 23Ah
	ActiveImpersonationInfo		BYTE		?	; 23Bh
	PerformanceCountHigh		UINT		?	; 23Ch
	ThreadListEntry				LIST_ENTRY	<>	; 240h
ETHREAD ENDS
PETHREAD typedef PTR ETHREAD

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

KQUEUE STRUCT		 ;sizeof = 28h
	Header			DISPATCHER_HEADER	<>
	EntryListHead	LIST_ENTRY			<>	; 10h
	CurrentCount	DWORD				?	; 18h
	MaximumCount	DWORD				?	; 1Ch
	ThreadListHead	LIST_ENTRY			<>	; 20h
KQUEUE ENDS
PKQUEUE typedef PTR KQUEUE

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB_LDR_DATA STRUCT					; sizeof = 24h
	dwLength						DWORD		?	; original name Length
	Initialized						BYTE		?	; 04h
									db 	3 dup(?)	; padding
	SsHandle						PVOID		?	; 08h
	InLoadOrderModuleList			LIST_ENTRY	<>	; 0Ch
	InMemoryOrderModuleList			LIST_ENTRY	<>	; 14h
	InInitializationOrderModuleList	LIST_ENTRY	<>	; 1Ch
PEB_LDR_DATA ENDS
PPEB_LDR_DATA typedef PTR PEB_LDR_DATA

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RTL_DRIVE_LETTER_CURDIR STRUCT	; sizeof = 10h
	Flags		WORD		?
	woLength	WORD		?	; 02h original name Length
	TimeStamp	DWORD		?	; 04h
	DosPath		_STRING		<>	; 08h
RTL_DRIVE_LETTER_CURDIR ENDS
PRTL_DRIVE_LETTER_CURDIR typedef PTR RTL_DRIVE_LETTER_CURDIR

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CURDIR STRUCT	; sizeof 0Ch
	DosPath		UNICODE_STRING	<>
	Handle		PVOID			?	; 08h
CURDIR ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB_FREE_BLOCK STRUCT	; sizeof = 8
	Next		PVOID	?	; PTR PEB_FREE_BLOCK
	dwSize		DWORD	?	; original name Size
PEB_FREE_BLOCK ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

RTL_USER_PROCESS_PARAMETERS STRUCT	; sizeof = 290h
	MaximumLength		DWORD			?
	dwLength			DWORD			?	; 004h original name Length
	Flags				DWORD			?	; 008h
	DebugFlags			DWORD			?	; 00Ch
	ConsoleHandle		PVOID			?	; 010h
	ConsoleFlags		DWORD			?	; 014h
	StandardInput		PVOID			?	; 018h
	StandardOutput		PVOID			?	; 01Ch
	StandardError		PVOID			?	; 020h
	CurrentDirectory	CURDIR			<>	; 024h
	DllPath				UNICODE_STRING	<>	; 030h
	ImagePathName		UNICODE_STRING	<>	; 038h
	CommandLine			UNICODE_STRING	<>	; 040h
	Environment			PVOID			?	; 048h
	StartingX			DWORD			?	; 04Ch
	StartingY			DWORD			?	; 050h
	CountX				DWORD			?	; 054h
	CountY				DWORD			?	; 058h
	CountCharsX			DWORD			?	; 05Ch
	CountCharsY			DWORD			?	; 060h
	FillAttribute		DWORD			?	; 064h
	WindowFlags			DWORD			?	; 068h
	ShowWindowFlags		DWORD			?	; 06Ch
	WindowTitle			UNICODE_STRING	<>	; 070h
	DesktopInfo			UNICODE_STRING	<>	; 078h
	ShellInfo			UNICODE_STRING	<>	; 080h
	RuntimeData			UNICODE_STRING	<>	; 088h
	CurrentDirectores	RTL_DRIVE_LETTER_CURDIR 32 dup(<>)	; 090h
RTL_USER_PROCESS_PARAMETERS ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; located at 7FFDF000h
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

PEB STRUCT							; sizeof = 1E8h
	InheritedAddressSpace			BOOLEAN		?
	ReadImageFileExecOptions		BOOLEAN		?	; 001
	BeingDebugged					BOOLEAN		?	; 002
	SpareBool						BYTE		?	; 003
	Mutant							PVOID		?	; 004
	ImageBaseAddress				PVOID		?	; 008
	Ldr								PVOID		?	; 00Ch PTR PEB_LDR_DATA
	ProcessParameters				PVOID		?	; 010h PTR RTL_USER_PROCESS_PARAMETERS
	SubSystemData					PVOID		?	; 014h
	ProcessHeap						PVOID		?	; 018h
	FastPebLock						PVOID		?	; 01Ch
	FastPebLockRoutine				PVOID		?	; 020h
	FastPebUnlockRoutine			PVOID		?	; 024h
	EnvironmentUpdateCount			DWORD		?	; 028h
	KernelCallbackTable				PVOID		?	; 02Ch
	SystemReserved					DWORD	2 dup(?)	; 030h
	FreeList						PVOID		?	; 038h PTR PEB_FREE_BLOCK
	TlsExpansionCounter				DWORD		?	; 03Ch
	TlsBitmap						PVOID		?	; 040h
	TlsBitmapBits					DWORD	2 dup(?)	; 044h
	ReadOnlySharedMemoryBase		PVOID		?	; 04Ch
	ReadOnlySharedMemoryHeap		PVOID		?	; 050h
	ReadOnlyStaticServerData		PVOID		?	; 054h
	AnsiCodePageData				PVOID		?	; 058h
	OemCodePageData					PVOID		?	; 05Ch
	UnicodeCaseTableData			PVOID		?	; 060h
	NumberOfProcessors				DWORD		?	; 064h
	NtGlobalFlag					DWORD		?	; 068h
									DWORD		?	; 064h
	CriticalSectionTimeout			LARGE_INTEGER	<>	; 070h
	HeapSegmentReserve				DWORD		?	; 078h
	HeapSegmentCommit				DWORD		?	; 07Ch
	HeapDeCommitTotalFreeThreshold	DWORD		?	; 080h
	HeapDeCommitFreeBlockThreshold	DWORD		?	; 084h
	NumberOfHeaps					DWORD		?	; 088h
	MaximumNumberOfHeaps			DWORD		?	; 08Ch
	ProcessHeaps					PVOID		?	; 090h
	GdiSharedHandleTable			PVOID		?	; 094h
	ProcessStarterHelper			PVOID		?	; 098h
	GdiDCAttributeList				DWORD		?	; 09Ch
	LoaderLock						PVOID		?	; 0A0h
	OSMajorVersion					DWORD		?	; 0A4h
	OSMinorVersion					DWORD		?	; 0A8h
	OSBuildNumber					WORD		?	; 0ACh
	OSCSDVersion					WORD		?	; 0AEh
	OSPlatformId					DWORD		?	; 0B0h
	ImageSubsystem					DWORD		?	; 0B4h
	ImageSubsystemMajorVersion		DWORD		?	; 0B8h
	ImageSubsystemMinorVersion		DWORD		?	; 0BCh
	ImageProcessAffinityMask		DWORD		?	; 0C0h
	GdiHandleBuffer					DWORD	34 dup(?)	; 0C4h
	PostProcessInitRoutine			DWORD		?	; 14Ch
	TlsExpansionBitmap				PVOID		?	; 150h
	TlsExpansionBitmapBits			DWORD	32 dup(?)	; 154h
	SessionId						DWORD		?	; 1D4h
	AppCompatInfo					PVOID		?	; 1D8h
	CSDVersion						UNICODE_STRING	<>	; 1DCh
									DWORD		?	; ???
PEB ENDS
PPEB typedef PTR PEB

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
; Task Segment State
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


KiIoAccessMap STRUCT		; sizeof= 2024h
	DirectionMap	BYTE 32 dup(?)
	IoMap			BYTE 8196 dup(?)	; 020h
KiIoAccessMap ENDS

KTSS STRUCT			; sizeof = 20ACh
	Backlink		WORD		?	; 0000
	Reserved0		WORD		?	; 0000
	Esp0			DWORD		?	; 0004h
	Ss0				WORD		?	; 0008h
	Reserved1		WORD		?	; 000Ah
	NotUsed1		DWORD 4 dup(?)	; 000Ch
	rCR3			DWORD		?	; 001Ch original field name CR3
	Eip				DWORD		?	; 0020h
	NotUsed2		DWORD 9 dup(?)	; 0024h
	rEs				WORD		?	; 0048h original field name Es
	Reserved2		WORD		?	; 004Ah
	rCs				WORD		?	; 004Ch original field name Cs
	Reserved3		WORD		?	; 004Eh
	rSs				WORD		?	; 0050h original field name Ss
	Reserved4		WORD		?	; 0052h
	rDs				WORD		?	; 0054h original field name Ds
	Reserved5		WORD		?	; 0056h
	rFs				WORD		?	; 0058h original field name Fs
	Reserved6		WORD		?	; 005Ah
	rGs				WORD		?	; 005Ch original field name Gs
	Reserved7		WORD		?	; 005Eh
	LDT				WORD		?	; 0060h
	Reserved8		WORD		?	; 0062h
	Flags			WORD		?	; 0064h
	IoMapBase		WORD		?	; 0066h
	IoMaps			KiIoAccessMap	<>	; 0068h
	IntDirectionMap	BYTE 32 dup(?)	; 208Ch
KTSS ENDS

;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
;
; SYSTEM_INFORMATION_CLASS
;
; The system information classes available in the “free” (retail) build of the system are
; listed below along with a remark as to whether the information class can be queried, set, or both.
; Some of the information classes labeled “SystemNotImplementedXxx” are implemented in the “checked” build,
; and a few of these classes are briefly described later.
;
;:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

;typedef enum _SYSTEM_INFORMATION_CLASS {

												; Query		Set
SystemBasicInformation					equ 0	;	Y		N
SystemProcessorInformation				equ 1	;	Y		N
SystemPerformanceInformation			equ 2	;	Y		N
SystemTimeOfDayInformation				equ 3	;	Y		N
SystemNotImplemented1					equ 4	;	Y		N
SystemProcessesAndThreadsInformation	equ 5	;	Y		N
SystemCallCounts						equ 6	;	Y		N
SystemConfigurationInformation			equ 7	;	Y		N
SystemProcessorTimes					equ 8	;	Y		N
SystemGlobalFlag						equ 9	;	Y		Y
SystemNotImplemented2					equ 10	;	Y		N
SystemModuleInformation					equ 11	;	Y		N
SystemLockInformation					equ 12	;	Y		N
SystemNotImplemented3					equ 13	;	Y		N
SystemNotImplemented4					equ 14	;	Y		N
SystemNotImplemented5					equ 15	;	Y		N
SystemHandleInformation					equ 16	;	Y		N
SystemObjectInformation					equ 17	;	Y		N
SystemPagefileInformation				equ 18	;	Y		N
SystemInstructionEmulationCounts		equ 19	;	Y		N
SystemInvalidInfoClass1					equ 20
SystemCacheInformation					equ 21	;	Y		Y
SystemPoolTagInformation				equ 22	;	Y		N
SystemProcessorStatistics				equ 23	;	Y		N
SystemDpcInformation					equ 24	;	Y		Y
SystemNotImplemented6					equ 25	;	Y		N
SystemLoadImage							equ 26	;	N		Y
SystemUnloadImage						equ 27	;	N		Y
SystemTimeAdjustment					equ 28	;	Y		Y
SystemNotImplemented7					equ 29	;	Y		N
SystemNotImplemented8					equ 30	;	Y		N
SystemNotImplemented9					equ 31	;	Y		N
SystemCrashDumpInformation				equ 32	;	Y		N
SystemExceptionInformation				equ 33	;	Y		N
SystemCrashDumpStateInformation			equ 34	;	Y		Y/N
SystemKernelDebuggerInformation			equ 35	;	Y		N
SystemContextSwitchInformation			equ 36	;	Y		N
SystemRegistryQuotaInformation			equ 37	;	Y		Y
SystemLoadAndCallImage					equ 38	;	N		Y
SystemPrioritySeparation				equ 39	;	N		Y
SystemNotImplemented10					equ 40	;	Y		N
SystemNotImplemented11					equ 41	;	Y		N
SystemInvalidInfoClass2					equ 42
SystemInvalidInfoClass3					equ 43
SystemTimeZoneInformation				equ 44	;	Y		N
SystemLookasideInformation				equ 45	;	Y		N
SystemSetTimeSlipEvent					equ 46	;	N		Y
SystemCreateSession						equ 47	;	N		Y
SystemDeleteSession						equ 48	;	N		Y
SystemInvalidInfoClass4					equ 49
SystemRangeStartInformation				equ 50	;	Y		N
SystemVerifierInformation				equ 51	;	Y		Y
SystemAddVerifier						equ 52	;	N		Y
SystemSessionProcessesInformation		equ 53	;	Y		N


SYSTEM_MODULE_INFORMATION STRUCT		;Information Class 11
	Reserved			DWORD	2 dup(?)
	Base				PVOID	?
	cbSize				DWORD	?	; original name Size
	Flags				DWORD	?
	Index				WORD	?
	Unknown				WORD	?
	LoadCount			WORD	?
	ModuleNameOffset	WORD	?
	ImageName			CHAR 256 dup(?)
SYSTEM_MODULE_INFORMATION ENDS
PSYSTEM_MODULE_INFORMATION typedef PTR SYSTEM_MODULE_INFORMATION
