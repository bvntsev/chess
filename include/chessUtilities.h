#ifndef CHESSUTILITIES
#define CHESSUTILITIES

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

#define PRINT_DASH printf("==============================================\n");

enum color { none = 0, black = 1, white = 2 };

enum type_pieces
{ empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, queen = 1, king = 3};

struct piece {
  enum type_pieces  type;
  enum color        side;
};

struct square {
  enum color        side;
  char              pos;
  struct piece      obj;
};

struct ChessGame {
  struct square *** ChessBoard;
  enum color        user_side;
};

void create_board(struct ChessGame *obj);

void printf_board(struct square *** table);

char *get_str_position(const u8 row, const u8 column);

u8 get_num_position(const char *str);

#endif // CHESSUTILITIES
