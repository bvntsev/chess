#include "../include/CHESSutil.h"
#include "../include/CHESSsession.h"
#include "../include/CHESSlogging.h"
#include "../include/CHESSpieces.h"

#include <stdio.h>
#include <stdlib.h>

// last_move = {opos, npos}
extern uint8_t last_move[2];


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


static struct piece *
check_between_direct_line (struct square ***ChessBoard, uint8_t *opos,
                            uint8_t *npos)
{
    if (NPOS_XP == OPOS_XP) {
        uint8_t min = (OPOS_YP < NPOS_YP) ? OPOS_YP : NPOS_YP;
        uint8_t max = (OPOS_YP > NPOS_YP) ? OPOS_YP : NPOS_YP;
        for (uint8_t between = min + 1; between < max; ++between)
            if (ChessBoard[OPOS_XP][between]->obj.type != empty)
                return &ChessBoard[OPOS_XP][between]->obj;
    }
    else {
        uint8_t min = (OPOS_XP < NPOS_XP) ? OPOS_XP : NPOS_XP;
        uint8_t max = (OPOS_XP > NPOS_XP) ? OPOS_XP : NPOS_XP;
        printf("%d %d\n", min, max); 
        for (uint8_t between = min + 1; between < max; ++between)
        {
            printf("%d %d %d\n", min, between, max); 
            if (ChessBoard[between][OPOS_YP]->obj.type != empty)
                return &ChessBoard[between][OPOS_YP]->obj;
        }
    }
    return NULL;
}


