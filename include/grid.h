#ifndef GRID_H
#define GRID_H

#include "types.h"

#define BOARD_SIZE 8

// Top left corner of board in the nametable
#define BOARD_TILE_X 4
#define BOARD_TILE_Y 3
// Size of a board square in tiles
#define SQUARE_TILES 3

// Highlighted variants of square tiles
#define HIGHLIGHT_OFFSET 0xA0

// Background nametable
extern const unsigned char board[1024];

// 1-indexed board coordinates to screen coordinates
extern const val file_px[1 + BOARD_SIZE];
extern const val rank_py[1 + BOARD_SIZE];

#endif
