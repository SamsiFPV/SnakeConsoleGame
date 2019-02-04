// Author:					Raphael Seitz
// Filename:				main.c
// Started:					09.01.2019
// EOD (End of Development):
// Description:				Classic Snake Game
// Version:					1.2.2
// Changelog:
// 09.01.2019 - v0.0.0:		Making of file (rphii)
// 09.01.2019 - v1.2.1:		Uploaded file on Github (Samsi)
// 01.02.2019 - v1.2.2:		Cleaned code and translated to english (Samsi)

//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

//Version
#define V_MAIN 1
#define V_SIDE 2
#define V_REVISION 2

//FUNCTIONALITY
#define FIELD_BMAX 39
#define FIELD_HMAX 39
#define FIELD_BMIN 3
#define FIELD_HMIN 3
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

int main(void)
{
    //VARIABLE DECLARATION / INITIALISING
    char cMainmenu;
    char cOptionsmenu;
    char cFieldmenu;
    /*
	cField is for defining field options
    like width and height, and if you can
    go over the edge without dying
	*/
    char cField[4]; //0 = width; 1 = height; 2 = over the edge allowed or not;
    char cArt[FIELD_BMAX + 1][FIELD_HMAX + 1];
    int iField[FIELD_BMAX + 1][FIELD_HMAX + 1];
    int i;      //Countervariable
    int k;      //Countervariable
    int artk;   //Return: Output for display
    int iMoves; //Number of Moves //Counter
    int iFruit[6];    //Fruitposition: [0] = Y; [1] = Y; [2] = SNAKE X; [3] = SNAKE Y; [4] = SNAKE length
    char cDirectionInput; //Direction of the snake, 0 = up, 1 = right, 2 = down, 3 = left
    char cDirection;
    char cWinLose;    //Win / Lose 0 = neutral; 1 = lose; 2 = win
    FILE *fp = NULL;    //pointer for writing file
    time_t t = time(NULL);      //for date
    struct tm tm = *localtime(&t);  //for date
    srand((int)time(NULL)); //initialise seed
    for(i = 0; i <= sizeof(cField); i++)
    {
        cField[i] = 0;
    }
    cField[0] = 9; //Width (9 corresponds to 10)
    cField[1] = 9; //Height (9 corresponds to 10)
    int iButton;

    clock_t cClock;
    int iTimeStart;
    int iSpeed = 800;	//Speed of the game

    //PROGRAM
    do
    {
        //Mainmenu listing
        system("CLS");
        printf(" * CONSOLE SNAKE *\n");
        printf(" ----Mainmenu----\n");
        printf(" 1 : Exit\n");
        printf(" 2 : New Game\n");
        printf(" 3 : Options\n");
        printf(" 4 : Highscores (WIP)\n");
        printf(" -----------------\n");
        printf(" Version: %i.%i.%i\n", V_MAIN, V_SIDE, V_REVISION);
        printf(" Main code: rphii\n");
        printf(" Cleanup & english translation: SamsiFPV\n");

        //Mainmenu input
        do
        {
            cMainmenu = getch();
        }
        while(cMainmenu < '1' || cMainmenu > '4');

        //Mainmenu execution
        switch(cMainmenu)
        {
            case '2': //New game

			    ///Initialise game
                cWinLose = 0;
                cDirectionInput = 0;
                iMoves = 0;
                for(i = 0; i <= sizeof(iFruit); i++)
                {
                    iFruit[i] = 0;
                }
                for(i = 0; i <= FIELD_BMAX; i++)
                {
                    for(k = 0; k <= FIELD_HMAX; k++)
                    {
                        iField[i][k] = 0;
                    }
                }
                ///Game Start
                system("CLS");
                iField[cField[0]/2][cField[1]/2] = 1;  //Snake starting value
                //iFruit[0] = cField[0]/2; //Fruit position X
                //iFruit[1] = cField[1]/2; //Fruit position Y
                iFruit[2] = cField[0]/1; //Snake position Y
                iFruit[3] = cField[1]/2; //Snake position X
                iFruit[4]++;    //Extend snakelength
                /*
                The do while loop repeats the set new fruit
				command as long as the fruit spawns on the snake
                */
                do  //set new fruit
                {
                    iFruit[0] = rand() % (cField[1] + 1);	//pos X
                    iFruit[1] = rand() % (cField[0] + 1);	//pos Y
                }
                while(iField[iFruit[1]][iFruit[0]] != 0); 	//do as long as fruit lands on snake

                iTimeStart = clock();    //CLOCK
                do
                {
                    do
                    {
                        if(kbhit()) //If Keyboard is pressed
                        {
                            iButton = getch();   //Read button
                            //Look for WASD buttons
                            if (iButton == 224 || iButton == 'w' || iButton == 'a' || iButton == 's' || iButton == 'd')      //If WASD is pressed
                            {
                                if(iButton == 224)
                                {
                                    iButton = getch();
                                }
                                if(iButton == 75 || iButton == 'a') //75 = Left
                                {
                                	/*The snake can only go left or right of its current
									direction, so this if loop is to make sure you cannot
									go left when the snake is going right */
                                    if(cDirection != 1) //If the snake isnt going right
                                    {
                                        cDirectionInput = 3; //Set snake directioninput to left
                                    }
                                } 
                                if(iButton == 77 || iButton == 'd') //77 = right
                                {
                                    if (cDirection != 3) //If the snake isnt going left
                                    {
                                        cDirectionInput = 1; //Set snake directioninput to right
                                    }
                                }
                                if(iButton == 72 || iButton == 'w') //72 = up
                            	{
                                    if (cDirection != 2) //If the snake isnt going down
                                    {
                                        cDirectionInput = 0; //Set snake directioninput to up
                                    }
                                }
                                if(iButton == 80 || iButton == 's') //80 = down
                                {
                                    if (cDirection != 0) //If the snake isnt going up
                                    {
                                        cDirectionInput = 2; //Set snake directioninput to down
                                    }
                                }
                            }
                        }
                        if(clock() - iTimeStart >= iSpeed)  //When enough time is over
                        {
                            iTimeStart = clock();           //CLOCK
                            cDirection = cDirectionInput;   //take over direction input
                            
                            if(cDirection == 0) //upwards
                            {
                                iFruit[2]--;				//Change position of the snake
                                if(iFruit[2] < 0)			//If the snake is over the edge
                                {
                                    if(cField[2] == 0)		//If over the edge is not allowed
                                    {
                                        cWinLose = 1;   //Lost
                                    }
                                    else
                                    {
                                    	iFruit[2] = cField[0];	//Set snake to the other edge of the field
                                    }
                                }
                            }
                            if(cDirection == 1) //rightwards
                            {
                                iFruit[3]++;				//Change position of the snake
                                if(iFruit[3] > cField[1])   //If the snake is over the edge
                                {
                                    if(cField[2] == 0)		//If over the edge is not allowed
                                    {
                                        cWinLose = 1;   //Lost
                                    }
                                    else
                                    {
                                    iFruit[3] = 0;				//Set snake to the other edge of the field
                                    }
                                }
                            }
                            if(cDirection == 2) //downwards
                            {
                                iFruit[2]++;                //Change position of the snake
                                if(iFruit[2] > cField[0])   //If the snake is over the edge
                                {
                                    if(cField[2] == 0)		//If over the edge is not allowed
                                    {
                                    	cWinLose = 1;   //Lost
                                    }
                                    else
                                	{
                                        iFruit[2] = 0;   		//Set snake to the other edge of the field
                                	}
                                }
                            }
                            if(cDirection == 3) //leftwards
                            {
                                iFruit[3]--;               	//Change position of the snake
                                if(iFruit[3] < 0)         	//If the snake is over the edge
                                {
                                    if(cField[2] == 0)		//If over the edge is not allowed
                                    {
                                        cWinLose = 1;   //Lost
                                    }
                                	else
                                	{
                                    	iFruit[3] = cField[1];	//Set snake to the other edge of the field
                                	}
                            	}
                            }
                        	//When fruit is picked up
                            if(iFruit[0] == iFruit[3] && iFruit[1] == iFruit[2])    //If there is a fruit at the current pos.
                            {
                            	for(i = 0; i <= cField[0]; i++)   //Vertical
                                {
                                    for(k = 0; k <= cField[1]; k++)   //Horizontal
                                    {
                                        if(iField[k][i] != 0)    //If field has some content
                                        {
                                        	/* When a fruit is picked up, the snake gets longer.
                                        	This is achieved by */
                                            iField[k][i]++;  //Inkrement field
                                        }
                                    }
                            	}
                                iFruit[4]++;    //Increment fruit counter
                            }
                            //Lose: Collision with the snake itself, or move the head
                            if(iField[iFruit[2]][iFruit[3]] >= 2)    //If the head of the snake moves into its body
                            {
                                cWinLose = 1;   //Lose
                            }
                            else
                            {
                            iField[iFruit[2]][iFruit[3]] = iFruit[4];    //place new head infront of the moving direction
                            }
                            //Delete snake / minus 1 each pos.
                            for(i = 0; i <= cField[0]; i++)      //Vertical
                            {
                                for(k = 0; k <= cField[1]; k++)  //Horizontal
                                {
                                    if(iField[i][k] >= 1)
                                    {
                                        iField[i][k]--;  //dekrement
                                    }
                                }
                            }
                            //Fruit collected
                            iField[iFruit[2]][iFruit[3]] = iFruit[4];
                            //New fruit needed
                            while(iField[iFruit[1]][iFruit[0]] != 0 && iFruit[4] != (cField[0] + 1) * (cField[1] + 1)) //As long as fruit lands on snake
                            {
                                iFruit[0] = rand() % (cField[1] + 1);    //pos X
                                iFruit[1] = rand() % (cField[0] + 1);    //pos Y
                            }
                        	//Draw field
                            system("CLS");
                            for(i = 0; i <= (cField[0] + 2); i++)      //Vertical plus 2 because of the borders
                            {
                                for(k = 0; k <= (cField[1] + 2); k++)  //Horizontal plus 2 because of the borders
                                {
                                    if(i == 0 || i == cField[0] + 2)
                                    {
                                        if(cField[1] + 2 != k)
                                        {
                                        printf("%c%c", BORDERH, BORDERH); //horizontal border
                                        }
                                    }
                                    else if(i != 0 && i != cField[0] + 2 && (k == 0 || k == cField[1] + 2))
                                    {
                                    	printf("%c", BORDERV); //vertical border
                                    }
                                    else
                                    {
                                        artk = make_art(iField[i - 1][k - 1], iFruit[1], iFruit[0], i - 1, k - 1, iFruit[4]);   //Current pixel calculation
                                        if(artk == SNAKE_FRUIT)
                                        {
                                            printf("%c%c", artk, artk + 1);         //Output current pixel
                                        }
                                    	else
                                        {
                                            printf("%c%c", artk, artk);         //Output current pixel
                                        }
                                    }
                                }
                                printf("\n");                   //new row
                            }
                            printf("%i / %i\n", iFruit[4], (cField[0] + 1) * (cField[1] + 1));    //Progress
                            iMoves++;
                            if(iFruit[4] >= (cField[0] + 1) * (cField[1] + 1))    //If fieldwidth * fieldheight =< fruit collected
                            {
                                cWinLose = 2;   //Win
                            }
                        }
                    }
                    while (cWinLose == 0); //while not Lost or Won
                    if(cWinLose == 1)   //Lost
                    {
                        printf("LOST!\n");
                    }
                    if(cWinLose == 2)   //Won
                    {
                        printf("WON!\n");
                    }
                    //Game end info
                    printf("Moves: %i\n", iMoves);
                    //Save in .txt
                    fp = fopen("highscores_snake.txt", "a+");
                    if(cWinLose == 1)   //Lost
                    {
                        fprintf(fp, "%04d-%02d-%02d LOST: %3i of %3i, Over the edge: %i, Speed: %4i, Height: %2i, Width: %2i, %4i Moves\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, iFruit[4], (cField[0] + 1) * (cField[1] + 1), cField[2], iSpeed, cField[0] + 1, cField[1] + 1, iMoves);
                    }
                    if(cWinLose == 2)   //Won
                    {
                        fprintf(fp, "%04d-%02d-%02d WON: %3i of %3i, Over th edge: %i, Speed: %4i, Height: %2i, Width: %2i, %4i Moves\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, iFruit[4], (cField[0] + 1) * (cField[1] + 1), cField[2], iSpeed, cField[0] + 1, cField[1] + 1, iMoves);
                    }
                    printf("highscores_snake.txt\n");
                    printf("Mainmenu: ESC\n");

                    fclose(fp);
                    do
                    {
                        iButton = getch();
                    }
                    while(iButton != 27);
                }
                while (iButton != 27); //While game isnt exited (via ESC)
                break;
            case '3': //Optionsmenu
                do
                {
                    //Optionsmenu listing
                    list_menu_options();

                    //Optionsmenu input
                    do
                    {
                        cOptionsmenu = getch();
                    }
                    while (cOptionsmenu < '1' || cOptionsmenu > '3'); //until input is valid

                    //Optionsmenu execution
                    switch (cOptionsmenu)
                    {
                        case '2': //MENU: Field
                            do
                            {
                                system("CLS");
                                list_menu_field(); //List fieldmenu
                                do
                                {
                                cFieldmenu = getch();
                                }
                                while (cFieldmenu < '1' || cFieldmenu > '4'); //until input is valid

                                switch (cFieldmenu)
                                {
                                    case '2': //OPTION: Fieldheight
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field(); //list OPTIONMENU1
                                            //Output current height
                                            printf("[Arrow keys] <Fieldheight: %i>\n", cField[0] + 1);

                                            //Search for arrow keys
                                            iButton = getch();
                                            if (iButton == 224)
                                            {
                                                iButton = getch();
                                                if (iButton == 75) //75 = Left
                                                {
                                                    if (cField[0] > FIELD_HMIN)
                                                    {
                                                        cField[0]--;
                                                    }
                                                }
                                                if (iButton == 77) //77 = Right
                                                {
                                                    if (cField[0] < FIELD_HMAX)
                                                    {
                                                        cField[0]++;
                                                    }
                                                }
                                            }
                                            //to exit optionmenu 1
                                            if(iButton == '1' || iButton == '3' || iButton == '4')
                                            {
                                                cFieldmenu = iButton;
                                            }
                                        }
                                        while (cFieldmenu == '2' && cFieldmenu != '1');
                                        iButton = 0;
                                        break;
                                    case '3': //OPTION: Fieldwidth
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field(); //list optionmenu 1
                                            //Output current width
                                            printf("[Arrow keys] <Fieldwidth: %i>\n", cField[1] + 1);

                                            //Search for arrow keys
                                            iButton = getch();
                                            if (iButton == 224)
                                            {
                                                iButton = getch();
                                                if (iButton == 75) //75 = Left
                                                {
                                                    if (cField[1] > FIELD_BMIN)
                                                    {
                                                        cField[1]--;
                                                    }
                                                }
                                                if (iButton == 77) //77 = Right
                                                {
                                                    if (cField[1] < FIELD_BMAX)
                                                    {
                                                        cField[1]++;
                                                    }
                                                }
                                            }
                                            //to exit optionmenu 1
                                            if(iButton == '1' || iButton == '2' || iButton == '4')
                                            {
                                                cFieldmenu = iButton;
                                            }
                                        }
                                        while (cFieldmenu == '3' && cFieldmenu != '1');
                                        iButton = 0;
                                        break;
                                    case '4': //OPTION: "Over the edge"
                                        do
                                        {
                                            system("CLS");
                                            list_menu_field(); //List optionmenu 1
                                            //Output Current state of "Over the edge"
                                            if (cField[2] == 0)
                                            {
                                                printf("[ARROW KEYS] <%i: NO>\n", cField[2]);
                                            }
                                            if (cField[2] == 1)
                                            {
                                                printf("[ARROW KEYS] <%i: YES>\n", cField[2]);
                                            }

                                            //Search for arrow keys
                                            iButton = getch();
                                            if (iButton == 224)
                                            {
                                                iButton = getch();
                                                if (iButton == 75 || iButton == 77)
                                                {
                                                    cField[2] ^= 1;
                                                }
                                            }

                                            //to exit fieldmenu
                                            if(iButton == '1' || iButton == '2' || iButton == '3')
                                            {
                                                cFieldmenu = iButton;
                                            }
                                        }
                                        while (cFieldmenu == '4' && cFieldmenu != '1');
                                        iButton = 0;
                                        break;
                                    default:
                                        break;
                                }
                            } while(cOptionsmenu == '2' && cFieldmenu != '1');
                            break;
                        case '3': //MENU: Speed
                            do
                            {
                                //Output current settings
                                system("CLS");
                                list_menu_options();
                                printf("[ARROW KEYS] <Speed: %i>\n", iSpeed);

                                //search for arrow keys
                                iButton = getch();
                                if(iButton == 224)
                                {
                                    iButton = getch();
                                    if(iButton == 75) //75 = Left
                                    {
                                        if(iSpeed > SPEED_MIN)
                                        {
                                            iSpeed -= SPEED_STEP;
                                        }
                                    }
                                    if(iButton == 77) //77 = Right
                                    {
                                        if(iSpeed < SPEED_MAX)
                                        {
                                            iSpeed += SPEED_STEP;
                                        }
                                    }
                                }
                                //to exit optionmenu
                                if(iButton == '1' || iButton == '2')
                                {
                                    cOptionsmenu = iButton;
                                }
                            } while(cOptionsmenu == '3' && cOptionsmenu != '1');
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
    } while(cMainmenu != '1'); //USELESS??

    return 0;
}

int list_menu_options(void)	//Optionmenu listing
{
    system("CLS");
    printf("----Optionsmenu----\n");
    printf("1 : Back\n");
    printf("2 : Field\n");
    printf("3 : Speed\n\n");
}
int list_menu_field(void)	//Fieldmenu listing
{
    printf("----Field settings----\n");
    printf("1 : Back\n");
    printf("2 : Fieldheight\n");
    printf("3 : Fieldwidth\n");
    printf("4 : Over the edge\n\n");
}
int make_art(int iFieldpr, int iFieldFruitY, int iFieldFruitX, int pos_y, int pos_x, int iFruits) //constructs the playground char by char
{
    //INTITALISE / DEKLARISE
    int i = 0;                  //Countervariable (Horizontal)
    int iArtpr = 0;             //"Charactervariable"
    //CODE
    //Make ART
    if(iFieldpr == iFruits) 	//When the value for the head is at the current pixel
    {
        iArtpr = SNAKE_HEAD; 	//Return the pixel of the snakehead
    }
    else if(pos_y == iFieldFruitY && pos_x == iFieldFruitX) //When a fruit is at the current pixel
    {
        iArtpr = SNAKE_FRUIT;	//Set fruit
    }
    else if(iFieldpr == 0) 		//When no value is at the current pixel
    {
        iArtpr = SNAKE_OFF;   	//Set value to zero
    }
    else if(iFieldpr >= 1)   	//When the value for the body is at the current pixel
    {
        iArtpr = SNAKE_BODY;    //Return pixel for the snake body
    }
    return(iArtpr); 			//Return field borders
}
