#include "../include/CHESSpieces.h"
#include "../include/CHESSutil.h"
#include "../include/CHESSlogging.h"

#include <stdlib.h>


enum attack_t
square_state_upd (enum color color, enum attack_t type)
{
    switch (type)
    {
        case safe_square:
            return color == white ? by_white      : by_black;
        case by_black:
            return color == white ? both_attacked : by_black;
        case by_white:
            return color == black ? both_attacked : by_white;
        case both_attacked:
            return both_attacked;
    }
}


uint8_t
knight_npos_update (struct square ***ChessBoard, struct piece *obj,
                    uint8_t *npos)
{
    if (NPOS_XP > 1)
    {
        if (NPOS_YP == 0)
        {
            ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked);
        }
        else if (NPOS_YP == 7)
        {
            ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked);
        }
        else
        {
            ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 2][NPOS_YP - 1]->attacked);

            ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 2][NPOS_YP + 1]->attacked);
        }
    }
    if (NPOS_XP < 6)
    {
        if (NPOS_YP == 0)
        {
            ChessBoard[NPOS_XP + 2][NPOS_YP + 1]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP + 2][NPOS_YP + 1]->attacked);
        }

        else if (NPOS_YP == 7)
        {
            ChessBoard[NPOS_XP + 2][NPOS_YP - 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP + 2][NPOS_YP - 1]->attacked);
        }
        else
        {
            ChessBoard[NPOS_XP + 2][NPOS_YP - 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP + 2][NPOS_YP - 1]->attacked);

            ChessBoard[NPOS_XP + 2][NPOS_YP + 1]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP + 2][NPOS_YP + 1]->attacked);
        }
    }
    if (NPOS_YP > 1)
    {
        if (NPOS_XP == 0)
        {
            ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked);
        }
        else if (NPOS_XP == 7)
        {
            ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked);
        }
        else
        {
            ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP + 1][NPOS_YP - 2]->attacked);

            ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 1][NPOS_YP - 2]->attacked);
        }
    }
    if (NPOS_YP < 6)
    {
        if (NPOS_XP == 0)
        {
            ChessBoard[NPOS_XP + 1][NPOS_YP + 2]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP + 1][NPOS_YP + 2]->attacked);
        }
        else if (NPOS_XP == 7)
        {
            ChessBoard[NPOS_XP - 1][NPOS_YP + 2]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 1][NPOS_YP + 2]->attacked);
        }
        else
        {
            ChessBoard[NPOS_XP + 1][NPOS_YP + 2]->attacked = 
            square_state_upd(obj->side, 
                    ChessBoard[NPOS_XP + 1][NPOS_YP + 2]->attacked);

            ChessBoard[NPOS_XP - 1][NPOS_YP + 2]->attacked = 
            square_state_upd(obj->side,
                    ChessBoard[NPOS_XP - 1][NPOS_YP + 2]->attacked);
        }
    }
    return 0;
}


uint8_t
pawn_npos_update    (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);
uint8_t
bishop_npos_update  (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);
uint8_t
knight_npos_update  (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);
uint8_t
rook_npos_update    (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);
uint8_t
queen_npos_update   (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);
uint8_t
king_npos_update    (struct square ***ChessBoard,
                    struct piece *obj, uint8_t *npos);


uint8_t
pawn_opos_update (struct square ***ChessBoard, uint8_t *opos);
uint8_t
bishop_opos_update (struct square ***ChessBoard, uint8_t *opos);
uint8_t
knight_opos_update (struct square ***ChessBoard, uint8_t *opos)
{
    
}
uint8_t
rook_opos_update (struct square ***ChessBoard, uint8_t *opos);
uint8_t
queen_opos_update (struct square ***ChessBoard, uint8_t *opos);
uint8_t
king_opos_update (struct square ***ChessBoard, uint8_t *opos);
