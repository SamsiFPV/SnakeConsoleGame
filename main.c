//Autor:        Raphael Seitz
//Dateiname:    main.c
//Begonnen:     09.01.2019
//Beendet:
//Beschreibung: Klassisches Snake Spiel

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//INFORMATION
#define V_MAIN      1   //Hauptversion
#define V_SIDE      2   //Nebenversion
#define V_REVISION  1   //Revisionsnummer
//FUNCTIONALITY
#define FELD_BMAX 39
#define FELD_HMAX 39
#define FELD_BMIN 3
#define FELD_HMIN 3
#define SPEED_MIN 50
#define SPEED_MAX 2000
#define SPEED_STEP 50

//CUSTOMISATION
#define SNAKE_HEAD  219
#define SNAKE_BODY  177
#define SNAKE_FRUIT 40
#define SNAKE_OFF   32
#define BORDERH     196
#define BORDERV     179

int main()
{
    //VARIABLENDEKLATATION / INITIALISIERUNG
    char cHauptmenu;
    char cOptionsmenu;
    char cFeldmenu;
    char cFeld[4];                  //0 = Breite; 1 = höhe; 2 = über rand;
    char cArt[FELD_BMAX + 1][FELD_HMAX + 1];
    int iFeld[FELD_BMAX + 1][FELD_HMAX + 1];
    int i;                          //Zählervariable
    int k;                          //Zählervariable
    int artk;                       //Rückgabe: Ausgabe für display
    int iMoves;                     //Anzahl Bewegungen ///Zähler
    int iFruit[6];                  //Fruchtposition: [0] = Y; [1] = Y; [2] = SNAKE X; [3] = SNAKE Y; [4] = SNAKE length
    char cDirectionInput;           //Richtung von der Schlange, 0 = hinauf, uhrzeiger
    char cDirection;
    char cWinLose;                  //Win / Lose 0 = neutral; 1 = lose; 2 = win
    FILE *fp = NULL;                //pointer für dateischreiben
    time_t t = time(NULL);          //Für DATUM
    struct tm tm = *localtime(&t);  //Für DATUM
    srand((int)time(NULL));         //Zufallsgenerator initialisieren
    for(i = 0; i <= sizeof(cFeld); i++)
    {
        cFeld[i] = 0;
    }
    cFeld[0] = 9; //Breite (9 entspricht 10)
    cFeld[1] = 9; //Höhe   (9 entspricht 10)
    int iTaste;

    clock_t cClock;
    int iTimeStart;
    int iGeschwindigkeit = 1000;

    //PROGRAMM
    do
    {
        //Hauptmenuauflistung
        system("CLS");
        printf("* CONSOLE SNAKE *\n");
        printf("----Hauptmenu----\n");
        printf("1 : Beenden\n");
        printf("2 : Neues Spiel\n");
        printf("3 : Optionen\n");
        printf("4 : Highscores (WIP)\n");
        printf("-----------------\n");
        printf("Version: %i.%i.%i\n", V_MAIN, V_SIDE, V_REVISION);
        printf("By: rphii\n");
        printf("Help: SamsiFPV\n");

        //Hauptmenuerfassung
        do
        {
            cHauptmenu = getch();
        }
        while (cHauptmenu < '1' || cHauptmenu > '4');

        //Hauptmenuausührung
        switch (cHauptmenu)
        {
            case '2': //Neues Spiel
                ///Spiel initialisieren
                cWinLose = 0;
                cDirectionInput = 0;
                iMoves = 0;
                for(i = 0; i <= sizeof(iFruit); i++)
                {
                    iFruit[i] = 0;
                }
                for(i = 0; i <= FELD_BMAX; i++)
                {
                    for(k = 0; k <= FELD_HMAX; k++)
                    {
                        iFeld[i][k] = 0;
                    }
                }
                ///Spiel Start
                system("CLS");
                iFeld[cFeld[0]/2][cFeld[1]/2] = 1;  //Schlange Anfangswert
                iFruit[2] = cFeld[0]/1;             //Schlange position Y
                iFruit[3] = cFeld[1]/2;             //Schlange position X
                iFruit[4]++;                        //Schlangenlänge erweitern
                //neue Frucht Setzen
                do
                {
                    iFruit[0] = rand() % (cFeld[1] + 1);    //pos X
                    iFruit[1] = rand() % (cFeld[0] + 1);    //pos Y
                }
                while(iFeld[iFruit[1]][iFruit[0]] != 0);    //solange Frucht auf Schlange landet)

                iTimeStart = clock();                       //CLOCK
                do
                {
                    do
                    {
                        //printf("%i\n", clock());
                        if(kbhit())                         //Wenn Tastendruck Tastatur
                        {
                            iTaste = getch();               //Taste einlesen
                            //printf("1:%i|", iTaste);      //DEBUG: Taste ausgeben
                            //Pfeiltasten Suchen
                            if (iTaste == 224 || iTaste == 'w' || iTaste == 'a' || iTaste == 's' || iTaste == 'd')  //Pfeiltaste
                            {
                                if(iTaste == 224)
                                {
                                    iTaste = getch();
                                }
                                //printf("2:%i|", iTaste);          //DEBUG: Taste ausgeben
                                if (iTaste == 75 || iTaste == 'a')  //75 = Links
                                {
                                    if (cDirection != 1)            //Wenn nicht rechts
                                    {
                                        cDirectionInput = 3;
                                    }
                                }
                                if (iTaste == 77 || iTaste == 'd')  //77 = Rechts
                                {
                                    if (cDirection != 3)            //wenn nicht links
                                    {
                                        cDirectionInput = 1;
                                    }
                                }
                                if (iTaste == 72 || iTaste == 'w')  //77 = Hinauf
                                {
                                    if (cDirection != 2)            //wenn nicht hinab
                                    {
                                        cDirectionInput = 0;
                                    }
                                }
                                if (iTaste == 80 || iTaste == 's')  //77 = Hinab
                                {
                                    if (cDirection != 0)            //wenn nicht hinauf
                                    {
                                        cDirectionInput = 2;
                                    }
                                }
                            }
                        }
                        //Visalisieren
                        if(clock() - iTimeStart >= iGeschwindigkeit)  //Wenn genug Zeit vergangen ist
                        {
                            iTimeStart = clock();                   //CLOCK
                            cDirection = cDirectionInput;           //Richtung übernehmen
                            ///Feld berechnen
                            //Schlange verschieben
                            if(cDirection == 0) //Nach oben
                            {
                                iFruit[2]--;                        //pos. ändern
                                if(iFruit[2] < 0)                   //rand erreicht
                                {
                                    if(cFeld[2] == 0)               //über Rand aktiv
                                    {
                                        cWinLose = 1;               //Verloren
                                    }
                                    else
                                    {
                                    iFruit[2] = cFeld[0];           //auf richtige Position setzen
                                    }
                                }
                            }
                            if(cDirection == 1)                     //Nach rechts
                            {
                                iFruit[3]++;                        //pos. ändern
                                if(iFruit[3] > cFeld[1])            //rand erreicht
                                {
                                    if(cFeld[2] == 0)               //über Rand aktiv
                                    {
                                        cWinLose = 1;               //Verloren
                                    }
                                    else
                                    {
                                    iFruit[3] = 0;                  //auf richtige Position setzen
                                    }
                                }
                            }
                            if(cDirection == 2)                     //Nach unten
                            {
                                iFruit[2]++;                        //pos. ändern
                                if(iFruit[2] > cFeld[0])            //rand erreicht
                                {
                                    if(cFeld[2] == 0)               //über Rand aktiv
                                    {
                                        cWinLose = 1;               //Verloren
                                    }
                                    else
                                    {
                                    iFruit[2] = 0;                  //auf richtige Position setzen
                                    }
                                }
                            }
                            if(cDirection == 3)                     //Nach links
                            {
                                iFruit[3]--;                        //pos. ändern
                                if(iFruit[3] < 0)                   //rand erreicht
                                {
                                    if(cFeld[2] == 0)               //über Rand aktiv
                                    {
                                        cWinLose = 1;               //Verloren
                                    }
                                    else
                                    {
                                    iFruit[3] = cFeld[1];           //auf richtige Position setzen
                                    }
                                }
                            }
                            //Wenn Frucht eingesammelt
                            if(iFruit[0] == iFruit[3] && iFruit[1] == iFruit[2])    //Wenn an aktueller Stelle eine Frucht
                            {
                                for(i = 0; i <= cFeld[0]; i++)                      //Vertikal
                                {
                                    for(k = 0; k <= cFeld[1]; k++)                  //Horizontal
                                    {
                                        if(iFeld[k][i] != 0)                        //Wenn Feld einen inhalt hat
                                        {
                                            iFeld[k][i]++;                          //Feld inkrementieren
                                        }
                                    }
                                }
                                iFruit[4]++;                                        //Frucht inkrementieren
                            }
                            //Lose: Collision mit sich selbst
                            if(iFeld[iFruit[2]][iFruit[3]] >= 2)                    //Wenn der Kopf in sich geht
                            {
                                cWinLose = 1;                                       //Verloren
                            }
                            else
                            {
                            iFeld[iFruit[2]][iFruit[3]] = iFruit[4];                //Neuen Kopf platzieren
                            }
                            //Schlange löschen / minus 1 jede Pos.
                            for(i = 0; i <= cFeld[0]; i++)                          //Vertikal mal
                            {
                                for(k = 0; k <= cFeld[1]; k++)                      //Horizontal mal
                                {
                                    if(iFeld[i][k] >= 1)
                                    {
                                        iFeld[i][k]--;                              //dekrementieren
                                    }
                                }
                            }
                            //Frucht eingesammelt
                            iFeld[iFruit[2]][iFruit[3]] = iFruit[4];
                            //Neue Frucht benötigt
                            while(iFeld[iFruit[1]][iFruit[0]] != 0 && iFruit[4] != (cFeld[0] + 1) * (cFeld[1] + 1)) //solange Frucht auf Schlange landet
                            {
                                iFruit[0] = rand() % (cFeld[1] + 1);                //pos X
                                iFruit[1] = rand() % (cFeld[0] + 1);                //pos Y
                            }
                            //Feld zeichnen
                            system("CLS");
                            for(i = 0; i <= (cFeld[0] + 2); i++)                    //Vertikal mal plus 2 wegen Balken
                            {
                                for(k = 0; k <= (cFeld[1] + 2); k++)                //Horizontal mal plus 2 wegen Balken
                                {
                                    if(i == 0 || i == cFeld[0] + 2)
                                    {
                                        if(cFeld[1] + 2 != k)
                                        {
                                        printf("%c%c", BORDERH, BORDERH);           //horizontaler balken
                                        }
                                    }
                                    else if(i != 0 && i != cFeld[0] + 2 && (k == 0 || k == cFeld[1] + 2))
                                    {
                                        printf("%c", BORDERV);                      //vertikaler balken
                                    }
                                    else
                                    {
                                        artk = make_art(iFeld[i - 1][k - 1], iFruit[1], iFruit[0], i - 1, k - 1, iFruit[4]);   //Aktuelles Pixel berechnen
                                        if(artk == SNAKE_FRUIT)
                                        {
                                            printf("%c%c", artk, artk + 1);         //Aktuelles pixel Printen / ausgeben
                                        }
                                        else
                                        {
                                            printf("%c%c", artk, artk);             //Aktuelles pixel Printen / ausgeben
                                        }
                                    }

                                }
                                printf("\n");                                       //neue zeile
                            }
                            printf("%i / %i\n", iFruit[4], (cFeld[0] + 1) * (cFeld[1] + 1));    //Fortschritt
                            iMoves++;
                            if(iFruit[4] >= (cFeld[0] + 1) * (cFeld[1] + 1))        //Wenn Felbreite * Feldhöhe früchte gesammelt
                            {
                                cWinLose = 2;                                       //Gewonnen
                            }
                        }
                    }
                    while (cWinLose == 0);      //Während nicht verloren / gewonnen
                    if(cWinLose == 1)           //Verloren
                    {
                        printf("VERLOREN!\n");
                    }
                    if(cWinLose == 2)           //Gewonnen
                    {
                        printf("GEWONNEN!\n");
                    }
                    //Spielende Info
                    printf("Zuege: %i\n", iMoves);
                    //Speichern in .txt
                    fp = fopen("highscores_snake.txt", "a+");
                    if(cWinLose == 1)           //Verloren
                    {
                        fprintf(fp, "%04d-%02d-%02d VERLOREN: %3i von %3i, Über Rand: %i, Geschwindigkeit: %4i, höhe: %2i, breite: %2i, %4i Züge\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, iFruit[4], (cFeld[0] + 1) * (cFeld[1] + 1), cFeld[2], iGeschwindigkeit, cFeld[0] + 1, cFeld[1] + 1, iMoves);
                    }
                    if(cWinLose == 2)           //Gewonnen
                    {
                        fprintf(fp, "%04d-%02d-%02d GEWONNEN: %3i von %3i, Über Rand: %i, Geschwindigkeit: %4i, höhe: %2i, breite: %2i, %4i Züge\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, iFruit[4], (cFeld[0] + 1) * (cFeld[1] + 1), cFeld[2], iGeschwindigkeit, cFeld[0] + 1, cFeld[1] + 1, iMoves);
                    }
                    printf("highscores_snake.txt\n");
                    printf("Haupmenu: ESC\n");

                    fclose(fp);
                    do
                    {
                        iTaste = getch();
                    }
                    while(iTaste != 27);
                }
                while (iTaste != 27);                                       //Während Spiel nicht verlassen wird (via ESC)

                break;
            case '3':                                                       //Optionsmenu
                do
                {
                    //Optionsmenuauflistung
                    list_menu_options();
                    //Optionsmenuerfassung
                    do
                    {
                        cOptionsmenu = getch();
                    }
                    while (cOptionsmenu < '1' || cOptionsmenu > '3');       //Bis richtiges Menu ausgewählt wurde

                    //Optionsmenuausführung
                    switch (cOptionsmenu)
                    {
                        case '2':                                           //MENU: Feld
                            do
                            {
                                system("CLS");
                                list_menu_field();                          //Feldmenu auflisten
                                do
                                {
                                cFeldmenu = getch();
                                }
                                while (cFeldmenu < '1' || cFeldmenu > '4'); //Bis richtiges Menu ausgewählt wurde

                                switch (cFeldmenu)
                                {
                                    case '2':                               //OPTION: Feldhoehe
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field();              //UNTERMENU1 Auflisten
                                            //Akutelle hoehe ausgeben
                                            printf("[Pfeiltasten] <Feldhoehe: %i>\n", cFeld[0] + 1);

                                            //Pfeiltasten Suchen
                                            iTaste = getch();
                                            if (iTaste == 224)
                                            {
                                                iTaste = getch();
                                                if (iTaste == 75)           //75 = Links
                                                {
                                                    if (cFeld[0] > FELD_HMIN)
                                                    {
                                                        cFeld[0]--;
                                                    }
                                                }
                                                if (iTaste == 77)           //77 = Rechts
                                                {
                                                    if (cFeld[0] < FELD_HMAX)
                                                    {
                                                        cFeld[0]++;
                                                    }
                                                }
                                            }
                                            //um aus untermenu 1 zurückzugehen
                                            if(iTaste == '1' || iTaste == '3' || iTaste == '4')
                                            {
                                                cFeldmenu = iTaste;
                                            }
                                        }
                                        while (cFeldmenu == '2' && cFeldmenu != '1');
                                        iTaste = 0;
                                        break;
                                    case '3':                               //OPTION: Feldbreite
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field();              //UNTERMENU1 Auflisten
                                            //Akutelle breite ausgeben
                                            printf("[Pfeiltasten] <Feldbreite: %i>\n", cFeld[1] + 1);
                                            //Pfeiltasten Suchen
                                            iTaste = getch();
                                            if (iTaste == 224)
                                            {
                                                iTaste = getch();
                                                if (iTaste == 75)           //75 = Links
                                                {
                                                    if (cFeld[1] > FELD_BMIN)
                                                    {
                                                        cFeld[1]--;
                                                    }
                                                }
                                                if (iTaste == 77)           //77 = Rechts
                                                {
                                                    if (cFeld[1] < FELD_BMAX)
                                                    {
                                                        cFeld[1]++;
                                                    }
                                                }
                                            }
                                            //um aus untermenu 1 zurückzugehen
                                            if(iTaste == '1' || iTaste == '2' || iTaste == '4')
                                            {
                                                cFeldmenu = iTaste;
                                            }
                                        }
                                        while (cFeldmenu == '3' && cFeldmenu != '1');
                                        iTaste = 0;
                                        break;
                                    case '4':                               //OPTION: "Ueber Rand hinaus gehen"
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field();              //UNTERMENU1 Auflisten
                                            //Status von "Ueber Rand hinaus gehen" rausgeben
                                            if (cFeld[2] == 0)
                                            {
                                                printf("[Pfeiltasten] <%i: NEIN>\n", cFeld[2]);
                                            }
                                            if (cFeld[2] == 1)
                                            {
                                                printf("[Pfeiltasten] <%i: JA>\n", cFeld[2]);
                                            }
                                            //Pfeiltasten Suchen
                                            iTaste = getch();
                                            if (iTaste == 224)
                                            {
                                                iTaste = getch();
                                                if (iTaste == 75 || iTaste == 77)
                                                {
                                                    cFeld[2] ^= 1;
                                                }
                                            }
                                            //um aus Feldmenu zurückzugehen
                                            if(iTaste == '1' || iTaste == '2' || iTaste == '3')
                                            {
                                                cFeldmenu = iTaste;
                                            }
                                        }
                                        while (cFeldmenu == '4' && cFeldmenu != '1');
                                        iTaste = 0;
                                        break;
                                    default:
                                        break;
                                }
                            }
                            while (cOptionsmenu == '2' && cFeldmenu != '1');
                            break;
                        case '3':                                           //MENU: Geschwindigkeit
                            do
                            {
                                //Aktuelle Einstellung ausgeben
                                system("CLS");
                                list_menu_options();
                                printf("[Pfeiltasten] <Geschwindigkeit: %i>\n", iGeschwindigkeit);
                                //Pfeiltasten Suchen
                                iTaste = getch();
                                if (iTaste == 224)
                                {
                                    iTaste = getch();
                                    if (iTaste == 75)                       //75 = Links
                                    {
                                        if (iGeschwindigkeit > SPEED_MIN)
                                        {
                                            iGeschwindigkeit -= SPEED_STEP;
                                        }
                                    }
                                    if (iTaste == 77)                       //77 = Rechts
                                    {
                                        if (iGeschwindigkeit < SPEED_MAX)
                                        {
                                            iGeschwindigkeit += SPEED_STEP;
                                        }
                                    }
                                }
                                //um aus untermenu zurückzugehen
                                if(iTaste == '1' || iTaste == '2')
                                {
                                    cOptionsmenu = iTaste;
                                }
                            }
                            while (cOptionsmenu == '3' && cOptionsmenu != '1');
                            break;
                        default:
                            break;
                    }
                }
                while (cOptionsmenu == '2' && cOptionsmenu != '1');
                break;
            case '4': //Highscores
                break;
            default:
                break;
        }
    }
    while (cHauptmenu != '1');

    return 0;
}

