.code
db  "Version 3.0 !!!"
DemoCryptBegin:
dd 'ERAS'
DemoReminder proc blabla:DWORD
    nop
    ret
DemoReminder endp


GetHourValue proc hnd:DWORD,ID:DWORD,pVal:DWORD,Usage:DWORD,pOpt:DWORD
LOCAL StVal[20]:BYTE
LOCAL Hour:DWORD
LOCAL Minutes:DWORD
    pushad
    mov    edi, pVal
    assume edi : ptr DAYAUTH
    freed  StVal,sizeof StVal
    invoke GetDlgItemText,hnd,ID,addr StVal,sizeof StVal
    .if eax != 0
       .if Usage == GET_DURATION_VALUES
            invoke ustr2dw,addr StVal
            .if eax > 1440
                jmp __overflow
            .else
                m2m    dword ptr [edi].Duration, eax
            .endif
        .elseif Usage == GET_HOUR_VALUES
            lea    esi, StVal
            mov    ecx, 2
__recheck:             
            mov    al, byte ptr [esi]
            .if (al < 30h) || (al > 39h)
                jmp    __bad
            .endif
            inc    esi
            dec    ecx
            jnz    __recheck
            cmp    byte ptr [esi], ':'
            jnz    __bad
            inc    esi
            mov    ecx, 2
__recheck2:            
            mov    al, byte ptr [esi]
            .if (al < 30h) || (al > 39h)
                jmp    __bad
            .endif
            inc    esi
            dec    ecx
            jnz    __recheck2
            cmp    byte ptr [esi],0
            jnz    __bad
            lea    esi, StVal
            mov    ax, word ptr [esi]
            mov    dword ptr [Hour], 0
            mov    word ptr [Hour],ax
            invoke ustr2dw,addr Hour
            .if eax > 23
                jmp    __overflow
            .endif
            mov    ecx, 60
            mul    ecx
            mov    dword ptr [Hour], eax
            mov    ax, word ptr [esi+3]
            mov    dword ptr [Minutes],0
            mov    word ptr [Minutes],ax
            invoke ustr2dw,addr Minutes
            .if eax > 59
                jmp    __bad
            .endif
            add    dword ptr [Hour], eax
            mov    eax, dword ptr [Hour]
            .if eax > 1439
                jmp    __overflow
            .else
                m2m    dword ptr [edi].BaseHour, eax
                add    eax, dword ptr [edi].Duration
                .if eax > 1439
                    jmp   __overflow
                .endif
            .endif
        .endif
    .endif
    mov    esi, pOpt
    assume esi : ptr DAYHND
    m2m    dword ptr [edi].AuthEd, dword ptr [esi].IsActive
    assume edi : nothing
    assume esi : nothing
    popad
    mov    eax, 1
    ret
__overflow:
    invoke MessageBoxA,hnd,addr TooLargeValue,addr AppName,0
    jmp @F
__bad:
    invoke MessageBoxA,hnd,addr BadValues,addr AppName,NULL
@@:
    freed  IniFile.WeekAuth, sizeof WEEKAUTH
    popad
    mov    eax, 0
    ret
GetHourValue endp

SetValues proc pHnd:DWORD, pValues:DWORD, Usage:DWORD
LOCAL buf[20]:BYTE
    mov    edi, pValues
    mov    esi, pHnd
    assume edi : ptr DAYAUTH
    assume esi : ptr DAYHND
    .if Usage == SET_VALUES_FORCE
        push   [edi].AuthEd
        m2m    [edi].AuthEd, 1
    .endif
    .if ([edi].AuthEd == 1)
        m2m    [esi].IsActive,1
        mov    eax, dword ptr [edi].Duration
        .if eax != 0 
            freed  buf,sizeof buf
            invoke wsprintf,addr buf,addr nfilter,eax
            invoke SetWindowText,[esi].DurationHnd,addr buf
        .endif
        mov eax, dword ptr [edi].BaseHour
        .if eax != 0
            freed  buf,sizeof buf
            xor    edx, edx
            mov    ecx, 60
            div    ecx
            invoke wsprintf,addr buf,addr nhfilter,eax,edx
            invoke SetWindowText,[esi].HourHnd,addr buf
        .endif
    .else
        m2m    [esi].IsActive,0
    .endif
    .if Usage == SET_VALUES_FORCE
        pop    [edi].AuthEd
        m2m    [esi].IsActive, [edi].AuthEd
    .endif
    invoke SendMessage,[esi].ButtonHnd,BM_SETCHECK,[esi].IsActive,0
    invoke EnableWindow,[esi].DurationHnd,[esi].IsActive
    invoke EnableWindow,[esi].HourHnd,[esi].IsActive
    assume esi : nothing
    assume edi : nothing
    ret
SetValues endp
;:::::::::: HndLoop function, a little bit complex :::::::::;
;
; pHnd designe de façon générale l'endroit où seront placées et lues les valeurs en memoire.
;
; baseID est l'ID de base des controles où effectuer les opérations.
;        
; pOptional : pointeur vers la structure WeekHnd appropriée.
;             
;
HndLoop proc pHnd:DWORD, baseID:DWORD, hnd:DWORD,Usage:DWORD,pOptional:DWORD
LOCAL use:DWORD
        pushad
        mov    edi, pHnd
        mov    ebx, baseID
        mov    esi, pOptional
        mov    ecx, 7
