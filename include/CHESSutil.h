#ifndef CHESSUTIL_H
#define CHESSUTIL_H

#include <stdint.h>

#define PRINT_DASH printf("==============================================\n");

enum color 
{ none = 0, black = 1, white = 2 };

enum type_pieces
{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, king = 1, queen = 3 };

struct piece
{
  enum color        side;
  enum type_pieces  type;
};

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

struct square
{
  enum color        side;
  enum attack_t     attacked;
  int8_t            pos;
  struct piece      obj;
};

struct ChessGame
{
  enum color        user_side;
  struct square ***ChessBoard;
};

struct square **
*create_ChessBoard(struct square ***ChessBoard, enum color *user_side);

#endif // CHESSUTIL_H