int list_menu_options(void)                 //Optionsmenu auflisten
{
    system("CLS");
    printf("----Optionsmenu----\n");
    printf("1 : Zurueck\n");
    printf("2 : Feld\n");
    printf("3 : Geschwindigkeit\n\n");
}
int list_menu_field(void)                   //Feldmenu auflisten
{
    printf("----Feldeinstellungen----\n");
    printf("1 : Zurueck\n");
    printf("2 : Feldhoehe\n");
    printf("3 : Feldbreite\n");
    printf("4 : Ueber Rand hinaus gehen\n\n");
}
int make_art(int iFeldpr, int iFeldFruitY, int iFeldFruitX, int pos_y, int pos_x, int iFruits) //erstellt char für char (für das Visuelle Feld)
{
    //INTIIALISIEREN / DEKLARIEREN
    int i = 0;                                              //Zählvariable (Horizontal)
    int iArtpr = 0;                                         //"Zeichenvariable"
    //CODE
    if(iFeldpr == iFruits)                                  //Wenn der Kopf der Wert im Input an der aktuellen Position ist
    {
        iArtpr = SNAKE_HEAD;                                //Gibt char des Kopfes zurück
    }
    else if(pos_y == iFeldFruitY && pos_x == iFeldFruitX)   //Wenn an aktueller Stelle eine Frucht
    {
        iArtpr = SNAKE_FRUIT;                               //Gibt char der Frucht zurück
    }
    else if(iFeldpr == 0)                                   //Wenn kein Wert im Input an der aktuellen Position
    {
        iArtpr = SNAKE_OFF;                                 //Gibt char von nichts zurück
    }
    else if(iFeldpr >= 1)                                   //Wenn Wert im Input an der aktuellen Position
    {
        iArtpr = SNAKE_BODY;                                //Gibt char des Körpers zurück
    }
    return(iArtpr);                                         //Gezeichnete Linie Zurückgeben
}
