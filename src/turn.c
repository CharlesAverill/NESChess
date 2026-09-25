#include "turn.h"

bool black_turn;

routine(init_turn) {
    black_turn = false;
}

routine(finish_turn) {
    black_turn = !black_turn;
}
