#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"
#include "piece.h"

#define index_of_pos(x,y) ((x) + (8 * (y)) - 9)
#define index_of_piece(p) index_of_pos((p).x, (p).y)
#define posx_of_index(i)  ((i) % 8 + 1)
#define posy_of_index(i)  ((i) / 8 + 1)

extern bool needs_update;
extern val  legal_moves[30];

typedef enum Occupied {
    NotOccupied,
    BlackOccupied,
    WhiteOccupied
} Occupied;

bool is_legal_move(Class class, bool black, val x, val y, val tx, val ty);
val  get_legal_moves(Class class, bool black, val x, val y);

#endif
