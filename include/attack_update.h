#ifndef ATTACK_UPDATE_H
#define ATTACK_UPDATE_H

#include "../include/util.h"

uint8_t
pawn_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				uint8_t (*upd_func)(struct square *, enum color_t *));
uint8_t
bishop_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  uint8_t (*upd_func)(struct square *, enum color_t *));
uint8_t
knight_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  uint8_t (*upd_func)(struct square *, enum color_t *));
uint8_t
rook_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				uint8_t (*upd_func)(struct square *, enum color_t *));
uint8_t
queen_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				 uint8_t (*upd_func)(struct square *, enum color_t *));
uint8_t
king_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				uint8_t (*upd_func)(struct square *, enum color_t *));

uint8_t square_state_upd_by_attacking(struct square *obj,
								   enum color_t *color);
								   
uint8_t square_state_upd_by_leaving(struct square *obj,
								 enum color_t *color);
								 
uint8_t
square_is_safe(struct square *sq, enum color_t *side);


uint8_t
square_is_free(struct square *sq, enum color_t *side);


#define POS_X  (pos - 1)  / 8
#define POS_Y  (pos - 1)  % 8
#define POS_XP (*pos - 1) / 8
#define POS_YP (*pos - 1) % 8

#define SQ_UPD_ATTCK square_state_upd_by_attacking
#define SQ_UPD_LEAVE square_state_upd_by_leaving

#endif // ATTACK_UPDATE_H
