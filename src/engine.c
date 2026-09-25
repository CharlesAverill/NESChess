#include "engine.h"
#include "string.h"

bool needs_update = true;

static Occupied board[64];
static val i;
static routine(update_occupied) {
    memset(board, NotOccupied, 64);
    for (i = 0; i < 16; i++) {
        if (!black_pieces[i].captured) board[index_of_piece(black_pieces[i])] = BlackOccupied;
        if (!white_pieces[i].captured) board[index_of_piece(white_pieces[i])] = WhiteOccupied;
    }
    needs_update = false;
}

static Occupied _occupied(val x) {
    if (needs_update)
        update_occupied();

    return board[x];
}
#define occupied(x, y) _occupied(index_of_pos(x, y))

// Determine the legal moves of a piece and place their indeces in legal_moves
static sval forward;
bool is_legal_move(Class class, bool black, val x, val y, val tx, val ty) {
    switch(class) {
        case Pawn:
            if (tx < 1 || tx > 8 || ty < 1 || ty > 8)
                return false;

            forward = black ? -1 : 1;
            // One step forward
            if (x == tx && y + forward == ty && occupied(tx, ty) == NotOccupied)
                return true;
            // Two steps forward
            if (x == tx && y + forward + forward == ty && y == (black ? 7 : 2) &&
                    occupied(x, y + forward) == NotOccupied &&
                    occupied(x, y + forward + forward) == NotOccupied)
                return true;
            // Diagonal
            if ((x == tx + 1 || x == tx - 1) && y + forward == ty &&
                    occupied(tx, ty) == (black ? WhiteOccupied : BlackOccupied))
                return true;
            break;
        default:
            return false;
    }

    return false;
}

val legal_moves[30];
static val move_idx;
val get_legal_moves(Class class, bool black, val x, val y) {
    move_idx = 0;

    switch(class) {
        case Pawn:
            // Determine which direction is forward
            forward = black ? -1 : 1;
            if (is_legal_move(Pawn, black, x, y, x, y + forward))
                legal_moves[move_idx++] = index_of_pos(x, y + forward);
            if (is_legal_move(Pawn, black, x, y, x, y + forward + forward))
                legal_moves[move_idx++] = index_of_pos(x, y + forward + forward);
            if (is_legal_move(Pawn, black, x, y, x - 1, y + forward))
                legal_moves[move_idx++] = index_of_pos(x - 1, y + forward);
            if (is_legal_move(Pawn, black, x, y, x + 1, y + forward))
                legal_moves[move_idx++] = index_of_pos(x + 1, y + forward);
            break;
    }

    return move_idx;
}
