GameWall Source Code
====================

This used to be a toy project of mine when I was a teenager.
Found out those sources on an old hard drive. Figured it was funny.
Let's save them somewhere for posterity !

Featuring the worst&funniest hacks ever possible on a Windows platform.

- 100% pure ASM code
- Code that dynamically changes itself (jump in the middle of instructions, etc)
- Decryption layers for some parts of the code
- Hard Demo: the paying features are physically removed from the compiled code and reinserted when you buy the keyfile, which is encrypted, relocatable code...
- Kernel drivers for Windows 9x & NT (!) to hide the process by patching the kernel process structures in-memory
- Extremely invasive library that hooks itself in any process to detect if there's usage of DirectX DLLs somewhere
- Funny network propagation of the settings via named network pipes exploitation

I don't remember any more atrocities so far, but there probably are.

And yeah, I did not use version control then.



Original README (french, funny)
===============================



                   _____________________
.............::::::::  GameWall v2.4  ::::::::.............



    Ceci est une version française et fonctionnelle du GameWall, le seul 
outil existant actuellement capable de bloquer tout jeu vidéo utilisant les 
technologies DirectX ou OpenGL : en bref, 99 pour cent d'entre eux !

_
1\ Utilité

    Son utilité est évidente : dans les milieux scolaires/universitaires, ou 
    dans le monde du travail, le GameWall a sa place parmi les logiciels de 
    controle patronal, afin d'optimiser l'utilisation faite des matériels 
    informatiques mis à la disposition de l'étudiant ou de l'employé.
    De meme, chez les particuliers le GameWall peut etre un atout précieux pour 
    faire respecter à ses enfants ou à soi meme des horaires précis de détente.
    

  Fonctionnalités :
    
    -   Indétectable par n'importe quel listeur de processus, toutes versions de 
        Windows confondues
                
    -   Une forte sécurité : algorithmes cryptographiques divers ( CRC32, MD5 ) 
        utilisés pour chaque vérification de données
        
    -   Une protection par mot de passe, modifiable, controle l'accès à la 
        configuration du GameWall. Ces mots de passe sont ne peuvent etre ni 
        devinés, ni trouvés ( algorithme MD5 irréversible ) .
        
    -   Possibilité de sélectionner, parmi les jeux lancés ( dont le lancement a 
        été détecté )  des programmes autorisés à etre utilisés et que le 
        GameWall ne bloquera donc plus à l'avenir, jusqu'à retirement de la 
        liste privilégiée. ( liste cryptée notamment )
        
    -   Sécurité dans les options de configuration : la moindre modification 
        externe des paramètres du programme entrainera une remise en fonction 
        des valeurs par défaut.
        
    -   Controle horaire : il est possible de sélectionner, pour chaque jour de 
        la semaine, une durée à ne pas dépasser, durée pouvant s'étaler, en 
        plusieurs fois si vous le souhaitez, sur toute la journée, ou 
        seulement à partir d'une heure que vous pouvez fixer. Utilisation 
        intuitive.
        
   -    Lancement discret à chaque redémarrage de l'ordinateur : encore une 
        fois, impossible de désactiver cette relance sauf à la désinstallation.
	    ( ou au choix de l'administrateur )
        
   -    Possibilité, activée par défaut, d'empecher l'utilisateur de modifier la 
        date ou l'heure de l'ordinateur : à mettre en relation avec le controle 
        horaire !
        
   -    Quelques autres fonctionnalités, plus discrètes et à détailler : 
        affichage d'un message préventif à la dernière minute avant 
        fermeture d'un jeu vidéo, possibilité de supprimer aux non-
        administrateurs l'accès à la configuration...
        
   -    Jeux vidéo Windows de base bloqués.
        
  Non implémenté actuellement :
  
    -   Améliorer le contrôle horaire selon ce qui est prévu.


_
2\ Utilisation
    Installez puis lancez le GameWall. Il suffit de presser la touche F12 pour 
    obtenir une fenetre demandant le mot de passe d'accès à la configuration.
    Ce mot de passe est par défaut 'First GameWall Password', sans les 
    guillemets et en respectant la casse.
    Celui-ci peut etre modifié dans un des onglets de la boite de configuration.
    Pour plus de précisions, voyez le fichier d'aide de base sur
    http://perso.wanadoo.fr/farsight/gamewall.htm




Langage de programmation utilisé  : Assembleur Win32
Compilateur utilisé               : MASM32 ( free )

Systemes d'exploitation supportés : Win95/98/NT/2000/XP   

  [ non testé sous Windows 2003 Server à l'heure actuelle ]
