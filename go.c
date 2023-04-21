# include "go.h"

int main(int argc, char* argv[]){
    printf("%s\n", INTERSECTION);
    printf("%s %s %s %s\n", TOP, RIGHT, BOTTOM, LEFT);
    printf("%s %s %s %s\n", TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT);
    printf("%s %s\n", WHITE_CIRCLE, BLACK_CIRCLE);
    return 0;
}
void printBoard(int size){
    
    for(int row = 0; row < size; row++){
        printf("%s", TEXT_FONT);
        for(int column = 0; column < size; column++){
            //Print pieces
            if(row == 7 && column == 2)                 printf("%s", WHITE_CIRCLE);
            else if(row == 2 && column == 7)            printf("%s", BLACK_CIRCLE);
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
    printf("%s", CLEAR_FONT);
}