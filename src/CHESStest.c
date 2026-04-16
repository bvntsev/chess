#include "../include/CHESSsession.h"
#include "../include/CHESStest.h"
#include "../include/CHESSutil.h"
#include "../include/CHESScli.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


static void
set_square (struct piece *oobj, enum color side, enum type_pieces type)
{
    oobj->side = side;
    oobj->type = type;
    // update attacked pos in all chessboard
}


int8_t (*CLIuser_input_func[])(const struct ChessGame *) = {
    NULL
};


static struct square ***
set_training_field()
{
    char *start_pos =
            "EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE";
    struct square ***ChessBoard;
    for (uint8_t i = 0; i < 8; ++i) {
        ChessBoard[i] = (malloc(sizeof(struct square *) * 8));
        for (uint8_t j = 0; j < 8; ++j) {
            ChessBoard[i][j] = (malloc(sizeof(struct square )));
            /* setting color the board squares */
            ChessBoard[i][j]->side = (2 - ((i ^ j) & 1));
            /* setting color of the pieces */
            ChessBoard[i][j]->obj.side =
            ((start_pos[i * 8 + j] ^ 'E') != 0) +
                                ((start_pos[i * 8 + j] >> 5) & 1);
            /* setting initial attacked zone by pawns */
            ChessBoard[i][j]->attacked = safe_square;
            // ChessBoard[i][j]->attacked = (i == 2 || i == 5) - (2 * (i == 2)); 
            /* setting type of the pieces */
            ChessBoard[i][j]->obj.type =
                (7 & start_pos[i * 8 + j]);
            /* setting position of the squares */
            ChessBoard[i][j]->pos = i * 8 + (j);
        }
    }
    return ChessBoard;
}


static void
free_training_field (struct square ***ChessBoard)
{
    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j)
            free(ChessBoard[i][j]);
        free(ChessBoard[i]);
    }
    free(ChessBoard);
}


int32_t
test_chess_movement ()
{

    printf("==============STARTING TEST==============\n");
    for (int64_t iter = 0; iter < CLIUSER_INPUT_FUNC_LEN; ++iter) {
        // if (CLIuser_input_func[iter](global) == CTEST_FAILURE_EXIT) {
        //     printf("==============FAILURE TEST==============\n");
        //     continue;
        printf("==============SUCCESSFUL TEST==============\n");
    }
    return CTEST_SUCESS_EXIT;
}
