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
    for (;;)
    {
        struct ChessGame *global;
        global = (struct ChessGame *)(malloc(sizeof(struct ChessGame)));
        uint8_t exit_code;
        if (global)
        {
            global->user_side = white;
            user_side = &global->user_side;
            global->ChessBoard = (struct square ***)
                                 (malloc(sizeof(struct square **) * 8));
            create_ChessBoard(global->ChessBoard, &global->user_side);
            exit_code = CLI_run_session_pvp(global);
        }
        for (int8_t i = 0; i < 8; ++i)
        {
            for (int8_t j = 0; j < 8; ++j)
                free(global->ChessBoard[i][j]);
            free(global->ChessBoard[i]);
        }
        free(global->ChessBoard);
        free(global);
        switch (exit_code) {
            case EXIT_CODE:
                break;
            case RELOAD_CODE:
                printf("====================RELOADED===================\n");
                new_debug_record("reloaded cli game");
                continue;
            default:
            {
                close_debug_stream();
                return -1;
            }
        }
        close_debug_stream();
        break;
    }
    return 0;
}
