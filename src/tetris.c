#include "tetris.h"

// globals
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// game logic
int timer = 0;
int dropAt = 6000;
int speedDrop = FALSE;


// current piece / rotation
struct Tetromino current_piece;
int current_rotation = 0;

// initialize board
int board[BOARD_WIDTH][BOARD_HEIGHT];


// helper method
int get_random(int bound) { return rand() % bound; }


// app init method
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	// set random seed
	srand(time(NULL));

	// init board to -1 (null)
	for (int x = 0; x < BOARD_WIDTH; x++)
	{
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			board[x][y] = -1;
		}
	}

	// set first current piece
	current_piece = copy_tetromino(get_random_tetromino());
	current_piece.toTerminate = FALSE;

	// SDL metadata
	SDL_SetAppMetadata("Tetris", "1.0", "com.tetris");

	// try to initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// create window and renderer
	if (!SDL_CreateWindowAndRenderer("Tetris", BOARD_WIDTH * PIECE_SIZE, BOARD_HEIGHT * PIECE_SIZE, 0, &window, &renderer))
	{
		SDL_Log("Couldn't create window or renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

// event handling
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	switch (event->type)
	{
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
			break;

		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			handle_key_event(event);
			break;
	}

	return SDL_APP_CONTINUE;
}


// once per frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);

	// clear window
	SDL_RenderClear(renderer);


	// draw the board
	draw_board();


	// present
	SDL_RenderPresent(renderer);
	
	return SDL_APP_CONTINUE;
}

// sdl cleans up window and renderer
void SDL_AppQuit(void* appstate, SDL_AppResult result) { }


void handle_key_event(SDL_Event* event)
{
	// bool
	int down = event->type == SDL_EVENT_KEY_DOWN;

	if (!down)
	{
		if (event->key.scancode == SDL_SCANCODE_DOWN || event->key.scancode == SDL_SCANCODE_S)
			speedDrop = false;
		
		return;
	}

	switch (event->key.scancode)
	{
		// moving the tetromino to the left
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			if (future_pos_is_possible(&current_piece, -1, 0, current_rotation))
				move_tetromino(&current_piece, -1, 0);
			break;

		// moving the tetromino to the right
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			if (future_pos_is_possible(&current_piece, 1, 0, current_rotation))
				move_tetromino(&current_piece, 1, 0);
			break;
		
		// rotating the tetromino
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			if (future_pos_is_possible(&current_piece, 0, 0, (current_rotation + 1) % 4))
				current_rotation = (current_rotation + 1) % 4;
			break;

		// hard drop piece when pressing space
		case SDL_SCANCODE_SPACE:
			hard_drop(&current_piece, current_rotation);
			timer = dropAt;
			break;

		// speed drop toggle on
		case SDL_SCANCODE_DOWN:
		case SDL_SCANCODE_S:
			speedDrop = true;
			break;
		
		// clear all
		case SDL_SCANCODE_C:
			for (int x = 0; x < BOARD_WIDTH; x++)
				for (int y = 0; y < BOARD_HEIGHT; y++)
					board[x][y] = -1;
			break;
	}
}


void draw_block(int x, int y, enum BlockColor c)
{
	// loads the color into the float array according to block color c
	float color[3];
	get_color(color, c);

	SDL_SetRenderDrawColorFloat(renderer, color[0], color[1], color[2], SDL_ALPHA_OPAQUE_FLOAT);
	
	SDL_FRect rect = { .x = x * PIECE_SIZE, .y = y * PIECE_SIZE, .w = PIECE_SIZE, .h = PIECE_SIZE };

	// draw rect
	SDL_RenderFillRect(renderer, &rect);
}


void draw_board()
{
	timer++;

	// simple drop timer (if speed drop is on, then thresh is lower)
	if (timer > dropAt * (speedDrop ? 0.15 : 1.0))
	{
		timer = 0;

		// check if current tetromino touches the ground
		tetromino_on_ground(&current_piece, current_rotation);

		// if not drop it
		if (!current_piece.toTerminate)
			drop_tetromino(&current_piece);

		// otherwise add the next piece
		else
			next_piece();
	}

	// draws the current tetromino
	for (int i = 0; i < 4; i++)
	{
		struct Block block = current_piece.blockRotations[current_rotation][i];

		draw_block(block.x, block.y, block.color);
	}

	// draws the board
	for (int x = 0; x < BOARD_WIDTH; x++)
		for (int y = 0; y < BOARD_HEIGHT; y++)
			if (board[x][y] >= 0)
				draw_block(x, y, board[x][y]);
}


// checks if any rows are to be deleted and shifts them down
void check_for_rows()
{
	// loop through all the rows
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		// only one block needs to be free so the row isn't full
		int rowFull = TRUE;
		for (int x = 0; x < BOARD_WIDTH; x++)
			if (board[x][y] < 0)
				rowFull = FALSE;	


		// guard case
		if (!rowFull)
			continue;
		
		// shift all the rows behind the one to be deleted down by one
		for (int yy = y; yy > 0; yy--)
			for (int xx = 0; xx < BOARD_WIDTH; xx++)
				board[xx][yy] = board[xx][yy - 1];
	}
}


void next_piece()
{
	// add blocks to the board
	for (int i = 0; i < 4; i++)
	{
		struct Block block = current_piece.blockRotations[current_rotation][i];
		board[block.x][block.y] = block.color;
	}

	// chose next piece
	current_piece = copy_tetromino(get_random_tetromino());
	current_piece.toTerminate = FALSE;
	current_rotation = 0;

	// check for any completed rows after blocks are added to array
	check_for_rows();
}