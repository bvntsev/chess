#include <stdlib.h>

#include "../include/CHESSutil.h"
#include "../include/CHESSsession.h"
#include "../include/CHESScli.h"
#include "../include/CHESSlogging.h"


enum color *user_side;
// int32_t
// main (int32_t argc, char *argv[])
// {
int32_t
main (void)
{
    new_debug_record("New main start");
    struct ChessGame *global;
    global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
    if (global)
    {
        global->user_side = white;
        user_side = &global->user_side;
        global->ChessBoard = (struct square ***)
                             (malloc(sizeof(struct square **) * 8));
        create_ChessBoard(global->ChessBoard, &global->user_side);
        CLI_run_session(global);
    }
    for (int8_t i = 0; i < 8; ++i)
    {
        for (int8_t j = 0; j < 8; ++j)
            free(global->ChessBoard[i][j]);
        free(global->ChessBoard[i]);
    }
    free(global->ChessBoard);
    free(global);
    close_debug_stream();
    return 0;
}
