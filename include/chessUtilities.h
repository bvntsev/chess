#ifndef CHESSUTILITIES

typedef unsigned char u8;
typedef unsigned short u16;     /*  */
typedef unsigned int u32;
typedef unsigned long u64;

enum color { black = 0, white = 1 };

enum type_pieces { empty = 5, pawn = 0, knight = 6, bishop = 2, rook = 4, queen = 1, king = 3};

/* #define TAG_WHITE_PAWN 17 */
/* #define TAG_BLACK_PAWN 25 */

/* #define WHITE_KNIGHT 18 */
/* #define BLACK_KNIGHT 26 */

/* #define WHITE_BISHOP 19 */
/* #define BLACK_BISHOP 27 */

/* #define WHITE_ROOK 20 */
/* #define BLACK_ROOK 28 */

/* #define WHITE_QUEEN 21 */
/* #define BLACK_QUEEN 29 */

/* #define WHITE_KING 22 */
/* #define BLACK_KING 30 */




struct Piece {
  // char *current_position; ??
  enum type_pieces type;
  enum color side;
};

struct square {
  enum color side;
  char current_position[2];
  struct Piece obj;
};

struct ChessGame {
  struct square ***ChessBoard;
  enum color user_side;
};

/* ***->** */
/* ***->**[i] */
/* **->* */
/* **->*[i] */
/* * -> variable[i] */
/* * -> variable */

void CreateBoard(struct ChessGame *obj);

void printf_board(struct square *** table);

#endif // CHESSUTILITIES
