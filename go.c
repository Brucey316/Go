# include "go.h"

int running;
Board board;

int main(int argc, char* argv[]){
    int size = TEST;
    createBoard(size);
    for(int turns = 0; running == 0; turns++ ){
        printBoard(size, board);
        makeMove(turns, board, size);
        compressBoardState(board, size);
    }
    destroyBoard(size);
    return 0;
}
uint16_t getUserInput(){
    char input[10];
    int column, row;
    printf("Make a move (<column #> <row#>):");
    fgets(input, sizeof(input), stdin);
    char* token = strtok(input, " ");
    column =  atoi(token) - 1;
    token = strtok(NULL, " ");
    if(token == NULL)
        row = -1; //this will trigger
    else
        row =  atoi(token) - 1;
     printf("Input: %u,%u\n", column, row);
     fflush(stdin);
    return (column << 8) + row;
}
void makeMove(int turn, Board board, int size){
    uint16_t temp = getUserInput();//my attempt at returning two variables in C
    uint8_t column = (temp & 0xFF00) >> 8;
    uint8_t row = temp & 0xFF;
    printf("Make move: %u,%u\n", column, row);

    while(!isValidMove(row,column,board, size)){
        printf("ERROR: Move was invalid\n");
        uint16_t temp = getUserInput();
        uint8_t column = (temp & 0xFF00) >> 8;
        uint8_t row = temp & 0xFF;
        printf("Make move: %u,%u\n", column, row);
    }
    board[row][column] = turn%2+1;
}
int isValidMove(int row, int column, Board board, int size){
    //check if index of row and column is within bounds
    if(row < 0 || row >= size || column < 0 || column >= size) return 0;
    //make sure space is empty
    if(board[row][column] != 0) return 0;
    //else return true
    return 1;
}
void createBoard(int size){
    //allocate memory for the board
    board = (uint8_t**) malloc(sizeof(uint8_t*)*size);
    for(int i = 0; i < size; i++){
        board[i] = (uint8_t*) malloc(sizeof(uint8_t)*size);
    }
}
void destroyBoard(int size){
    //free allocated memory for board
    for(int i = 0; i < size; i++){
        free(board[i]);
    }
    free(board);
}
void printBoard(int size, Board board){
    for(int row = 0; row < size; row++){
        printf("%s", TEXT_FONT);
        for(int column = 0; column < size; column++){
            //Print pieces
            if(board[size-row-1][column] == 1)           printf("%s", BLACK_CIRCLE); //black is player 1
            else if(board[size-row-1][column] == 2)      printf("%s", WHITE_CIRCLE); //white is player 2
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
}
/*Function: Compress Board State
    Params:
        Board board: the board to be compressed
        int size: the size of the board
    Return:
        CompressedBoard: this is the bits of a board compressed
    Since a board position can be represented in 2 bits, the board state is converted
    into each position being only two bits and stored in large 64bit unsigned integers
    for easy iteration and easy == comparison
*/
CompressedBoard compressBoardState(Board board, int size){
    //printf("Compression: %d\n", ((int)ceil((size*size)/32.0)));
    //initialize the size of the board to hold all the positions
    CompressedBoard cb = (uint64_t*) malloc(sizeof(uint64_t) * (int) ceil( (size*size)/32.0 ));
    //set the amount of bytes read in to 0
    int bytes = 0;
    //iterate through every position
    for (int row = 0; row < size; row++) {
        for(int column = 0; column < size; column++){
            //fancy math to store bytes into array
            //int floor division is to get the index in the array to store the bytes
            //&3 gets the least 2 significant bits that store the position's state
            //shift alligns the 2 bits into the 64bit unsigned int according to their position on the board
            cb[(int)floor(bytes/32)] += ((uint64_t)(board[row][column] & 0x3)) << ((32 - (bytes % 32))*2-2);
            bytes++;
        }
    }
    /*
    for(int i = 0; i < ((int)ceil((size*size)/32.0)); i++){
        printf("%d: %llu\n",i, cb[i]);
        printf("%d: %llX\n", i, cb[i]);
    }
    */
    return cb;
}