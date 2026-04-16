#include "../include/CHESSutil.h"
#include "../include/CHESSsession.h"
#include "../include/CHESSlogging.h"

#include <stdio.h>
#include <stdlib.h>


static uint8_t
opos_update (struct square ***ChessBoard, uint8_t *opos)
{
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


static enum attack_t
npos_atkd_t_sq_upd (enum color ocolor, enum attack_t type)
{
    if (type == safe_square) return ocolor == white ? by_white :   by_black;
    if (type == by_black) return ocolor == white ? both_attacked : by_black;
    if (type == by_white) return ocolor == black ? both_attacked : by_white;
    return safe_square;
}


static uint8_t
npos_update (struct square ***ChessBoard, struct piece *oobj, uint8_t *npos)
{
    switch (ChessBoard[NPOS_XP][NPOS_YP]->obj.type) {
        case pawn: {
        }
        case knight: {
            break;
            if (NPOS_XP > 1) {
                if (NPOS_YP == 0) {
                               ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked);
                }
            else if (NPOS_YP == 7) {
                               ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked);
            }
            else {
                               ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked);

                               ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked);
            }

                               ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked);

                               ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked = 
    npos_atkd_t_sq_upd(oobj->side, ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked);
                }
            }
            if (NPOS_XP < 6) {

            }
        case king: {}
        case queen: {}
        case rook: {}
        case bishop: {break;}
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
}


void
user_move (struct ChessGame *global, uint8_t *opos, uint8_t *npos)
{
    npos_update(global->ChessBoard, &global->ChessBoard[OPOS_XP][OPOS_YP]->obj, npos);
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.type =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type;
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side;

    opos_update(global->ChessBoard, opos);
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type = empty;
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side = none;
}


static uint8_t
find_figure(struct square ***ChessBoard, enum color pcolor,
            enum type_pieces type)
{
    for (uint8_t i = 0; i < 8; ++i)
        for (uint8_t j = 0; j < 8; ++j)
            if (ChessBoard[i][j]->obj.type == type &&
                    ChessBoard[i][j]->obj.side == pcolor) {
                return ChessBoard[i][j]->pos;
            }
    return 255;
}


static struct square *
check_between_direct_line (struct square ***ChessBoard, uint8_t *opos,
                            uint8_t *npos, uint8_t opos_arg)
{
    if (opos_arg == OPOS_XP) {
        uint8_t min = (OPOS_YP < NPOS_YP) ? OPOS_YP : NPOS_YP;
        uint8_t max = (OPOS_YP > NPOS_YP) ? OPOS_YP : NPOS_YP;
        for (uint8_t between = min + 1; between < max; ++between)
            if (ChessBoard[OPOS_XP][between]->obj.type != empty)
                return ChessBoard[OPOS_XP][between];
    }
    else {
        uint8_t min = (OPOS_XP < NPOS_XP) ? OPOS_XP : NPOS_XP;
        uint8_t max = (OPOS_XP > NPOS_XP) ? OPOS_XP : NPOS_XP;
        for (uint8_t between = min + 1; between < max; ++between)
            if (ChessBoard[between][OPOS_YP]->obj.type != empty)
                return ChessBoard[between][OPOS_YP];
    }
    return NULL;
}

