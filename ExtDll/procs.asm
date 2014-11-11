HookProc2:
    pop    RetAddr
    push   offset GoBack
    push   eax
    mov    eax, dword ptr [esp+16]
    mov    lpData,eax
    pop    eax
BytesToSave:
    db     18 dup(90h)                                      ;
    jmp    hSMW
GoBack:
    push   ebx
    mov    ebx, lpData
    cmp    byte ptr [ebx],'_'
    jnz    @F
    mov    eax, 1
@@:
    pop    ebx
    jmp    RetAddr
EndHookProc2:

HideRegedit proc
    pushad
    mov    IsRegeditHooked,1
    invoke GetModuleHandle,addr K32Str
    invoke GetProcAddress,eax,addr SMStr
    mov    hSMW, eax
    invoke VirtualProtect,hSMW,5,PAGE_EXECUTE_READWRITE,addr read
    mov    eax, offset HookProc2
    sub    eax, hSMW
    sub    eax, 5
    mov    dword ptr [AssJump+1],eax
    
    push   offset tbl
    call   disasm_init
    add    esp, 4
    mov    edx, hSMW
    mov    ebx, 4
@@:
    push   edx
    push   offset tbl
    call   disasm_main
    add    esp,8
    add    edx, eax
    dec    ebx
    jnz    @B
           
    sub    edx, hSMW
    mcopy  hSMW,offset BytesToSave,edx
    
    mcopy  offset AssJump,hSMW, 5

    add    hSMW, edx
    popad
    ret
HideRegedit endp          


;----------------------------------------- retrieves the current module's name
GetName proc
    pushad
    invoke GetModuleFileNameA,0,addr CmdLine, sizeof CmdLine
    invoke NameFromPath,addr CmdLine,addr ExeName
    invoke CharLower,addr ExeName
    invoke lstrlen, addr ExeName
    add    eax, 4
    mov    sName, eax
    popad
    ret
GetName endp

;-------------------------------------- Is it a privilegied file ?
IsPriv proc pBuf:DWORD
    mov    edi, pBuf
@@:
    invoke lstrcmpi,addr ExeName,edi
    cmp    eax,0
    jz     Privilegied
    invoke lstrlen, edi
    add    edi, eax
    cmp    dword ptr [edi],0
    jz     @F
    inc    edi
    jmp    @B
Privilegied:
    mov    eax, 1
    ret
@@:
    xor    eax, eax
    ret
IsPriv endp


VerifyDate proc
LOCAL RetV:DWORD
LOCAL decal:DWORD
LOCAL DayOfWeek:DWORD
LOCAL ActualHour:DWORD
    pushad
    invoke GetLocalTime,addr systt
    lea    edi, IniFile.WeekAuth.Week
    assume edi : ptr DAYAUTH
    .if [edi].AuthEd == 0
        xor    eax, eax
        mov    ax, word ptr systt.wDayOfWeek
        .if eax == 0
            mov    eax, 7
        .endif
        dec    eax
        mov    DayOfWeek, eax
        assume edi : nothing
        lea    edi, IniFile.WeekAuth.Monday.AuthEd
        mov    ecx, sizeof DAYAUTH
        mul    ecx
        mov    ecx, eax
        mov    ebx, dword ptr [edi+ecx]
        .if ebx == 1
            mov    decal, ecx
            jmp    __AuthEd
        .else
            mov    RetV, DATE_NO_DAY_ALLOWED
            jmp    __enddatecheck
        .endif
    .else
        mov decal, 7*sizeof DAYAUTH
    .endif
