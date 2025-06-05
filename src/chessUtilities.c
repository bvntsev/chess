#include "../include/chessUtilities.h"
#include <stdlib.h>
#include <stdio.h>

void setDefaultPiece() {}

void CreateBoard(struct ChessGame * global) {
  char *start_piece_position =
      "dnbkqbndppppppppeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeePPPPPPPPDNBKQBND";
  global->ChessBoard = (malloc(sizeof(struct square **) * 8));


  for (int i = 0; i < 8; ++i) {
    global->ChessBoard[i] = (malloc(sizeof(struct square *) * 8));
    for (int j = 0; j < 8; ++j) {
      global->ChessBoard[i][j] = (malloc(sizeof(struct square *)));
      global->ChessBoard[i][j]->obj.side = (32 & start_piece_position[i * 8 + j])/32;
      global->ChessBoard[i][j]->obj.type = (7 & start_piece_position[i * 8 + j]);
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
      printf("%d ", table[i][j]->obj.type);
    }
    printf("\n");
  }
}
