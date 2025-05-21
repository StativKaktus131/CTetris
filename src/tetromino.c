#include "tetromino.h"
#include "util.h"
#include <stdio.h>

// keeps track of all tetrominos
struct Tetromino* tetrominos[7] = { &PIECE_I, &PIECE_O, &PIECE_T, &PIECE_Z, &PIECE_N, &PIECE_L, &PIECE_J };


// populates the float* color (float[]) according to the BlockColor c
void get_color(float* color, enum BlockColor c)
{
	switch (c)
	{
		case LIGHT_BLUE:
			color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 1.0;
			break;
		
		case YELLOW:
			color[0] = 1.0;
			color[1] = 1.0;
			color[2] = 0.0;
			break;
		
		case PURPLE:
			color[0] = 1.0;
			color[1] = 0.0;
			color[2] = 1.0;
			break;
		
		case ORANGE:
			color[0] = 1.0;
			color[1] = 0.5;
			color[2] = 0.0;
			break;
		
		case DARK_BLUE:
			color[0] = 0.0;
			color[1] = 0.0;
			color[2] = 1.0;
			break;
		
		case RED:
			color[0] = 1.0;
			color[1] = 0.0;
			color[2] = 0.0;
			break;

		case GREEN:
			color[0] = 0.0;
			color[1] = 1.0;
			color[2] = 0.0;
			break;
	}
}

// returns a Tetromino with the same constellations as the tetromino in the parameter
struct Tetromino copy_tetromino(struct Tetromino* tetromino)
{
	// declare return Tetromino
	struct Tetromino ret;

	// loop through all rotations and blocks
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// set the parameters
			ret.blockRotations[i][j].x = tetromino->blockRotations[i][j].x;
			ret.blockRotations[i][j].y = tetromino->blockRotations[i][j].y;
			ret.blockRotations[i][j].color = tetromino->blockRotations[i][j].color;
		}
	}

	return ret;
}

// returns a block with the same position and color as the block in the parameter
struct Block copy_block(struct Block* block)
{
	return (struct Block) { .x = block->x, .y = block->y, .color = block->color };
}

// basically a macro for moving the tetromino down one spot
void drop_tetromino(struct Tetromino* tetromino)
{
	move_tetromino(tetromino, 0, 1);
}

// moves all blocks of the tetromino in the given direction
void move_tetromino(struct Tetromino* tetromino, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tetromino->blockRotations[i][j].x += x;
			tetromino->blockRotations[i][j].y += y;
		}
	}
}

// checks if the position is either occupied or out of bounds and returns the inverse
int pos_is_possible(int x, int y)
{
	return x >= 0 && x < BOARD_WIDTH && board[x][y] < 0 && y < BOARD_HEIGHT;
}

// for checking if you can move / rotate a tetromino
int future_pos_is_possible(struct Tetromino* tetromino, int dx, int dy, int rotation)
{
	for (int i = 0; i < 4; i++)
	{
		// get future variables
		int fx = tetromino->blockRotations[rotation][i].x + dx;
		int fy = tetromino->blockRotations[rotation][i].y + dy;

		// if only one block is not possible, future position of tetromino is not as well
		if (!pos_is_possible(fx, fy))
			return FALSE;
	}

	// if all pass return true
	return TRUE;
}


// if the tetromino is on the ground (board_height or another block) it flags it for termination
void tetromino_on_ground(struct Tetromino* tetromino, int rotation)
{
	for (int i = 0; i < 4; i++)
	{
		// where the next Y value would be
		int nextY = tetromino->blockRotations[rotation][i].y + 1;
		int x = tetromino->blockRotations[rotation][i].x;

		// if nextY wouldn't be possible then terminate tetromino
		if (nextY == BOARD_HEIGHT || board[x][nextY] >= 0)
			tetromino->toTerminate = TRUE;
	}
}

void hard_drop(struct Tetromino* tetromino, int rotation)
{
	// just drop tetromino while it can drop once, then check if it should be terminated
	while (!tetromino->toTerminate)
	{
		drop_tetromino(tetromino);
		tetromino_on_ground(tetromino, rotation);
	}
}


