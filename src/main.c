#include <stdio.h>
#include <stdlib.h>

#include "../include/util.h"
#include "../include/engine.h"
#include "../include/cli.h"
#include "../include/logging.h"


enum color_t *user_side;
// int32_t
// main (int32_t argc, char *argv[])
// {
int32_t
main (void)
{

    DEBUG_MSG("New main start");
    for (;;)
    {
        struct chess *global;
        global = (struct chess *)(malloc(sizeof(struct chess)));
        uint8_t exit_code;
        if (global)
        {
			global->user_side = white;
			global->kpos_b = 5;
			global->kpos_w = 61;
            global->pawn_transformation = empty;
            global->last_move[0] = 0;
            global->last_move[1] = 0;
            user_side = &global->user_side;
            create_board(global->board);
            /* set_training_board(global->board, */
                    /* "EEEEEEEE" */
                    /* "EEEBEEEE" */
                    /* "EEpEEEEE" */
                    /* "EbEEEEEE" */
                    /* "EEEEEEEE" */
                    /* "EEEEEEEE" */
                    /* "EEEEEEEE" */
                    /* "EEEEEEEE"); */
                    /* "EEEEEEEE" */
                    /* "EEEEEEEE" */
                    /* "EBEEEEEE" */
                    /* "EEEEEEEE" */
                    /* "EEEpEEEE" */
                    /* "EEEEkEEE" */
                    /* "EEEEEEEE" */
                    /* "EEEEEEEE"); /\* C5 -- FIXED *\/ */
 			init_attacking_board(global->board);
            exit_code = CLI_run_session_pvp(global);
        }
        free(global);
        switch (exit_code) {
            case EXIT_CODE:
                break;
            case RELOAD_CODE:
                printf("========RELOADED=======\n");
                DEBUG_MSG("reloaded cli game");
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
