#include "../include/CHESSpieces.h"
#include "../include/CHESSutil.h"
#include "../include/CHESSlogging.h"

#include <stdint.h>
#include <stdlib.h>


void square_state_upd_by_attacking(struct square *sq,
								   enum color_t *color) {
	if (*color == white)
		sq->w_attack++;
	else
		sq->b_attack++;
}


void square_state_upd_by_leaving(struct square *sq,
								 enum color_t *color) {
	if (*color == white && sq->w_attack > 0)
		sq->w_attack--;
	else if (*color == black && sq->b_attack > 0) 
		sq->b_attack--;
}


uint8_t
knight_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  void (*upd_func)(struct square *, enum color_t *))
{
	if (POS_XP > 1)
    {
		if (POS_YP == 0)
        {
			upd_func(&board[POS_XP - 2][POS_YP + 1], side);                  
				  
        }
		else if (POS_YP == 7)
        {
			upd_func(&board[POS_XP - 2][POS_YP - 1], side);
        }
		else
        {
			upd_func(&board[POS_XP - 2][POS_YP - 1], side);

			upd_func(&board[POS_XP - 2][POS_YP + 1], side);
        }
    }
	if (POS_XP < 6)
    {
		if (POS_YP == 0)
        {
			upd_func(&board[POS_XP + 2][POS_YP + 1], side);
        }

		else if (POS_YP == 7)
        {
			upd_func(&board[POS_XP + 2][POS_YP - 1], side);
        }
		else
        {
			upd_func(&board[POS_XP + 2][POS_YP - 1], side);

			upd_func(&board[POS_XP + 2][POS_YP + 1], side);
        }
    }
	if (POS_YP > 1)
    {
		if (POS_XP == 0)
        {
			upd_func(&board[POS_XP + 1][POS_YP - 2], side);
        }
		else if (POS_XP == 7)
        {
			upd_func(&board[POS_XP - 1][POS_YP - 2], side);
        }
		else
        {
			upd_func(&board[POS_XP + 1][POS_YP - 2], side);

			upd_func(&board[POS_XP - 1][POS_YP - 2], side);
        }
    }
	if (POS_YP < 6)
    {
		if (POS_XP == 0)
        {
			upd_func(&board[POS_XP + 1][POS_YP + 2], side);
        }
		else if (POS_XP == 7)
        {
			upd_func(&board[POS_XP - 1][POS_YP + 2], side);
        }
		else
        {
			upd_func(&board[POS_XP + 1][POS_YP + 2], side);

			upd_func(&board[POS_XP - 1][POS_YP + 2], side);
        }
    } 
	return 0;
}


uint8_t
pawn_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
                void (*upd_func)(struct square *, enum color_t *)) {
// If POS_XP == 7 and side is black than I get SEGM  
#define PAWN_XLEVEL POS_XP + (*side == black) - (*side == white)
	if (POS_YP == 0) {
		upd_func(&board[PAWN_XLEVEL][POS_YP + 1], side);
		
	}
	else if (POS_YP == 7) {
		upd_func(&board[PAWN_XLEVEL][POS_YP - 1], side);
		
	} else {
		upd_func(&board[PAWN_XLEVEL][POS_YP + 1], side);
		
		upd_func(&board[PAWN_XLEVEL][POS_YP - 1], side);
		
	}
	return 0;
#undef PAWN_XLEVEL
}



static void
direct_line_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				   void (*upd_func)(struct square *, enum color_t *)) {
	for (uint8_t j = POS_YP + 1; j < 8; ++j) {
		upd_func(&board[POS_XP][j], side);
		if (board[POS_XP][j].obj.type != empty)
			break;
	}
	for (int8_t j = POS_YP - 1; j >= 0; --j) {
		upd_func(&board[POS_XP][j], side);          
		if (board[POS_XP][j].obj.type != empty)
			break;
	}
	for (uint8_t i = POS_XP + 1; i < 8; ++i) {
		upd_func(&board[i][POS_YP], side);          
		if (board[i][POS_YP].obj.type != empty)
			break;
	}
	for (int8_t i = POS_XP - 1; i >= 0; --i) {
		upd_func(&board[i][POS_YP], side);
		if (board[i][POS_YP].obj.type != empty)
			break;
	}
}


