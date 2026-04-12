#include "../include/chessUtilities.h"
#include "../include/GameSession.h"
#include "../include/CHESSlogging.h"

#include <stdio.h>
#include <stdlib.h>


static uint8_t opos_update(
        struct square ***ChessBoard, uint8_t *opos
        ) {
    switch (ChessBoard[OPOS_XP][OPOS_YP]->obj.type) {
        case pawn: {}
        case knight: {}
        case king: {}
        case queen: {}
        case rook: {}
        case bishop: {}
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
}


static uint8_t npos_update(
        struct square ***ChessBoard, uint8_t *npos
        ) {
    switch (ChessBoard[NPOS_XP][NPOS_YP]->obj.type) {
        case pawn: {}
        case knight: {}
        case king: {}
        case queen: {}
        case rook: {}
        case bishop: {}
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
}


void user_move(
        struct ChessGame *global, uint8_t *opos, uint8_t *npos
        ) {

    npos_update(global->ChessBoard, npos);
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.type =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type;
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side;

    opos_update(global->ChessBoard, opos);
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type = empty;
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side = none;
}


static uint8_t find_figure(struct square ***ChessBoard, 
            enum color pcolor, enum type_pieces type) {
    for (uint8_t i = 0; i < 8; ++i)
        for (uint8_t j = 0; j < 8; ++j)
            if (ChessBoard[i][j]->obj.type == type &&
                    ChessBoard[i][j]->obj.side == pcolor) {
                return ChessBoard[i][j]->pos;
            }
    return 255;
}


// BUG: A piece sees the "ghost" pieces on empty line
//( Not always, need to find and need to fix why it happens )
// ( It happened not only on vertical, horizontal too )
static struct square *check_between_direct_line(
        struct square ***ChessBoard, uint8_t *opos, uint8_t *kpos, 
        uint8_t opos_arg)
        {
    uint8_t min = (*kpos % 8 < opos_arg) ? *kpos : *opos - 1;
    uint8_t max = (*kpos % 8 > opos_arg) ? *kpos : *opos - 1;
    for (uint8_t between = min + 1; between < max; ++between) {
        if (opos_arg == OPOS_YP) {
            if (ChessBoard[OPOS_XP][between % 8]->obj.type != empty)
                return ChessBoard[OPOS_XP][between % 8];
        }
        else {
            if (ChessBoard[between / 8][OPOS_YP]->obj.type != empty) {
                return ChessBoard[between / 8][OPOS_YP];
            }
        }
    }
    return NULL;
}

// BUG: This doesn't work If the pieces are 1 square apart
static struct square *check_between_diagonal(
        struct square ***ChessBoard, uint8_t *opos, uint8_t *kpos) {
    uint8_t min = (*kpos / 8 < OPOS_XP) ? *kpos : *opos - 1;
    uint8_t max = (*kpos / 8 > OPOS_XP) ? *kpos : *opos - 1;
    if (min % 8 < max % 8) {
        for (uint8_t diag = min / 8 + 1; min < max / 8; ++diag) {
            if (ChessBoard[diag][min % 8 + (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 + (diag - (min / 8))];
        }
    }
    else {
        for (uint8_t diag = min / 8 + 1; min < max / 8; ++diag) {
            if (ChessBoard[diag][min % 8 - (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 + (diag - (min / 8))];
        }

    }
    printf("DIAGONAL CHECKING END WITHOUT FIGURE\n");
    return NULL;
}


// BUG: Diagonal checking works poor. Doesn't see any piece between
// friendly non-king piece and enemy diagonal piece (queen/bishop)
// At first need to fix bug with checking between diagonal
// (probably than I'd find a solve problem completely
static uint8_t check_hidden_king_attack(
        struct ChessGame *global, uint8_t *opos) {
    uint8_t kpos = find_figure(global->ChessBoard, global->user_side, king);
    if (kpos > 64) {
        printf("KING IS MISSING\n");
        exit(-1);
    }
    if (OPOS_XP == kpos / 8) { // horizontal
        printf("HORIZONTAL\n");
        struct square *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos, OPOS_YP);
        if (non_empty == NULL) {
            if (OPOS_YP > kpos % 8) {
                for (uint8_t horiz = OPOS_YP + 1; horiz < 8; ++horiz) {
                    if (global->ChessBoard[OPOS_XP][horiz]->obj.type != empty) {
                        non_empty = global->ChessBoard[OPOS_XP][horiz];
                        break;
                    }
                }
            }
            else {
                for (int8_t horiz = OPOS_YP - 1; horiz >= 0; --horiz) {
                    if (global->ChessBoard[OPOS_XP][horiz]->obj.type != empty) {
                        non_empty = global->ChessBoard[OPOS_XP][horiz];
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->obj.side == global->user_side)
            return 0;
        if (non_empty->obj.type == rook || non_empty->obj.type == queen)
            return 1;
        printf("It's correct move \n");
        return 0;
    }

    if (OPOS_YP == kpos % 8) { // vertical
        printf("VERTICAL\n");
        struct square *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos, OPOS_XP);
        if (non_empty == NULL) {
            if (OPOS_XP < kpos / 8) {
                for (int8_t vert = OPOS_XP - 1; vert >= 0; --vert) {
                    if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                        non_empty = global->ChessBoard[vert][OPOS_YP];
                        break;
                    }
                }
            }
            else {
                for (uint8_t vert = OPOS_XP + 1; vert < 8; ++vert) {
                    if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                        non_empty = global->ChessBoard[vert][OPOS_YP];
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->obj.side == global->user_side)
            return 0;
        if (non_empty->obj.type == rook || non_empty->obj.type == queen)
            return 2;
        printf("It's correct move \n");
        return 0;
    }
    // DIAGONAL
    if (abs(OPOS_XP - (kpos / 8)) == abs(OPOS_YP - (kpos % 8))) {
        printf("DIAGONAL\n");
        uint8_t min = (kpos / 8 < OPOS_XP) ? kpos : *opos - 1;
        uint8_t max = (kpos / 8 > OPOS_XP) ? kpos : *opos - 1;
        struct square *non_empty = check_between_diagonal(
                global->ChessBoard, opos, &kpos);
        if (non_empty == NULL) {
            if (OPOS_XP < kpos / 8) {
                if (OPOS_YP < kpos % 8) {
                    for (int8_t diag = OPOS_XP - 1; diag >= 0 && OPOS_YP - (OPOS_XP - diag) >= 0; --diag)
                        if (global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)] ->obj. type != empty)
                            non_empty = global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)];
                }
                else {
                    for (int8_t diag = OPOS_XP - 1; diag >= 0 && OPOS_YP + (OPOS_XP - diag) < 8; --diag)
                        if (global->ChessBoard[diag][OPOS_YP + (OPOS_XP - diag)] ->obj. type != empty)
                            non_empty = global->ChessBoard[diag][OPOS_YP + (OPOS_XP - diag)];
                }
            }
            else {
                if (OPOS_YP < kpos % 8) {
                    for (int8_t diag = OPOS_XP + 1; diag < 8 && OPOS_YP - (diag - OPOS_XP) >= 0; diag++) 
                        if (global->ChessBoard[diag][OPOS_YP - (diag - OPOS_XP)] ->obj. type != empty)
                            non_empty = global->ChessBoard[diag][OPOS_YP - (diag - OPOS_XP)];
                }
                else {
                    for (int8_t diag = OPOS_XP + 1; diag < 8 && OPOS_YP + (diag - OPOS_XP) < 8; diag++) 
                        if (global->ChessBoard[diag][OPOS_YP + (diag - OPOS_XP)] ->obj. type != empty)
                            non_empty = global->ChessBoard[diag][OPOS_YP + (diag - OPOS_XP)];
                }
            }
        }
        if (non_empty == NULL || non_empty->obj.side == global->user_side)
            return 0;
        if (non_empty->obj.type == bishop || non_empty->obj.type == queen)
            return 2;
        printf("It's correct move \n");
        return 0;
    }
    return 0;
}


// TODO: Check move is only forward and etc.
static int32_t check_pawn_move(
        struct square ***ChessBoard, uint8_t *opos, uint8_t *npos
        ) {
    if (OPOS_YP != NPOS_YP) { // atacking
        if ((OPOS_XP == 1 || OPOS_XP == 6)) {}
    }
    else {
        if (abs(OPOS_XP - NPOS_XP) > 2) 
            return ERROR_PAWN_LEN_MORE_THEN_2;
        else if (abs(OPOS_XP - NPOS_XP) == 2 && OPOS_XP != 6)
            return ERROR_PAWN_MOVE_2_NOT_IN_START;
        else {

        }
    }
    return 0;
}


int32_t check_correct_of_movement(
    struct ChessGame *global, uint8_t *opos, uint8_t *npos) {
    if (*opos == *npos) {
        printf("ERROR_MINOR_MOVE\n");
        return ERROR_MINOR_MOVE;
    }
    if (global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side == global->user_side) {
        printf("ERROR_INPUT_FRIENDLY_ATTACK\n");
        return ERROR_INPUT_FRIENDLY_ATTACK;
    }
    if (global->ChessBoard[OPOS_XP][OPOS_YP]->attacked == -1 &&
            check_hidden_king_attack(global, opos)) {
        printf("Failure check_correct_of_movement\n");
        return -1;
    }
    
    switch (global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type) {
        case pawn: { return check_pawn_move(global->ChessBoard, opos, npos); }
        case knight: {
            if (((abs(OPOS_XP - NPOS_XP) == 1 && abs(OPOS_YP - NPOS_YP) == 2) ||
                ((abs(OPOS_XP - NPOS_XP) == 2 && abs(OPOS_YP - NPOS_YP) == 1))))
                break;
            return ERROR_KNIGHT_INCORRECT_MOVE;
        }
        case king: {
            if (global->ChessBoard[NPOS_XP][NPOS_YP]->attacked < 0) {
                new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
                return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
            }
            if (abs(OPOS_XP - NPOS_XP) > 1 || abs(NPOS_YP - OPOS_YP) > 1 ) {
                new_debug_record("ERROR_KING_LEN_MORE_THEN_1");
                return ERROR_KING_LEN_MORE_THEN_1;
            }
            break;
        }
        case queen: {}
        case rook: {
            if (OPOS_XP == NPOS_XP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_YP) != NULL) {
                    new_debug_record("ERROR_BETWEEN_OPOS_NPOS_PIECES");
                    return ERROR_BETWEEN_OPOS_NPOS_PIECES;
                }
                else
                    break;
            }
            else if (OPOS_YP == NPOS_YP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_XP) != NULL) {
                    new_debug_record("ERROR_BETWEEN_OPOS_NPOS_PIECES");
                    return ERROR_BETWEEN_OPOS_NPOS_PIECES;
                }
                else {
                    break;
                }
            }
            else {
                new_debug_record("ERROR_ROOK_INCORRECT_MOVE");
                return ERROR_ROOK_INCORRECT_MOVE;
            }
        }
        case bishop: {}
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
    new_debug_record("Successful check_correct_of_movement %d %d", 
            *opos, *npos);
    return 0;
}
