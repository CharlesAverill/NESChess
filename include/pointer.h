#ifndef POINTER_H
#define POINTER_H

#include "types.h"
#include "piece.h"

typedef struct Pointer {
    val x, y;

    // Piece currently held by pointer
    Piece *holding_piece;
} Pointer;

routine(init_pointer);
routine(update_pointer);
render_routine(Pointer);

#endif