static void
diagonal_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
                void (*upd_func)(struct square *, enum color_t *)) {
	DEBUG_MSG("diagonal_update");
	for (uint8_t j = POS_YP + 1; j < 8 && POS_XP - (j - POS_YP) >= 0; ++j) {
		DEBUG_MSG("iter l1 %d\n", j);
		upd_func(&board[POS_XP - (j - POS_YP)][j], side);
		if (board[POS_XP - (j - POS_YP)][j].obj.type != empty)
			break;
	}
	DEBUG_MSG("loop #2\n");
	for (uint8_t j = POS_YP + 1; j < 8 && POS_XP + (j - POS_YP) < 8; ++j) {
		DEBUG_MSG("iter l3 %d\n", j);
		upd_func(&board[POS_XP + (j - POS_YP)][j], side);
		if (board[POS_XP + (j - POS_YP)][j].obj.type != empty)
			break;
	}
	DEBUG_MSG("loop #3\n");
	for (int8_t j = POS_YP - 1; j >= 0 && POS_XP - (POS_YP - j) >= 0; --j) {
		DEBUG_MSG("iter l2 %d\n", j);
		upd_func(&board[POS_XP - (POS_YP - j)][j], side);
		if (board[POS_XP - (POS_YP - j)][j].obj.type != empty)
			break;
		
	}
	DEBUG_MSG("loop #4\n");
	for (int8_t j = POS_YP - 1; j >= 0 && POS_XP + (POS_YP - j) < 8; --j) {
		DEBUG_MSG("iter l4 %d\n", j);
		upd_func(&board[POS_XP + (POS_YP - j)][j], side);
		if (board[POS_XP + (POS_YP - j)][j].obj.type != empty)
			break;
	}
}


uint8_t
bishop_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  void (*upd_func)(struct square *, enum color_t *)) {
	diagonal_update(board, side, pos, upd_func);
	return 0;
}


uint8_t
rook_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
                void (*upd_func)(struct square *, enum color_t *)) {
	direct_line_update(board, side, pos, upd_func);
	return 0;
}


uint8_t
queen_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
                 void (*upd_func)(struct square *, enum color_t *)) {
	direct_line_update(board, side, pos, upd_func);
	diagonal_update(board, side, pos, upd_func);      
	return 0;
}  


uint8_t
king_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
                void (*upd_func)(struct square *, enum color_t *)) {
	if (POS_XP == 0) {
		if (POS_YP < 7) {
			upd_func(&board[POS_XP + 1][POS_YP + 1], side);
			upd_func(&board[POS_XP][POS_YP + 1], side);
		}
		if (POS_YP > 0) {
			upd_func(&board[POS_XP + 1][POS_YP - 1], side);
			upd_func(&board[POS_XP][POS_YP - 1], side);
		}      
		upd_func(&board[POS_XP + 1][POS_YP], side);
	}
	else if (POS_XP == 7) {
		if (POS_YP < 7) {
			upd_func(&board[POS_XP - 1][POS_YP + 1], side);
			upd_func(&board[POS_XP][POS_YP + 1], side);
		}
		if (POS_YP > 0) {
			upd_func(&board[POS_XP - 1][POS_YP - 1], side);
			upd_func(&board[POS_XP][POS_YP - 1], side);
		}      
		upd_func(&board[POS_XP - 1][POS_YP], side);
	} else {
		if (POS_YP < 7) {
			upd_func(&board[POS_XP + 1][POS_YP + 1], side);
			upd_func(&board[POS_XP][POS_YP + 1], side);						
			upd_func(&board[POS_XP - 1][POS_YP + 1], side);
		}
		if (POS_YP > 0) {
			upd_func(&board[POS_XP + 1][POS_YP - 1], side);

			upd_func(&board[POS_XP - 1][POS_YP - 1], side);
			upd_func(&board[POS_XP][POS_YP - 1], side);                        
		}      
		upd_func(&board[POS_XP + 1][POS_YP], side);
		upd_func(&board[POS_XP - 1][POS_YP], side);
	}
   	return 0;
}    
