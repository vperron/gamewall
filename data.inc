
.const
;------------------------------------------------    prototypes and numerics
    IO_USER_FUNC_CODE_BASE EQU (0800h)                                      
    METHOD_BUFFERED        EQU (000h)
    IOC_GETAPIID        EQU (IO_USER_FUNC_CODE_BASE + 0100h) or METHOD_BUFFERED
    IOC_GETCURPID       EQU (IO_USER_FUNC_CODE_BASE + 0200h) or METHOD_BUFFERED
    IOC_HOOK            EQU (IO_USER_FUNC_CODE_BASE + 01000h) or METHOD_BUFFERED
    IOC_UNHOOK          EQU (IO_USER_FUNC_CODE_BASE + 01100h) or METHOD_BUFFERED
    CHECKSUM_RSET       equ 0
    CHECKSUM_RGET       equ 1
    CHECKSUM_FGET       equ 2
    DISABLE_ITEMS       equ 0
    ENABLE_ITEMS        equ 1
    GET_HANDLES         equ 2
    GET_HOUR_VALUES     equ 3
    GET_DURATION_VALUES equ 4
    DISABLE_EDITSONLY   equ 5
    SET_VALUES          equ 6
    SET_VALUES_FORCE    equ 7
    SET_VALUES_NULL     equ 8
    SwitchSingle        PROTO :DWORD,:DWORD
    GetHourValue        PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
    TimeDateDlgProc     PROTO :DWORD,:DWORD,:DWORD,:DWORD
    NagProc             PROTO :DWORD,:DWORD,:DWORD,:DWORD
    OptionsDlgProc      PROTO :DWORD,:DWORD,:DWORD,:DWORD
    SelectDlgProc       PROTO :DWORD,:DWORD,:DWORD,:DWORD
    SecDlgProc          PROTO :DWORD,:DWORD,:DWORD,:DWORD
    TabProc             PROTO :DWORD,:DWORD,:DWORD,:DWORD
    HndLoop             PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
    DlgProc 	        PROTO :DWORD,:DWORD,:DWORD,:DWORD
    DlgVerif 	        PROTO :DWORD,:DWORD,:DWORD,:DWORD
    DlgNewPass 	        PROTO :DWORD,:DWORD,:DWORD,:DWORD
    DlgLicense 	        PROTO :DWORD,:DWORD,:DWORD,:DWORD
    ;BlankProc 	        PROTO :DWORD,:DWORD,:DWORD,:DWORD
    SetValues           PROTO :DWORD,:DWORD,:DWORD
    SaveCheckSum        PROTO :DWORD
    DemoReminder        PROTO :DWORD
    SetDemoOrNot        PROTO
    TraqueTitres        PROTO
    HideMeKMD           PROTO
    UnHideMeKMD         PROTO
    HideMe9x            PROTO
    UnHideMe9x          PROTO
    CreatePassWindow    PROTO :DWORD
    CreateNag           PROTO
    SetDefaultsInControls PROTO
    DoUnHookAndClose    PROTO
    AccessConfigFiles   PROTO :DWORD,:DWORD,:DWORD
    OpenPipe            PROTO
    IsUserAdmin         PROTO
    
