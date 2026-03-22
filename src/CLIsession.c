/* parsing input, display, output */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/CLIsession.h"
#include "../include/chessUtilities.h"

static u8 find_obj(const char *str, const char obj) {
  for (u32 i = 0; i < strlen(str); ++i)
    if(str[i] == obj)
      return 1;
  return 0;
}

char **parsing_user_move(const char *user_move) {

  u8 beats = 0;

  for (u32 i = 0; i < strlen(user_move); ++i)
    {
      if ((find_obj("abcdefgh+#=QRBNK12345678O-", user_move[i]))) {
          if (user_move[i] == 'x') beats = 1;
        }
      else
        return NULL;
    }
  char *start_pos;
  char *final_pos;


  

  /* example */
  /* pawn */
  /*   d5 */
  /*   cxd4 */
  /*   bxc1=Q+ */

  /* Rfb5; */
  /* Rfxb5; */
  /* Rfxb5+ */
  /* Rfxb5# */
  /* Rbxc1+ */
  /* if (find_obj(user_move, 'x')) */


  
}

static char get_fig_symbol(enum type_pieces *type, enum color *color) {
    // printf(*color == black ? "IT IS BLACK\n" : "IT IS WHITE\n");
    switch(*type) {
        case pawn:      return *color == black ? 'p' : 'P';
        case king:      return *color == black ? 'k' : 'K';
        case queen:     return *color == black ? 'q' : 'Q';
        case rook:      return *color == black ? 'r' : 'R';
        case bishop:    return *color == black ? 'b' : 'B';
        case knight:    return *color == black ? 'n' : 'N';
        case empty:     return '.';
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
};


void print_ChessBoard_CLI(struct square ***ChessBoard, enum color *user_side) {
    // printf_ascii_charactrers();
    switch (*user_side) {
        case white: {
            printf("    A B C D E F G H\n");
            for (u8 i = 0; i < 8; ++i) {
                printf("%d | ", 8 - i);
                for (u8 j = 0; j < 8; ++j) {
                    printf("%c ", get_fig_symbol(&ChessBoard[i][j]->obj.type,
                                                &ChessBoard[i][j]->obj.side));
                }
                printf("| %d\n", 8 - i);
            }
            printf("    A B C D E F G H\n");
            break;
        }
        case black: {
            printf("    H G F E D C B A\n");
            for (u8 i = 0; i < 8; ++i) {
                printf("%d | ", i + 1);
                for (u8 j = 0; j < 8; ++j) {
                    printf("%c ", get_fig_symbol(
                                &ChessBoard[i][j]->obj.type,
                                &ChessBoard[i][j]->obj.side));
                }
                printf("| %d\n", i + 1);
            }
            printf("    H G F E D C B A\n");
            break;
        }
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
};

void parsing_input(const char *inp) {};
