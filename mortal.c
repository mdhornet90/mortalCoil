#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int horizX;
    int vertY;
    int direction[4];
    int usedSpacesCounter;
    int curPos;
    int lastDir;
    int spacesLeft;
    int *directionsUsed;
    int firstMove;
    char *board;
}path;

int main(void)
{
    //Prototypes
    void initBoard(path* arg);
    void printBoard(path *arg);
    void solver(path *arg);
    //Variables
    path level, reset;
    int x, y, i;
    int area, boardStartFlag = 0;
    FILE *filePtr;

    system("curl -s \"http://www.hacker.org/coil/?name=Toddler%20Stomper&spw=e2f5b99f7599f74c83eb5b18aeb9bbf1\" | grep -m 1 \"FlashVars\" | grep -o \"x[^\\\"]*\" > ~/Desktop/Programming/C/MortalCoil/test.txt");
    filePtr = fopen("test.txt","rb");
    printf("Receiving board...\n");
    fscanf(filePtr,"x=%i&y=%i", &level.horizX, &level.vertY); //Takes its inputs from the command curl -s "http://www.hacker.org/coil/?name=Toddler%20Stomper&spw=e2f5b99f7599f74c83eb5b18aeb9bbf1"
                                                 //| grep -m 1 "FlashVars" | grep -o "x[^\"]*"| sed "s/[^0-9.X]\+/ /g"|./mortal |
                                                 //curl -s "http://www.hacker.org/coil/?name=Toddler%20Stomper&spw=e2f5b99f7599f74c83eb5b18aeb9bbf1&`cat`" | grep "Level: [0-9]\+" -o
    area = level.horizX * level.vertY;

    level.board = (char*)malloc(area * sizeof(char)); //Allocate enough spaces for initial board (to be overwritten later)
    fscanf(filePtr,"&board=%s", level.board);
    initBoard(&level);
    level.directionsUsed = (int *)malloc(level.spacesLeft * sizeof(int));
    printf("\nInitial Board:\n");
    printBoard(&level);

    level.direction[0] = 1; //Defining directions
    level.direction[1] = level.horizX;
    level.direction[2] = -1;
    level.direction[3] = -level.horizX;

    area = level.horizX * level.vertY;

    memcpy(&reset, &level, sizeof(path)); //Deep copy
    reset.board = (char *)malloc(area * sizeof(char));
    strcpy(reset.board, level.board);

    while( level.curPos < (level.horizX * level.vertY) ) //Will exit loop when path has been completely traversed,
    {                                                    //or when all possible starting points have been exhausted
        while(level.board[level.curPos] == 'X' || level.board[level.curPos] == '-') //Finds its first starting position the first time through
            level.curPos++;

        level.lastDir = 0;
        printf("\n Starting Position: %i\n", level.curPos);
        solver(&level);
        if( level.spacesLeft == 0 )
            break;
        else
            strcpy(level.board, reset.board);
        level.lastDir = 1;
        solver(&level);
        if( level.spacesLeft == 0 )
            break;
        else
            strcpy(level.board, reset.board);
        level.curPos++;
    }

    level.board[level.curPos] = '*';
    printf("\n\nFinal Board: \n");
    printBoard(&level);

    x = 0;

    for( y = 0; y < level.vertY; y++ )
    {
        for( ; x < ( level.horizX * (y + 1) ); x++ )
            if( level.board[x] == '*' )
            {
                boardStartFlag = 1;
                break;
            }
        if( boardStartFlag )
            break;
    }

    x = x % level.horizX - 1;
    y--;

    printf("http://www.hacker.org/coil/index.php?name=Toddler%%20Stomper&spw=e2f5b99f7599f74c83eb5b18aeb9bbf1&path=");
    for ( i = level.usedSpacesCounter -1; i > 0; i-- )
        switch( level.directionsUsed[i] )
        {
            case 0:
                printf("R");
                break;
            case 1:
                printf("D");
                break;
            case 2:
                printf("L");
                break;
            case 3:
                printf("U");
                break;
            default:
                printf(" ");
        }
        printf("&x=%i&y=%i\n",x,y);

    return 0;
}