static struct square *
check_between_diagonal (struct square ***ChessBoard, uint8_t *opos,
                        uint8_t *npos)
{
    uint8_t min = (OPOS_XP < NPOS_XP) ? *opos - 1 : *npos - 1;
    uint8_t max = (OPOS_XP > NPOS_XP) ? *opos - 1 : *npos - 1;
    if (min % 8 < max % 8)
    {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag)
        {
            if (ChessBoard[diag][min % 8 + (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 + (diag - (min / 8))];
        }
    }
    else
    {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag)
        {
            if (ChessBoard[diag][min % 8 - (diag - (min / 8))]->obj.type
                    != empty)
                return ChessBoard[diag][min % 8 - (diag - (min / 8))];
        }
    }
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
        struct piece *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos);
        // segm fault because kpos in [0, 63]. Func use - 1 for opos/npos
        kpos--;
        if (non_empty == NULL) {
            if (OPOS_YP > kpos % 8) {
                for (uint8_t horiz = OPOS_YP + 1; horiz < 8; ++horiz) {
                    if (global->ChessBoard[OPOS_XP][horiz]->obj.type != empty) {
                        non_empty = &global->ChessBoard[OPOS_XP][horiz]->obj;
                        break;
                    }
                }
            }
            else {
                for (int8_t horiz = OPOS_YP - 1; horiz >= 0; --horiz) {
                    if (global->ChessBoard[OPOS_XP][horiz]->obj.type != empty) {
                        non_empty = &global->ChessBoard[OPOS_XP][horiz]->obj;
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->side == global->user_side)
            return 0;
        if (non_empty->type == rook || non_empty->type == queen)
            return 1;
        printf("It's correct move \n");
        return 0;
    }

    if (OPOS_YP == kpos % 8) { // vertical
        printf("VERTICAL\n");
        kpos++;
        struct piece *non_empty = check_between_direct_line(
                        global->ChessBoard, opos, &kpos);
        // segm fault because kpos in [0, 63]. Func use - 1 for opos/npos
        kpos--;
        if (non_empty == NULL) {
            if (OPOS_XP < kpos / 8) {
                for (int8_t vert = OPOS_XP - 1; vert >= 0; --vert) {
                    if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                        non_empty = &global->ChessBoard[vert][OPOS_YP]->obj;
                        break;
                    }
                }
            }
            else {
                for (uint8_t vert = OPOS_XP + 1; vert < 8; ++vert) {
                    if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                        non_empty = &global->ChessBoard[vert][OPOS_YP]->obj;
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->side == global->user_side)
            return 0;
        if (non_empty->type == rook || non_empty->type == queen)
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


static int32_t
check_pawn_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    if ((NPOS_XP - OPOS_XP >= 0 && ChessBoard[OPOS_XP][OPOS_YP]->obj.side
            == white)
        || (OPOS_XP - NPOS_XP >= 0 && ChessBoard[OPOS_XP][OPOS_YP]->obj.side
            == black))
    {
        new_debug_record("ERROR_PAWN_ILLEGAL_DIRECTION");
        return ERROR_PAWN_ILLEGAL_DIRECTION;
    }
    else if (abs(OPOS_XP - NPOS_XP) > 2)
    {
        new_debug_record("ERROR_PAWN_LEN_MORE_THEN_2");
        return ERROR_PAWN_LEN_MORE_THEN_2;
    }
    else if (OPOS_YP != NPOS_YP) // attacking
    {
        if (abs(OPOS_YP - NPOS_YP) > 1 || abs(OPOS_XP - NPOS_XP) > 1)
        {
            new_debug_record("ERROR_PAWN_ILLEGAL_DIAGONAL_ATTACK");
            return ERROR_PAWN_ILLEGAL_DIAGONAL_ATTACK;
        }
        if (ChessBoard[NPOS_XP][NPOS_YP]->obj.type == empty)
        {
            if (ChessBoard[(last_move[1] - 1) / 8][(last_move[1] - 1) % 8]
                    ->obj.type == pawn)
            {
                if (abs(((last_move[1] - 1) / 8) - ((last_move[0] - 1) / 8))
                        == 2)
                    if (NPOS_YP == ((last_move[1] - 1) % 8))
                        return 0;
            }
            new_debug_record("ERROR_PAWN_ATTACK_EMPTY_SQUARE");
            return ERROR_PAWN_ATTACK_EMPTY_SQUARE;
        }
    }
    else
    {
        if (abs(OPOS_XP - NPOS_XP) == 2)
        {
            if ((OPOS_XP == 1 && ChessBoard[OPOS_XP][OPOS_YP]->obj.side
                    == black) 
                || (OPOS_XP == 6 && ChessBoard[OPOS_XP][OPOS_YP]->obj.side
                    == white))
            {
                new_debug_record("a pawn move in 2 square is correct");
                return 0;
            }
            else
            {
                new_debug_record("ERROR_PAWN_MOVE_2_NOT_IN_START");
                return ERROR_PAWN_MOVE_2_NOT_IN_START;
            }
        }
        if (ChessBoard[NPOS_XP][NPOS_YP]->obj.type != empty)
        {
            new_debug_record("ERROR_PAWN_MEET_BARRIER");
            return ERROR_PAWN_MEET_BARRIER;
        }
    }
    return 0;
}


static int32_t
check_bishop_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    if (abs(OPOS_XP - NPOS_XP) != abs(OPOS_YP - NPOS_YP))
    {
        new_debug_record("Incorrect bishop move");
        return ERROR_BISHOP_INCORRECT_MOVE;
    }
    if (check_between_diagonal(ChessBoard, opos, npos))
    {
        new_debug_record("A piece between diagonal move bishop");
        return ERROR_BISHOP_MOVE_THROUGHOUT_A_PIECE;
    }
    return 0;
}


static int32_t
check_knight_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{

    if (((abs(OPOS_XP - NPOS_XP) == 1 && abs(OPOS_YP - NPOS_YP) == 2)
        || ((abs(OPOS_XP - NPOS_XP) == 2 && abs(OPOS_YP - NPOS_YP) == 1))))
        return 0;
    return ERROR_KNIGHT_INCORRECT_MOVE;
}


static int32_t
check_rook_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    if (OPOS_XP == NPOS_XP)
    {
        if (check_between_direct_line(ChessBoard, opos, npos) != NULL)
        {
            new_debug_record("XP ERROR_BETWEEN_OPOS_NPOS_PIECES");
            return ERROR_BETWEEN_OPOS_NPOS_PIECES;
        }
        else
            return 0;
    }
    else if (OPOS_YP == NPOS_YP)
    {
        if (check_between_direct_line(ChessBoard, opos, npos) != NULL)
        {
            new_debug_record("YP ERROR_BETWEEN_OPOS_NPOS_PIECES");
            return ERROR_BETWEEN_OPOS_NPOS_PIECES;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        new_debug_record("ERROR_ROOK_INCORRECT_MOVE");
        return ERROR_ROOK_INCORRECT_MOVE;
    }
}


static int32_t
check_queen_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    if (OPOS_XP == NPOS_XP)
    {
        if (check_between_direct_line(ChessBoard, opos, npos) != NULL)
        {
            new_debug_record("XP ERROR_BETWEEN_OPOS_NPOS_PIECES");
            return ERROR_BETWEEN_OPOS_NPOS_PIECES;
        }
        else
        {
            return 0;
        }
    }
    else if (OPOS_YP == NPOS_YP)
    {
        if (check_between_direct_line(ChessBoard, opos, npos) != NULL)
        {
            new_debug_record("YP ERROR_BETWEEN_OPOS_NPOS_PIECES");
            return ERROR_BETWEEN_OPOS_NPOS_PIECES;
        }
        else
        {
            return 0;
        }
    }
    else if (abs(OPOS_XP - NPOS_XP) == abs(OPOS_YP - NPOS_YP))
    {
        if (check_between_diagonal(ChessBoard, opos, npos))
        {
            new_debug_record("A piece between diagonal move queen");
            return ERROR_QUEEN_MOVE_THROUGHOUT_A_PIECE;
        }
    }
    else
    {
        new_debug_record("ERROR_QUEEN_INCORRECT_MOVE");
        return ERROR_QUEEN_INCORRECT_MOVE;
    }
    return 0; 
}


static int32_t
check_king_move (struct square ***ChessBoard, uint8_t *opos, uint8_t *npos)
{
    // Bad realization. Code repeat. Need to rewrite in the future
    switch (ChessBoard[NPOS_XP][NPOS_YP]->attacked)
    {
        case by_white:
            {
                if (ChessBoard[OPOS_XP][OPOS_YP]->obj.side == black)
                    {
                        new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
                        return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
                    }
                break;
            }
        case by_black:
            {
                if (ChessBoard[OPOS_XP][OPOS_YP]->obj.side == white)
                    {
                        new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
                        return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
                    }
                break;
            }
        case both_attacked:
            {
                new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
                return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
            }
        default:
            break;
    }
    if (abs(OPOS_XP - NPOS_XP) > 1 || abs(NPOS_YP - OPOS_YP) > 1)
    {
        new_debug_record("ERROR_KING_LEN_MORE_THEN_1");
        return ERROR_KING_LEN_MORE_THEN_1;
    }
    return 0;
}


// TODO: Need to make a best efficient system for checking square on attacking
// by pieces on the board (need to add check every square on attacked which
// have been attack by move figure)
static uint8_t
opos_update (struct square ***ChessBoard, uint8_t *opos)
{
    struct piece *non_empty;
    uint8_t edge;
    enum attack_t type;
/* square_state_upd(); */
    edge = ((*opos / 8) + 1) * 8;
    non_empty = check_between_direct_line(ChessBoard, opos, &edge); 
    /* if (non_empty && (non_empty->type == queen || non_empty->type == rook)) */
}


// TODO: end this function using kngiht_npos_update as example
static uint8_t
npos_update (struct square ***ChessBoard, struct piece *obj, uint8_t *npos)
{
    switch (obj->type)
    {
        case pawn:
        {
            break;
        }
        case knight: return knight_npos_update(ChessBoard, obj, npos);
        case king: {}
        case queen: {}
        case rook: {}
        case bishop: {break;}
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
}


void
user_move(struct ChessGame *global, uint8_t *opos, uint8_t *npos)
{
    opos_update(global->ChessBoard, opos);
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type = empty;
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side = none;
    
    npos_update(global->ChessBoard,
            &global->ChessBoard[OPOS_XP][OPOS_YP]->obj, npos);
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.type =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type;
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side;
}


int32_t
check_correct_of_movement (struct ChessGame *global, uint8_t *opos,
                            uint8_t *npos)
{
    printf("last move - %d %d\n", last_move[0], last_move[1]);
    if (*opos == *npos)
    {
        printf("ERROR_MINOR_MOVE\n");
        return ERROR_MINOR_MOVE;
    }
    if (global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side
            == global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side)
    {
        printf("ERROR_INPUT_FRIENDLY_ATTACK\n");
        return ERROR_INPUT_FRIENDLY_ATTACK;
    }
    if  (global->ChessBoard[OPOS_XP][OPOS_YP]->attacked ==
        (global->user_side == white ? by_black : by_white) &&
            check_hidden_king_attack(global, opos))
    {
        printf("Failure check_correct_of_movement\n");
        return -1;
    }
    
    switch (global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type)
    {
        case pawn:
        {
            return check_pawn_move(global->ChessBoard, opos, npos);
        }
        case knight:
        {
            return check_knight_move(global->ChessBoard, opos, npos);
        }
        case king:
        {
            return check_king_move(global->ChessBoard, opos, npos);
        }
        case queen:
        {
            return check_queen_move(global->ChessBoard, opos, npos);
        }
        case rook:
        {
            return check_rook_move(global->ChessBoard, opos, npos);
        }
        case bishop:
        {
            return check_bishop_move(global->ChessBoard, opos, npos);
        }
        case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
    return 0;
}
