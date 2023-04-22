#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define TEST 16
#define BEGGINER 9
#define INTERMEDIATE 13
#define EXPERT 19

#define INTERSECTION "\u253C"

#define TOP "\u252C"
#define LEFT "\u251C"
#define BOTTOM "\u2534"
#define RIGHT "\u2524"

#define TOP_LEFT "\u250C"
#define TOP_RIGHT "\u2510"
#define BOTTOM_LEFT "\u2514"
#define BOTTOM_RIGHT "\u2518"

#define WHITE_CIRCLE "\u25CB"
#define BLACK_CIRCLE "\u25CF"

#define TEXT_FONT "\033[38;5;0;48;5;250m"
#define CLEAR_FONT "\033[0m"

typedef uint8_t** Board;
typedef uint64_t* CompressedBoard;

uint16_t getUserInput();
void makeMove(int, Board, int);
int isValidMove(int, int, Board, int);
void createBoard(int);
void destroyBoard(int);
void printBoard(int, Board);
CompressedBoard compressBoardState(Board, int);