__AuthEd:
    xor    eax, eax
    mov    ax, word ptr systt.wHour
    mov    ecx, 60
    mul    ecx
    xor    ecx, ecx
    mov    cx, word ptr systt.wMinute
    add    eax, ecx
    mov    ActualHour, eax
    lea    edi, IniFile.WeekAuth.Monday
    add    edi, decal
    assume edi : ptr DAYAUTH
    .if [edi].BaseHour != 0
        mov eax, ActualHour
        .if eax < [edi].BaseHour
            mov    RetV, DATE_HOUR_NOT_VALID
            jmp    __enddatecheck
        .endif
    .endif
    .if [edi].Duration != 0
        .if ImAGame == 0
            mov    ImAGame, 1
            .if Counter == 0
                m2m    SharedBaseHour, ActualHour
            .endif
            inc    Counter
        .endif
        mov    eax, ActualHour
        sub    eax, SharedBaseHour
        add    eax, TimeElapsedBefore
        mov    SharedDuration, eax
        mov    ebx, [edi].Duration
        dec    ebx
        .if (ebx == eax) && (LimitShown == 0)
            mov    LimitShown, 1
            mov    RetV, DATE_DURATION_LIMIT
            jmp    __enddatecheck
        .endif
        .if [edi].Duration < eax
            mov    RetV, DATE_DURATION_OUT
            jmp    __enddatecheck
        .endif            
    .endif
    mov    RetV, DATE_VALID
__enddatecheck:
    popad
    mov    eax, RetV
    ret
VerifyDate endp

MakeRgn PROC hDC:HDC,PicWidth:DWORD,PicHeight:DWORD

    LOCAL X:DWORD                     ; Col value
    LOCAL Y:DWORD                     ; Row value
    LOCAL StartLineX:DWORD            ; start location of a region
    LOCAL FullRgn:DWORD               ; Combination of all Regions found so far
    LOCAL LineRgn:DWORD               ; Current region
    LOCAL TransparentColor:DWORD      ; color that is used to indicate transparency
    LOCAL InFirstRgn:DWORD            ; First Region state 
    LOCAL Inline:DWORD                ; Inline Region state

    mov InFirstRgn,1
    mov Inline,FALSE
    mov X,0
    mov Y,0
    mov StartLineX,0

    mov TransparentColor,00h           ; here black is the tranparent color :)
    
    mov ebx, PicHeight
    .WHILE Y < ebx
        mov ebx, PicWidth
        .WHILE X <= ebx
            invoke GetPixel,hDC,X,Y
            mov ebx, PicWidth
            .IF eax==TransparentColor || X==ebx
                .IF Inline==1 
                    dec Inline
                    mov ebx,Y
                    inc ebx
                    invoke CreateRectRgn,StartLineX,Y,X,ebx
                    mov LineRgn,eax
                    .IF InFirstRgn==1 
                        m2m FullRgn,LineRgn
                        dec InFirstRgn
                    .ELSE
                        invoke CombineRgn,FullRgn,FullRgn,LineRgn,RGN_OR
                        invoke DeleteObject,LineRgn
                    .ENDIF
                .ENDIF
            .ELSE
                .IF Inline==0
                    inc Inline
                    m2m StartLineX,X
                .ENDIF
            .ENDIF
            inc X
            mov ebx,PicWidth
        .ENDW
        inc Y
        mov X,0
        mov ebx,PicHeight
    .ENDW
    mov eax,FullRgn
    ret
MakeRgn ENDP

