#include "../include/CHESSutil.h"
#include <stdlib.h>
#include <stdio.h>


/* static void printf_binary(char *value) { */
/*     for (int i = 7; i >= 0; --i) */
/*         printf("%i", ((1 << i) & (*value)) >> i); */
/*     printf("\n"); */
/* } */


/* static void printf_ascii_charactrers() { */
/*     PRINT_DASH */
/*     printf("Number\tCharacter\tBinary\n"); */
/*     for (char iter = 'A'; iter < 'A' + 26; ++iter) { */
/*     // for (unsigned char iter = 1; iter < 255; ++iter) { */
/*         printf("%d\t%c\t\t", iter, iter); */
/*         printf_binary(&iter); */
/*     } */
/*     PRINT_DASH */
/*     for (char iter = 'a'; iter < 'a' + 26; ++iter) { */
/*         printf("%d\t%c\t\t", iter, iter); */
/*         printf_binary(&iter); */
/*     } */
/*     PRINT_DASH */
/* }; */

struct square ***set_training_board(struct square ***board, char *custom_pos) {
	/* printf_ascii_charactrers(); */
	for (uint8_t i = 0; i < 8; ++i) {
        board[i] = (malloc(sizeof(struct square *) * 8));
        for (uint8_t j = 0; j < 8; ++j) {
            board[i][j] = (malloc(sizeof(struct square )));
            /* setting color the board squares */
            board[i][j]->side = (2 - ((i ^ j) & 1));
            /* setting color of the pieces */
			board[i][j]->obj.side =
                ((custom_pos[i * 8 + j] ^ 'E') != 0) +
				((custom_pos[i * 8 + j] >> 5) & 1);
            /* setting initial attacked zone by pawns */
			/* board[i][j]->attacked = (i == 2 || i == 5) - (2 * (i == 2));
			 */
			board[i][j]->attacked = safe_square;
            /* setting type of the pieces */
            board[i][j]->obj.type =
                (7 & custom_pos[i * 8 + j]);
            /* setting position of the squares */
            board[i][j]->pos = i * 8 + (j);

            board[i][j]->b_attack = 0;
            board[i][j]->w_attack = 0;
		}
    }
    return board;  
}


struct square ***create_board(struct square ***board) {
  // here are low register are WHITE and high are BLACK  
    char *start_pos = 
        "DNBQKBNDPPPPPPPPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEppppppppdnbqkbnd";
    for (uint8_t i = 0; i < 8; ++i) {
        board[i] = (malloc(sizeof(struct square *) * 8));
        for (uint8_t j = 0; j < 8; ++j) {
            board[i][j] = (malloc(sizeof(struct square )));
            /* setting color the board squares */
            board[i][j]->side = (2 - ((i ^ j) & 1));
            /* setting color of the pieces */
                board[i][j]->obj.side =
                ((start_pos[i * 8 + j] ^ 'E') != 0) +
                                    ((start_pos[i * 8 + j] >> 5) & 1);
            /* setting initial attacked zone by pawns */
            board[i][j]->attacked = (i == 2 || i == 5) - (2 * (i == 2)); 
            /* setting type of the pieces */
            board[i][j]->obj.type =
                (7 & start_pos[i * 8 + j]);
            /* setting position of the squares */
            board[i][j]->pos = i * 8 + (j);
            board[i][j]->b_attack = i == 2;
            board[i][j]->w_attack = i == 5;
        }
    }
    return board;
}
