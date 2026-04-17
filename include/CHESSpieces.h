#ifndef CHESSPIECES
#define CHESSPIECES

#include "../include/CHESSutil.h"

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
pawn_opos_update    (struct square ***ChessBoard, uint8_t *opos);
uint8_t
bishop_opos_update  (struct square ***ChessBoard, uint8_t *opos);
uint8_t
knight_opos_update  (struct square ***ChessBoard, uint8_t *opos);
uint8_t
rook_opos_update    (struct square ***ChessBoard, uint8_t *opos);
uint8_t
queen_opos_update   (struct square ***ChessBoard, uint8_t *opos);
uint8_t
king_opos_update    (struct square ***ChessBoard, uint8_t *opos);


enum attack_t
square_state_upd (enum color color, enum attack_t type);

#define OPOS_X  (opos - 1)  / 8
#define OPOS_Y  (opos - 1)  % 8
#define OPOS_XP (*opos - 1) / 8
#define OPOS_YP (*opos - 1) % 8


#define NPOS_X  (npos - 1)  / 8
#define NPOS_Y  (npos - 1)  % 8
#define NPOS_XP (*npos - 1) / 8
#define NPOS_YP (*npos - 1) % 8

#endif // CHESSPIECES