struct Tetromino* get_random_tetromino()
{
	return tetrominos[get_random(sizeof(tetrominos) / sizeof(tetrominos[0]))];
}





// ~PIECE, DEFINITIONS

struct Tetromino PIECE_I = 
{
	.blockRotations = 
	{
		{
			{ .x = 1, .y = 0, .color = LIGHT_BLUE },
			{ .x = 1, .y = 1, .color = LIGHT_BLUE },
			{ .x = 1, .y = 2, .color = LIGHT_BLUE },
			{ .x = 1, .y = 3, .color = LIGHT_BLUE },
		},
		{
			{ .x = 0, .y = 1, .color = LIGHT_BLUE },
			{ .x = 1, .y = 1, .color = LIGHT_BLUE },
			{ .x = 2, .y = 1, .color = LIGHT_BLUE },
			{ .x = 3, .y = 1, .color = LIGHT_BLUE },
		},
		{
			{ .x = 1, .y = 0, .color = LIGHT_BLUE },
			{ .x = 1, .y = 1, .color = LIGHT_BLUE },
			{ .x = 1, .y = 2, .color = LIGHT_BLUE },
			{ .x = 1, .y = 3, .color = LIGHT_BLUE },
		},
		{
			{ .x = 0, .y = 1, .color = LIGHT_BLUE },
			{ .x = 1, .y = 1, .color = LIGHT_BLUE },
			{ .x = 2, .y = 1, .color = LIGHT_BLUE },
			{ .x = 3, .y = 1, .color = LIGHT_BLUE },
		},
	}
};

struct Tetromino PIECE_O = 
{
	.blockRotations = 
	{
		{
			{ .x = 0, .y = 0, .color = YELLOW },
			{ .x = 1, .y = 0, .color = YELLOW },
			{ .x = 0, .y = 1, .color = YELLOW },
			{ .x = 1, .y = 1, .color = YELLOW },
		},
		{
			{ .x = 0, .y = 0, .color = YELLOW },
			{ .x = 1, .y = 0, .color = YELLOW },
			{ .x = 0, .y = 1, .color = YELLOW },
			{ .x = 1, .y = 1, .color = YELLOW },
		},
		{
			{ .x = 0, .y = 0, .color = YELLOW },
			{ .x = 1, .y = 0, .color = YELLOW },
			{ .x = 0, .y = 1, .color = YELLOW },
			{ .x = 1, .y = 1, .color = YELLOW },
		},
		{
			{ .x = 0, .y = 0, .color = YELLOW },
			{ .x = 1, .y = 0, .color = YELLOW },
			{ .x = 0, .y = 1, .color = YELLOW },
			{ .x = 1, .y = 1, .color = YELLOW },
		},
	}
};

struct Tetromino PIECE_T = 
{
	.blockRotations = 
	{
		{
			{ .x = 1, .y = 0, .color = PURPLE },
			{ .x = 0, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 1, .color = PURPLE },
			{ .x = 2, .y = 1, .color = PURPLE },
		},
		{
			{ .x = 1, .y = 0, .color = PURPLE },
			{ .x = 1, .y = 1, .color = PURPLE },
			{ .x = 2, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 2, .color = PURPLE },
		},
		{
			{ .x = 0, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 1, .color = PURPLE },
			{ .x = 2, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 2, .color = PURPLE },
		},
		{
			{ .x = 1, .y = 0, .color = PURPLE },
			{ .x = 0, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 1, .color = PURPLE },
			{ .x = 1, .y = 2, .color = PURPLE },
		},
	}
};

