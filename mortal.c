#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct path
{
    struct path *prev;
    bool leftRight[2];
    bool upDown[2];
    int position;
    int spacesMoved;
    struct path *next;
};

struct level
{
    char *board;
    int numberSolved;
    int curPosition;
    int size;
    int totalSolved;
    int x;
    int y;
    struct path traversal;
};

int main(void)
{
    int i, X = 0;
    void solver(struct level *arg);
    struct level progLevel;
    char temp[100];

    scanf("%i %i %s", &progLevel.x, &progLevel.y, temp);

    progLevel.board = temp;
    progLevel.curPosition = 0;
    progLevel.totalSolved = 0;
    progLevel.traversal.prev = 0;
    progLevel.traversal.next = 0;
    progLevel.traversal.upDown[0] = 0;
    progLevel.traversal.leftRight[0] = 0;

    for ( i = 0; i < (progLevel.x * progLevel.y); i++ )
        if ( progLevel.board[i] == 'X' )
            X++;

    progLevel.size = (progLevel.x * progLevel.y) - X;

    return 0;
}

void solver(struct level *arg)
{
    bool testUp(struct level *arg);
    void newPath(struct path *arg);
    struct level *tmp = arg;
    while(1)
    {
        while( tmp->board[tmp->curPosition] == 'X' )
            tmp->curPosition++;
        if( testUp(tmp) )
        {
            newPath(*(tmp->traversal));
            while( testUp(tmp) )
            {
            }
        }
    }
}

bool testUp(struct level *arg)
{
    struct level *tmp = arg;
    return true;
}

void newPath(struct path *arg)
{
    struct path *tmp = arg;
    struct path *newEntry = (struct path *) malloc(sizeof(struct path));

    tmp->next = newEntry;
    newEntry->prev = tmp;
    newEntry->next = 0;
}
