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
			global->kpos_b = 5;
			global->kpos_w = 61;
            global->pawn_transformation = empty;
            global->last_move[0] = 0;
            global->last_move[1] = 0;
            user_side = &global->user_side;
            /* create_board(global->board); */            
            set_training_board(global->board,
                    "EEEEPEEE"
                    "EEEpEEEE"
                    "EEEEPEEE"
                    "EEEEEkEE"
                    "EEEEEEEE"
                    "EEEEEEEE"
                    "pppppppp"
                    "dnbqkbnd");
            exit_code = CLI_run_session_pvp(global);
        }
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