void solver(path *arg)
{
    //Prototype
    void printBoard(path* arg);
    //variables
    path tempLevel, *tmp = arg;
    int area = arg->horizX * arg->vertY, firstTime = 1, done = 0;

    memcpy(&tempLevel, arg, sizeof(path));
    tempLevel.board = (char *)malloc(area * sizeof(char));
    strcpy(tempLevel.board,arg->board);

    if( tempLevel.lastDir == 0 || tempLevel.lastDir == 2 )
    {
        while ( done <= 2 )
        {
            if( tempLevel.board[tempLevel.curPos + tempLevel.direction[1]] == '.' )
            {
                tempLevel.board[tempLevel.curPos] = '.';

                while( tempLevel.board[tempLevel.curPos] == '.' )
                {
                    if ( !firstTime || tempLevel.firstMove )
                        tempLevel.spacesLeft--;
                    if ( tempLevel.firstMove )
                    {
                        tempLevel.firstMove = 0;
                        tempLevel.board[tempLevel.curPos] = '*';
                    }
                    else
                        tempLevel.board[tempLevel.curPos] = '-';
                    firstTime = 0;
                    tempLevel.curPos += tempLevel.direction[1];
                }
                tempLevel.curPos += tempLevel.direction[3];
                tempLevel.lastDir = 1;
                printBoard(&tempLevel);
                solver(&tempLevel);
            }
            done++;
            if( tempLevel.board[tempLevel.curPos + tempLevel.direction[3]] == '.' )
            {
                tempLevel.board[tempLevel.curPos] = '.';
                while( tempLevel.board[tempLevel.curPos] == '.' )
                {
                    if ( !firstTime || tempLevel.firstMove )
                        tempLevel.spacesLeft--;
                    if ( tempLevel.firstMove )
                    {
                        tempLevel.firstMove = 0;
                        tempLevel.board[tempLevel.curPos] = '*';
                    }
                    else
                        tempLevel.board[tempLevel.curPos] = '-';
                    firstTime = 0;
                    tempLevel.curPos += tempLevel.direction[3];
                }
                tempLevel.curPos += tempLevel.direction[1];
                tempLevel.lastDir = 3;
                printBoard(&tempLevel);
                solver(&tempLevel);
            }
            done++;
        }
    }
    else if( tempLevel.lastDir == 1 || tempLevel.lastDir == 3 )
    {
        done = 0;
        while ( done <= 2 )
        {
            if( tempLevel.board[tempLevel.curPos + tempLevel.direction[0]] == '.' )
            {
            tempLevel.board[tempLevel.curPos] = '.';

                while( tempLevel.board[tempLevel.curPos] == '.' )
                {
                    if ( !firstTime || tempLevel.firstMove )
                        tempLevel.spacesLeft--;
                    if ( tempLevel.firstMove )
                    {
                        tempLevel.firstMove = 0;
                        tempLevel.board[tempLevel.curPos] = '*';
                    }
                    else
                        tempLevel.board[tempLevel.curPos] = '-';
                    firstTime = 0;
                    tempLevel.curPos += tempLevel.direction[0];
                }
                tempLevel.curPos += tempLevel.direction[2];
                tempLevel.lastDir = 0;
                printBoard(&tempLevel);
                solver(&tempLevel);
            }
            done++;
            if( tempLevel.board[tempLevel.curPos + tempLevel.direction[2]] == '.' )
            {
                tempLevel.board[tempLevel.curPos] = '.';
                while( tempLevel.board[tempLevel.curPos] == '.' )
                {
                    if ( !firstTime || tempLevel.firstMove )
                        tempLevel.spacesLeft--;
                    if ( tempLevel.firstMove )
                    {
                        tempLevel.firstMove = 0;
                        tempLevel.board[tempLevel.curPos] = '*';
                    }
                    else
                        tempLevel.board[tempLevel.curPos] = '-';
                    firstTime = 0;
                    tempLevel.curPos += tempLevel.direction[2];
                }
                tempLevel.curPos += tempLevel.direction[0];
                tempLevel.lastDir = 2;
                printBoard(&tempLevel);
                solver(&tempLevel);
            }
            done++;
        }
    }
    if ( tempLevel.spacesLeft == 0 )
    {
        tempLevel.directionsUsed[tempLevel.usedSpacesCounter] = tempLevel.lastDir;
        tmp->directionsUsed[tempLevel.usedSpacesCounter] = tempLevel.directionsUsed[tempLevel.usedSpacesCounter];
        tempLevel.usedSpacesCounter++;
        tmp->usedSpacesCounter = tempLevel.usedSpacesCounter;
        tmp->spacesLeft = 0;
    }
    //tmp->board[tmp->curPos] = '.';
    printf("\nUndoing...current position is %i...\n", tempLevel.curPos);
    free(tempLevel.board);
    return;
}

void initBoard(path* arg)
{
    path *tmp = arg;
    tmp->spacesLeft = 0;
    tmp->curPos = 0;
    tmp->usedSpacesCounter = 0;
    tmp->firstMove = 1;

    tmp->horizX += 2;                                   //Set new board dimensions
    tmp->vertY += 2;
    int newArea = tmp->horizX * tmp->vertY;
    char *tempBoard = (char *)malloc(sizeof(char) * newArea); //temporary board
    int i, j, k = 0, l;

    for ( i = 0; i < (tmp->horizX); i++ ) //All X's as the top row
        tempBoard[i] = 'X';

    for( j = 0; j < tmp->vertY - 2; i++, j++ ) //Interweave original board with more X's as border
    {
        tempBoard[i] = 'X';
        i++;
        for( l = 0; l < tmp->horizX - 2; i++, k++, l++ )
            tempBoard[i] = arg->board[k];
        tempBoard[i] = 'X';
    }

    for (; i < newArea; i++ ) //All X's as the bottom row
        tempBoard[i] = 'X';

    free(tmp->board); //Prevent memory leaks
    tmp->board = (char *)malloc(sizeof(char) * newArea); //Resize board
    tmp->board = tempBoard; //Move new board in

    for ( i = 0; i< newArea; i++ ) //Count the initial number of free spaces the program will have to traverse
        if( tmp->board[i] == '.' )
            tmp->spacesLeft++;

    return;
}

void printBoard(path *arg)
{
    int i = 0, j;
    path *tmp = arg;

    printf("\n");

    for( j = 0; j < tmp->vertY; j++ ) //Parsing the board for printing so it appears as a neat 2D array
    {
        printf("    ");
        for(; i < tmp->horizX * (j + 1); i++ )
            printf("%c ", tmp->board[i]);
        printf("\n");
    }
    return;
}
