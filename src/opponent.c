#include "opponent.h"
#include "engine.h"
#include "piece.h"
#include "neslib.h"
#include "turn.h"

static val n_legal, index;
static Piece *piece;
routine(opponent_move) {
    n_legal = 0;

    while(true) {
        index = rand8() % 16;
        piece = &black_pieces[index];
        if(piece->captured)
            continue;

        n_legal = get_legal_moves(piece->class, true, piece->x, piece->y);
        if (n_legal == 0)
            continue;

        break;
    }
    
    move_piece_at_index(index, true, legal_moves[rand8() % n_legal]);

    finish_turn();
    needs_update = true;
}

render_routine(Opponent) {
    return sprid;
}