static struct square *
check_between_diagonal (struct square ***ChessBoard, uint8_t *opos,
                        uint8_t *npos)
{
    uint8_t min = (OPOS_XP < NPOS_XP) ? *opos - 1 : *npos - 1;
    if (*opos - 1 == min) printf("opos is min\n");
    else printf("opos is max\n");
    uint8_t max = (OPOS_XP > NPOS_XP) ? *opos - 1 : *npos - 1;
    printf("min %d max %d\n", min, max);
    if (min % 8 < max % 8) {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag) {
            if (ChessBoard[diag][min % 8 + (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 + (diag - (min / 8))];
        }
    }
    else {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag) {
            new_debug_record("%d", ChessBoard[diag][min % 8 - (diag - (min / 8))]->obj.type);
            if (ChessBoard[diag][min % 8 - (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 - (diag - (min / 8))];
        }
    }
    printf("DIAGONAL CHECKING END WITHOUT FIGURE\n");
    return NULL;
}


static uint8_t check_hidden_king_attack(
        struct ChessGame *global, uint8_t *opos) {
    uint8_t kpos = find_figure(global->ChessBoard, global->user_side, king);
    if (kpos > 64) {
        printf("KING IS MISSING\n");
        exit(-1);
    }
    if (OPOS_XP == kpos / 8) { // horizontal
        printf("HORIZONTAL\n");
        kpos++;
        struct square *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos, OPOS_XP);
        // segm fault because kpos in [0, 63]. Func use - 1 for opos/npos
        kpos--;
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
        kpos++;
        struct square *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos, OPOS_YP);
        // segm fault because kpos in [0, 63]. Func use - 1 for opos/npos
        kpos--;
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
        ++kpos;
        struct square *non_empty = check_between_diagonal(
                global->ChessBoard, opos, &kpos);
        --kpos;
        if (non_empty == NULL) {
            if (OPOS_XP < kpos / 8) {
                if (OPOS_YP < kpos % 8) {
                    new_debug_record("diagonal loop #1");
                    for (int8_t diag = OPOS_XP - 1; diag >= 0 && OPOS_YP - (OPOS_XP - diag) >= 0; --diag) {
                        // new_debug_record("%d", global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)]->obj.type);
                        if (global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)]->obj.type != empty) {
                            non_empty = global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)];
                            break;
                        }
                    }
                }
                else {
                    new_debug_record("diagonal loop #2");
                    for (int8_t diag = OPOS_XP - 1; diag >= 0 && OPOS_YP + (OPOS_XP - diag) < 8; --diag)
                        if (global->ChessBoard[diag][OPOS_YP + (OPOS_XP - diag)]->obj.type != empty) {
                            non_empty = global->ChessBoard[diag][OPOS_YP + (OPOS_XP - diag)];
                            break;
                        }
                }
            }
            else {
                new_debug_record("diagonal loop #3");
                if (OPOS_YP < kpos % 8) {
                    for (int8_t diag = OPOS_XP + 1; diag < 8 && OPOS_YP - (diag - OPOS_XP) >= 0; diag++) 
                        if (global->ChessBoard[diag][OPOS_YP - (diag - OPOS_XP)]->obj.type != empty) {
                            non_empty = global->ChessBoard[diag][OPOS_YP - (diag - OPOS_XP)];
                            break;
                        }
                }
                else {
                    new_debug_record("diagonal loop #4");
                    for (int8_t diag = OPOS_XP + 1; diag < 8 && OPOS_YP + (diag - OPOS_XP) < 8; diag++) 
                        if (global->ChessBoard[diag][OPOS_YP + (diag - OPOS_XP)]->obj.type != empty) {
                            non_empty = global->ChessBoard[diag][OPOS_YP + (diag - OPOS_XP)];
                            break;
                        }
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
static int32_t
check_pawn_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    if (OPOS_YP != NPOS_YP) { // atacking
        if ((OPOS_XP == 1 || OPOS_XP == 6)) {}
    }
    else {
        if (abs(OPOS_XP - NPOS_XP) > 2) 
            return ERROR_PAWN_LEN_MORE_THEN_2;
        else if (ChessBoard[NPOS_XP][NPOS_YP]->obj.type != empty)
            return ERROR_PAWN_MEET_BARRIER;
        else {

        }
    }
    return 0;
}


int32_t
check_correct_of_movement (struct ChessGame *global, uint8_t *opos,
                            uint8_t *npos)
{
    if (*opos == *npos) {
        printf("ERROR_MINOR_MOVE\n");
        return ERROR_MINOR_MOVE;
    }
    if (global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side != global->user_side)
    {
        printf("ERROR_MOVE_FIGURE_OF_OTHER_SIDE\n");
        return ERROR_FOREIGN_PIECES;
    }
    if (global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side
            == global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side) {
        printf("ERROR_INPUT_FRIENDLY_ATTACK\n");
        return ERROR_INPUT_FRIENDLY_ATTACK;
    }
    if  (global->ChessBoard[OPOS_XP][OPOS_YP]->attacked ==
        (global->user_side == white ? by_black : by_white) &&
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
            if (global->ChessBoard[NPOS_XP][NPOS_YP]->attacked ==
                (global->ChessBoard[OPOS_XP][NPOS_YP]->obj.side ==
                 white ? by_black : by_white)) {
                new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
                return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
            }
            if (abs(OPOS_XP - NPOS_XP) > 1 || abs(NPOS_YP - OPOS_YP) > 1 ) {
                new_debug_record("ERROR_KING_LEN_MORE_THEN_1");
                return ERROR_KING_LEN_MORE_THEN_1;
            }
            break;
        }
        case queen: {
            if (OPOS_XP == NPOS_XP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_XP) != NULL) {
                    new_debug_record("XP ERROR_BETWEEN_OPOS_NPOS_PIECES");
                    return ERROR_BETWEEN_OPOS_NPOS_PIECES;
                }
                else {
                    break;
                }
            }
            else if (OPOS_YP == NPOS_YP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_YP) != NULL) {
                    new_debug_record("YP ERROR_BETWEEN_OPOS_NPOS_PIECES");
                    return ERROR_BETWEEN_OPOS_NPOS_PIECES;
                }
                else {
                    break;
                }
            }
            else if (abs(OPOS_XP - NPOS_XP) == abs(OPOS_YP - NPOS_YP)) {
                if (check_between_diagonal(global->ChessBoard, opos, npos)) {
                    new_debug_record("A piece between diagonal move queen");
                    return ERROR_QUEEN_MOVE_THROUGHOUT_A_PIECE;
                }
            }
            else {
                new_debug_record("ERROR_QUEEN_INCORRECT_MOVE");
                return ERROR_QUEEN_INCORRECT_MOVE;
            }
            break; 
        }
        case rook: {
            if (OPOS_XP == NPOS_XP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_XP) != NULL) {
                    new_debug_record("XP ERROR_BETWEEN_OPOS_NPOS_PIECES");
                    return ERROR_BETWEEN_OPOS_NPOS_PIECES;
                }
                else
                    break;
            }
            else if (OPOS_YP == NPOS_YP) {
                if (check_between_direct_line(global->ChessBoard, opos, npos,
                            OPOS_YP) != NULL) {
                    new_debug_record("YP ERROR_BETWEEN_OPOS_NPOS_PIECES");
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
        case bishop: {
            if (abs(OPOS_XP - NPOS_XP) != abs(OPOS_YP - NPOS_YP)) {
                new_debug_record("Incorrect bishop move");
                return ERROR_BISHOP_INCORRECT_MOVE;
            }
            if (check_between_diagonal(global->ChessBoard, opos, npos)) {
                new_debug_record("A piece between diagonal move bishop");
                return ERROR_BISHOP_MOVE_THROUGHOUT_A_PIECE;
            }
            break;
        }
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
    new_debug_record("Successful check_correct_of_movement %d %d", 
            *opos, *npos);
    return 0;
}
