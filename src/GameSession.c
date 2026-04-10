#include "../include/chessUtilities.h"
#include "../include/GameSession.h"

#include <stdio.h>
#include <stdlib.h>

void user_move(
        struct ChessGame *global, uint8_t *opos, uint8_t *npos
        ) {
    // for a while just replace. In the future I add completly checking correct
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.type =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type;
    global->ChessBoard[NPOS_XP][NPOS_YP]->obj.side =
        global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side;

    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type = empty;
    global->ChessBoard[OPOS_XP][OPOS_YP]->obj.side = none;
}


uint8_t find_figure(struct square ***ChessBoard, 
            enum color pcolor, enum type_pieces type) {
    for (uint8_t i = 0; i < 8; ++i)
        for (uint8_t j = 0; j < 8; ++j)
            if (ChessBoard[i][j]->obj.type == type &&
                    ChessBoard[i][j]->obj.side == pcolor)
                return ChessBoard[i][j]->pos;
    return 255;
}


static uint8_t check_hidden_king_attack(
        struct ChessGame *global, uint8_t *opos) {
    uint8_t kpos = find_figure(global->ChessBoard, global->user_side, king);
    if (kpos > 64) {
        printf("KING IS MISSING\n");
        exit(-1);
    }
    if (OPOS_XP == (kpos - 1) / 8) {
        printf("VERTICAL\n");
        struct square *non_empty = NULL;
        if (OPOS_YP < (kpos - 1) % 8)
            for (uint8_t horiz = OPOS_YP - 1; horiz >= 0; --horiz) {
                if (global->ChessBoard[OPOS_YP][horiz]->obj.type != empty) {
                    non_empty = global->ChessBoard[OPOS_YP][horiz];
                    break;
                }
            }
        else
            for (uint8_t horiz = OPOS_YP + 1; horiz < 8; 
                    ++horiz) {
                if (global->ChessBoard[OPOS_YP][horiz]->obj.type != empty) {
                    non_empty = global->ChessBoard[OPOS_YP][horiz];
                    break;
                }
            }
        if (non_empty != NULL)
            printf("non_empty_is %d %d %d\n", non_empty->obj.type, non_empty->obj.side, non_empty->pos - 1);
        else printf("NULL %d\n", __LINE__);
        if (non_empty == NULL || non_empty->obj.side == global->user_side)
            return 0;
        if (non_empty->obj.type == rook || non_empty->obj.type == queen)
            return 1;
        printf("It's correct move \n");
        return 0;
    }

    if (OPOS_YP == (kpos - 1) % 8) {
        printf("HORIZONTAL\n");
        struct square *non_empty = NULL;
        if (OPOS_XP < (kpos - 1) / 8)
            for (uint8_t vert = OPOS_XP - 1; vert >= 0; --vert) {
                if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                    non_empty = global->ChessBoard[vert][OPOS_YP];
                    break;
                }
            }
        else
            for (uint8_t vert = OPOS_YP + 1; vert < 8; 
                    ++vert) {
                if (global->ChessBoard[vert][OPOS_YP]->obj.type != empty) {
                    non_empty = global->ChessBoard[vert][OPOS_YP];
                    break;
                }
            }
        if (non_empty != NULL)
            printf("non_empty_is %d %d %d\n", non_empty->obj.type, non_empty->obj.side, non_empty->pos - 1);
        else printf("NULL %d\n", __LINE__);
        if (non_empty == NULL || non_empty->obj.side == global->user_side)
            return 0;
        if (non_empty->obj.type == rook || non_empty->obj.type == queen)
            return 2;
        printf("It's correct move \n");
        return 0;
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
    
    if (global->ChessBoard[OPOS_XP][OPOS_YP]->attacked == -1) {
        printf("Failure check_correct_of_movement\n");
        return check_hidden_king_attack(global, opos);
    }
    // if (global->ChessBoard[OPOS_XP][OPOS_YP] &&
    //         check_hidden_king_attack(global->ChessBoard, opos))
    // switch(global->ChessBoard[OPOS_XP][OPOS_YP]->obj.type) {

    // }
    printf("Successful check_correct_of_movement\n");
    return 0;
}
