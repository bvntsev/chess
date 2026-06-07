#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
			global->player_side = black;
            global->pawn_transformation = empty;
            global->last_move[0] = 0;
            global->last_move[1] = 0;
			global->status = session_active;
            user_side = &global->player_side;
            /* create_board(global->board); */
            set_training_board(global->board,
                    "KEEEEEEE"
                    "EEdEEEPE"
                    "EdEEEPpE"
                    "EEEEEpEE"
                    "EEEEEPEn"
                    "EEEEEpPE"
                    "EEEEEEpE"
                    "EEEkEEEE");
			global->kpos_b = find_figure(global->board, black, king);
			global->kpos_w = find_figure(global->board, white, king);
			if (global->kpos_b == 255 || global->kpos_w == 255) {
				printf("Missing King white or black side\n");
				printf("You need to check kings were added to board\n");
				sleep(3);
				exit(0);
			}

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
			case GAME_STATUS_END_CHECKMATE_BLACK:
			case GAME_STATUS_END_CHECKMATE_WHITE:
			case GAME_STATUS_END_STALEMATE:
				printf("After 5 sec game will close");
				fflush(stdout);
				#if DEBUG == 1
        		close_debug_stream();
				#endif
				sleep(5);
				return 0;
			case GAME_STATUS_SESSION_ACTIVE:
            default: {
				fflush(stdout);
              	close_debug_stream();
              	return -1;
            }
        }
		
		#if DEBUG == 1
        close_debug_stream();
		#endif
		
        break;
    }
    return 0;
}
