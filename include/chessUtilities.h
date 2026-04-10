#ifndef CHESSUTILITIES
#define CHESSUTILITIES

#include <stdint.h>

#define PRINT_DASH printf("==============================================\n");

#ifndef CLISSSION
enum color { none = 0, black = 1, white = 2 };
#endif

enum type_pieces
{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, king = 1, queen = 3 };

struct piece {
  enum color        side;
  enum type_pieces  type;
};

struct square {
  enum color        side;
  int8_t            pos;
  int8_t            attacked;
  struct piece      obj;
};

struct ChessGame {
  enum color        user_side;
  struct square ***ChessBoard;
};

struct square ***create_ChessBoard(struct square ***ChessBoard,
                                                        enum color *user_side);

#endif // CHESSUTILITIES
