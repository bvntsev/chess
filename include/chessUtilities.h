#ifndef CHESSUTILITIES
#define CHESSUTILITIES

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

enum color { black = 0, white = 1 };

enum type_pieces { empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, queen = 1, king = 3};

struct piece {
  // char *current_position; ??
  enum type_pieces type;
  enum color side;
};

struct square {
  u8 unprotected_position_white;
  u8 unprotected_position_black;
  enum color side;
  char *current_position;
  struct piece obj;
};

struct ChessGame {
  struct square ***ChessBoard;
  enum color user_side;
};

void CreateBoard(struct ChessGame *obj);

void printf_board(struct square *** table);

char *get_str_position(const u8 row, const u8 column);

u8 get_num_position(const char *str);

#endif // CHESSUTILITIES
