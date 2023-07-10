#include "checkScore.h"

int size = BEGGINER;

int main(int agrc, char* argv[]){
    uint8_t board[9][9] =   {     
                                {0,1,2,0,0,1,2,0,0},
                                {1,0,2,0,0,0,1,0,0},
                                {2,2,2,0,0,0,1,1,1},
                                {0,0,0,0,0,1,2,2,2},
                                {0,0,0,0,0,1,2,1,0},
                                {2,2,2,2,0,1,2,1,2},
                                {0,0,0,2,0,1,2,1,0},
                                {0,1,0,2,0,1,2,2,2},
                                {0,0,0,2,0,0,1,1,1}
                            };
    //board = (Board) board;
    printBoard(board);
    return 0;
}

void calculateScore(int* player1, int* player2, uint8_t board[9][9]){
    //check for atari
    //all shapes with 7 or more interior spaces are alive
    //"liberty shortage"
    //  pieces have no liberties due to suicide

    //check for seki
}

void printBoard(uint8_t board[9][9]){
    for(int row = 0; row < size; row++){
        printf("%s%d", TEXT_FONT, size-row);
        for(int column = 0; column < size; column++){
            //Print pieces
            if( board[size-row-1][column] == 1)           printf("%s", BLACK_CIRCLE); //black is player 1
            else if( board[size-row-1][column] == 2)      printf("%s", WHITE_CIRCLE); //white is player 2
            //Print blank corners
            else if(row == 0 && column == 0)            printf("%s", TOP_LEFT);
            else if(row == 0 && column == size-1)       printf("%s", TOP_RIGHT);
            else if(row == size-1 && column == 0)       printf("%s", BOTTOM_LEFT);
            else if(row == size-1 && column == size-1)  printf("%s", BOTTOM_RIGHT);
            //Print blank sides
            else if(row == 0)                           printf("%s", TOP);
            else if(row == size-1)                      printf("%s", BOTTOM);
            else if(column == 0)                        printf("%s", LEFT);
            else if(column == size-1)                   printf("%s", RIGHT);
            //Print blank internal spaces
            else                                        printf("%s", INTERSECTION);
        }
        printf("%s", CLEAR_FONT);
        printf("\n");
    }
    printf("%s ", TEXT_FONT);
    for(int column=0; column < size; column++){
        printf("%d", column+1);
    }
    printf("%s\n", CLEAR_FONT);
}