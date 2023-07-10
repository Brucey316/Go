# include "engine.h"

int running;
Board board;
int size;
int turn;
int total_pieces;
struct PastBoards history;


int main(int argc, char* argv[]){
    size = BEGGINER;
    //create the board
    createBoard(size);
    //initialize the history structure
    //store a list of states by the amount of pieces on the board
    history.states = (CompressedBoard**) malloc(sizeof(CompressedBoard*)*size*size);
    //store a list of sizes for the lists that are stored
    history.sizes = (int*) malloc(sizeof(int) * size * size);
    //set all state sizes to 0
    memset(history.sizes,0,size*size);
    //basic game loop, iterate turns while game is running
    for(turn = 0; running == 0; turn++ ){
        printBoard();
        makeMove();
        compressBoardState();
    }
    //clean up board
    //TODO: clean up history
    destroyBoard(size);
    return 0;
}

/*  Function: Get User Input
        Param: void
        Return:
            uint16_t: combination of column and row values. 
                        column takes upper 8 bits and row takes lower 8
                        (number returned is adjusted to align with indices and not ui board row/column)
    This function asks user for input on where they would like to make a move
*/
void getUserInput(int* row, int* column){
    char input[10];

    printf("Make a move (<column #> <row#>):");
    fgets(input, sizeof(input), stdin);

    char* token = strtok(input, " ");
    *column =  atoi(token) - 1;
    token = strtok(NULL, " ");

    if(token == NULL)
        *row = -1; //this will trigger
    else
        *row =  atoi(token) - 1;

    printf("Input: %d,%d\n", *column, *row);
    fflush(stdin);
}

/*  Function: Make Move
        Params:
            int turn: number of turns made (determine's whose turn it is)
            Board board: board state before move
            int size: size of board
        Return: void
    Askes user for input on where they want to make a move and then
    updates the board if it is a valid move
*/
void makeMove(){
    int row = -1, column = -1;
    getUserInput(&row, &column);//my attempt at returning two variables in C
    printf("Make move: %d,%d\n", column, row);

    while(!isValidMove(row,column)){
        printf("ERROR: Move was invalid\n");
        getUserInput(&row, &column);
        printf("Make move: %d,%d\n", column, row);
    }
    printf("Move made\n");
    board[row][column] = turn%2+1;
    total_pieces++;
}