@@:
        push   ecx
        .if Usage == GET_HANDLES
            invoke GetDlgItem,hnd,ebx
            mov    dword ptr [edi], eax
            add    edi, sizeof DAYHND
        .elseif (Usage == GET_DURATION_VALUES) || (Usage == GET_HOUR_VALUES)
            invoke GetHourValue,hnd,ebx,edi,Usage,esi
            add    esi, sizeof DAYHND
            add    edi, sizeof DAYAUTH
            .if eax == 0
                pop    ecx
                jmp    __endlooperr
            .endif
        .elseif (Usage ==  DISABLE_ITEMS) || (Usage == ENABLE_ITEMS) || (Usage == DISABLE_EDITSONLY)
            assume edi : ptr DAYHND
            .if Usage == DISABLE_EDITSONLY
                m2m    use, 0
            .else
                m2m    use, Usage
                invoke EnableWindow,[edi].ButtonHnd,use
            .endif
            invoke EnableWindow,[edi].DurationHnd,use
            invoke EnableWindow,[edi].HourHnd,use
            assume edi : nothing
            add    edi, sizeof DAYHND
        .elseif (Usage == SET_VALUES) || (Usage == SET_VALUES_FORCE)
            invoke SetValues, esi, edi, Usage
            add    edi, sizeof DAYAUTH
            add    esi, sizeof DAYHND
        .elseif (Usage == SET_VALUES_NULL)
            assume edi : ptr DAYAUTH
            assume esi : ptr DAYHND
            mov    [edi].Duration, 0
            mov    [edi].BaseHour, 0
            invoke SetWindowText, [esi].DurationHnd, NULL
            invoke SetWindowText, [esi].HourHnd, NULL
            assume edi : nothing
            assume esi : nothing
            add    edi, sizeof DAYAUTH
            add    esi, sizeof DAYHND            
        .endif
        inc    ebx
        pop    ecx
        dec    ecx
        jnz    @B
        popad
        mov    eax, 1
        ret
__endlooperr:
        popad 
        xor    eax, eax
        ret
HndLoop endp

SwitchSingle proc pHnd:DWORD, pAuth:DWORD
    pushad
    mov    edi, pHnd
    mov    esi, pAuth
    assume edi : ptr DAYHND
    assume esi : ptr DAYAUTH
    xor    dword ptr [edi].IsActive, 1
    xor    dword ptr [esi].AuthEd, 1
    invoke EnableWindow,[edi].DurationHnd,[edi].IsActive
    invoke EnableWindow,[edi].HourHnd,[edi].IsActive
    assume edi : nothing
    assume esi : nothing
    popad
    ret
SwitchSingle endp

TimeDateDlgProc proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD  
    .if uMsg == WM_INITDIALOG
        
        m2m    hParent, lParam
        invoke GetDlgItem,hDlg,1026
        mov    WeekHnd.Week.ButtonHnd, eax
        invoke GetDlgItem,hDlg,1027
        mov    WeekHnd.Week.DurationHnd, eax
        invoke GetDlgItem,hDlg,1028
        mov    WeekHnd.Week.HourHnd, eax
        invoke HndLoop,addr WeekHnd.Monday.ButtonHnd,1001,hDlg,GET_HANDLES,0
        invoke HndLoop,addr WeekHnd.Monday.DurationHnd,1008,hDlg,GET_HANDLES,0
        invoke HndLoop,addr WeekHnd.Monday.HourHnd,1015,hDlg,GET_HANDLES,0
        
        invoke HndLoop,addr WeekHnd.Monday.ButtonHnd,0,hDlg,DISABLE_EDITSONLY,0
        
        invoke HndLoop,addr IniFile.WeekAuth.Monday,0,hDlg,SET_VALUES,addr WeekHnd.Monday
        
        invoke SetValues,addr WeekHnd.Week,addr IniFile.WeekAuth.Week,NULL
        .if WeekHnd.Week.IsActive == 1
            invoke HndLoop,addr WeekHnd.Monday,0,hDlg,DISABLE_ITEMS,0
        .endif
        mov    eax, 1
        ret
    .elseif uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 1026
            invoke HndLoop,addr WeekHnd.Monday.ButtonHnd,0,hDlg,WeekHnd.Week.IsActive,0
            .if WeekHnd.Week.IsActive == 1
                invoke HndLoop,addr IniFile.WeekAuth.Monday,0,hDlg,SET_VALUES,addr WeekHnd.Monday
            .endif
            invoke SwitchSingle,addr WeekHnd.Week,addr IniFile.WeekAuth.Week
        .elseif ax == 1029
            freed  IniFile.WeekAuth,sizeof WEEKAUTH
            invoke GetHourValue,hDlg,1027,addr IniFile.WeekAuth.Week,GET_DURATION_VALUES,addr WeekHnd.Week
            invoke GetHourValue,hDlg,1028,addr IniFile.WeekAuth.Week,GET_HOUR_VALUES,addr WeekHnd.Week
            invoke HndLoop,addr IniFile.WeekAuth.Monday,1008,hDlg,GET_DURATION_VALUES,addr WeekHnd.Monday
            invoke HndLoop,addr IniFile.WeekAuth.Monday,1015,hDlg,GET_HOUR_VALUES,addr WeekHnd.Monday
            mov    eax, sizeof MD5RESULT
            add    eax, CURRENT_INIDEFAULT_BETWEEN_VALUES
            invoke SetFilePointer,hInit,eax,0,FILE_BEGIN
     		invoke WriteFile,hInit,addr IniFile.WeekAuth,sizeof WEEKAUTH,addr read, NULL
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
     		invoke SaveCheckSum,CHECKSUM_RSET
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
        .elseif ax == 1001
            invoke SwitchSingle,addr WeekHnd.Monday,addr IniFile.WeekAuth.Monday
        .elseif ax == 1002
            invoke SwitchSingle,addr WeekHnd.Tuesday,addr IniFile.WeekAuth.Tuesday
        .elseif ax == 1003
            invoke SwitchSingle,addr WeekHnd.Wednesday,addr IniFile.WeekAuth.Wednesday
        .elseif ax == 1004
            invoke SwitchSingle,addr WeekHnd.Thursday,addr IniFile.WeekAuth.Thursday
        .elseif ax == 1005
            invoke SwitchSingle,addr WeekHnd.Friday,addr IniFile.WeekAuth.Friday
        .elseif ax == 1006
            invoke SwitchSingle,addr WeekHnd.Saturday,addr IniFile.WeekAuth.Saturday
        .elseif ax == 1007
            invoke SwitchSingle,addr WeekHnd.Sunday,addr IniFile.WeekAuth.Sunday
        .elseif ax == 1030
            invoke EndDialog,hParent,NULL
        .elseif ax == 1025
            invoke GetHourValue,hDlg,1023,addr DurationSet,GET_DURATION_VALUES,addr hDurationSet
            invoke GetHourValue,hDlg,1024,addr DurationSet,GET_HOUR_VALUES,addr hDurationSet
            lea    edi, IniFile.WeekAuth.Monday
            mov    ecx, 7
