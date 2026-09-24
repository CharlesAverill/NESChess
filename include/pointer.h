#ifndef POINTER_H
#define POINTER_H

#include "types.h"

typedef struct Pointer {
    val x, y;

    // Piece currently held by pointer
    bool holding_piece;
    val held_x, held_y;
} Pointer;

routine(init_pointer);
routine(update_pointer);
render_routine(Pointer);

#endif
