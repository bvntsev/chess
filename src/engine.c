#include "../include/util.h"
#include "../include/engine.h"
#include "../include/logging.h"
#include "../include/attack_update.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>


static void
print_square_info(struct square *sq) {
	printf("side %d ", 	sq->side);
	printf("b_attack %d ",  sq->b_attack);
	printf("w_attack %d ",  sq->w_attack);
	printf("obj.type %d ",  sq->obj.type);
	printf("obj.side %d\n", sq->obj.side);
}


static struct piece *
check_between_direct_line (struct square (*board)[8], uint8_t *opos,
						   uint8_t *npos)
{
    if (NPOS_XP == OPOS_XP) {
        uint8_t min = (OPOS_YP < NPOS_YP) ? OPOS_YP : NPOS_YP;
        uint8_t max = (OPOS_YP > NPOS_YP) ? OPOS_YP : NPOS_YP;
        for (uint8_t between = min + 1; between < max; ++between)
            if (board[OPOS_XP][between].obj.type != empty)
                return &board[OPOS_XP][between].obj;
    }
    else {
        uint8_t min = (OPOS_XP < NPOS_XP) ? OPOS_XP : NPOS_XP;
        uint8_t max = (OPOS_XP > NPOS_XP) ? OPOS_XP : NPOS_XP;
        for (uint8_t between = min + 1; between < max; ++between)
        {
            if (board[between][OPOS_YP].obj.type != empty)
                return &board[between][OPOS_YP].obj;
        }
    }
    return NULL;
}


static struct square *
check_between_diagonal (struct square (*board)[8], uint8_t *opos,
                        uint8_t *npos)
{
    uint8_t min = (OPOS_XP < NPOS_XP) ? *opos - 1 : *npos - 1;
    uint8_t max = (OPOS_XP > NPOS_XP) ? *opos - 1 : *npos - 1;
    if (min % 8 < max % 8)
    {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag)
        {
            if (board[diag][min % 8 + (diag - (min / 8))].obj.type
				!= empty)
                return &board[diag][min % 8 + (diag - (min / 8))];
        }
    }
    else
    {
        for (uint8_t diag = min / 8 + 1; diag < max / 8; ++diag)
        {
            if (board[diag][min % 8 - (diag - (min / 8))].obj.type
				!= empty)
                return &board[diag][min % 8 - (diag - (min / 8))];
        }
    }
    return NULL;
}


