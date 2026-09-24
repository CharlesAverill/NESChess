#include "neslib.h"
#include "piece.h"
#include "grid.h"

static Piece black_pieces[16];
static Piece white_pieces[16];
static val selected_i;

static val i;
routine(init_pieces) {
    // Pawns
    for(i = 0; i < 8; i++) {
        black_pieces[i].class = Pawn;
        black_pieces[i].captured = false;
        black_pieces[i].x = i + 1;
        black_pieces[i].y = 7;

        white_pieces[i].class = Pawn;
        white_pieces[i].captured = false;
        white_pieces[i].x = i + 1;
        white_pieces[i].y = 2;
    }

    // Knights
    for(; i < 10; i++) {
        black_pieces[i].class = Knight;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 8 ? 2 : 7;
        black_pieces[i].y = 8;

        white_pieces[i].class = Knight;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 8 ? 2 : 7;
        white_pieces[i].y = 1;
    }

    // Bishops
    for(; i < 12; i++) {
        black_pieces[i].class = Bishop;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 10 ? 3 : 6;
        black_pieces[i].y = 8;

        white_pieces[i].class = Bishop;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 10 ? 3 : 6;
        white_pieces[i].y = 1;
    }

    // Rooks
    for(; i < 14; i++) {
        black_pieces[i].class = Rook;
        black_pieces[i].captured = false;
        black_pieces[i].x = i == 12 ? 1 : 8;
        black_pieces[i].y = 8;

        white_pieces[i].class = Rook;
        white_pieces[i].captured = false;
        white_pieces[i].x = i == 12 ? 1 : 8;
        white_pieces[i].y = 1;
    }

    // Kings
    black_pieces[i].class = King;
    black_pieces[i].captured = false;
    black_pieces[i].x = 5;
    black_pieces[i].y = 8;

    white_pieces[i].class = King;
    white_pieces[i].captured = false;
    white_pieces[i].x = 5;
    white_pieces[i].y = 1;

    i++;

    // Queens
    black_pieces[i].class = Queen;
    black_pieces[i].captured = false;
    black_pieces[i].x = 4;
    black_pieces[i].y = 8;

    white_pieces[i].class = Queen;
    white_pieces[i].captured = false;
    white_pieces[i].x = 4;
    white_pieces[i].y = 1;


    selected_i = -1;
}

static bool selected_black, found;
bool __fastcall__ select_piece(val x, val y) {
    found = false;

    for(i = 0; i < 16; i++) {
        if (black_pieces[i].x == x && black_pieces[i].y == y) {
            selected_i = i;
            selected_black = true;
            found = true;
            break;
        }
    }

    if (found)
        return true;

    for(i = 0; i < 16; i++) {
        if (white_pieces[i].x == x && white_pieces[i].y == y) {
            selected_i = i;
            selected_black = false;
            found = true;
            break;
        }
    }

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
