#include <stdio.h>
#include "../include/chessUtilities.h"
#include <stdlib.h>
#include "../include/GameSession.h"

struct square ***table = NULL;

int main(int argc, char *argv[]) {
  struct ChessGame *global = (malloc(sizeof(struct ChessGame)));

  table = global->ChessBoard;

  CreateBoard(global);
  printf_board(global->ChessBoard);
  printf("hello worldd");
  return 0;
}