@@:
            pushad
            mcopy  offset DurationSet,edi,8
            popad
            add    edi, sizeof DAYAUTH
            dec    ecx
            jnz    @B
            invoke HndLoop,addr IniFile.WeekAuth.Monday,0,hDlg,SET_VALUES_FORCE,addr WeekHnd.Monday
            
        .elseif ax == 1022
            invoke HndLoop,addr IniFile.WeekAuth.Monday,1008,hDlg,SET_VALUES_NULL,addr WeekHnd.Monday
            lea    edi, IniFile.WeekAuth.Week
            lea    esi, WeekHnd.Week
            assume edi : ptr DAYAUTH
            mov    [edi].Duration, 0
            mov    [edi].BaseHour, 0
            invoke SetValues, esi, edi, SET_VALUES_NULL
            assume edi : nothing
            
        .endif
    .endif  
    xor eax, eax
    ret
TimeDateDlgProc endp

DlgNewPass proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    pushad
    .if uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 211
            invoke RtlZeroMemory,addr PassPhrase,sizeof PassPhrase
            invoke GetWindowText,hEditPass,addr PassPhrase,50
            .if eax > 6
                push   eax
                invoke RtlZeroMemory,addr IniFile.Md5Password,sizeof MD5RESULT
                pop    eax
     		    invoke procMD5hash,addr PassPhrase,eax,addr IniFile.Md5Password
     		    invoke SetFilePointer,hInit,0,0,FILE_BEGIN
     		    invoke WriteFile,hInit,addr IniFile.Md5Password,sizeof MD5RESULT,addr read, NULL
     		    .if eax == 0
                    invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
                .endif
     		    invoke SaveCheckSum,CHECKSUM_RSET
     		    .if eax == 1
                    invoke MessageBoxA,hDlg,addr szPassChanged,addr AppName,NULL
                .else
                    invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
                .endif
                invoke EndDialog,hDlg,NULL
            .else
     	        invoke MessageBoxA,hDlg,addr szMoreChars,addr AppName,NULL   
     	    .endif
        .endif
    .elseif uMsg == WM_INITDIALOG
        invoke SetWindowText,hDlg,addr NewPassTitle
        invoke GetDlgItem,hDlg,210
        mov    hEditPass, eax
        invoke RtlZeroMemory,addr PassPhrase,sizeof PassPhrase
        popad 
        mov    eax, 1
        ret
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hDlg,NULL
    .endif
    popad
    xor eax, eax
    ret    
DlgNewPass endp


dd 'ERAS'
dd 10 dup(0)
DemoCryptEnd:
dd 10 dup(0)


include ACF.asm

SaveCheckSum proc Usage:DWORD
LOCAL HeaderSum:DWORD
LOCAL CheckSum:DWORD
LOCAL hKey:DWORD
LOCAL sData:DWORD
LOCAL dType:DWORD

    invoke IsUserAdmin
    .if eax == 0
        jmp endSC
    .endif
    .if Usage == CHECKSUM_RSET
        invoke MapFileAndCheckSum,addr pIniDir,addr HeaderSum,addr CheckSum
        .if eax != CHECKSUM_SUCCESS
            mov    eax, 0
            ret
        .endif
        invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,addr GKey,NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,addr hKey,addr read
        invoke RegSetValueEx,hKey,addr ICSum,NULL,REG_DWORD,addr CheckSum,sizeof CheckSum
        invoke RegCloseKey,hKey
        mov    eax, 1
        ret
    .elseif Usage == CHECKSUM_RGET
        mov    CheckSum, 0
        mov    sData, 4h
        invoke RegCreateKeyEx,HKEY_LOCAL_MACHINE,addr GKey,NULL,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,addr hKey,addr read
        invoke RegQueryValueEx,hKey,addr ICSum,NULL,addr dType,addr CheckSum,addr sData
        push   eax
        invoke RegCloseKey,hKey
        pop    eax
        .if eax != ERROR_SUCCESS
            mov    eax, 0
            ret
        .endif
        mov    eax, CheckSum
        ret
    .elseif Usage == CHECKSUM_FGET
        invoke MapFileAndCheckSum,addr pIniDir,addr HeaderSum,addr CheckSum
        .if eax != CHECKSUM_SUCCESS
            mov    eax, 0
            ret
        .endif
        mov eax, CheckSum
        ret
    .endif
    
    ret
    
endSC:
    mov eax, 1
    ret
SaveCheckSum endp

DoUnHookAndClose proc
    invoke GetProcAddress,hGDll,addr UIHStr
    call   eax
    .if NTOs == 0
        invoke UnHideMe9x
    .endif
    invoke ReleaseMutex, hMutex
    invoke CloseHandle,hMutex
    invoke RegCloseKey,Khnd
    invoke FreeLibrary,addr DllPath
    invoke CloseHandle,hInit
    invoke ExitProcess,0
DoUnHookAndClose endp

