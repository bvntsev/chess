#include <stdio.h>
#include <stdlib.h>

#include "../include/chessUtilities.h"
#include "../include/GameSession.h"
#include "../include/CLIsession.h"


struct square ***ChessBoard = NULL;


int main(int argc, char *argv[]) {
    struct ChessGame *global;
    global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
    if (global) {
        if (argc > 1)
            global->user_side = (argv[1][0] == 'w') + 1;
        else
            global->user_side = white; // black incorrect symb white incorrect numb

        global->ChessBoard = create_ChessBoard(ChessBoard, &global->user_side);
        ChessBoard = global->ChessBoard;
        run_session(global);
    }
    free(global);
    return 0;
}
