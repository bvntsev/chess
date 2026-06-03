#ifndef CHESSUTIL_H
#define CHESSUTIL_H

#include <stdint.h>

#define PRINT_DASH printf("==============================================\n");

enum color_t
{ none = 0, black = 1, white = 2 };

enum piece_t

{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, king = 3, queen = 1 };

enum attack_t
{
    by_white = 1,
    by_black = -1,
    safe_square = 0,
    both_attacked = 8
};

enum action_t
{
    attacking,
    leaving
};


struct piece
{
	enum color_t side;
	enum piece_t type;
};


struct square
{
	uint8_t w_attack;
	int8_t pos;
    uint8_t 			b_attack;
	enum color_t      	side;
	struct piece 		obj;
 // It needs for optimization of after move alghoritm
};


struct chess {
	uint8_t kpos_w;
	uint8_t kpos_b;
    uint8_t last_move[2];
    enum piece_t pawn_transformation;
	enum color_t user_side;
	struct square board[8][8];

};

struct square *
create_board(struct square (*board)[8]);

struct square *set_training_board(struct square (*board)[8], char *custom_pos);

enum attack_t get_attack_t(struct square *sq);

#endif // CHESSUTIL_H