SetDefaultsInControls proc
LOCAL val[10]:BYTE
    .if NTOs == 0
        m2m    IniFile.IsUserPrevented, 0
        invoke EnableWindow,hUserPrevented,FALSE
    .endif
    invoke SendMessage,hTextVisible,BM_SETCHECK,IniFile.IsTextVisible,0
    invoke SendMessage,hCplPrevented,BM_SETCHECK,IniFile.IsCplPrevented,0
    invoke SendMessage,hNagOff,BM_SETCHECK,IniFile.IsNagOff,0
    invoke SendMessage,hUserPrevented,BM_SETCHECK,IniFile.IsUserPrevented,0
    invoke SendMessage,hWinGames,BM_SETCHECK,IniFile.AreWinGamesBlocked,0
    invoke SendMessage,hARestart,BM_SETCHECK,IniFile.IsAutoRestarted,0
    invoke SendMessage,hUInfo,BM_SETCHECK,IniFile.IsUserInformed,0
    invoke SendMessage,hNetMode,BM_SETCHECK,IniFile.IsNetwork,0
        invoke EnableWindow,hIsServer,IniFile.IsNetwork
        invoke EnableWindow,hIsClient,IniFile.IsNetwork
        invoke SendMessage,hIsServer,BM_SETCHECK,IniFile.IsServer,0
        invoke SendMessage,hIsClient,BM_SETCHECK,IniFile.IsClient,0
        invoke EnableWindow,hServName,IniFile.IsNetwork
        invoke SendMessage,hServName,WM_SETTEXT,0,addr IniFile.ServCompName
        
    mov    eax, 1
    ret
SetDefaultsInControls endp

CreateNag proc
    xor    esi,esi
    mov    wc.cbSize,sizeof WNDCLASSEX
    mov    wc.style,CS_HREDRAW or CS_VREDRAW or CS_DBLCLKS
    mov    wc.lpfnWndProc,offset NagProc
    mov    wc.cbClsExtra,esi
    mov    wc.cbWndExtra,esi
    m2m    wc.hInstance,hInst
    mov    wc.hbrBackground,0
    mov    wc.lpszMenuName,esi
    mov    wc.lpszClassName,offset ClassName
    invoke LoadIcon,hInst,200
    mov    hIcon,eax
    m2m    wc.hIcon,hIcon
    m2m    wc.hIconSm,hIcon
    invoke LoadCursor,esi,IDC_ARROW
    mov    wc.hCursor,eax
    invoke RegisterClassEx,addr wc
  
    invoke GetSystemMetrics,SM_CXSCREEN
    sub    eax,WxDim
    shr    eax,1
    m2m    ebx,eax
 
    invoke GetSystemMetrics,SM_CYSCREEN
    sub    eax,WyDim
    shr    eax,1
 
    invoke CreateWindowEx,esi,addr ClassName,addr AppName,WS_POPUP or WS_SYSMENU,ebx,eax,WxDim,WyDim,esi,esi,hInst,esi
    mov    hWnd,eax
 
    invoke UpdateWindow,hWnd
    .while 1
       xor    eax,eax
       invoke GetMessage,addr msg,eax,eax,eax
       .break .if (!eax)
       invoke TranslateMessage,addr msg
       invoke DispatchMessage,addr msg
    .endw
    mov    eax,msg.wParam
    invoke ExitThread,0
    ret
CreateNag endp

TabProc proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    pushad
    .if uMsg == WM_NOTIFY
        mov    eax, lParam
	    mov    eax, (NMHDR PTR [eax]).code
	    .if eax == TCN_SELCHANGE
            mov    ebx,CurrentTab
            push   ebx
	        mov    eax,[SelectDlgHwnd+ebx*4]
	        invoke ShowWindow,eax,SW_HIDE
	        invoke SendMessage,hwndTab,TCM_GETCURSEL,0,0
	        mov    CurrentTab,eax
	        pop    ebx
	        cmp    eax, 1
	        jnz    @F
	        cmp    DemoFilePresent, 1
            jz    @F
            invoke MessageBoxA,hDlg,addr SorryDemoVersion,addr AppName,0
            invoke SendMessage,hwndTab,TCM_SETCURSEL,ebx,0
            mov    CurrentTab, ebx
            mov    eax, ebx
@@:
	        mov    ebx, dword ptr [SelectDlgHwnd+eax*4]
 	        invoke ShowWindow,EBX,SW_SHOWDEFAULT
        .endif

    .elseif uMsg == WM_INITDIALOG
        m2m    hDlgParent,hDlg
        
        m2m    iccex.dwSize, sizeof iccex
        m2m    iccex.dwICC, ICC_TAB_CLASSES
        invoke InitCommonControlsEx,addr iccex
	    
	    invoke GetDlgItem,hDlg,1000
	    m2m    hwndTab,eax
	    
	    m2m    ItemStruct.imask,TCIF_TEXT
	    m2m    ItemStruct.lpReserved1,0	
	    m2m    ItemStruct.lpReserved2,0 	
	    m2m    ItemStruct.pszText,offset TabTitle1 	
	    m2m    ItemStruct.cchTextMax,sizeof TabTitle1	
  	    m2m    ItemStruct.iImage,0
 	    m2m    ItemStruct.lParam,0
	    invoke SendMessage,hwndTab,TCM_INSERTITEM,0,addr ItemStruct

	    m2m    ItemStruct.pszText,offset TabTitle2
	    m2m    ItemStruct.cchTextMax,sizeof TabTitle2
	    invoke SendMessage,hwndTab,TCM_INSERTITEM,1,addr ItemStruct
	    
	    m2m    ItemStruct.pszText,offset TabTitle3
	    m2m    ItemStruct.cchTextMax,sizeof TabTitle3
	    invoke SendMessage,hwndTab,TCM_INSERTITEM,2,addr ItemStruct
	    
	    m2m    ItemStruct.pszText,offset TabTitle4
	    m2m    ItemStruct.cchTextMax,sizeof TabTitle4
	    invoke SendMessage,hwndTab,TCM_INSERTITEM,3,addr ItemStruct
	    
	    m2m    eax,offset SelectDlgProc
	    invoke CreateDialogParam,hInst,100,hwndTab,eax,hDlg
	    mov    SelectDlgHwnd,eax
        
        cmp    DemoFilePresent, 1
        jnz    @F
        m2m    eax,offset TimeDateDlgProc
        invoke CreateDialogParam,hInst,103,hwndTab,eax,hDlg
	    mov    TimeDateDlgHwnd,eax
