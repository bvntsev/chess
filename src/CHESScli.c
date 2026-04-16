/* parsing input, display, output */

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>

#include "../include/CHESSutil.h"
#include "../include/CHESSsession.h"
#include "../include/CHESScli.h"
#include "../include/CHESSlogging.h"

// static enum type_pieces get_pieces_type(uint8_t symb) {
//     symb = GET_STANDART_SYMBOL(symb);
//     switch (symb) {
//         case 'n':   return knight;
//         case 'q':   return queen;
//         case 'k':   return king;
//         case 'r':   return rook;
//         case 'b':   return bishop;
//         case 'p':   return pawn;
//         default :   return empty;
//     }
// }


static int8_t
check_correct_input (char **pos)
{
    if ((pos[0][1] && pos[1][1]) && ((pos[0][1] > '0' && pos[0][1] < '9') &&
                                        (pos[1][1] > '0' && pos[1][1] < '9')))
        return 0;
    return ERROR_INPUT_INCORRECT_SYMBOL;
}


static int8_t
get_fig_symbol (enum type_pieces *type, enum color *color)
{
    // printf(*color == black ? "IT IS BLACK\n" : "IT IS WHITE\n");
    switch(*type)
    {
        case pawn:      return *color == black ? 'p' : 'P';
        case king:      return *color == black ? 'k' : 'K';
        case queen:     return *color == black ? 'q' : 'Q';
        case rook:      return *color == black ? 'r' : 'R';
        case bishop:    return *color == black ? 'b' : 'B';
        case knight:    return *color == black ? 'n' : 'N';
        case empty:     return '.';
        default:
        {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
};


static void 
print_ChessBoard_CLI (struct square ***ChessBoard, enum color *user_side)
{
    switch (*user_side)
    {
        case white:
        {
            printf("    A B C D E F G H\n");
            for (int8_t i = 0; i < 8; ++i) {
                printf("%d | ", 8 - i);
                for (uint8_t j = 0; j < 8; ++j) {
                    printf("%c ", get_fig_symbol(&ChessBoard[i][j]->obj.type,
                                                &ChessBoard[i][j]->obj.side));
                }
                printf("| %d\n", 8 - i);
            }
            printf("    A B C D E F G H\n");
            break;
        }
        case black: {
            printf("    H G F E D C B A\n");
            for (int8_t i = 7; i >= 0; --i) {
                printf("%d | ", 8 - i);
                for (int8_t j = 7; j >= 0; --j) {
                    printf("%c ", get_fig_symbol(
                                &ChessBoard[i][j]->obj.type,
                                &ChessBoard[i][j]->obj.side));
                }
                printf("| %d\n", 8 - i);
            }
            printf("    H G F E D C B A\n");
            break;
        }
        default: {
            printf("%s:%d Incorrect color error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
};


static void
printf_debug (struct square ***ChessBoard)
{
    // printf_ascii_charactrers();
    // list
    // for (uint8_t i = 0; i < 8; ++i) {
    //     for (uint8_t j = 0; j < 8; ++j) {
    //         printf("%d. %d '%c'\n", ChessBoard[i][j]->pos, ChessBoard[i][j]->obj.side, get_fig_symbol(&ChessBoard[i][j]->obj.type, &ChessBoard[i][j]->obj.side));
    //     }
    // }

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->attacked);
        }
        printf("\n");
    }
    printf("\n");

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.side);
        }
        printf("\n");
        }
    printf("\n");

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.type);
        }
        printf("\n");
    }
    printf("\n");

    for (uint8_t i = 0; i < 8; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->pos);
        }
        printf("\n");
    }
    printf("\n");
}


static uint8_t
get_pos_value (char *symbol, char *number)
{
    return (GET_STANDART_SYMBOL(*symbol) - 96) + ((57 - *number - 1) * 8);
}


