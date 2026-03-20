#include <stdio.h>
#include "../include/chessUtilities.h"
#include <stdlib.h>
#include "../include/GameSession.h"


struct square ***table = NULL;


int main(int argc, char *argv[]) {
    struct ChessGame *global;
    global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
    if (global) {
        table = global->ChessBoard;
        if (argc > 1)
            global->user_side = (argv[1][0] == 'w') + 1;
        else
            global->user_side = white;
        create_board(global);

        printf_board(global->ChessBoard);
    }
    return 0;
}