@@:	            
        m2m    eax,offset OptionsDlgProc
	    invoke CreateDialogParam,hInst,104,hwndTab,eax,hDlg
	    mov    OptionsDlgHwnd,eax
	    
	    m2m    eax,offset SecDlgProc
	    invoke CreateDialogParam,hInst,106,hwndTab,eax,hDlg
	    mov    SecDlgHwnd,eax
	    
	    m2m    CurrentTab,0 				
        
	    invoke ShowWindow,SelectDlgHwnd,SW_SHOW
        popad
        mov    eax, 1
        ret
        
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hDlg,NULL
    .endif
    popad
    xor eax, eax
    ret
TabProc endp

SecDlgProc proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    .if uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 1002
            invoke RtlZeroMemory,addr SecBuf,sizeof SecBuf
            invoke GetWindowText,hEditPass,addr SecBuf,480
            
            lea edi, SecBuf
Bo:
            cmp dword ptr [edi],0
            jz  B1
            cmp byte ptr [edi], ","
            jnz @F
            mov byte ptr [edi], 0
@@:
            inc edi
            jmp Bo
B1:
            sub edi, offset SecBuf
            invoke AccessConfigFiles,9,addr SecBuf,edi
        .elseif ax == 1003
            invoke EndDialog,hParent,0
        .endif
    .elseif uMsg == WM_INITDIALOG
        invoke GetDlgItem,hDlg,1000
        mov hEditPass, eax
        invoke AccessConfigFiles,8,addr SecBuf,500
        
        lea edi, SecBuf
        inc edi
Bo1:
            cmp dword ptr [edi],0
            jz  B11
            cmp byte ptr [edi], 0
            jnz @F
            mov byte ptr [edi], ","
@@:
            inc edi
            jmp Bo1
B11:
        
        invoke SetWindowText,hEditPass,addr SecBuf
        mov eax, 1
        ret
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hDlg,NULL
    .endif
    xor eax, eax
    ret
SecDlgProc endp

SelectDlgProc	proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    pushad
    .if uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 1003
            invoke EndDialog,hParent,0
        .elseif ax == 2000
            invoke SendMessage,hList1,LB_GETCURSEL,0,0
            mov    IndexItem, eax
            invoke SendMessage,hList1,LB_GETTEXT,IndexItem,ADDR Buffer
            mov    sBuffer,eax
            invoke SendMessage,hList2,LB_ADDSTRING,NULL,addr Buffer
            lea    edi, PrivBuf
@@:
            invoke lstrlen,edi
            cmp    eax, 0
            jz     @F
            inc    eax
            add    edi, eax
            cmp    dword ptr [edi], 0
            jnz    @B
@@:
            invoke lstrcpy,edi,addr Buffer
            add    edi, sBuffer
            sub    edi, offset PrivBuf
            mov    sPriv, edi
            
        .elseif ax==1002
            
            invoke AccessConfigFiles,4,addr PrivBuf,sizeof PrivBuf
            
        .elseif ax == 2001
            invoke SendMessage,hList2,LB_GETCURSEL,0,0
            mov    IndexItem, eax
            invoke SendMessage,hList2,LB_DELETESTRING,IndexItem,0
            invoke RtlZeroMemory,addr PrivBuf,sizeof PrivBuf
            invoke SendMessage,hList2,LB_GETCOUNT,0,0
            mov    nCount,eax
            xor    edi, edi
            lea    esi,PrivBuf
            cmp    eax, 0
            jz     nomore
@@:
            invoke SendMessage,hList2,LB_GETTEXT,edi,ADDR Buffer
            push   eax
            invoke lstrcpy,esi,addr Buffer
            pop    eax
            add    esi,eax
            inc    esi
            inc    edi
            cmp    edi, nCount
            jnz    @B
nomore:
            sub    esi, offset PrivBuf
            mov    sPriv,esi
        .endif
        
    .elseif uMsg == WM_INITDIALOG
        m2m    hParent, lParam
        invoke GetDlgItem,hDlg,500
        mov    hList1, eax
        invoke GetDlgItem,hDlg,501
        mov    hList2, eax
        invoke RtlZeroMemory,addr PrivBuf,sizeof PrivBuf
        invoke RtlZeroMemory,addr LaunchBuf,sizeof LaunchBuf
;-------------------------------------------------------------------------- Get our privilegied names file

        invoke AccessConfigFiles,2,addr PrivBuf,sizeof PrivBuf
        invoke AccessConfigFiles,1,addr LaunchBuf,sizeof LaunchBuf
        
        lea    edi, LaunchBuf
@@:
        invoke lstrcpy,addr Buffer,edi
        invoke lstrlen,addr Buffer
        mov    sBuffer,eax
        cmp    eax, 0
        jz     @F
        invoke SendMessage,hList1,LB_ADDSTRING,0,addr Buffer
        add    edi, sBuffer
        inc    edi
        cmp    dword ptr [edi],0
        jnz    @B
@@:
        lea    edi, PrivBuf
@@:
        invoke lstrcpy,addr Buffer,edi
        invoke lstrlen,addr Buffer
        mov    sBuffer,eax
        cmp    eax, 0
        jz     @F
        invoke SendMessage,hList2,LB_ADDSTRING,0,addr Buffer
        add    edi, sBuffer
        inc    edi
        cmp    dword ptr [edi],0
        jnz    @B
@@:
        sub    edi, offset PrivBuf
        mov    sPriv, edi
        popad 
        mov    eax, 1
        ret
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hParent,0
    .endif
    popad
    xor eax, eax
    ret
SelectDlgProc endp

