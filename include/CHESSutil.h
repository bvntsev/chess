#ifndef CHESSUTIL_H
#define CHESSUTIL_H

#include <stdint.h>

#define PRINT_DASH printf("==============================================\n");

enum color_t
{ none = 0, black = 1, white = 2 };

enum piece_t
{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, king = 1, queen = 3 };

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
  enum color_t      side;
  enum attack_t     attacked;
  int8_t            pos;
  struct piece      obj;
};

struct chess
{
  enum color_t user_side;
  struct square ***board;
};

struct square ***
create_board(struct square ***board);

#endif // CHESSUTIL_H
