#ifndef PIECE_H
#define PIECE_H

#include "types.h"

#define TOP_ROW 0x80
#define BOT_ROW 0x90

typedef enum Class {
    Pawn,
    Rook,
    Bishop,
    Knight,
    King,
    Queen
} Class;

typedef struct Piece {
    Class class;
    bool black;
    bool captured;
    val x, y;
} Piece;

extern Piece black_pieces[16];
extern Piece white_pieces[16];

render_routine(Pieces);
routine(init_pieces);

Piece* __fastcall__ select_piece(val x, val y);
void __fastcall__ move_selected_piece(val x, val y);
routine(deselect_piece);

#endif
