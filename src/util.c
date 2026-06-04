#include "../include/util.h"

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

enum attack_t get_attack_t(struct square *sq) {
	if (sq->b_attack >= 1 && sq->w_attack >= 1)
		return both_attacked;
	if (sq->b_attack >= 1)
		return by_black;
	if (sq->w_attack >= 1)
		return by_white;
	else return safe_square;
}  


struct square *set_training_board(struct square (*board)[8], char *custom_pos) {
	/* printf_ascii_charactrers(); */
	for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            /* setting color the board squares */
            board[i][j].side = (2 - ((i ^ j) & 1));
            /* setting color of the pieces */
			board[i][j].obj.side =
                ((custom_pos[i * 8 + j] ^ 'E') != 0) +
				((custom_pos[i * 8 + j] >> 5) & 1);
            /* setting initial attacked zone by pawns */
			/* board[i][j].attacked = (i == 2 || i == 5) - (2 * (i == 2));
			 */
            /* setting type of the pieces */
            board[i][j].obj.type =
                (7 & custom_pos[i * 8 + j]);
            /* setting position of the squares */
            board[i][j].pos = i * 8 + (j);
            board[i][j].b_attack = 0;
            board[i][j].w_attack = 0;
		}
    }
    return &(board[0][0]);  
}


struct square *create_board(struct square (*board)[8]) {
  // here are low register are WHITE and high are BLACK  
    char *start_pos = 
        "DNBQKBND"
        "PPPPPPPP"
        "EEEEEEEE"
        "EEEEEEEE"
        "EEEEEEEE"
        "EEEEEEEE"
        "pppppppp"
        "dnbqkbnd";
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            /* setting color the board squares */
            board[i][j].side = (2 - ((i ^ j) & 1));
            /* setting color of the pieces */
                board[i][j].obj.side =
                ((start_pos[i * 8 + j] ^ 'E') != 0) +
                                    ((start_pos[i * 8 + j] >> 5) & 1);
            /* setting initial attacked zone by pawns */
            /* setting type of the pieces */
            board[i][j].obj.type =
                (7 & start_pos[i * 8 + j]);
            /* setting position of the squares */
            board[i][j].pos = i * 8 + (j);
            board[i][j].b_attack = 0;
            board[i][j].w_attack = 0;
        }
    }
    return &(board[0][0]);
}
