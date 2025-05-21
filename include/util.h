#include <stdlib.h>
#include <time.h>

#define PIECE_SIZE 32

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define FALSE 0
#define TRUE 1

extern int board[BOARD_WIDTH][BOARD_HEIGHT];

int get_random(int bound);