;--------------------------------------------- string constants
    Commands            dd 'REAL','REAP','WRIL','WRIP'
    nfilter             db "%u",0
    nhfilter            db "%02u:%02u",0
    TooLargeValue       db "Vous avez entré un nombre de minutes trop grand !",10,13,"Veuillez corriger ou laissez le champ vide si vous souhaitez que la journée entière soit bloquée.",0
    BadValues           db "Vous avez entré une valeur incorrecte.",10,13,"Rappel :",10,13,"   Syntaxe pour la durée : un simple chiffre",10,13,"   Syntaxe pour l'heure : hh:mm",0
    IHStr		        db "InstallHook",0
    UIHStr		        db "UnInstallHook",0
    SubKey		        db "Software\Microsoft\Windows\CurrentVersion\Run",0
    GKey                db "Software\GameWall",0
    ICSum               db "ICSum",0
    ServName		    db "_acm32 Service",0
    DllName		        db "\gamewall.dll",0
    Typ 		        db "BINARY",0
    ButClass            db "BUTTON",0
    VisitAuth           db "Site du GameWall",0
    MailAuth            db "Mailer l'auteur",0
    lpPage              db "http://perso.wanadoo.fr/farsight",0
    lpMail              db "mailto:perron.victor@gmail.com",0
    lpOperation         db "open",0
    
    SorryDemoVersion    db "Désolé mais cette fonctionnalité est désactivée dans la version de démonstration.",10,13,"Contactez l'auteur à perron.victor@gmail.com pour obtenir une version complète",10,13,"ou rendez-vous sur le site http://perso.wanadoo.fr/farsight pour plus d'informations",0
    TypeCorrectly       db "Vous avez entré une valeur incorrecte. Veuillez ré-entrer la clé correctement.",0
    szDemoPassChanged   db "Votre clé a été enregistrée avec succès ! Merci de votre achat !",0
    AmbiguousCase       db "Vous avez entré une clé mais le fichier de licence est introuvable ou corrompu.",10,13,"Supprimer le fichier de Licence et retourner à une version de démonstration ?",0
    DemoTitle           db "Entrez votre clé de licence ici : chiffres seulement !",0
    
    PrivFileName	    db "\pPriv32.txt",0
    LaunchFileName	    db "\pLoadEx32.txt",0
    SecFileName         db "\pSecExt32.txt",0
    YetLoaded           db "Le GameWall est déjà en cours d'exécution. Veuillez le femer avant d'essayer de le recharger.",0
    FirstTimeText       db "Bonjour, cher utilisateur !",10,13,"C'est apparemment la première fois que vous lancez le GameWall.",10,13,"La clé d'accès à la configuration se trouve dans le fichier ReadMe.",0
                        db 10,13,'Souvenez-vous-en ou vous ne pourrez pas désactiver ce logiciel.',10,13,"Merci d'avoir choisi le GameWall !",0
    szNTDll             db "ntdll.dll",0
    szU32Dll            db "USER32.dll",0
    szMgBox             db "MessageBoxA",0
    szNQSI              db "NtQuerySystemInformation",0
    szErrorUnexp        db "Une erreur interne s'est déclarée. Relancez ou réinstallez le GameWall et en cas de problème contactez votre revendeur.",0
    szAccessError       db "Une erreur s'est déclarée lors de l'accès au fichier GameWall.ini.",10,13,"Peut-être est-il utilisé par une autre application.",0
    szInvalidPass       db "Le mot de passe que vous avez entré est invalide.",0
    NewPassTitle        db "Entrez un nouveau mot de passe et cliquez OK.",0
    szMoreChars         db "Veuillez entrer plus de 6 caractères !",0
    szPassChanged       db "Vous avez changé avec succès le mot de passe du GameWall. Ne l'oubliez pas !",0
    szOptChanged        db "Vous avez changé avec succès les options du GameWall.",0
    MutexName           db "GameWallMutexCheckIsPreviouslyRunned",0
    FileModified        db "La configuration du GameWall a été corrompue. La configuration par défaut a été remise en place, notamment le mot de passe d'accès.",0
    szDemoLic           db "Cette version est enregistrée !",10,13,"Clé de licence : %u",0
    szServNotReady      db "Programme serveur inactif ou absent - Veuillez vérifier le nom de serveur",0
    
        ForbiddenFiles      db "Démineur",0
                        db "Pinball 3D pour Windows - Cadet de l'Espace",0
                        db "FreeCell",0
                        db "Solitaire",0
                        db "Spider",0
                        db "Atout Pique sur Internet",0
                        db "Backgammon sur Internet",0
                        db "La Dame de Pique en Réseau",0
                        db "Dame de Pique sur Internet",0
                        db "Reversi sur Internet",0,0,0,0