/*  Function: Is Valid Move?
        Params:
            int row: the row index of the board
            int column: the column index of the board
            Board board: the board state
            int size: the size of the board
        Return:
            int: whether or not the move is valid
*/
int isValidMove(int row, int column){
    //check if index of row and column is within bounds
    if(row < 0 || row >= size || column < 0 || column >= size){printf("OUT OF BOUNDS\n"); return 0;}
    //make sure space is empty
    if(board[row][column] != 0){printf("TAKEN\n"); return 0;}

    //temporarily make move
    board[row][column] = turn%2+1;
    total_pieces++;
    
    //----- Check can capture in all orthagonal directions-----
    printf("checking for capture\n");
    //  if adjacent piece is an opponent check if it no liberties
    // if no liberties found, flip all captured pieces
    uint8_t** captured = malloc(1);
    int captured_size = 0;
    int prev_size = 0;

    uint8_t player = turn%2+1;
    uint8_t opponent = player ^ 3;

    if(column-1 >= 0 && board[row][column-1] == opponent){
        //if not capturable, remove visited from list and clear flags
        if( !capture(row, column-1, opponent, captured, &captured_size)){
            //unmark the ignored pieces
            clearFlags(4, captured+prev_size, captured_size-prev_size);
            //'free' the pieces that were visited
            captured = realloc(captured, prev_size);
            //reset captured size
            captured_size = prev_size;
        }
        else prev_size = captured_size;
    }
    if(column+1 < size && board[row][column+1] == opponent){
        //if not capturable, remove visited from list and clear flags
        if( !capture(row, column+1, opponent, captured, &captured_size) ){ 
            //unmark the ignored pieces
            clearFlags(4, captured+prev_size, captured_size-prev_size);
            //'free' the pieces that were visited
            captured = realloc(captured, prev_size);
            //reset captured size
            captured_size = prev_size;
        }
        else prev_size = captured_size;
    }
    if(row-1 >= 0 && board[row-1][column] == opponent) {
        //if not capturable, remove visited from list and clear flags
        if( !capture(row-1, column, opponent, captured, &captured_size)){
            //unmark the ignored pieces
            clearFlags(4, captured+prev_size, captured_size-prev_size);
            //'free' the pieces that were visited
            captured = realloc(captured, prev_size);
            //reset captured size
            captured_size = prev_size;
        }
        else prev_size = captured_size;
    }
    if(row+1 < size && board[row+1][column] == opponent){
        //if not capturable, remove visited from list and clear flags
        if( !capture(row+1, column, opponent, captured, &captured_size)){
            //unmark the ignored pieces
            clearFlags(4, captured+prev_size, captured_size-prev_size);
            //'free' the pieces that were visited
            captured = realloc(captured, prev_size);
            //reset captured size
            captured_size = prev_size;
        }
    }
    printf("done capturing\n");
    if(captured_size > 0){
        printf("some captured\n");
        //'capture' the pieces
        flipPieces(opponent, captured, captured_size);
        //clear the flag
        clearFlags(4, captured, captured_size);
    }
    else{
        printf("none captured\n");
        free(captured);
    }

    //--------- check for suicides if no pieces captured -------------
    printf("Checking suicide\n");
    if(captured_size == 0){
        //Check suicide
        uint8_t** self = malloc(1);
        int self_size = 0;
        int capturable;
        if((capturable = capture(row, column, player, self, &self_size))){
            printf("SUICIDE!!!!\n");
            clearFlags(4, self, self_size);
        }
        free(self);
        //if suicide return 0
        if(capturable){
            board[row][column] = 0;
            total_pieces--;
            return 0;
        } 
    }
    printf("Checking Ko\n");
    //-------------- check ko rule -----------------
    CompressedBoard temp = compressBoardState();
    printf("TOTAL PIECES: %d\n", total_pieces);
    int ko = checkKo(temp);
    printf("KO: %d\n", checkKo(temp));

    //if board state is reoccuring of past...
    if(ko == 0){
         printf("Violates Ko\n");
        if(captured_size > 0){
            flipPieces(opponent, captured, captured_size);
            free(captured);
            board[row][column] = 0;
            total_pieces--;
            return 0;
        }
    }
    printf("saving board state\n");
    //save board state to history
    history.sizes[total_pieces]++;
    history.states[total_pieces] = realloc(history.states,history.sizes[total_pieces]);
    history.states[total_pieces][history.sizes[total_pieces]-1] = temp;
    
    //undo temp move
    if(captured_size > 0)
        free(captured);
    board[row][column] = 0;
    total_pieces--;

    //else return true
    return 1;
}
/*
    Function: Capture
        Param:
            int row: row where player piece exists
            int column: column where player piece exists
            int player: player to check if surrounded
            uint8_t**: list of all the addresses of the pieces visited
            int* count: count of the size of the list
        Return:
            if possible to capture
*/
int capture(int row, int column, int player, uint8_t** visited, int* count){
    board[row][column] |= 4; //set the visited flag
    uint8_t** temp_visited = realloc(visited,sizeof(*count+1));
    if(temp_visited == NULL){
        printf("MEMORY FAILURE\n");
        free(visited);
        exit(1);
    }
    visited = temp_visited;

    //printf("visiting (%p): %d %d (%p)\n", visited, column, row, &(board[row][column]));
    visited[*count] = &(board[row][column]);
    *count+=1;
    
    //check if any blank spaces, return NULL if found
    if( (row-1 >= 0 && board[row-1][column] == 0)
        || (row+1 < size && board[row+1][column] == 0)
        || (column-1 >= 0 && board[row][column-1] == 0)
        || (column+1 < size && board[row][column+1] == 0) ){
            //printf("BLANK AREA FOUND\n");
            return 0;
    }
    //printf("Done checking blanks\n");
    //will not trigger if has been visited due to 0b100 on visited pieces
    if(row-1 >= 0 && board[row-1][column] == player){
        if(!capture(row-1, column, player, visited, count)){
            return 0;
        }
    }
    if(row+1 < size && board[row+1][column] == player){
        if(!capture(row+1, column, player, visited, count)){
            return 0;
        } 
    }
    if(column-1 >= 0 && board[row][column-1] == player){
        if(!capture(row, column-1, player, visited, count)){
            return 0;
        }
    }
    if(column+1 < size && board[row][column+1] == player){
        if(!capture(row, column+1, player, visited, count)){
            return 0;
        }
    }
    //printf("No blanks found\n");
    return 1;
}
/*
    Function: Check Ko 
        Param:
            CompressedBoard currentState: the current state of all the pieces on the board
        return:
            if state passes the Ko rule
            1: no ko rule broken
            0: if ko rule is broken
        Check if board state has ever occurred before
*/
int checkKo(CompressedBoard currentState){
    int num_indices = (int) ceil( ( size * size ) / 32.0 );
    for(int states = 0; states < history.sizes[total_pieces]; states++){
        int matching = 0;
        for(int elements = 0; elements < num_indices; elements++){
            if(currentState[elements] == history.states[total_pieces][states][elements]) matching++;
        }
        if(matching == num_indices) return 0;
    }
    return 1;
}
/*  Function: Flip Pieces
        Param:
            int player:     the player of the pieces to be inverts
                            can either set or clear the player's pieces
            int* spaces:    an array of spaces to be flipped
            int size:   size of array
*/
void flipPieces(uint8_t player, uint8_t** spaces, int size){
    for(int i = 0; i < size; i++){
        *(spaces[i]) ^= player;
        if( (*(spaces[i])&3) == 0) total_pieces--;
        else total_pieces++;
    }
}
/*  Function: Clear Flag
        Param:
            int flag:     the flag of the space to be cleared
            int* spaces:    an array of spaces to be flipped
            int size:   size of array
*/
void clearFlags(uint8_t flag, uint8_t** spaces, int size){
    //printf("Clearing %d flag(s)\n", size);
    //printf("spaces = %p\n", spaces);
    for(int i = 0; i < size; i++){
        //printf("clearing %p (%X & %X)\n", spaces[i], *(spaces[i])&0xFF, flag^0xFF);
        *(spaces[i]) &= flag^0xFF;
    }
}
/*  Function: Create Board
        Param:
            int size: size of board to be allocated
        return: void
    Allocate memory to store the board state
*/
void createBoard(){
    //allocate memory for the board
    board = (uint8_t**) malloc(sizeof(uint8_t*)*size);
    for(int i = 0; i < size; i++){
        board[i] = (uint8_t*) malloc(sizeof(uint8_t)*size);
    }
}
/*  Function: Destroy Board
        Params: int size
        Retrun: void
    Deallocates the memory from board
*/
void destroyBoard(){
    //free allocated memory for board
    for(int i = 0; i < size; i++){
        free(board[i]);
    }
    free(board);
}
/*  Function: Print Board
        Params:
            Board board: board to be printed
            int size: size of board to be printed
        Return: void
    This function prints out the board using unicode characters
    iteration through the unsigned 8bit int array is done
        value of 0 prints out the respective intersection
        value of 1 prints out a black piece
        value of 2 prints out a white piece
*/
void printBoard(){
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
/*  Function: Compress Board State
        Params:
            Board board: the board to be compressed
            int size: the size of the board
        Return:
            CompressedBoard: this is the bits of a board compressed
    Since a board position can be represented in 2 bits, the board state is converted
    into each position being only two bits and stored in large 64bit unsigned integers
    for easy iteration and easy == comparison
*/
CompressedBoard compressBoardState(){
    //debug statement printf("Compression: %d\n", ((int)ceil((size*size)/32.0)));
    //initialize the size of the board to hold all the positions
    CompressedBoard cb = (CompressedBoard) malloc(sizeof(uint64_t) * (int) ceil( (size*size)/32.0 ));
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
    return cb;
}