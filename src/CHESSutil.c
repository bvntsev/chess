#include "../include/CHESSutil.h"
#include <stdlib.h>
#include <stdio.h>


// static void printf_binary(char *value) {
//     for (int i = 7; i >= 0; --i)
//         printf("%i", ((1 << i) & (*value)) >> i);
//     printf("\n");
// }


// static void printf_ascii_charactrers() {
//     PRINT_DASH
//     printf("Number\tCharacter\tBinary\n");
//     for (char iter = 'A'; iter < 'A' + 26; ++iter) {
//     // for (unsigned char iter = 1; iter < 255; ++iter) {
//         printf("%d\t\t%c\t\t\t", iter, iter);
//         printf_binary(&iter);
//     }
//     PRINT_DASH
//     for (char iter = 'a'; iter < 'a' + 26; ++iter) {
//         printf("%d\t\t%c\t\t\t", iter, iter);
//         printf_binary(&iter);
//     }
//     PRINT_DASH
// };


struct square ***
create_board (struct square ***board)
{
    char *start_pos = 
        "DNBKQBNDPPPPPPPPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEppppppppdnbkqbnd";
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
        }
    }
    return board;
}
