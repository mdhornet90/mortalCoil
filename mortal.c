#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct path
{
    int horizX;
    int vertY;
    int direction[4];
    int startPos;
    int curPos; //Current position
    char *board;
};

int main(void)
{
    //Prototypes
    void addBorder(struct path* arg);
    void printBoard(struct path *arg);
    //Variables
    struct path level;
    int area;

    printf("Receiving board...\n");
    scanf("%i %i", &level.horizX, &level.vertY);
    area = level.horizX * level.vertY;

    level.board = (char*)malloc(area * sizeof(char));
    scanf("%s", level.board);

    level.direction[0] = 1;
    level.direction[1] = level.horizX;
    level.direction[2] = -1;
    level.direction[3] = -level.horizX;

    printf("Before adding border:\n\n");
    printBoard(&level);

    addBorder(&level);

    printf("After adding border:\n\n");
    printBoard(&level);

    return 0;
}



void addBorder(struct path* arg)
{
    struct path *tmp = arg;

    tmp->horizX += 2;
    tmp->vertY += 2;
    int newArea = tmp->horizX * tmp->vertY;
    char *tempBoard = (char *)malloc(sizeof(char) * newArea);
    int i, j, k = 0, l;

    for ( i = 0; i < (tmp->horizX); i++ )
        tempBoard[i] = 'X';
    for( j = 0; j < tmp->vertY - 2; i++, j++ )
    {
        tempBoard[i] = 'X';
        i++;
        for( l = 0; l < tmp->horizX - 2; i++, k++, l++ )
            tempBoard[i] = arg->board[k];
        tempBoard[i] = 'X';
    }
    for ( i; i < newArea; i++ )
        tempBoard[i] = 'X';

    free(tmp->board);
    tmp->board = (char *)malloc(sizeof(char) * newArea);
    tmp->board = tempBoard;

    return;
}

void printBoard(struct path *arg)
{
    int i = 0, j;
    struct path *tmp = arg;

    for( j = 0; j < tmp->vertY; j++ )
    {
        printf("    ");
        for( i; i < tmp->horizX * (j + 1); i++ )
            printf("%c ", tmp->board[i]);
        printf("\n");
    }
    printf("\n");

    return;
}