OptionsDlgProc proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    .if uMsg == WM_INITDIALOG
        m2m    hParent, lParam
        invoke GetDlgItem,hDlg,1012
        mov    hTextVisible, eax
        invoke GetDlgItem,hDlg,1006
        mov    hUserPrevented, eax
        invoke GetDlgItem,hDlg,1007
        mov    hCplPrevented, eax
        invoke GetDlgItem,hDlg,1008
        mov    hNagOff, eax
        invoke GetDlgItem,hDlg,1013
        mov    hARestart, eax
        invoke GetDlgItem,hDlg,1014
        mov    hUInfo, eax
        invoke GetDlgItem,hDlg,1015
        mov    hWinGames, eax
        invoke GetDlgItem,hDlg,1248
        mov    hLicKey, eax
        invoke GetDlgItem,hDlg,1249
        mov    hVisitAuth,eax
        invoke GetDlgItem,hDlg,1016
        mov    hMailAuth,eax
        invoke GetDlgItem,hDlg,1017
        mov    hNetMode,eax
        invoke GetDlgItem,hDlg,1018
        mov    hIsServer,eax
        invoke GetDlgItem,hDlg,1019
        mov    hIsClient,eax
        invoke GetDlgItem,hDlg,1020
        mov    hServName,eax
        
        call   SetDefaultsInControls
        mov    eax, 1
        ret
            
    .elseif uMsg == WM_COMMAND
        mov eax, wParam
        .if ax==1005
            cmp    DemoFilePresent, 1
            jnz    @F
            invoke DialogBoxParam,hInst,105,NULL,addr DlgNewPass,NULL
            jmp __dem1
@@:
	        invoke MessageBoxA,hDlg,addr SorryDemoVersion,addr AppName,0
__dem1:
        .elseif ax == 1004
            invoke EndDialog,hParent,NULL
            invoke DoUnHookAndClose
        .elseif ax == 1248
            cmp    dword ptr [DemoCryptBegin], 'ERAS'
            jnz @F
            invoke wsprintf,addr DemoBuf,addr szDemoLic,IniFile.DemoPass
            invoke MessageBoxA,hDlg,addr DemoBuf,addr AppName,0
            jmp    __235
@@:
            invoke DialogBoxParam,hInst,105,NULL,addr DlgLicense,NULL
__235:
        .elseif ax == 1010
            freed ServBuf,sizeof ServBuf
            .if (IniFile.IsNetwork == 1) && (IniFile.IsClient == 1)
                freed  IniFile.ServCompName,40
                invoke GetWindowText,hServName,addr IniFile.ServCompName, 38
                invoke lstrcat,addr ServBuf,addr IniFile.ServCompName
                invoke lstrcat,addr ServBuf,addr ClientPipeEnd
                invoke CreateFile,addr ServBuf,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0
                .if eax == -1
                    invoke GetLastError
                    .if eax != ERROR_PIPE_BUSY
                        invoke MessageBoxA,hDlg,addr szServNotReady,addr AppName,0
                        mov    dword ptr [IniFile.IsNetwork], 0
                        freed  IniFile.ServCompName,40
                        call   SetDefaultsInControls
                        jmp    @F
                        
                    .endif
                .endif
                invoke CloseHandle,eax
            .endif
@@:
            invoke SetFilePointer,hInit,sizeof MD5RESULT,0,FILE_BEGIN
     		invoke WriteFile,hInit,addr IniFile.IsTextVisible,CURRENT_INIDEFAULT_BETWEEN_VALUES,addr read, NULL
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
     		invoke SaveCheckSum,CHECKSUM_RSET
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
@@:
        .elseif ax == 1012
            xor    dword ptr [IniFile.IsTextVisible], 1
            
        .elseif ax == 1006
            xor    dword ptr [IniFile.IsUserPrevented], 1
            
        .elseif ax == 1007
            xor    dword ptr [IniFile.IsCplPrevented], 1
            
        .elseif ax == 1008
            xor    dword ptr [IniFile.IsNagOff], 1
            
        .elseif ax == 1013
            xor    dword ptr [IniFile.IsAutoRestarted], 1
            .if    IniFile.IsAutoRestarted==0
                invoke RegDeleteValue,Khnd,addr ServName
            .else
                invoke RegSetValueEx,Khnd,addr ServName,NULL,REG_SZ,addr value,sizeof value
            .endif
            
        .elseif ax == 1014
            xor    dword ptr [IniFile.IsUserInformed], 1
            
        .elseif ax == 1015
            xor    dword ptr [IniFile.AreWinGamesBlocked], 1
            
        .elseif ax == 1017
            .if IniFile.IsClient == 1
                freed  ServBuf,sizeof ServBuf
                invoke lstrcat,addr ServBuf,addr IniFile.ServCompName
                invoke lstrcat,addr ServBuf,addr ClientPipeEnd
                invoke CreateFile,addr ServBuf,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0
                .if eax == -1
                    invoke GetLastError
                    .if eax != ERROR_PIPE_BUSY
                        invoke MessageBoxA,hDlg,addr szServNotReady,addr AppName,0
                        mov    dword ptr [IniFile.IsNetwork], 0
                        freed  IniFile.ServCompName,40
                        call   SetDefaultsInControls
                        jmp    @F
                        
                    .endif
                .endif
                invoke CloseHandle,eax
            .elseif IniFile.IsServer == 1    
                 invoke GetStartupInfo,ADDR startInfo 
                 invoke GetAppPath,addr CmdBuf
                 invoke lstrcat,addr CmdBuf,addr szServProg
                 invoke CreateProcess,ADDR CmdBuf,NULL,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,ADDR startInfo,ADDR processInfo 
                 invoke CloseHandle,processInfo.hThread 
                 invoke CloseHandle,processInfo.hProcess
            .endif
            
            xor     dword ptr [IniFile.IsNetwork], 1
            invoke EnableWindow,hIsServer,IniFile.IsNetwork
            invoke EnableWindow,hIsClient,IniFile.IsNetwork
            invoke SendMessage,hIsServer,BM_SETCHECK,IniFile.IsServer,0
            invoke SendMessage,hIsClient,BM_SETCHECK,IniFile.IsClient,0
            invoke EnableWindow,hServName,IniFile.IsNetwork
            invoke SendMessage,hServName,WM_SETTEXT,0,addr IniFile.ServCompName

