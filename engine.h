#include "go.h"

void getUserInput(int* row, int* column);
void makeMove();
int isValidMove(int row, int column);
int capture(int row, int column, int player, uint8_t** visited, int* count);
int checkKo(CompressedBoard currentState);
void flipPieces(uint8_t player, uint8_t** spaces, int size);
void clearFlags(uint8_t flag, uint8_t** spaces, int size);
void createBoard();
void destroyBoard();
void printBoard();
CompressedBoard compressBoardState();