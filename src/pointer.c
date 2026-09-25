#include "neslib.h"
#include "nesdoug.h"
#include "controls.h"
#include "pointer.h"
#include "piece.h"
#include "grid.h"
#include "engine.h"

static Pointer pointer;

static val drawn_x, drawn_y;

routine(init_pointer) {
    pointer.x = 5;
    pointer.y = 3;
    pointer.holding_piece = NULL;

    drawn_x = 0;
    drawn_y = 0;
}

static Piece *p;
routine(update_pointer) {
    if (triggered(LEFT) && 1 < pointer.x)
        pointer.x -= 1;
    else if (triggered(RIGHT) && pointer.x < BOARD_SIZE)
        pointer.x += 1;
    else if (triggered(UP) && pointer.y < BOARD_SIZE)
        pointer.y += 1;
    else if (triggered(DOWN) && 1 < pointer.y)
        pointer.y -= 1;
    else if (triggered(A)) {
        if(pointer.holding_piece) {
            if (is_legal_move(pointer.holding_piece->class, pointer.holding_piece->black, pointer.holding_piece->x, pointer.holding_piece->y, pointer.x, pointer.y)) {
                if (p = piece_at(pointer.x, pointer.y)) {
                    p->captured = true;
                }
                move_selected_piece(pointer.x, pointer.y);
                deselect_piece();
                pointer.holding_piece = NULL;
                needs_update = true;
            } else {
                pointer.holding_piece = NULL;
                deselect_piece();
            }
        } else if(p = select_piece(pointer.x, pointer.y))
            pointer.holding_piece = p;
    } else if (triggered(B) && pointer.holding_piece) {
        pointer.holding_piece = NULL;
        deselect_piece();
    }
}

static val sq_x, sq_y, tile_offset;
static val row_buf[SQUARE_TILES];
static bigval addr;
static val j, k;

static void redraw_square(void) {
    addr = NTADR_A(BOARD_TILE_X + SQUARE_TILES * (sq_x - 1),
                   BOARD_TILE_Y + SQUARE_TILES * (BOARD_SIZE - sq_y));
    for(j = 0; j < SQUARE_TILES; j++) {
        for(k = 0; k < SQUARE_TILES; k++) {
            row_buf[k] = board[(addr - NAMETABLE_A) + k] + tile_offset;
        }
        multi_vram_buffer_horz(row_buf, SQUARE_TILES, addr);
        addr += 32;
    }
}

render_routine(Pointer) {
    if(pointer.x == drawn_x && pointer.y == drawn_y) return sprid;

    // Restore the previously-highlighted square
    if(drawn_x) {
        sq_x = drawn_x;
        sq_y = drawn_y;
        tile_offset = 0;
        redraw_square();
    }

    sq_x = drawn_x = pointer.x;
    sq_y = drawn_y = pointer.y;
    tile_offset = HIGHLIGHT_OFFSET;
    redraw_square();

    return sprid;
}
