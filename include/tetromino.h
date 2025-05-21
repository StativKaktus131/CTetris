#pragma once

enum BlockColor
{
	LIGHT_BLUE,
	YELLOW,
	PURPLE,
	ORANGE,
	DARK_BLUE,
	RED,
	GREEN
};


struct Block
{
	int x, y;
	enum BlockColor color;
};

// Tetrominos have 4 different constellations of their 4 blocks
struct Tetromino
{
	struct Block blockRotations[4][4];
	int toTerminate;
};

// copy methods for the structs
struct Tetromino copy_tetromino(struct Tetromino* tetromino);
struct Block copy_block(struct Block* block);

// helper methods
void get_color(float* color, enum BlockColor c);
void drop_tetromino(struct Tetromino* tetromino);
void move_tetromino(struct Tetromino* tetromino, int x, int y);
void hard_drop(struct Tetromino* tetromino, int rotation);
struct Tetromino* get_random_tetromino();

// checks / flags
int future_pos_is_possible(struct Tetromino* tetromino, int dx, int dy, int rotation);
int pos_is_possible(int x, int y);
void tetromino_on_ground(struct Tetromino* tetromino, int rotation);

// declare all pieces
extern struct Tetromino PIECE_I;
extern struct Tetromino PIECE_O;
extern struct Tetromino PIECE_T;
extern struct Tetromino PIECE_Z;
extern struct Tetromino PIECE_N;
extern struct Tetromino PIECE_L;
extern struct Tetromino PIECE_J;
