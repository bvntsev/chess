#include "../include/CHESSpieces.h"
#include "../include/CHESSutil.h"
#include "../include/CHESSlogging.h"

#include <stdlib.h>


enum attack_t
square_state_upd_by_attacking (enum color_t *color, enum attack_t type)
{
	switch (type)
    {
    case safe_square:
		return *color == white ? by_white      : by_black;
    case by_black:
		return *color == white ? both_attacked : by_black;
    case by_white:
		return *color == black ? both_attacked : by_white;
    case both_attacked:
		return both_attacked;
    }
	fprintf(stderr, "Error: unexpected square_state_uba finish");
	exit(-1);
}


enum attack_t
square_state_upd_by_leaving (enum color_t *color, enum attack_t type)
{
	switch (type)
    {
    case safe_square:
		return safe_square;
    case by_black:
		printf("by_black\n");
		fflush(stdout);      
		return *color == white ? by_black : safe_square;
    case by_white:
		printf("by_white\n");
		fflush(stdout);      
		return *color == black ? by_white : safe_square;
    case both_attacked:
		printf("both\n");
		fflush(stdout);      
		return *color == white ? by_black : by_white;
    }
	fprintf(stderr, "Error: unexpected square_state_ubl finish");
	exit(-1);
}


uint8_t
knight_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
				  enum attack_t (*upd_func)(enum color_t *, enum attack_t))
{
	if (POS_XP > 1)
    {
		if (POS_YP == 0)
        {
			board[POS_XP - 2][POS_YP + 1]->attacked = 
				upd_func(side, 
						 board[POS_XP - 2][POS_YP + 1]->attacked);
        }
		else if (POS_YP == 7)
        {
			board[POS_XP - 2][POS_YP - 1]->attacked = 
				upd_func(side,
						 board[POS_XP - 2][POS_YP - 1]->attacked);
        }
		else
        {
			board[POS_XP - 2][POS_YP - 1]->attacked = 
				upd_func(side,
						 board[POS_XP - 2][POS_YP - 1]->attacked);

			board[POS_XP - 2][POS_YP + 1]->attacked = 
				upd_func(side,
						 board[POS_XP - 2][POS_YP + 1]->attacked);
        }
    }
	if (POS_XP < 6)
    {
		if (POS_YP == 0)
        {
			board[POS_XP + 2][POS_YP + 1]->attacked = 
				upd_func(side, 
						 board[POS_XP + 2][POS_YP + 1]->attacked);
        }

		else if (POS_YP == 7)
        {
			board[POS_XP + 2][POS_YP - 1]->attacked = 
				upd_func(side,
						 board[POS_XP + 2][POS_YP - 1]->attacked);
        }
		else
        {
			board[POS_XP + 2][POS_YP - 1]->attacked = 
				upd_func(side,
						 board[POS_XP + 2][POS_YP - 1]->attacked);

			board[POS_XP + 2][POS_YP + 1]->attacked = 
				upd_func(side,
						 board[POS_XP + 2][POS_YP + 1]->attacked);
        }
    }
	if (POS_YP > 1)
    {
		if (POS_XP == 0)
        {
			board[POS_XP + 1][POS_YP - 2]->attacked = 
				upd_func(side, 
						 board[POS_XP + 1][POS_YP - 2]->attacked);
        }
		else if (POS_XP == 7)
        {
			board[POS_XP - 1][POS_YP - 2]->attacked = 
				upd_func(side,
						 board[POS_XP - 1][POS_YP - 2]->attacked);
        }
		else
        {
			board[POS_XP + 1][POS_YP - 2]->attacked = 
				upd_func(side, 
						 board[POS_XP + 1][POS_YP - 2]->attacked);

			board[POS_XP - 1][POS_YP - 2]->attacked = 
				upd_func(side,
						 board[POS_XP - 1][POS_YP - 2]->attacked);
        }
    }
	if (POS_YP < 6)
    {
		if (POS_XP == 0)
        {
			board[POS_XP + 1][POS_YP + 2]->attacked = 
				upd_func(side, 
						 board[POS_XP + 1][POS_YP + 2]->attacked);
        }
		else if (POS_XP == 7)
        {
			board[POS_XP - 1][POS_YP + 2]->attacked = 
				upd_func(side,
						 board[POS_XP - 1][POS_YP + 2]->attacked);
        }
		else
        {
			board[POS_XP + 1][POS_YP + 2]->attacked = 
				upd_func(side, 
						 board[POS_XP + 1][POS_YP + 2]->attacked);

			board[POS_XP - 1][POS_YP + 2]->attacked = 
				upd_func(side,
						 board[POS_XP - 1][POS_YP + 2]->attacked);
        }
    } 
	return 0;
}