;--------------------------------------------- Constantes pour le Nag Screen
    WxDim               equ 300
    WyDim               equ 200
    MyNameAndVer        db "version 3.0 (C) Victor Perron 2006",0
    FName               db "Tahoma",0
    ClassName           db 'NAGSCREEN',0
    TabTitle1	        db "Filtre d'applications",0				
    TabTitle2	        db "Contrôle horaire",0
    TabTitle3	        db "Options",0
    TabTitle4           db "Editeur Secondaire",0
    GWallIni            db "GameWall.ini",0
    GWallDemo           db "License.key",0

        
.DATA
        SecBuf              db 500 dup (0)
.DATA?
DataBegin LABEL byte
;------------------------------------------------    general datas
    DemoBuf             db 80 dup(?)
    LicSignature        dd ?
    hDlgParent             dd ?
    U32Hnd              dd ?
    hMutex              dd ?
    hLicKey             dd ?
    hDemo               dd ?
    demoread            dd ?
    DemoFilePresent     dd ?
    pDir                db 500 dup(?)
    pIniDir             db 150 dup(?)
    pDemoDir            db 500 dup(?)
    hInit               dd ?
    PE32		        PROCESSENTRY32 <>
    PassPhrase          db 60 dup(?)
    DllPath         	dd 120 dup(?)
    hDll		        dd ?
    pDll		        dd ?
    sDll		        dd ?
    read 		        dd ?
    hInst               dd ?
    Khnd		        dd ?
    value		        db 150 dup(?)
    LocalName		    db 50 dup(?)
    hList1              dd ?
    hList2              dd ?
    hEditPass           dd ?
    IndexItem           dd ?
    Buffer              db 50 dup(?)
    nCount              dd ?
    sBuffer             dd ?
    sName               dd ?
    ExeName             db 50   dup(?)
    PrivFilePath	    db 150  dup(?)
    LaunchFilePath	    db 150  dup(?)
    PrivBuf		        db 500  dup(?)
    LaunchBuf		    db 1500 dup(?)
    sPriv               dd ?
    Pass                dd ?
    NTOs                dd ?
    hGDll               dd ?
    Count_Titles        dd ?
    TitlesOTable        dd 20 dup(?)
    pID                 dd ?
    ServBuf             db 80 dup(?)
    CmdBuf              db 80 dup(?)
    startInfo           STARTUPINFO <>
    processInfo         PROCESS_INFORMATION <>
;---------------------------------------- Authorizations
    
    hTextVisible        dd ?
    hUserPrevented      dd ?
    hCplPrevented       dd ?
    hNagOff             dd ?
    hARestart           dd ?
    hUInfo              dd ?
    hWinGames           dd ?
    hVisitAuth          dd ?
    hMailAuth           dd ?
    hNetMode            dd ?
    hIsServer           dd ?
    hIsClient           dd ?
    hServName           dd ?
;---------------------------------------- Datas pour les tabs
    iccex               INITCOMMONCONTROLSEX <>
    hwndTab             dd ?
    ItemStruct  	    TC_ITEM <?>	
    SelectDlgHwnd       dd ?
    TimeDateDlgHwnd     dd ?
    OptionsDlgHwnd      dd ?
    SecDlgHwnd          dd ?
    CurrentTab          dd ?
    hParent             dd ?
    
;---------------------------------------- Datas pour le nag
    wc                  WNDCLASSEX <?>
    msg                 MSG <?>
    hWnd 		        dd ?
    hBmpGI		        dd ?
    hIcon               dd ?
    NagThID             dd ?
;---------------------------------------- Datas pour le kmd
    scm                 dd ?
    sHnd                dd ?
    hDrv                dd ?

    IniFile             INIFILE <>
    WeekHnd             WEEKHND <>
    DurationSet         DAYAUTH <?>
    hDurationSet        DAYHND <?>

    tbl                 db 2048 dup(?)

DataEnd LABEL byte 
