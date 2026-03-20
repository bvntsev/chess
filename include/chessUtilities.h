#ifndef CHESSUTILITIES
#define CHESSUTILITIES

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef unsigned long   u64;

#define PRINT_DASH printf("==============================================\n");

enum color { none = 0, black = 1, white = 2 };

enum type_pieces
{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, king = 1, queen = 3};

struct piece {
  enum color        side;
  enum type_pieces  type;
};

struct square {
  char              pos;
  enum color        side;
  struct piece      obj;
};

struct ChessGame {
  enum color        user_side;
  struct square ***ChessBoard;
};

struct square ***create_ChessBoard(struct square ***ChessBoard,
                                                        enum color *user_side);

#endif // CHESSUTILITIES
