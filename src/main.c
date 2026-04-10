#include <stdlib.h>

#include "../include/chessUtilities.h"
#include "../include/GameSession.h"
#include "../include/CLIsession.h"


struct square ***ChessBoard = NULL;


int32_t main(int32_t argc, char *argv[]) {
    struct ChessGame *global;
    global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
    if (global) {
        if (argc > 1)
            global->user_side = (argv[1][0] == 'w') + 1;
        else
            global->user_side = white;

        global->ChessBoard = create_ChessBoard(ChessBoard, &global->user_side);
        ChessBoard = global->ChessBoard;
        CLI_run_session(global);
    }
    for (int8_t i = 0; i < 8; ++i) {
        for (int8_t j = 0; j < 8; ++j)
            free(global->ChessBoard[i][j]);
        free(global->ChessBoard[i]);
    }
    free(global->ChessBoard);
    free(global);
    return 0;
}
