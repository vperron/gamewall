; made with protoize by f0dder
; with some hand-made fixes
; Four-F, four-f@mail.ru
; last update 10-Dec-2002

XLATEOBJ_piVector PROTO STDCALL :DWORD
XLATEOBJ_iXlate PROTO STDCALL :DWORD, :DWORD
XLATEOBJ_hGetColorTransform PROTO STDCALL :DWORD
XLATEOBJ_cGetPalette PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
XFORMOBJ_iGetXform PROTO STDCALL :DWORD, :DWORD
XFORMOBJ_iGetFloatObjXform PROTO STDCALL :DWORD, :DWORD
XFORMOBJ_bApplyXform PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
WNDOBJ_vSetConsumer PROTO STDCALL :DWORD, :DWORD
WNDOBJ_cEnumStart PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
WNDOBJ_bEnum PROTO STDCALL :DWORD, :DWORD, :DWORD
VidMemFree PROTO STDCALL :DWORD, :DWORD
STROBJ_vEnumStart PROTO STDCALL :DWORD
STROBJ_fxCharacterExtra PROTO STDCALL :DWORD
STROBJ_fxBreakExtra PROTO STDCALL :DWORD
STROBJ_dwGetCodePage PROTO STDCALL :DWORD
STROBJ_bGetAdvanceWidths PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
STROBJ_bEnumPositionsOnly PROTO STDCALL :DWORD, :DWORD, :DWORD
STROBJ_bEnum PROTO STDCALL :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeToMultiByteN PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlUpcaseUnicodeChar PROTO STDCALL :DWORD
RtlUnwind PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteSize PROTO STDCALL :DWORD, :DWORD, :DWORD
RtlUnicodeToMultiByteN PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlRaiseException PROTO STDCALL :DWORD
RtlMultiByteToUnicodeN PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
RtlAnsiCharToUnicodeChar PROTO STDCALL :DWORD
PATHOBJ_vGetBounds PROTO STDCALL :DWORD, :DWORD
PATHOBJ_vEnumStartClipLines PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
PATHOBJ_vEnumStart PROTO STDCALL :DWORD
PATHOBJ_bPolyLineTo PROTO STDCALL :DWORD, :DWORD, :DWORD
PATHOBJ_bPolyBezierTo PROTO STDCALL :DWORD, :DWORD, :DWORD
PATHOBJ_bMoveTo PROTO STDCALL :DWORD, :DWORD, :DWORD
PATHOBJ_bEnumClipLines PROTO STDCALL :DWORD, :DWORD, :DWORD
PATHOBJ_bEnum PROTO STDCALL :DWORD, :DWORD
PATHOBJ_bCloseFigure PROTO STDCALL :DWORD
PALOBJ_cGetColors PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
HeapVidMemAllocAligned PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HT_Get8BPPMaskPalette PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
HT_Get8BPPFormatPalette PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
HT_ComputeRGBGammaTable PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FONTOBJ_vGetInfo PROTO STDCALL :DWORD, :DWORD, :DWORD
FONTOBJ_pxoGetXform PROTO STDCALL :DWORD
FONTOBJ_pwszFontFilePaths PROTO STDCALL :DWORD, :DWORD
FONTOBJ_pvTrueTypeFontFile PROTO STDCALL :DWORD, :DWORD
FONTOBJ_pjOpenTypeTablePointer PROTO STDCALL :DWORD, :DWORD, :DWORD
FONTOBJ_pifi PROTO STDCALL :DWORD
FONTOBJ_pfdg PROTO STDCALL :DWORD
FONTOBJ_pQueryGlyphAttrs PROTO STDCALL :DWORD, :DWORD
FONTOBJ_cGetGlyphs PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
FONTOBJ_cGetAllGlyphHandles PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_SubLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_SubFloat PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Sub PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_SubFloatObj PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_SetLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_SetFloat PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Neg PROTO STDCALL :DWORD
FLOATOBJ_MulLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_MulFloat PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Mul PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_MulFloatObj PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_LessThanLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_LessThan PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_GreaterThanLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_GreaterThan PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_GetLong PROTO STDCALL :DWORD
FLOATOBJ_GetFloat PROTO STDCALL :DWORD
FLOATOBJ_EqualLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Equal PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_DivLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_DivFloat PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Div PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_DivFloatObj PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_AddLong PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_AddFloat PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_Add PROTO STDCALL :DWORD, :DWORD
FLOATOBJ_AddFloatObj PROTO STDCALL :DWORD, :DWORD
EngWritePrinter PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngWideCharToMultiByte PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngWaitForSingleObject PROTO STDCALL :DWORD, :DWORD
EngUnsecureMem PROTO STDCALL :DWORD
EngUnmapFontFileFD PROTO STDCALL :DWORD
EngUnmapFontFile PROTO STDCALL :DWORD
EngUnmapFile PROTO STDCALL :DWORD
EngUnmapEvent PROTO STDCALL :DWORD
EngUnlockSurface PROTO STDCALL :DWORD
EngUnlockDriverObj PROTO STDCALL :DWORD
EngUnlockDirectDrawSurface PROTO STDCALL :DWORD
EngUnloadImage PROTO STDCALL :DWORD
EngUnicodeToMultiByteN PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngTransparentBlt PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngTextOut PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStrokePath PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStrokeAndFillPath PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStretchBltROP PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngStretchBlt PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSort PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngSetPrinterData PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSetPointerTag PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSetPointerShape PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngSetLastError PROTO STDCALL :DWORD
EngSetEvent PROTO STDCALL :DWORD
EngSecureMem PROTO STDCALL :DWORD, :DWORD
EngSaveFloatingPointState PROTO STDCALL :DWORD, :DWORD
EngRestoreFloatingPointState PROTO STDCALL :DWORD
EngReleaseSemaphore PROTO STDCALL :DWORD
EngQuerySystemAttribute PROTO STDCALL :DWORD, :DWORD
EngQueryPerformanceFrequency PROTO STDCALL :DWORD
EngQueryPerformanceCounter PROTO STDCALL :DWORD
EngQueryPalette PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngQueryLocalTime PROTO STDCALL :DWORD
EngQueryDeviceAttribute PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngProbeForReadAndWrite PROTO STDCALL :DWORD, :DWORD, :DWORD
EngProbeForRead PROTO STDCALL :DWORD, :DWORD, :DWORD
EngPlgBlt PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngPaint PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMultiByteToWideChar PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMultiByteToUnicodeN PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMulDiv PROTO STDCALL :DWORD, :DWORD, :DWORD
EngMovePointer PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngModifySurface PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngMarkBandingSurface PROTO STDCALL :DWORD
EngMapModule PROTO STDCALL :DWORD, :DWORD
EngMapFontFileFD PROTO STDCALL :DWORD, :DWORD, :DWORD
EngMapFontFile PROTO STDCALL :DWORD, :DWORD, :DWORD
EngMapFile PROTO STDCALL :DWORD, :DWORD, :DWORD
EngMapEvent PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngLpkInstalled PROTO STDCALL
EngLockSurface PROTO STDCALL :DWORD
EngLockDriverObj PROTO STDCALL :DWORD
EngLockDirectDrawSurface PROTO STDCALL :DWORD
EngLoadModuleForWrite PROTO STDCALL :DWORD, :DWORD
EngLoadModule PROTO STDCALL :DWORD
EngLoadImage PROTO STDCALL :DWORD
EngLineTo PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngInitializeSafeSemaphore PROTO STDCALL :DWORD
EngGradientFill PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetType1FontList PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetTickCount PROTO STDCALL
EngGetProcessHandle PROTO STDCALL
EngGetPrinterDriver PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetPrinterDataFileName PROTO STDCALL :DWORD
EngGetPrinterData PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetPrinter PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetLastError PROTO STDCALL
EngGetForm PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngGetFilePath PROTO STDCALL :DWORD, :DWORD
EngGetFileChangeTime PROTO STDCALL :DWORD, :DWORD
EngGetDriverName PROTO STDCALL :DWORD
EngGetCurrentThreadId PROTO STDCALL
EngGetCurrentProcessId PROTO STDCALL
EngGetCurrentCodePage PROTO STDCALL :DWORD, :DWORD
EngFreeUserMem PROTO STDCALL :DWORD
EngFreePrivateUserMem PROTO STDCALL :DWORD, :DWORD
EngFreeModule PROTO STDCALL :DWORD
EngFreeMem PROTO STDCALL :DWORD
EngFntCacheLookUp PROTO STDCALL :DWORD, :DWORD
EngFntCacheFault PROTO STDCALL :DWORD, :DWORD
EngFntCacheAlloc PROTO STDCALL :DWORD, :DWORD
EngFindResource PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngFindImageProcAddress PROTO STDCALL :DWORD, :DWORD
EngFillPath PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngFileWrite PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngFileIoControl PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngEraseSurface PROTO STDCALL :DWORD, :DWORD, :DWORD
EngEnumForms PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngDitherColor PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngDeviceIoControl PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngDeleteWnd PROTO STDCALL :DWORD
EngDeleteSurface PROTO STDCALL :DWORD
EngDeleteSemaphore PROTO STDCALL :DWORD
EngDeleteSafeSemaphore PROTO STDCALL :DWORD
EngDeletePath PROTO STDCALL :DWORD
EngDeletePalette PROTO STDCALL :DWORD
EngDeleteFile PROTO STDCALL :DWORD
EngDeleteEvent PROTO STDCALL :DWORD
EngDeleteDriverObj PROTO STDCALL :DWORD, :DWORD, :DWORD
EngDeleteClip PROTO STDCALL :DWORD
EngDebugPrint PROTO STDCALL :DWORD, :DWORD, :DWORD
EngDebugBreak PROTO STDCALL
EngCreateWnd PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCreateSemaphore PROTO STDCALL
EngCreatePath PROTO STDCALL
EngCreatePalette PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCreateEvent PROTO STDCALL :DWORD
EngCreateDriverObj PROTO STDCALL :DWORD, :DWORD, :DWORD
EngCreateDeviceSurface PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngCreateDeviceBitmap PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD
EngCreateClip PROTO STDCALL
EngCreateBitmap PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngCopyBits PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngControlSprites PROTO STDCALL :DWORD, :DWORD
EngComputeGlyphSet PROTO STDCALL :DWORD, :DWORD, :DWORD
EngCheckAbort PROTO STDCALL :DWORD
EngBitBlt PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngAssociateSurface PROTO STDCALL :DWORD, :DWORD, :DWORD
EngAlphaBlend PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
EngAllocUserMem PROTO STDCALL :DWORD, :DWORD
EngAllocPrivateUserMem PROTO STDCALL :DWORD, :DWORD, :DWORD
EngAllocMem PROTO STDCALL :DWORD, :DWORD, :DWORD
EngAcquireSemaphore PROTO STDCALL :DWORD
CLIPOBJ_ppoGetPath PROTO STDCALL :DWORD
CLIPOBJ_cEnumStart PROTO STDCALL :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
CLIPOBJ_bEnum PROTO STDCALL :DWORD, :DWORD, :DWORD
BRUSHOBJ_ulGetBrushColor PROTO STDCALL :DWORD
BRUSHOBJ_pvGetRbrush PROTO STDCALL :DWORD
BRUSHOBJ_pvAllocRbrush PROTO STDCALL :DWORD, :DWORD
BRUSHOBJ_hGetColorTransform PROTO STDCALL :DWORD

