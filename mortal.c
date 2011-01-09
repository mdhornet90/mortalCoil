#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int horizX;
    int vertY;
    int direction[4];
    int curPos;
    int lastDir;
    int spacesLeft;
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
    int area;

    printf("Receiving board...\n");
    scanf("%i %i", &level.horizX, &level.vertY); //Takes its inputs from the command curl -s http://www.hacker.org/coil/ |
                                                 //grep -m 1 "FlashVars" | grep -o "x[^\"]*"| sed "s/[^0-9.X]\+/ /g"|./mortal
    area = level.horizX * level.vertY;

    level.board = (char*)malloc(area * sizeof(char)); //Allocate enough spaces for initial board (to be overwritten later)
    scanf("%s", level.board);

    level.direction[0] = 1; //Defining directions
    level.direction[1] = level.horizX;
    level.direction[2] = -1;
    level.direction[3] = -level.horizX;

    printBoard(&level);
    initBoard(&level);
    area = level.horizX * level.vertY;

    memcpy(&reset, &level, sizeof(path)); //Deep copy
    reset.board = (char *)malloc(area * sizeof(char));
    strcpy(reset.board, level.board);

    while( level.curPos < (level.horizX * level.vertY) ) //Will exit loop when path has been completely traversed,
    {                                         //or when all possible starting points have been exhausted
        while(level.board[level.curPos] == 'X') //Finds its first starting position the first time through
            level.curPos++;

        level.lastDir = 0;
        solver(&level);
        if( level.spacesLeft == 0 )
            break;
        else
        {
            /*memcpy(&level, &reset, sizeof(path));
            free(level.board);
            level.board = (char *)malloc(area * sizeof(char));*/
            strcpy(level.board, reset.board);
        }
        level.lastDir = 1;
        solver(&level);
        if( level.spacesLeft == 0 )
            break;
        else
        {
            /*memcpy(&level, &reset, sizeof(path));
            free(level.board);
            level.board = (char *)malloc(area * sizeof(char));*/
            strcpy(level.board, reset.board);
        }
        level.curPos++;
    }

    return 0;
}

void solver(path *arg)
{
    path tempLevel;
    int area = arg->horizX * arg->vertY;

    memcpy(&tempLevel, arg, sizeof(path));
    tempLevel.board = (char *)malloc(area * sizeof(char));
    strcpy(tempLevel.board,arg->board);

    if( tempLevel.lastDir == 0 || tempLevel.lastDir == 2 )
    {
        if ( tempLevel.board[tempLevel.curPos] + tempLevel.direction[1] != 'X' )
        {
            while ( tempLevel.board[tempLevel.curPos] != 'X' )
            {
                tempLevel.board[tempLevel.curPos] = '-';
                tempLevel.curPos += tempLevel.direction[1];
                tempLevel.spacesLeft--;
            }
            tempLevel.curPos += tempLevel.direction[3];
            tempLevel.lastDir = 1;
            solver(&tempLevel);
        }
        else if ( tempLevel.board[tempLevel.curPos] + tempLevel.direction[3] != 'X' )
        {
            while ( tempLevel.board[tempLevel.curPos] != 'X' )
            {
                tempLevel.board[tempLevel.curPos] = '-';
                tempLevel.curPos += tempLevel.direction[3];
                tempLevel.spacesLeft--;
            }
            tempLevel.curPos += tempLevel.direction[1];
            tempLevel.lastDir = 3;
            solver(&tempLevel);
        }
    }

    else if( tempLevel.lastDir == 1 || tempLevel.lastDir == 3 )
    {
        if ( tempLevel.board[tempLevel.curPos] + tempLevel.direction[0] != 'X' )
        {
            while ( tempLevel.board[tempLevel.curPos] != 'X' )
            {
                tempLevel.board[tempLevel.curPos] = '-';
                tempLevel.curPos += tempLevel.direction[0];
                tempLevel.spacesLeft--;
            }
            tempLevel.curPos += tempLevel.direction[2];
            tempLevel.lastDir = 0;
            solver(&tempLevel);
        }
        else if ( tempLevel.board[tempLevel.curPos] + tempLevel.direction[2] != 'X' )
        {
            while ( tempLevel.board[tempLevel.curPos] != 'X' )
            {
                tempLevel.board[tempLevel.curPos] = '-';
                tempLevel.curPos += tempLevel.direction[2];
                tempLevel.spacesLeft--;
            }
            tempLevel.curPos += tempLevel.direction[0];
            tempLevel.lastDir = 2;
            solver(&tempLevel);
        }
    }
    free(tempLevel.board);
    return;
}

void initBoard(path* arg)
{
    path *tmp = arg;
    tmp->spacesLeft = 0;
    tmp->curPos = 0;

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

    for ( i; i < newArea; i++ ) //All X's as the bottom row
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

    for( j = 0; j < tmp->vertY; j++ ) //Parsing the board for printing so it appears as a neat 2D array
    {
        printf("    ");
        for( i; i < tmp->horizX * (j + 1); i++ )
            printf("%c ", tmp->board[i]);
        printf("\n");
    }
    printf("\n");

    return;
}
