#pragma once

// SDL definitions
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// STD
#include <stdio.h>

// custom
#include "util.h"
#include "tetromino.h"

void draw_board();
void handle_key_event(SDL_Event* event);
void next_piece();