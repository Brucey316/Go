#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//Board sizes
#define TEST 16
#define BEGGINER 9
#define INTERMEDIATE 13
#define EXPERT 19

//Interior Cross Sections
#define INTERSECTION "\u253C"
//Edges
#define TOP "\u252C"
#define LEFT "\u251C"
#define BOTTOM "\u2534"
#define RIGHT "\u2524"
//Corners
#define TOP_LEFT "\u250C"
#define TOP_RIGHT "\u2510"
#define BOTTOM_LEFT "\u2514"
#define BOTTOM_RIGHT "\u2518"
//Player pieces
#define WHITE_CIRCLE "\u25CB"
#define BLACK_CIRCLE "\u25CF"
//Board coloring
#define TEXT_FONT "\033[38;5;0;48;5;250m"
#define CLEAR_FONT "\033[0m"

//Types of boards
typedef uint8_t** Board;//normal board
typedef uint64_t* CompressedBoard;//compressed board state
typedef uint64_t** PastBoards;//history of board states

void getUserInput(int* row, int* column);
void makeMove();
int isValidMove(int row, int column);
int capture(int row, int column, int player, uint8_t** visited, int* count);
void flipPieces(uint8_t player, uint8_t** spaces, int size);
void clearFlags(uint8_t flag, uint8_t** spaces, int size);
void createBoard();
void destroyBoard();
void printBoard();
CompressedBoard compressBoardState();