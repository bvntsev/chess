#include <stdio.h>
#include <stdlib.h>

#include "../include/CHESSutil.h"
#include "../include/CHESSsession.h"
#include "../include/CHESScli.h"
#include "../include/CHESSlogging.h"


enum color_t *user_side;
// int32_t
// main (int32_t argc, char *argv[])
// {
int32_t
main (void)
{

    new_debug_record("New main start");
    for (;;)
    {
        struct chess *global;
        global = (struct chess *)(malloc(sizeof(struct chess)));
        uint8_t exit_code;
        if (global)
        {
            global->user_side = white;
            user_side = &global->user_side;
            global->board = (struct square ***)
                                 (malloc(sizeof(struct square **) * 8));
            create_board(global->board);
            /* set_training_board(global->board, "EEEEPEEEEEEpEEEEEEEEPEEEEEEEEkEEEEEEEEEEEEEEEEEEppppppppdnbqkbnd"); */
            exit_code = CLI_run_session_pvp(global);
        }
        for (int8_t i = 0; i < 8; ++i)
        {
            for (int8_t j = 0; j < 8; ++j)
                free(global->board[i][j]);
            free(global->board[i]);
        }
        free(global->board);
        free(global);
        switch (exit_code) {
            case EXIT_CODE:
                break;
            case RELOAD_CODE:
                printf("========RELOADED=======\n");
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
