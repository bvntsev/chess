#include <stdio.h>
#include "../include/chessUtilities.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  struct ChessGame *global = (malloc(sizeof(struct ChessGame)));;
  CreateBoard(global);
  printf_board(global->ChessBoard);
  printf("hello worldd");
  return 0;
}