static uint8_t check_hidden_king_attack(
    struct chess *global, uint8_t *opos) {
	uint8_t kpos = global->board[OPOS_XP][OPOS_YP].obj.side == white
		? global->kpos_w
		: global->kpos_b;
  
    if (OPOS_XP == KPOS_X) { // horizontal
        struct piece *non_empty = check_between_direct_line(
			global->board, opos, &kpos);
        if (non_empty == NULL) {
            if (OPOS_YP > KPOS_Y) {
                for (uint8_t horiz = OPOS_YP + 1; horiz < 8; ++horiz) {
                    if (global->board[OPOS_XP][horiz].obj.type != empty) {
                        non_empty = &global->board[OPOS_XP][horiz].obj;
                        break;
                    }
                }
            }
            else {
                for (int8_t horiz = OPOS_YP - 1; horiz >= 0; --horiz) {
                    if (global->board[OPOS_XP][horiz].obj.type != empty) {
                        non_empty = &global->board[OPOS_XP][horiz].obj;
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->side == global->player_side)
            return 0;
        if (non_empty->type == rook || non_empty->type == queen)
            return 1;
        printf("It's correct move \n");
        return 0;
    }

    if (OPOS_YP == KPOS_Y) { // vertical
        struct piece *non_empty = check_between_direct_line(
			global->board, opos, &kpos);
        if (non_empty == NULL) {
            if (OPOS_XP < KPOS_X) {
                for (int8_t vert = OPOS_XP - 1; vert >= 0; --vert) {
                    if (global->board[vert][OPOS_YP].obj.type != empty) {
                        non_empty = &global->board[vert][OPOS_YP].obj;
                        break;
                    }
                }
            }
            else {
                for (uint8_t vert = OPOS_XP + 1; vert < 8; ++vert) {
                    if (global->board[vert][OPOS_YP].obj.type != empty) {
                        non_empty = &global->board[vert][OPOS_YP].obj;
                        break;
                    }
                }
            }
        }
        if (non_empty == NULL || non_empty->side == global->player_side)
            return 0;
        if (non_empty->type == rook || non_empty->type == queen)
            return 2;
        return 0;
    }
    // DIAGONAL
    if (abs(OPOS_XP - KPOS_X) == abs(OPOS_YP - KPOS_Y)) {
        struct square *non_empty = check_between_diagonal(
			global->board, opos, &kpos);
	
        if (non_empty == NULL) {
            if (OPOS_XP < KPOS_X) {
                if (OPOS_YP < KPOS_Y) {
                    DEBUG_MSG("diagonal loop #1");
                    for (int8_t diag = OPOS_XP - 1;
                         diag >= 0 && OPOS_YP - (OPOS_XP - diag) >= 0; --diag) {
                        // new_debug_record("%d", global->ChessBoard[diag][OPOS_YP - (OPOS_XP - diag)]->obj.type);
                        if (global->board[diag][OPOS_YP - (OPOS_XP - diag)]
							.obj.type != empty) {
							non_empty = &global->
								board[diag][OPOS_YP - (OPOS_XP - diag)];
							break;
                        }
                    }
                }
                else {
                    DEBUG_MSG("diagonal loop #2");
                    for (int8_t diag = OPOS_XP - 1;
                         diag >= 0 && OPOS_YP + (OPOS_XP - diag) < 8; --diag)
						if (global->board[diag][OPOS_YP + (OPOS_XP - diag)]
							.obj.type != empty) {
							non_empty = &global->
								board[diag][OPOS_YP + (OPOS_XP - diag)];
							break;
                        }
                }
            }
            else {
                DEBUG_MSG("diagonal loop #3");
                if (OPOS_YP < KPOS_Y) {
					for (int8_t diag = OPOS_XP + 1;
						 diag < 8 && OPOS_YP - (diag - OPOS_XP) >= 0; diag++)
						if (global->board[diag][OPOS_YP - (diag - OPOS_XP)]
							.obj.type != empty) {
							non_empty = &global->
								board[diag][OPOS_YP - (diag - OPOS_XP)];
							break;
                        }
                }
                else {
                    DEBUG_MSG("diagonal loop #4");
                    for (int8_t diag = OPOS_XP + 1;
                         diag < 8 && OPOS_YP + (diag - OPOS_XP) < 8; diag++)
						if (global->board[diag][OPOS_YP + (diag - OPOS_XP)]
							.obj.type != empty) {
							non_empty = &global->
								board[diag][OPOS_YP + (diag - OPOS_XP)];
							break;
                        }
                }
            }
        }
        if (non_empty == NULL || non_empty->obj.side == global->player_side)
            return 0;
        if (non_empty->obj.type == bishop || non_empty->obj.type == queen)
            return 2;
        return 0;
    }
    return 0;
}


static int32_t
check_pawn_move (struct chess *global, uint8_t *opos, uint8_t *npos)
{
    if ((NPOS_XP - OPOS_XP >= 0 && global->board[OPOS_XP][OPOS_YP].obj.side
		 == white)
        || (OPOS_XP - NPOS_XP >= 0 && global->board[OPOS_XP][OPOS_YP].obj.side
            == black))
    {
        printf("ERROR_PAWN_ILLEGAL_DIRECTION");
        return ERROR_PAWN_ILLEGAL_DIRECTION;
    }
    else if (abs(OPOS_XP - NPOS_XP) > 2)
    {
        printf("ERROR_PAWN_LEN_MORE_THEN_2");
        return ERROR_PAWN_LEN_MORE_THEN_2;
    }
    else if (OPOS_YP != NPOS_YP) // attacking
    {
        if (abs(OPOS_YP - NPOS_YP) > 1 || abs(OPOS_XP - NPOS_XP) > 1)
        {
            printf("ERROR_PAWN_ILLEGAL_DIAGONAL_ATTACK");
            return ERROR_PAWN_ILLEGAL_DIAGONAL_ATTACK;
        }
        if (global->board[NPOS_XP][NPOS_YP].obj.type == empty)
        {
            if (global->board[(global->last_move[1] - 1) / 8][(global->last_move[1] - 1) % 8]
				.obj.type == pawn)
            {
                if (abs(((global->last_move[1] - 1) / 8) - ((global->last_move[0] - 1) / 8))
					== 2)
					if (NPOS_YP == ((global->last_move[1] - 1) % 8)) {
						pawn_pos_update(global->board, &global->board[(global->last_move[1] - 1) / 8][(global->last_move[1] - 1) % 8]
										.obj.side, &global->last_move[1], SQ_UPD_LEAVE);
						global->board[(global->last_move[1] - 1) / 8][(global->last_move[1] - 1) % 8]
							.obj.type = empty;
						global->board[(global->last_move[1] - 1) / 8][(global->last_move[1] - 1) % 8]
							.obj.side = none;
						return 0;
					}                    
            }
            printf("ERROR_PAWN_ATTACK_EMPTY_SQUARE");
            return ERROR_PAWN_ATTACK_EMPTY_SQUARE;
        }
    }
    else
    {
        if (abs(OPOS_XP - NPOS_XP) == 2)
        {
            if ((OPOS_XP == 1 && global->board[OPOS_XP][OPOS_YP].obj.side
				 == black) 
                || (OPOS_XP == 6 && global->board[OPOS_XP][OPOS_YP].obj.side
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
        if (global->board[NPOS_XP][NPOS_YP].obj.type != empty)
        {
            new_debug_record("ERROR_PAWN_MEET_BARRIER");
            return ERROR_PAWN_MEET_BARRIER;
        }
    }
    return 0;
}


static int32_t
check_bishop_move (struct square (*board)[8], uint8_t *opos, uint8_t *npos)
{
    if (abs(OPOS_XP - NPOS_XP) != abs(OPOS_YP - NPOS_YP))
    {
        new_debug_record("Incorrect bishop move");
        return ERROR_BISHOP_INCORRECT_MOVE;
    }
    if (check_between_diagonal(board, opos, npos))
    {
        new_debug_record("A piece between diagonal move bishop");
        return ERROR_BISHOP_MOVE_THROUGHOUT_A_PIECE;
    }
    return 0;
}


static int32_t
check_knight_move (uint8_t *opos, uint8_t *npos)
{

    if (((abs(OPOS_XP - NPOS_XP) == 1 && abs(OPOS_YP - NPOS_YP) == 2)
		 || ((abs(OPOS_XP - NPOS_XP) == 2 && abs(OPOS_YP - NPOS_YP) == 1))))
        return 0;
    return ERROR_KNIGHT_INCORRECT_MOVE;
}


static int32_t
check_rook_move (struct square (*board)[8], uint8_t *opos, uint8_t *npos)
{
    if (OPOS_XP == NPOS_XP)
    {
        if (check_between_direct_line(board, opos, npos) != NULL)
        {
            new_debug_record("XP ERROR_BETWEEN_OPOS_NPOS_PIECES");
            return ERROR_BETWEEN_OPOS_NPOS_PIECES;
        }
        else
            return 0;
    }
    else if (OPOS_YP == NPOS_YP)
    {
        if (check_between_direct_line(board, opos, npos) != NULL)
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
check_queen_move (struct square (*board)[8], uint8_t *opos, uint8_t *npos)
{
    if (OPOS_XP == NPOS_XP)
    {
        if (check_between_direct_line(board, opos, npos) != NULL)
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
        if (check_between_direct_line(board, opos, npos) != NULL)
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
        if (check_between_diagonal(board, opos, npos))
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
check_king_move (struct square (*board)[8], uint8_t *opos, uint8_t *npos)
{
    // Bad realization. Code repeat. Need to rewrite in the future
    switch (get_attack_t(&board[NPOS_XP][NPOS_YP]))
    {
	case by_white:
	{
		if (board[OPOS_XP][OPOS_YP].obj.side == black)
		{
			new_debug_record("ERROR_KING_MOVE_TO_ATTACKED_SQUARE");
			return ERROR_KING_MOVE_TO_ATTACKED_SQUARE; 
		}
		break;
	}
	case by_black:
	{
		if (board[OPOS_XP][OPOS_YP].obj.side == white)
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
static uint8_t clean_the_piece_attack(struct square (*board)[8], uint8_t *pos) {

	switch (board[POS_XP][POS_YP].obj.type) {
	case pawn:
		return pawn_pos_update(board, &board[POS_XP][POS_YP].obj.side,
							   pos, SQ_UPD_LEAVE);
	case knight:
		return knight_pos_update(board, &board[POS_XP][POS_YP].obj.side,
								 pos, SQ_UPD_LEAVE);
	case king: 
		return king_pos_update(board, &board[POS_XP][POS_YP].obj.side,
							   pos, SQ_UPD_LEAVE);
	case queen:
		return queen_pos_update(board, &board[POS_XP][POS_YP].obj.side,
								pos, SQ_UPD_LEAVE);
	case rook:
		return rook_pos_update(board, &board[POS_XP][POS_YP].obj.side,
							   pos, SQ_UPD_LEAVE);
	case bishop:
		return bishop_pos_update(board, &board[POS_XP][POS_YP].obj.side,
								 pos, SQ_UPD_LEAVE);
	case empty: { return 1; }
	}
	/* if (non_empty && (non_empty->type == queen || non_empty->type ==
	 * rook)) */
	return 1;
}


// TODO: end this function using kngiht_npos_update as example
static uint8_t npos_update(struct square (*board)[8], enum color_t side,
                           enum piece_t type, uint8_t *npos) {
	if (side == none)
		return ERROR_INPUT_ABSENT_PIECES;
  
    switch (type) {
    case pawn:
		return pawn_pos_update(board, &side, npos, SQ_UPD_ATTCK);
	case knight:
		return knight_pos_update(board, &side, npos, SQ_UPD_ATTCK);
	case king:
		return king_pos_update(board, &side, npos, SQ_UPD_ATTCK);
	case queen:
		return queen_pos_update(board, &side, npos, SQ_UPD_ATTCK);
	case rook: 
		return rook_pos_update(board, &side, npos, SQ_UPD_ATTCK);
	case bishop:
		return bishop_pos_update(board, &side, npos, SQ_UPD_ATTCK);
    }
	return ERROR_INPUT_ABSENT_PIECES;
}


static uint8_t *check_hidden_attack(struct square (*board)[8], uint8_t *pos,
                                    uint8_t (*upd_func)(struct square *,
                                                              enum color_t *))
{
	DEBUG_MSG("check_hidden_attack open");
	struct piece *non_empty = NULL;

	if (POS_YP != 0) {
		for (uint8_t j = POS_YP + 1; j < 8; ++j)
			if (board[POS_XP][j].obj.type != empty) {
				non_empty = &board[POS_XP][j].obj;
				break;
			}
		if (non_empty &&
			(non_empty->type == rook || non_empty->type == queen)) {
			DEBUG_MSG("loop #1");
			for (int8_t j = POS_YP - 1; j >= 0; --j) {
				upd_func(
					&board[POS_XP][j], &non_empty->side);
				if (board[POS_XP][j].obj.type != empty) break;
			}
		}
		non_empty = NULL;
                
		for (uint8_t j = POS_YP + 1; j < 8 && POS_XP - (j - POS_YP) >= 0; ++j)
			if (board[POS_XP - (j - POS_YP)][j].obj.type != empty) {
				non_empty = &board[POS_XP - (j - POS_YP)][j].obj;
				break;
			}
		if (non_empty &&
			(non_empty->type == bishop || non_empty->type == queen)) {
			for (int8_t j = POS_YP - 1;
				 j >= 0 && POS_XP + (POS_YP - j) < 8; --j) {
				 upd_func(
					&board[POS_XP + (POS_YP - j)][j], &non_empty->side);
				if (board[POS_XP + (POS_YP - j)][j].obj.type != empty) break;
			}
		}
		non_empty = NULL;

		for (uint8_t j = POS_YP + 1; j < 8 && POS_XP + (j - POS_YP) < 8; ++j)
			if (board[POS_XP + (j - POS_YP)][j].obj.type != empty) {
				non_empty = &board[POS_XP + (j - POS_YP)][j].obj;
				break;
			}
		if (non_empty &&
			(non_empty->type == bishop || non_empty->type == queen)) {
			for (int8_t j = POS_YP - 1;
				 j >= 0 && POS_XP - (POS_YP - j) >= 0; --j) {
				upd_func(
					&board[POS_XP - (POS_YP - j)][j], &non_empty->side);
				if (board[POS_XP - (POS_YP - j)][j].obj.type != empty) break;
			}
		}
	}
	non_empty = NULL;     
        
	if (POS_YP != 7) {
		for (int8_t j = POS_YP - 1; j >= 0; --j)
			if (board[POS_XP][j].obj.type != empty) {
				non_empty = &board[POS_XP][j].obj;
				break;
			}           
		if (non_empty && (non_empty->type == rook ||
						  non_empty->type == queen)) {
			DEBUG_MSG("loop #2");
			for (int8_t j = POS_YP + 1; j < 8; ++j) {
				upd_func(
					&board[POS_XP][j], &non_empty->side);
				if (board[POS_XP][j].obj.type != empty) break;
			}
		}
		non_empty = NULL;

		for (int8_t j = POS_YP - 1; j >= 0 && POS_XP - (POS_YP - j) >= 0; --j)
			if (board[POS_XP - (POS_YP - j)][j].obj.type != empty) {
				non_empty = &board[POS_XP - (POS_YP - j)][j].obj;
				break;
			}
		if (non_empty &&
			(non_empty->type == bishop || non_empty->type == queen)) {
			for (uint8_t j = POS_YP + 1;
				 j < 8 && POS_XP + (j - POS_YP) < 8; ++j) {
				upd_func(
					&board[POS_XP + (j - POS_YP)][j], &non_empty->side);
				if (board[POS_XP + (j - POS_YP)][j].obj.type != empty) break;
			}
		}
		non_empty = NULL;

		for (int8_t j = POS_YP - 1; j >= 0 && POS_XP + (POS_YP - j) < 8; --j)
			if (board[POS_XP + (POS_YP - j)][j].obj.type != empty) {
				non_empty = &board[POS_XP + (POS_YP - j)][j].obj;
				break;
			}
		if (non_empty &&
			(non_empty->type == bishop || non_empty->type == queen)) {
			for (uint8_t j = POS_YP + 1;
				 j < 8 && POS_XP - (j - POS_YP) >= 0; ++j) {
				upd_func(
					&board[POS_XP - (j - POS_YP)][j], &non_empty->side);
				if (board[POS_XP - (j - POS_YP)][j].obj.type != empty) break;
			}
		}
	}
	non_empty = NULL;        
	if (POS_XP != 0) {
		for (uint8_t i = POS_XP + 1; i < 8; ++i)
			if (board[i][POS_YP].obj.type != empty) {
				non_empty = &board[i][POS_YP].obj;
				break;
			}           
		if (non_empty && (non_empty->type == rook ||
						  non_empty->type == queen)) {
			DEBUG_MSG("loop #3");
			for (int8_t i = POS_XP - 1; i >= 0; --i) {
				upd_func(&board[i][POS_YP], &non_empty->side);
				if (board[i][POS_YP].obj.type != empty) break;
			}
		}
	}
	non_empty = NULL;
	if (POS_XP != 7) {
		for (int8_t i = POS_XP - 1; i >= 0; --i)
			if (board[i][POS_YP].obj.type != empty) {
				non_empty = &board[i][POS_YP].obj;
				/* print_square_info(&board[i][POS_YP]); */
				break;
			}
		if (non_empty && (non_empty->type == rook ||
						  non_empty->type == queen)) {
			DEBUG_MSG("loop #4");
			for (int8_t i = POS_XP + 1; i < 8; ++i) {
				upd_func(&board[i][POS_YP], &non_empty->side);
				if (board[i][POS_YP].obj.type != empty) break;
			}
		}
	}
	non_empty = NULL;
	return 0;
}


static uint8_t count_free_squares(struct square (*board)[8], enum color_t side,
                           		enum piece_t type, uint8_t *npos) {
	if (side == none)
		return ERROR_INPUT_ABSENT_PIECES;
  
    switch (type) {
    case pawn: {
#define PAWN_XLEVEL (NPOS_XP + (side == black) - (side == white))
	uint8_t counter = 0;
	if (NPOS_YP == 0) {
		counter += (board[PAWN_XLEVEL][NPOS_YP + 1].obj.side != side
					&& board[PAWN_XLEVEL][NPOS_YP + 1].obj.type != empty);
	}
	else if (NPOS_YP == 7) {
		counter += (board[PAWN_XLEVEL][NPOS_YP - 1].obj.side != side
					&& board[PAWN_XLEVEL][NPOS_YP - 1].obj.type != empty);
		
	} else {
		counter += (board[PAWN_XLEVEL][NPOS_YP + 1].obj.side != side
					&& board[PAWN_XLEVEL][NPOS_YP + 1].obj.type != empty);
		counter += (board[PAWN_XLEVEL][NPOS_YP - 1].obj.side != side
					&& board[PAWN_XLEVEL][NPOS_YP - 1].obj.type != empty);
	}
	counter += board[PAWN_XLEVEL][NPOS_YP].obj.type == empty;
	return counter;
#undef PAWN_XLEVEL
	}
	case knight:
		return knight_pos_update(board, &side, npos, square_is_free);
	case king:
		return king_pos_update(board, &side, npos, square_is_safe);
	case queen:
		return queen_pos_update(board, &side, npos, square_is_free);
	case rook: 
		return rook_pos_update(board, &side, npos, square_is_free);
	case bishop:
		return bishop_pos_update(board, &side, npos, square_is_free);
    }
	return ERROR_INPUT_ABSENT_PIECES;
}


void
init_attacking_board(struct square (*board)[8]) {
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			uint8_t pos = i*8 + j + 1;
			npos_update(board, board[i][j].obj.side, board[i][j].obj.type, &pos);
		}
	}              
}


static uint8_t
get_number_of_available_move() {
	
}


uint8_t
check_on_stalemate_position(struct chess *global)
{
	/* Check on hidden attack on king */

	/*	At first need to get number of available move by king
	*	if a number equal 0 than continue check
	*/
	uint8_t *kpos = global->player_side == white
											? &global->kpos_b : &global->kpos_w;
	enum color_t side = global->player_side == white ? black : white;
	
	if (count_free_squares(global->board, side, king, kpos) != 0) {
		printf("king has free square\n");

		/* logical return if it's not checkmate */
	}
	

	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			if (global->board[i][j].obj.side != side) continue;
			uint8_t pos = i * 8 + j + 1;
			// hard condition TODO
			// Like If a piece freeze by hidden attack on the king
			// and he can beat attacking piece it's not stalemate
			if (check_hidden_king_attack(global, &pos)) {} 
			if (count_free_squares(global->board, side,
									global->board[i][j].obj.type, &pos) != 0) {
				printf("%d %dpiece has free square\n", i, j);
				return 0;
				
			}
		}
	}
	
	return 255;
}


uint8_t
user_move(struct chess *global, uint8_t *opos, uint8_t *npos) {
	/* Firstly If new pos was taken by someone else than clean enemy attacked
	   squares. Than I update by new figure this square ( just update new pos and
	   update each squares on new attacking by this a figure. Than I update old
	   square. Delete old attacked squares and update for hidden attack squares.
	   Like 2 rocks in horizontal and one change his position in on vertical. So
	   attack in square doesn't change */
	if (global->board[NPOS_XP][NPOS_YP].obj.type != empty)
		clean_the_piece_attack(global->board, npos); /* clean the enemy attack */

	clean_the_piece_attack(global->board, opos);
	global->board[NPOS_XP][NPOS_YP].obj.side =
									global->board[OPOS_XP][OPOS_YP].obj.side;
	if (global->pawn_transformation != empty) {

		global->board[OPOS_XP][OPOS_YP].obj.type = empty;
		global->board[OPOS_XP][OPOS_YP].obj.side = none;

		check_hidden_attack(global->board, opos, square_state_upd_by_attacking);

		global->board[NPOS_XP][NPOS_YP].obj.type = global->pawn_transformation;
		global->pawn_transformation = empty;
		npos_update(global->board, global->board[NPOS_XP][NPOS_YP].obj.side,
					global->board[NPOS_XP][NPOS_YP].obj.type, npos);
	} else {
		/* It made for memory optimization (Don't create new buffer variable */
		global->pawn_transformation = global->board[OPOS_XP][OPOS_YP].obj.type;

		global->board[OPOS_XP][OPOS_YP].obj.type = empty;
		global->board[OPOS_XP][OPOS_YP].obj.side = none;

		check_hidden_attack(global->board, opos, square_state_upd_by_attacking);

		global->board[NPOS_XP][NPOS_YP].obj.type = global->pawn_transformation;
		global->pawn_transformation = empty;
		npos_update(global->board, global->board[NPOS_XP][NPOS_YP].obj.side,
					global->board[NPOS_XP][NPOS_YP].obj.type, npos);
		// BUG: If I move my rook he doesn't know about old position
		// and attacking find loop just find it old position and stop
	}
	check_hidden_attack(global->board, npos, square_state_upd_by_leaving);
	if (global->board[NPOS_XP][NPOS_YP].obj.type == king) {
		if (global->board[NPOS_XP][NPOS_YP].obj.side == white)
			global->kpos_w = *npos;
		else
			global->kpos_b = *npos;                                  
	}
    if (check_on_stalemate_position(global) == 0) {
		printf("It isn't stalemate\n");
	} else {
		printf("It's stalemate\n");
		global->status = end_stalemate;
		return 0;
	}

	return 0;
}


static uint8_t
check_on_living_shield(struct chess *global, uint8_t *opos, uint8_t *npos) {
	struct chess buffer = *global;
	
	user_move(&buffer, opos, npos);
	
	return buffer.board[(buffer.kpos_b - 1) / 8][(buffer.kpos_b - 1) % 8]
		.w_attack > 0
		 || buffer.board[(buffer.kpos_w - 1) / 8][(buffer.kpos_w - 1) % 8]
		.b_attack > 0;
}


int32_t
check_correct_of_movement (struct chess *global, uint8_t *opos, uint8_t *npos) {


	/*
	*	It's just check the move correctly.
	*	It can't return end of the game and it'd working only if
	*	already sure stalemate and checkmate didn't happen
	*/
	if (global->board[(global->kpos_b - 1) / 8][(global->kpos_b - 1) % 8]
		.w_attack > 0) {
          if (global->board[OPOS_XP][OPOS_YP].obj.type == king &&
              global->board[NPOS_XP][NPOS_YP].w_attack > 0) {
			  printf("It's danger square! 1\n");
			  DEBUG_MSG("ERROR_KING_MOVE_TO_ATTACKED_SQUARE\n");
			  return ERROR_KING_MOVE_TO_ATTACKED_SQUARE;
		} else if (check_on_living_shield(global, opos, npos)) {
			  DEBUG_MSG("ERROR_USELESS_MOVE_DURING_KING_ATTACK\n");
			  printf("Your king underattack! 1\n");                  
			  return ERROR_USELESS_MOVE_DURING_KING_ATTACK;
		}
	} else if (global->board[(global->kpos_w - 1) / 8][(global->kpos_w - 1) % 8]
			   .b_attack > 0) {
		if (global->board[OPOS_XP][OPOS_YP].obj.type == king &&
			global->board[NPOS_XP][NPOS_YP].b_attack > 0) {
			DEBUG_MSG("ERROR_KING_MOVE_TO_ATTACKED_SQUARE\n");
			printf("It's danger square! 2\n");
			return ERROR_KING_MOVE_TO_ATTACKED_SQUARE;
		}
		else if (check_on_living_shield(global, opos, npos)) {
			DEBUG_MSG("ERROR_USELESS_MOVE_DURING_KING_ATTACK\n");
			printf("Your king underattack! 2\n");                  
			return ERROR_USELESS_MOVE_DURING_KING_ATTACK;
		}
	}
	
	if (*opos == *npos)
    {
        printf("ERROR_MINOR_MOVE\n");
        return ERROR_MINOR_MOVE;
    }
    if (global->board[NPOS_XP][NPOS_YP].obj.side
		== global->board[OPOS_XP][OPOS_YP].obj.side)
    {
        printf("ERROR_INPUT_FRIENDLY_ATTACK\n");
        return ERROR_INPUT_FRIENDLY_ATTACK;
    }
	
    if ((get_attack_t(&global->board[OPOS_XP][OPOS_YP])
			 == both_attacked
			|| get_attack_t(&global->board[OPOS_XP][OPOS_YP])
			 == (global->player_side == white ? by_black : by_white))
		 && check_hidden_king_attack(global, opos))
    {
        printf("Failure check_correct_of_movement\n");
        return -1;
    }
    
    switch (global->board[OPOS_XP][OPOS_YP].obj.type)
    {
	case pawn:
	{
		return check_pawn_move(global, opos, npos);
	}
	case knight:
	{
		return check_knight_move(opos, npos);
	}
	case king:
	{
		return check_king_move(global->board, opos, npos);
	}
	case queen:
	{
		return check_queen_move(global->board, opos, npos);
	}
	case rook:
	{
		return check_rook_move(global->board, opos, npos);
	}
	case bishop:
	{
		return check_bishop_move(global->board, opos, npos);
	}
	case empty: { return ERROR_INPUT_ABSENT_PIECES; }
    }
    return 0;
}