uint8_t
pawn_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
                enum attack_t (*upd_func)(enum color_t *, enum attack_t)) {
// If POS_XP == 7 and side is black than I get SEGM  
#define PAWN_XLEVEL POS_XP + (*side == black) - (*side == white)
	if (POS_YP == 0) {
		board[PAWN_XLEVEL][POS_YP + 1]->attacked =
			upd_func(
				side,
				board[PAWN_XLEVEL][POS_YP + 1]->attacked);
	}                  
	else if (POS_YP == 7) {
		board[PAWN_XLEVEL][POS_YP - 1]->attacked =
			upd_func(
				side,
				board[PAWN_XLEVEL][POS_YP - 1]->attacked);
        } else {
		printf("%d %d\n", POS_XP, POS_YP);
		board[PAWN_XLEVEL][POS_YP + 1]->attacked =
			upd_func(
				side,
				board[PAWN_XLEVEL][POS_YP + 1]->attacked);
		board[PAWN_XLEVEL][POS_YP - 1]->attacked =
			upd_func(
				side,
				board[PAWN_XLEVEL][POS_YP - 1]->attacked);
	}
	return 0;
#undef PAWN_XLEVEL
}


uint8_t
bishop_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
				  enum attack_t (*upd_func)(enum color_t *, enum attack_t)) {
	return 0;
}  


uint8_t
rook_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
				enum attack_t (*upd_func)(enum color_t *, enum attack_t)) {
	return 0;
}  


uint8_t
queen_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
                 enum attack_t (*upd_func)(enum color_t *, enum attack_t)) {
	return 0;
}  


uint8_t
king_pos_update(struct square ***board, enum color_t *side, uint8_t *pos,
                enum attack_t (*upd_func)(enum color_t *, enum attack_t)) {
	if (POS_XP == 0) {
		if (POS_YP < 7) {
			board[POS_XP + 1][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP + 1][POS_YP + 1]->attacked);
			board[POS_XP][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP + 1]->attacked);
		}
		if (POS_YP > 0) {
			board[POS_XP + 1][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP + 1][POS_YP - 1]->attacked);
			board[POS_XP][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP - 1]->attacked);
		}      
		board[POS_XP + 1][POS_YP]->attacked =
			upd_func(side, board[POS_XP + 1][POS_YP]->attacked);
	}
	else if (POS_XP == 7) {
		if (POS_YP < 7) {
			board[POS_XP - 1][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP - 1][POS_YP + 1]->attacked);
			board[POS_XP][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP + 1]->attacked);
		}
		if (POS_YP > 0) {
			board[POS_XP - 1][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP - 1][POS_YP - 1]->attacked);
			board[POS_XP][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP - 1]->attacked);
		}      
		board[POS_XP - 1][POS_YP]->attacked =
			upd_func(side, board[POS_XP - 1][POS_YP]->attacked);
        } else {
		printf("%d %d\n", POS_XP, POS_YP);
		if (POS_YP < 7) {
			board[POS_XP + 1][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP + 1][POS_YP + 1]->attacked);
			board[POS_XP][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP + 1]->attacked);						
			board[POS_XP - 1][POS_YP + 1]->attacked =
				upd_func(side, board[POS_XP - 1][POS_YP + 1]->attacked);
		}
		if (POS_YP > 0) {
			board[POS_XP + 1][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP + 1][POS_YP - 1]->attacked);

			board[POS_XP - 1][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP - 1][POS_YP - 1]->attacked);
			board[POS_XP][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP - 1]->attacked);                        
			board[POS_XP][POS_YP - 1]->attacked =
				upd_func(side, board[POS_XP][POS_YP - 1]->attacked); // here
		}      
		board[POS_XP + 1][POS_YP]->attacked =
			upd_func(side, board[POS_XP + 1][POS_YP]->attacked);
		board[POS_XP - 1][POS_YP]->attacked =
			upd_func(side, board[POS_XP - 1][POS_YP]->attacked);
	}
   	return 0;
}    
