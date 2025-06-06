#include "../include/chessUtilities.h"
#include <stdlib.h>
#include <stdio.h>

void setDefaultPiece() {}

char *get_str_position(u8 row, u8 column) {
  char *str = malloc(sizeof(char) * 3);
  str[0] = 'A' + column;
  str[1] = '1' + row;
  str[2] = '\0';
  return str;
}

u8 get_num_position(char *str) {
  return (str[0]-'A'+1)*10 + (str[1]-'1'+1);
}


void CreateBoard(struct ChessGame *global) {
  char *start_piece_position =
      "dnbkqbndppppppppeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeePPPPPPPPDNBKQBND";
  global->ChessBoard = (malloc(sizeof(struct square **) * 8));


  for (int i = 0; i < 8; ++i) {
    global->ChessBoard[i] = (malloc(sizeof(struct square *) * 8));
    for (int j = 0; j < 8; ++j) {

      global->ChessBoard[i][j] = (malloc(sizeof(struct square )));

      global->ChessBoard[i][j]->side =
          (i ^ j) & 1; /* set color the board square */

      global->ChessBoard[i][j]->obj.side =
          (32 & start_piece_position[i * 8 + j]) / 32;

      global->ChessBoard[i][j]->obj.type =
          (7 & start_piece_position[i * 8 + j]);

      global->ChessBoard[i][j]->current_position = get_str_position(i,j);
    }
  }
}

void printf_board(struct square *** table) {
  for (u8 i = 0; i < 8; ++i) {
    for (u8 j = 0; j < 8; ++j) {
      printf("%d ", table[i][j]->obj.side);
    }
    printf("\n");
  }
  printf("\n");

  for (u8 i = 0; i < 8; ++i) {
    for (u8 j = 0; j < 8; ++j) {
      printf("%d ", table[i][j]->side);
    }
    printf("\n");
  }
  printf("\n");

  for (u8 i = 0; i < 8; ++i) {
    for (u8 j = 0; j < 8; ++j) {
      printf("%d ", table[i][j]->obj.type);
    }
    printf("\n");
  }
  printf("\n");
  for (u8 i = 0; i < 8; ++i) {
    for (u8 j = 0; j < 8; ++j) {
      printf("%s ", table[i][j]->current_position);
    }
    printf("\n");
  }
}
