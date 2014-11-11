
.const
;------------------------------------------------    constantes du programme
;PROCESS_BASIC_INFORMATION STRUCT
;    ExitStatus          DWORD ?
;    PebBaseAddress      DWORD ?
;    AffinityMask        DWORD ?
;    BasePriority        DWORD ?
;    UniqueProcessId     DWORD ?
;    InheritedFromUniqueProcessId DWORD ?
;PROCESS_BASIC_INFORMATION ENDS
;
STATUS_BUFFER_TOO_SMALL		equ 0C0000023h
DATE_DAY_NOT_VALID          equ 0
DATE_VALID                  equ 1
DATE_HOUR_NOT_VALID         equ 2
DATE_NO_DAY_ALLOWED         equ 3
DATE_DURATION_OUT           equ 4
DATE_DURATION_LIMIT         equ 5

    GetName             PROTO
    IsPriv              PROTO :DWORD
    IsInTheList         PROTO
    Inscript            PROTO 
    WndProc             PROTO :DWORD,:DWORD,:DWORD,:DWORD
    DetectGame          PROTO :DWORD
    VerifyDate          PROTO
    DisplayMessage      PROTO
    MakeRgn             PROTO :DWORD,:DWORD,:DWORD
    AccessConfigFiles   PROTO :DWORD,:DWORD,:DWORD
    OpenPipe            PROTO
    Commands            dd 'REAL','REAP','WRIL','WRIP'
    FontName            db "Tahoma",0
    filter              db "You only have %d seconds left !",0
    GWallIni            db "\GameWall.ini",0
    dDrawStr		    db "ddraw.dll",0
    ogl32Str            db "opengl32.dll",0
    PrivFileName	    db "\pPriv32.txt",0
    LaunchFileName	    db "\pLoadEx32.txt",0
    SecFileName         db "\pSecExt32.txt",0
    Write               db 90h,90h
    regedtStr           db "regedit.exe",0
    K32Str              db "advapi32.dll",0
    SMStr               db "RegEnumValueW",0
    szRunDll            db "rundll32.exe",0
    szTimeDate          db "timedate.cpl",0    
    AuthRequired        db "The actual configuration of the GameWall does NOT allow you to change date or time.",0
    UserForbidden       db "Access to the GameWall control panel is prevented for non-administrative users.",0
    AuthFiles           db "wmplayer.exe",0
                        db "explorer.exe",0
                        db "iexplore.exe",0
                        db "msnmsgr.exe",0
                        db "rundll32.exe",0
                        db "winamp.exe",0
                        db "winamp3.exe",0
                        db "acdsee.exe",0
                        db "msimn.exe",0
                        db "acdsee32.exe",0,0,0,0
    ForbiddenFiles      db "winmine.exe",0
                        db "pinball.exe",0
                        db "freecell.exe",0
                        db "sol.exe",0
                        db "spider.exe",0
                        db "hrtzzm.exe",0
                        db "bckgzm.exe",0
                        db "rvsezm.exe",0
                        db "shvlzm.exe",0
                        db "mshearts.exe",0
                        db "chkrzm.exe",0,0,0,0
    
.data
;------------------------------------------------    general datas
    pIniDir             db 150 dup(0)
    hInit               dd 0
    hInstance           dd 0
    CmdLine		        db 200 dup(0)
    read 		        dd 0
    LocalName		    db 150 dup(0)
    sName               dd 0
    ExeName             db 50 dup(0)
    PrivFilePath	    db 150 dup(0)
    LaunchFilePath	    db 150 dup(0)
    PrivBuf		        db 500 dup(0)
    LaunchBuf		    db 1500 dup(0)
    SecondaryList       db 500 dup(0)
    AssJump		        db 0E9h,0,0,0,0
    IsRegeditHooked     db 0
    hSMW                dd 0
    lpData              dd 0
    RetAddr             dd 0
    StillKilled         db 0
    
    tdShown             dd 0
    myPID               dd 0
    hSnSh               dd 0
    Mod32               MODULEENTRY32 <0>
    HasBeenTested       dd 0
    tbl                 db 2048 dup(0)
    IniFile             INIFILE <>
    ImAGame             dd 0
    systt               SYSTEMTIME <>
    hLimit              dd 0
    LimitTHID           dd 0
    SecondsElapsed      dd 0
    hFont               dd 0
    hBmpGI              dd 0
    hGIDC               dd 0
    ExCode              dd 0
.data?
    LimitShown          dd ?
    Counter             dd ?
    hHook               dd ?
    MhHook              dd ?
    hWnd                dd ?
    ModifPtr            dd ?
    KFlag               db ?
    i                   dd ?
    pIni                dd ?
    SharedBaseHour      dd ?
    SharedDuration      dd ?
    TimeElapsedBefore   dd ?
    ServBuf             db 80 dup(?)
    CmdBuf          db 80 dup(?)