WndProc proc hWin:DWORD,uMsg:DWORD,wParam:DWORD, lParam:DWORD
LOCAL hDC:HDC
LOCAL ps:PAINTSTRUCT
LOCAL MyBuf[30]:BYTE

    .if uMsg==WM_CREATE
        m2m    SecondsElapsed, 60
        invoke SendMessage,hWin,WM_TIMER,0,0
        invoke SetTimer,hWin,1,1000,NULL
        mov    eax, 1
        ret

   .elseif uMsg==WM_DESTROY
        invoke DestroyWindow,hWin
        invoke ExitProcess, 0
        
   .elseif uMsg==WM_PAINT
        invoke BeginPaint,hWin,addr ps
        mov    hDC,eax
        xor    esi,esi
        invoke BitBlt,hDC,esi,esi,600,200,hGIDC,esi,esi,SRCCOPY
	    invoke EndPaint,hWin,addr ps

   .elseif uMsg==WM_TIMER
        .if SecondsElapsed == 0
            invoke KillTimer,hWin,1
            invoke SendMessageA,hWin,WM_DESTROY,0,0
        .endif
        invoke GetWindowDC,hWin
        mov    hDC,eax
        xor    edi,edi
        invoke CreateCompatibleDC,hDC        ; draw device
        mov    hGIDC,eax

        invoke CreateCompatibleBitmap,hDC,600,200
        mov    hBmpGI,eax
        invoke SelectObject,hGIDC,hBmpGI     ; device area initialisation (WxDim x WyDim)

        invoke GetStockObject,BLACK_BRUSH    ; get black brush
        invoke FillPath,hGIDC                ; fill device area with back brush

        invoke SetTextColor,hGIDC,0FFh   ; text initialisation (red colored)
        invoke SetBkMode,hGIDC,TRANSPARENT   ; transparent background text
        
        invoke CreateFont,28,edi,edi,edi,600,1,edi,edi,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH or FF_SWISS,ADDR FontName
        mov    hFont,eax
        invoke SelectObject,hGIDC,hFont
        dec    SecondsElapsed
        freed  MyBuf,sizeof MyBuf
        invoke wsprintf,addr MyBuf,addr filter,SecondsElapsed
        invoke lstrlen,addr MyBuf
        invoke TextOut,hGIDC,NULL,NULL,addr MyBuf,eax ; draw text to area
        
        invoke MakeRgn,hGIDC,600,200     ; make the visible/transparent region
        invoke SetWindowRgn,hWin,eax,1       ; set visible/tranparent region
        
        invoke ShowWindow,hWin,SW_SHOWNORMAL ; display regionned window
    .else
        invoke DefWindowProc,hWin,uMsg,wParam,lParam
        ret
    .endif
    xor eax,eax
    ret
    
WndProc endp


DisplayMessage proc
LOCAL wc    :WNDCLASSEX
LOCAL msg   :MSG

    xor    esi,esi
    mov    wc.cbSize,sizeof WNDCLASSEX
    mov    wc.style,CS_HREDRAW or CS_VREDRAW
    mov    wc.lpfnWndProc,offset WndProc
    mov    wc.cbClsExtra,esi
    mov    wc.cbWndExtra,esi
    m2m    wc.hInstance,hInstance
    invoke GetStockObject,HOLLOW_BRUSH
    mov    wc.hbrBackground,eax
    mov    wc.lpszMenuName,esi
    mov    wc.lpszClassName,offset AppName
    m2m    wc.hIcon,esi
    m2m    wc.hIconSm,esi
    invoke LoadCursor,esi,IDC_ARROW
    mov    wc.hCursor,eax

    invoke RegisterClassEx,addr wc

    invoke CreateWindowEx,WS_EX_TOPMOST or WS_EX_TOOLWINDOW,addr AppName,ADDR AppName,WS_POPUP,40,40,380,200,esi,esi,hInstance,esi
    mov    hLimit,eax

    .while 1
       xor    eax,eax
       invoke GetMessage, addr msg,eax,eax,eax
       .break .if (!eax)
       invoke TranslateMessage, addr msg
       invoke DispatchMessage, addr msg
    .endw
    mov eax,msg.wParam
    invoke GetCurrentThread
    invoke GetExitCodeThread,eax,addr ExCode
    invoke ExitThread,ExCode
    ret
DisplayMessage endp

DetectGame proc pDll:DWORD
    pushad
    .if IniFile.AreWinGamesBlocked == 1
        invoke IsPriv, addr ForbiddenFiles
        .if eax == 1
            jmp KillIt
        .endif
    .endif
    invoke AccessConfigFiles,LOAD_SECONDARY,addr SecondaryList,500
    invoke IsPriv, addr SecondaryList
    .if eax==1
        jmp KillIt
    .endif
    invoke GetModuleHandle,pDll                ; IMPORTANT : This is THE heart of the gamewall !!!
    .if eax != 0