@@:
        .elseif ax == 1018
            .if IniFile.IsServer == 0
                xor     dword ptr [IniFile.IsServer], 1
                xor     dword ptr [IniFile.IsClient], 1
                invoke SendMessage,hIsClient,BM_SETCHECK,IniFile.IsClient,0
            .endif   
            
        .elseif ax == 1019
            .if IniFile.IsClient == 0
                freed  IniFile.ServCompName,40
                freed  ServBuf,sizeof ServBuf
                invoke GetWindowText,hServName,addr IniFile.ServCompName, 38
                invoke lstrcat,addr ServBuf,addr IniFile.ServCompName
                invoke lstrcat,addr ServBuf,addr ClientPipeEnd
                invoke CreateFile,addr ServBuf,GENERIC_READ or GENERIC_WRITE ,FILE_SHARE_READ or FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,0
                .if eax == -1
                    invoke GetLastError
                    .if eax != ERROR_PIPE_BUSY
                        invoke MessageBoxA,hDlg,addr szServNotReady,addr AppName,0
                        mov    dword ptr [IniFile.IsNetwork], 0
                        freed  IniFile.ServCompName,40
                        call   SetDefaultsInControls
                        jmp    @F
                        
                    .endif
                .endif
                invoke CloseHandle,eax
                xor     dword ptr [IniFile.IsServer], 1
                xor     dword ptr [IniFile.IsClient], 1
                invoke SendMessage,hIsServer,BM_SETCHECK,IniFile.IsServer,0
@@:
            .endif
            
        .elseif ax == 1011
            invoke EndDialog,hParent,NULL
            
        .elseif ax == 1009
            mcopyd IniDefault.IsTextVisible,IniFile.IsTextVisible,CURRENT_INIDEFAULT_BETWEEN_VALUES

            invoke SetFilePointer,hInit,sizeof MD5RESULT,0,FILE_BEGIN
     		invoke WriteFile,hInit,addr IniFile.IsTextVisible,CURRENT_INIDEFAULT_BETWEEN_VALUES,addr read, NULL
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
     		invoke SaveCheckSum,CHECKSUM_RSET
     		.if eax == 0
                invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
            .endif
            call   SetDefaultsInControls
            
        .elseif ax == 1016
            invoke ShellExecute,hDlg,addr lpOperation, addr lpMail, NULL, NULL, SW_SHOWNORMAL 
        
        .elseif ax == 1249

            invoke ShellExecute,hDlg,addr lpOperation, addr lpPage, NULL, NULL, SW_SHOWNORMAL
        .endif
    .endif  
    xor eax, eax
    ret
OptionsDlgProc endp

DlgVerif proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    pushad
    .if uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 211
            invoke RtlZeroMemory,addr PassPhrase,sizeof PassPhrase
            invoke GetWindowText,hEditPass,addr PassPhrase,50
            .if eax > 6
                push   eax
                invoke RtlZeroMemory,addr stMD5Result,sizeof stMD5Result
                pop    eax
     		    invoke procMD5hash,addr PassPhrase,eax,addr stMD5Result
     		    mov    eax, stMD5Result.dtA
     		    cmp    eax, IniFile.Md5Password.dtA
     		    jnz    @F
     		    mov    eax, stMD5Result.dtB
     		    cmp    eax, IniFile.Md5Password.dtB
     		    jnz    @F
     		    mov    eax, stMD5Result.dtC
     		    cmp    eax, IniFile.Md5Password.dtC
     		    jnz    @F
     		    mov    eax, stMD5Result.dtD
     		    cmp    eax, IniFile.Md5Password.dtD
     		    jnz    @F
     		    mov    Pass, 1
@@:
     		.endif
     		invoke EndDialog,hDlg,NULL
        .endif
    .elseif uMsg == WM_INITDIALOG
        mov Pass,0
        invoke GetDlgItem,hDlg,210
        mov hEditPass, eax
        .if IniFile.IsTextVisible == 0
            xor    eax, eax
            mov    al, '*'
            invoke SendMessage,hEditPass,EM_SETPASSWORDCHAR,eax,0
        .endif
        invoke RtlZeroMemory,addr PassPhrase,sizeof PassPhrase
        invoke SetWindowPos,hDlg, HWND_TOPMOST,NULL,NULL,NULL,NULL,SWP_NOMOVE or SWP_NOSIZE 
        invoke FlashWindow, hDlg, TRUE
        invoke SetFocus,hEditPass
        popad 
        mov eax, 1
        ret
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hDlg,NULL
    .endif
    popad
    xor eax, eax
    ret
DlgVerif endp

DlgLicense proc hDlg:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
    pushad
    .if uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 211
            invoke RtlZeroMemory,addr PassPhrase,sizeof PassPhrase
            invoke GetDlgItemInt,hDlg,210,addr demoread,FALSE
            .if (eax == 0) || (demoread==FALSE)
                invoke MessageBoxA,hDlg,addr TypeCorrectly,addr AppName,NULL
                jmp __end2
            .endif
            lea    esi, IniFile.DemoPass
            mov    dword ptr [esi], eax
            sub    esi, offset IniFile
            push   esi
            invoke SetDemoOrNot
            pop    esi
            .if (eax==1)
                invoke SetFilePointer,hInit,esi,0,FILE_BEGIN
     		    invoke WriteFile,hInit,addr IniFile.DemoPass,4,addr read, NULL
     		    .if eax == 0
                    invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
                .endif
     		    invoke SaveCheckSum,CHECKSUM_RSET
     		    .if eax == 1
                    invoke MessageBoxA,hDlg,addr szDemoPassChanged,addr AppName,NULL
                    invoke EndDialog,hDlg,NULL
                .else
                    invoke MessageBoxA,hDlg,addr szAccessError,addr AppName,NULL
                .endif
                m2m    eax,offset TimeDateDlgProc
                invoke CreateDialogParam,hInst,103,hwndTab,eax,hDlgParent
	            mov    TimeDateDlgHwnd,eax
            .elseif (eax==3)
                invoke MessageBoxA,hDlg,addr szInvalidPass,addr AppName,NULL
            .else
                invoke EndDialog,hDlg,NULL
            .endif