;_local_unwind2 PROTO C :DWORD, :DWORD
_itow PROTO C :DWORD, :DWORD, :DWORD
_itoa PROTO C :DWORD, :DWORD, :DWORD
;_global_unwind2 PROTO C :DWORD
;_except_handler2 PROTO C
_abnormal_termination PROTO C
comment ^
DDVPTYPE_BROOKTREE PROTO C :VARARG
DDVPTYPE_CCIR656 PROTO C :VARARG
DDVPTYPE_E_HREFH_VREFH PROTO C :VARARG
DDVPTYPE_E_HREFH_VREFL PROTO C :VARARG
DDVPTYPE_E_HREFL_VREFH PROTO C :VARARG
DDVPTYPE_E_HREFL_VREFL PROTO C :VARARG
DDVPTYPE_PHILIPS PROTO C :VARARG
GUID_AGP_INTERFACE PROTO C :VARARG
GUID_ColorControlCallbacks PROTO C :VARARG
GUID_D3DCallbacks PROTO C :VARARG
GUID_D3DCallbacks2 PROTO C :VARARG
GUID_D3DCallbacks3 PROTO C :VARARG
GUID_D3DCaps PROTO C :VARARG
GUID_D3DExtendedCaps PROTO C :VARARG
GUID_D3DParseUnknownCommandCallback PROTO C :VARARG
GUID_DDMoreCaps PROTO C :VARARG
GUID_DDMoreSurfaceCaps PROTO C :VARARG
GUID_DDStereoMode PROTO C :VARARG
GUID_DxApi PROTO C :VARARG
GUID_GetHeapAlignment PROTO C :VARARG
GUID_KernelCallbacks PROTO C :VARARG
GUID_KernelCaps PROTO C :VARARG
GUID_Miscellaneous2Callbacks PROTO C :VARARG
GUID_MiscellaneousCallbacks PROTO C :VARARG
GUID_MotionCompCallbacks PROTO C :VARARG
GUID_NTCallbacks PROTO C :VARARG
GUID_NTPrivateDriverCaps PROTO C :VARARG
GUID_NonLocalVidMemCaps PROTO C :VARARG
GUID_UpdateNonLocalHeap PROTO C :VARARG
GUID_VideoPortCallbacks PROTO C :VARARG
GUID_VideoPortCaps PROTO C :VARARG
GUID_ZPixelFormats PROTO C :VARARG
IID_IDDVideoPortContainer PROTO C :VARARG
IID_IDirectDrawVideoPort PROTO C :VARARG
^