KillIt:
        invoke IsPriv,addr AuthFiles
        .if eax == 0
            invoke IsPriv,addr LaunchBuf
            .if eax == 0
                .if StillKilled != 1
                    invoke AccessConfigFiles,3,addr ExeName,sName
                    mov    StillKilled, 1
                .endif
            .endif
        .endif
        invoke IsPriv,addr AuthFiles            ; est ce un fichier privilégié par défaut ?
        .if eax == 0                            ; apparemment non, donc
            invoke IsPriv,addr PrivBuf          ; est ce un fichier privilégié déterminé par l'utilisateur ?
            .if eax == 0                        ; toujours pas
                invoke VerifyDate
                .if (eax == DATE_DAY_NOT_VALID) || (eax == DATE_HOUR_NOT_VALID) || (eax == DATE_NO_DAY_ALLOWED) || (eax == DATE_DURATION_OUT)
                    invoke ExitProcess,0            ; ciao
                .elseif eax == DATE_DURATION_LIMIT
                    .if IniFile.IsUserInformed == 1
                        invoke CreateThread,NULL,NULL,addr DisplayMessage,NULL,NULL,addr LimitTHID
                    .endif
                .endif
            .endif
        .endif
    .endif
    popad
    ret
DetectGame endp

IsUserAdmin proc
    invoke OpenSCManagerA,NULL,NULL,SC_MANAGER_ALL_ACCESS
    .if eax == 0
        invoke GetLastError
        .if eax == ERROR_ACCESS_DENIED
            xor    eax, eax
            ret
        .endif
    .else
        invoke CloseHandle,eax
        mov eax, 1
        ret
    .endif
IsUserAdmin endp

DoControlPanel proc
    invoke OpenProcess,PROCESS_ALL_ACCESS,NULL,hWnd
    push   eax
    invoke WriteProcessMemory,eax,ModifPtr,addr Write,2,addr read
    pop    eax
    invoke CloseHandle,eax
    ret
DoControlPanel endp
include ..\ACF.asm


;----------------------------------------------- This procedure is called at each hook callback
MainProc proc
    .if HasBeenTested == 0
        call   GetName        
        m2m    HasBeenTested, 1
    .endif
    invoke OpenProcess,PROCESS_VM_READ,NULL,hWnd
    push   eax
    invoke ReadProcessMemory,eax,pIni,addr IniFile,sizeof INIFILE,addr read
    pop    eax
    invoke CloseHandle,eax
;---------------------------------------------- some verifications on this name : special if it's regedit or rundll32
    invoke lstrcmpi,addr ExeName,addr szRunDll
    .if eax == 0
        invoke GetModuleHandle,addr szTimeDate
        .if eax != 0
            .if IniFile.IsCplPrevented == 1
                .if tdShown == 0                                          ; yes, we just block & end it ! Impossible to change the current time by this way...
                    mov    tdShown, 1
                    invoke MessageBoxA,0,addr AuthRequired,addr AppName,0
                    invoke ExitProcess,0
                .endif
            .endif
        .endif
    .endif
    .if IsRegeditHooked != 1
        invoke lstrcmpi,addr ExeName,addr regedtStr               ; else, is this process regedit ?
        .if eax == 0
            call   HideRegedit
        .endif
    .endif
    invoke DetectGame,addr dDrawStr
    invoke DetectGame,addr ogl32Str
    ret
MainProc endp

;------------------------------------------------------- proc‚dure de hook graphique
HookProc proc nCode:DWORD,wParam:DWORD,lParam:DWORD
    call   MainProc
    invoke CallNextHookEx,hHook,nCode,wParam,lParam
    ret
HookProc endp

;------------------------------------------------------- procédure de hook clavier
MHookProc proc nCode:DWORD,wParam:DWORD,lParam:DWORD
    mov    eax, wParam
    cmp    eax, VK_F11
    jnz    @F
    invoke GetKeyState,VK_LSHIFT
    shr    eax,16
    cmp    ax, -1
    jnz    @F
    invoke GetKeyState,VK_RSHIFT
    shr    eax,16
    cmp    ax, -1
    jnz    @F
    mov    ecx, cs
    xor    cl, cl
    jecxz  WinNT
    call   DoControlPanel
    jmp @F
WinNT:
    .if IniFile.IsUserPrevented == 0
        call   DoControlPanel
    .elseif IniFile.IsUserPrevented == 1
        call   IsUserAdmin
        .if eax == 0
            ; invoke MessageBoxA,0,addr UserForbidden,addr AppName,0
        .elseif eax == 1
            call   DoControlPanel
        .endif
    .endif
@@:
    assume edi:NOTHING
    call   MainProc
    invoke CallNextHookEx,MhHook,nCode,wParam,lParam
    ret
MHookProc endp