__end2:
        .endif
    .elseif uMsg == WM_INITDIALOG
        invoke SetWindowText,hDlg,addr DemoTitle
        invoke GetDlgItem,hDlg,210
        mov hEditPass, eax
        invoke SetWindowPos,hDlg, HWND_TOPMOST,NULL,NULL,NULL,NULL,SWP_NOMOVE or SWP_NOSIZE 
        invoke SetFocus,hEditPass
        popad 
        mov eax, 1
        ret
    .elseif uMsg==WM_CLOSE
        invoke EndDialog,hDlg,NULL
    .endif
    popad
    xor eax, eax
    ret
DlgLicense endp

SetDemoOrNot proc
    invoke GetAppPath,addr pDemoDir
    invoke lstrcat,addr pDemoDir,addr GWallDemo
    cmp dword ptr IniFile.DemoPass,0
    jz     demofin
    invoke CreateFile,addr pDemoDir,GENERIC_READ or GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL
    cmp    eax, 0FFFFFFFFh
    jnz    @F
        jmp demofin
@@:
    m2m    hDemo, eax
    invoke GetFileSize,hDemo,0
    cmp    eax, DemoCryptEnd-DemoCryptBegin
    jz     @F
        invoke CloseHandle,hDemo
        jmp demofin
@@:    
    m2m    DemoFilePresent, 1
    invoke SetFilePointer,hDemo,0,0,FILE_BEGIN
    invoke ReadFile,hDemo,addr LicSignature,4,addr read,NULL
    mov    eax, dword ptr IniFile.DemoPass
    mov    ebx, eax
    xor    ecx, ecx
    mov    cl, bl
    shr    ebx, 8
    add    cl, bl
    shr    ebx, 8
    add    cl, bl
    shr    ebx, 8
    add    cl, bl
    lea    esi, DemoCryptBegin
    pushad
    mov    edx, dword ptr [LicSignature]
    xor    edx, eax
    ror    edx, cl
    cmp    edx, 'ERAS'
    jz     @F
    popad
    invoke CloseHandle,hDemo
    mov    eax, 3
    ret
@@:
    invoke SetFilePointer,hDemo,0,0,FILE_BEGIN
    invoke ReadFile,hDemo,addr DemoCryptBegin,DemoCryptEnd-DemoCryptBegin,addr read,NULL
    popad
DemoDecrypt:
    mov    edx, dword ptr [esi]
    xor    edx, eax
    ror    edx, cl
    mov    dword ptr [esi], edx
    add    esi, 4
    cmp    esi, DemoCryptEnd
    jnae   DemoDecrypt
    invoke CloseHandle,hDemo
    mov    eax, 1
    ret
demofin:
    cmp    IniFile.DemoPass, 0
    jz     @F
    invoke MessageBoxA,0,addr AmbiguousCase,addr AppName,MB_YESNO
    cmp    eax, IDYES
    jnz    @F
    invoke DeleteFile,addr pDemoDir
    lea    esi, IniFile.DemoPass
    mov    dword ptr [esi], 0
    sub    esi, offset IniFile
    invoke SetFilePointer,hInit,esi,0,FILE_BEGIN
    invoke WriteFile,hInit,addr IniFile.DemoPass,4,addr read, NULL
    invoke SaveCheckSum,CHECKSUM_RSET
    ; License deleted and pass too
    mov eax, 2
    ret
@@:
    ; no password set
    xor eax, eax
    ret
SetDemoOrNot endp


NagProc PROC uses ebx ecx edx esi edi hwnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
LOCAL ps     :PAINTSTRUCT
LOCAL hDC    :HDC
LOCAL hMemDC :HDC
LOCAL rect   :RECT
 
    .if uMsg == WM_CREATE
        invoke BitmapFromResource,hInst,300
        mov    hBmpGI,eax
        invoke ShowWindow,hwnd,SW_SHOWNORMAL
        invoke SetTimer,hwnd,13h,2000,NULL
        mov    eax, 1
        ret        
        
    .elseif uMsg == WM_TIMER
        .if wParam == 13h
            invoke KillTimer,hwnd,13h
            invoke SendMessageA,hwnd,WM_DESTROY,0,0
            mov    eax, 1
            ret
        .endif
        
    .elseif uMsg == WM_DESTROY
        invoke DestroyWindow, hwnd          
        xor    eax, eax          
        ret              
                                                        
    .elseif uMsg == WM_PAINT                              
        invoke GetClientRect,hwnd,addr rect             
        invoke BeginPaint,hwnd,addr ps
        mov    hDC,eax
        invoke CreateCompatibleDC,hDC
        mov    hMemDC,eax
        invoke SelectObject,hMemDC,hBmpGI       
        xor    eax,eax
        invoke BitBlt,hDC,eax,eax,rect.right,rect.bottom,hMemDC,eax,eax,SRCCOPY  
        xor    edi, edi
        invoke CreateFont,14,edi,edi,edi,400,edi,edi,edi,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH or FF_SWISS,addr FName
        invoke SelectObject,hDC, eax
        invoke SetBkMode,hDC,TRANSPARENT
        invoke SetTextColor,hDC,0
        invoke TextOut,hDC,10,150,addr MyNameAndVer,sizeof MyNameAndVer-1
        
        invoke DeleteDC,hMemDC
        invoke EndPaint,hwnd,addr ps
        xor    eax,eax
        ret
 
    .endif
 
    ret
 
NagProc ENDP
                                                                                                                                                                                                                                                         