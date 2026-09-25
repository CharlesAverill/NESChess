#include "neslib.h"
#include "piece.h"
#include "grid.h"

static val selected_i;
Piece black_pieces[16];
Piece white_pieces[16];

static val i;
routine(init_pieces) {
    // Pawns
    for(i = 0; i < 8; i++) {
        black_pieces[i].class = Pawn;
        black_pieces[i].captured = false;
        black_pieces[i].x = i + 1;
        black_pieces[i].y = 7;
        black_pieces[i].black = true;

        white_pieces[i].class = Pawn;
        white_pieces[i].captured = false;
        white_pieces[i].x = i + 1;
        white_pieces[i].y = 2;
        white_pieces[i].black = false;
   }

    // Knights
    for(; i < 10; i++) {
        black_pieces[i].class = Knight;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 8 ? 2 : 7;
        black_pieces[i].y = 8;
        black_pieces[i].black = true;

        white_pieces[i].class = Knight;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 8 ? 2 : 7;
        white_pieces[i].y = 1;
        white_pieces[i].black = false;
   }

    // Bishops
    for(; i < 12; i++) {
        black_pieces[i].class = Bishop;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 10 ? 3 : 6;
        black_pieces[i].y = 8;
        black_pieces[i].black = true;

        white_pieces[i].class = Bishop;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 10 ? 3 : 6;
        white_pieces[i].y = 1;
        white_pieces[i].black = false;
   }

    // Rooks
    for(; i < 14; i++) {
        black_pieces[i].class = Rook;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 12 ? 1 : 8;
        black_pieces[i].y = 8;
        black_pieces[i].black = true;

        white_pieces[i].class = Rook;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 12 ? 1 : 8;
        white_pieces[i].y = 1;
        white_pieces[i].black = false;
   }

    // Kings
    black_pieces[i].class = King;
    black_pieces[i].captured = false;
    black_pieces[i].x = 5;
    black_pieces[i].y = 8;
    black_pieces[i].black = true;

    white_pieces[i].class = King;
    white_pieces[i].captured = false;
    white_pieces[i].x = 5;
    white_pieces[i].y = 1;
    white_pieces[i].black = false;

    i++;

    // Queens
    black_pieces[i].class = Queen;
    black_pieces[i].captured = false;
    black_pieces[i].x = 4;
    black_pieces[i].y = 8;
    black_pieces[i].black = true;

    white_pieces[i].class = Queen;
    white_pieces[i].captured = false;
    white_pieces[i].x = 4;
    white_pieces[i].y = 1;
    white_pieces[i].black = false;


    selected_i = -1;
}

static bool selected_black;
static Piece* found;
static val found_i;
// Find the uncaptured piece on (x, y)
Piece* __fastcall__ piece_at(val x, val y) {
    found = NULL;

    for(i = 0; i < 16; i++) {
        if (!black_pieces[i].captured && black_pieces[i].x == x && black_pieces[i].y == y) {
            found_i = i;
            found = &black_pieces[i];
            break;
        }
    }

    if (found)
        return found;

    for(i = 0; i < 16; i++) {
        if (!white_pieces[i].captured && white_pieces[i].x == x && white_pieces[i].y == y) {
            found_i = i;
            found = &white_pieces[i];
            break;
        }
    }

    return found;
}

Piece* __fastcall__ select_piece(val x, val y) {
    if(!piece_at(x, y))
        return NULL;
    selected_i = found_i;
    selected_black = found->black;
    return found;
}

routine(deselect_piece) {
    selected_i = -1;
}

void __fastcall__ move_selected_piece(val x, val y) {
    if(selected_black) {
        black_pieces[selected_i].x = x;
        black_pieces[selected_i].y = y;
    } else {
        white_pieces[selected_i].x = x;
        white_pieces[selected_i].y = y;
    }
}

#define BLACK_PAL    0
#define WHITE_PAL    1
#define SELECTED_PAL 2

static const Piece *p;
static val pal, px, py;
static void draw_piece(void) {
    if (p->captured) return;
    px = file_px[p->x];
    py = rank_py[p->y];
    sprid = oam_spr(px, py,     TOP_ROW + p->class, pal, sprid);
    sprid = oam_spr(px, py + 8, BOT_ROW + p->class, pal, sprid);
}

render_routine(Pieces) {
    for(i = 0; i < 16; i++) {
        p = &black_pieces[i];
        if (i == selected_i && selected_black)
            pal = SELECTED_PAL;
        else
            pal = BLACK_PAL;
        draw_piece();

        p = &white_pieces[i];
        if (i == selected_i && !selected_black)
            pal = SELECTED_PAL;
        else
            pal = WHITE_PAL;
        draw_piece();
    }

    return sprid;
}
