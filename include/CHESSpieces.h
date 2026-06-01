#ifndef CHESSPIECES
#define CHESSPIECES

#include "../include/CHESSutil.h"

uint8_t
pawn_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				enum attack_t (*upd_func)(struct square *, enum color_t *));
uint8_t
bishop_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  enum attack_t (*upd_func)(struct square *, enum color_t *));
uint8_t
knight_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				  enum attack_t (*upd_func)(struct square *, enum color_t *));
uint8_t
rook_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				enum attack_t (*upd_func)(struct square *, enum color_t *));
uint8_t
queen_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				 enum attack_t (*upd_func)(struct square *, enum color_t *));
uint8_t
king_pos_update(struct square (*board)[8], enum color_t *side, uint8_t *pos,
				enum attack_t (*upd_func)(struct square *, enum color_t *));

enum attack_t square_state_upd_by_attacking(struct square *obj,
                                            enum color_t *color);
enum attack_t square_state_upd_by_leaving(struct square *obj,
                                          enum color_t *color);

#define POS_X  (pos - 1)  / 8
#define POS_Y  (pos - 1)  % 8
#define POS_XP (*pos - 1) / 8
#define POS_YP (*pos - 1) % 8

#define SQ_UPD_ATTCK square_state_upd_by_attacking
#define SQ_UPD_LEAVE square_state_upd_by_leaving

#endif // CHESSPIECES