struct Tetromino PIECE_Z = 
{
	.blockRotations = 
	{
		{
			{ .x = 0, .y = 1, .color = RED },
			{ .x = 1, .y = 1, .color = RED },
			{ .x = 1, .y = 2, .color = RED },
			{ .x = 2, .y = 2, .color = RED },
		},
		{
			{ .x = 1, .y = 0, .color = RED },
			{ .x = 1, .y = 1, .color = RED },
			{ .x = 0, .y = 1, .color = RED },
			{ .x = 0, .y = 2, .color = RED },
		},
		{
			{ .x = 0, .y = 0, .color = RED },
			{ .x = 1, .y = 0, .color = RED },
			{ .x = 1, .y = 1, .color = RED },
			{ .x = 2, .y = 1, .color = RED },
		},
		{
			{ .x = 2, .y = 0, .color = RED },
			{ .x = 1, .y = 1, .color = RED },
			{ .x = 2, .y = 1, .color = RED },
			{ .x = 1, .y = 2, .color = RED },
		},
	}
};

struct Tetromino PIECE_N = 
{
	.blockRotations =
	{
		{
			{ .x = 1, .y = 1, .color = GREEN },
			{ .x = 2, .y = 1, .color = GREEN },
			{ .x = 0, .y = 2, .color = GREEN },
			{ .x = 1, .y = 2, .color = GREEN },
		},
		{
			{ .x = 0, .y = 0, .color = GREEN },
			{ .x = 0, .y = 1, .color = GREEN },
			{ .x = 1, .y = 1, .color = GREEN },
			{ .x = 1, .y = 2, .color = GREEN },
		},
		{
			{ .x = 1, .y = 0, .color = GREEN },
			{ .x = 2, .y = 0, .color = GREEN },
			{ .x = 0, .y = 1, .color = GREEN },
			{ .x = 1, .y = 1, .color = GREEN },
		},
		{
			{ .x = 1, .y = 0, .color = GREEN },
			{ .x = 1, .y = 1, .color = GREEN },
			{ .x = 2, .y = 1, .color = GREEN },
			{ .x = 2, .y = 2, .color = GREEN },
		},
	}
};

struct Tetromino PIECE_L = 
{
	.blockRotations =
	{
		{
			{ .x = 1, .y = 0, .color = DARK_BLUE },
			{ .x = 1, .y = 1, .color = DARK_BLUE },
			{ .x = 1, .y = 2, .color = DARK_BLUE },
			{ .x = 2, .y = 2, .color = DARK_BLUE },
		},
		{
			{ .x = 0, .y = 1, .color = DARK_BLUE },
			{ .x = 1, .y = 1, .color = DARK_BLUE },
			{ .x = 2, .y = 1, .color = DARK_BLUE },
			{ .x = 0, .y = 2, .color = DARK_BLUE },
		},
		{
			{ .x = 0, .y = 0, .color = DARK_BLUE },
			{ .x = 1, .y = 0, .color = DARK_BLUE },
			{ .x = 1, .y = 1, .color = DARK_BLUE },
			{ .x = 1, .y = 2, .color = DARK_BLUE },
		},
		{
			{ .x = 2, .y = 0, .color = DARK_BLUE },
			{ .x = 0, .y = 1, .color = DARK_BLUE },
			{ .x = 1, .y = 1, .color = DARK_BLUE },
			{ .x = 2, .y = 1, .color = DARK_BLUE },
		},
	}
};

struct Tetromino PIECE_J =
{
	.blockRotations = 
	{
		{
			{ .x = 1, .y = 0, .color = ORANGE },
			{ .x = 1, .y = 1, .color = ORANGE },
			{ .x = 0, .y = 2, .color = ORANGE },
			{ .x = 1, .y = 2, .color = ORANGE },
		},
		{
			{ .x = 0, .y = 0, .color = ORANGE },
			{ .x = 0, .y = 1, .color = ORANGE },
			{ .x = 1, .y = 1, .color = ORANGE },
			{ .x = 2, .y = 1, .color = ORANGE },
		},
		{
			{ .x = 1, .y = 0, .color = ORANGE },
			{ .x = 2, .y = 0, .color = ORANGE },
			{ .x = 1, .y = 1, .color = ORANGE },
			{ .x = 1, .y = 2, .color = ORANGE },
		},
		{
			{ .x = 0, .y = 1, .color = ORANGE },
			{ .x = 1, .y = 1, .color = ORANGE },
			{ .x = 2, .y = 1, .color = ORANGE },
			{ .x = 2, .y = 2, .color = ORANGE },
		},
	}
};