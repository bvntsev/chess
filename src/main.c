#include <stdio.h>
#include "../include/chessUtilities.h"
#include <stdlib.h>
#include "../include/GameSession.h"

struct square ***table = NULL;

int main(int argc, char *argv[]) {
  struct ChessGame *global;
  global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
  if (global){
    table = global->ChessBoard;
    CreateBoard(global);

    printf_board(global->ChessBoard);
    printf("hello worldd");
  }
  return 0;
}
