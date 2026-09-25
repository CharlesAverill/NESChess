#include "engine.h"
#include "string.h"

bool needs_update = true;

static Occupied board[64];
static val i, j;
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

// Determine if a move is legal for a given piece
static sval forward;
static val start, end;
static val dx, dy, cx, cy;
static sval step_x, step_y;
static Occupied opp_occupied;
bool is_legal_move(Class class, bool black, val x, val y, val tx, val ty) {
    if (tx < 1 || tx > 8 || ty < 1 || ty > 8)
        return false;

    opp_occupied = black ? WhiteOccupied : BlackOccupied;

    switch(class) {
        case Pawn:   
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
                    occupied(tx, ty) == opp_occupied)
                return true;
            break;
        case Rook:
            if (x == tx && y == ty)
                return false;

            // Check verticals
            if (x == tx) {
                start = (y < ty ? y : ty) + 1;
                end = y < ty ? ty : y;
                for(j = start; j < end; j++) {
                    if (occupied(x, j) != NotOccupied)
                        return false;
                }
            } else if (y == ty) { // Check horizontals
                start = (x < tx ? x : tx) + 1;
                end = x < tx ? tx : x;
                for(j = start; j < end; j++) {
                    if (occupied(j, y) != NotOccupied)
                        return false;
                }
            } else
                return false;

            return occupied(tx, ty) == NotOccupied || occupied(tx, ty) == opp_occupied;
        case Bishop:
            dx = x < tx ? tx - x : x - tx;
            dy = y < ty ? ty - y : y - ty;
            if (dx == 0 || dx != dy)
                return false;

            step_x = x < tx ? 1 : -1;
            step_y = y < ty ? 1 : -1;
            cx = x + step_x;
            cy = y + step_y;
            while (cx != tx) {
                if (occupied(cx, cy) != NotOccupied)
                    return false;
                cx += step_x;
                cy += step_y;
            }

            return occupied(tx, ty) == NotOccupied || occupied(tx, ty) == opp_occupied;
        case Knight:
            dx = x < tx ? tx - x : x - tx;
            dy = y < ty ? ty - y : y - ty;
            if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1)))
                return false;

            return occupied(tx, ty) == NotOccupied || occupied(tx, ty) == opp_occupied;
        case Queen:
            return is_legal_move(Rook, black, x, y, tx, ty) ||
                   is_legal_move(Bishop, black, x, y, tx, ty);
        case King:
            if (x == tx && y == ty)
                return false;
            return ((x == tx - 1 || x == tx || x == tx + 1) &&
                    (y == ty - 1 || y == ty || y == ty + 1) &&
                    (occupied(tx, ty) == NotOccupied || occupied(tx, ty) == opp_occupied));
        default:
            return false;
    }

    return false;
}

// Cardinal directions: {S, W, E, N,  SW, SE, NW, NE}
static const sval dir_x[8]    = { 0, -1, 1, 0,   -1,  1, -1, 1 };
static const sval dir_y[8]    = {-1,  0, 0, 1,   -1, -1,  1, 1 };
// Knight directions:   {RUU, RRU, RRD, RDD, LDD, LLD, LLU, LUU}
static const sval knight_x[8] = { 1,  2,  2,  1, -1, -2, -2, -1 };
static const sval knight_y[8] = { 2,  1, -1, -2, -2, -1,  1,  2 };

val legal_moves[30];
static val move_idx;
static val step_i, mx, my;
static Occupied here;

// Add each square along the rays dir_x/dir_y[first..last)
// Stop at board edges and pieces (opponent inclusive, friendly exclusive)
static void add_rays(bool black, val x, val y, val first, val last) {
    opp_occupied = black ? WhiteOccupied : BlackOccupied;
    for (step_i = first; step_i < last; step_i++) {
        mx = x + dir_x[step_i];
        my = y + dir_y[step_i];
        while (mx >= 1 && mx <= 8 && my >= 1 && my <= 8) {
            here = occupied(mx, my);
            if (here != NotOccupied && here != opp_occupied)
                break;
            legal_moves[move_idx++] = index_of_pos(mx, my);
            if (here != NotOccupied)
                break;
            mx += dir_x[step_i];
            my += dir_y[step_i];
        }
    }
}

// Add legal moves from the offset arrays off_x and off_y
static void add_steps(const sval *off_x, const sval *off_y, Class class, bool black, val x, val y) {
    for (step_i = 0; step_i < 8; step_i++) {
        mx = x + off_x[step_i];
        my = y + off_y[step_i];
        if (is_legal_move(class, black, x, y, mx, my))
            legal_moves[move_idx++] = index_of_pos(mx, my);
    }
}

// Determine the legal moves of a piece, place their board indices in
// legal_moves, and return how many there are
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
        case Rook:
            add_rays(black, x, y, 0, 4);
            break;
        case Bishop:
            add_rays(black, x, y, 4, 8);
            break;
        case Queen:
            add_rays(black, x, y, 0, 8);
            break;
        case Knight:
            add_steps(knight_x, knight_y, Knight, black, x, y);
            break;
        case King:
            add_steps(dir_x, dir_y, King, black, x, y);
            break;
    }

    return move_idx;
}