static uint8_t
input_proc (char *input, char **pos)
{
    if (strncmp(input, "--", 2) == 0) {
        input += 2;
        if (strncmp(input, "help", 4) == 0) {
            printf(
            "--help\t\tget this help message\n--exit\t\tdon't save and exit\n"
                    );
            return HELP_CODE;
        }
        else if(strncmp(input, "exit", 4) == 0) {
            return EXIT_CODE;
        }
        return INCORRECT_COMMAND_CODE;
    }
    else {
        int8_t len_pos = 0;
        for (uint8_t iter = 0; iter < strlen(input); ++iter) {
            if (len_pos == 2) break;
            if ('a' <= GET_STANDART_SYMBOL(input[iter]) && 
                 GET_STANDART_SYMBOL(input[iter]) <= 'h') {
                pos[len_pos++] = &input[iter];
            }
        }
        if (len_pos < 2)                return ERROR_INPUT_INCORRECT_LEN;
        if (check_correct_input(pos))   return ERROR_INPUT_INCORRECT_SYMBOL;
    }
    return 0;
}


uint8_t 
CLI_run_session (struct ChessGame *global)
{
    uint8_t status = GAME_STATUS_SESSION_ACTIVE;
    FILE *stream = new_logging(modern_move_logging);

    // print_func
    for (;status == GAME_STATUS_SESSION_ACTIVE;)
    {
        // printf_debug(global->ChessBoard);
        print_ChessBoard_CLI(global->ChessBoard, &global->user_side);
        char *user_input = NULL;
        size_t len = 0;
        ssize_t glread = getline(&user_input, &len, stdin);
        if ( glread != -1 && len)
        {
            char **pos = (char **)malloc(sizeof(char *) * 2);
            uint8_t err = input_proc(user_input, pos);
            switch (err)
            {
                case 0: break;
                case HELP_CODE:
                {
                    free(user_input);
                    free(pos);
                    continue;
                }
                case EXIT_CODE:
                {
                    free(user_input);
                    free(pos);
                    fclose(stream);
                    return 0;
                }
                case INCORRECT_COMMAND_CODE:
                {
                    printf("Incorrect input. Check --help\n");
                    free(user_input);
                    free(pos);
                    continue;
                }
                case ERROR_INPUT_INCORRECT_SYMBOL:
                {
                    printf("Incorrect input. Check --help\n");
                    free(user_input);
                    free(pos);
                    continue;
                }
                case ERROR_INPUT_INCORRECT_LEN:
                {
                    printf("Incorrect number of pos. Check --help\n");
                    free(user_input);
                    free(pos);
                    continue;
                }
                default:
                {
                    perror("Specific case. Check --help\n");
                    free(user_input);
                    free(pos);
                    continue;
                }

            }
            if (err == HELP_CODE) continue;
            uint8_t opos = get_pos_value(&pos[0][0], &pos[0][1]);
            if (global->ChessBoard[OPOS_X][OPOS_Y]->obj.type == empty)
            {
                printf("ERROR: EMPTY SQUARE MOVED\n");
                free(user_input); free(pos);
                continue;
            }
            uint8_t npos = get_pos_value(&pos[1][0], &pos[1][1]);
            if (!check_correct_of_movement(global, &opos, &npos))
            {
                int8_t err = SIMPLE_new_record(stream, &opos, &npos);
                if (err)
                {
                    free(user_input);
                    free(pos);
                    fclose(stream);
                    return -1;
                }
                else
                {
                    user_move(global, &opos, &npos);
                }
            }
            else
                printf("Invalid move\n");

            free(pos);
        }
        else
        {
            free(user_input);
            if (feof(stdin))
            {
                printf("Get eof, safe exit\n");
                fclose(stream);
                return 0;
            }
            else
            {
                perror("getline");
                fclose(stream);
                return ERROR_INPUT_GETLINE;
            }
        }
        free(user_input);
    }

    fclose(stream);
    return 0;
}
