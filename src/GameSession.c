#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "../include/chessUtilities.h"
#include "../include/GameSession.h"


char get_fig_symbol(enum type_pieces *type, enum color *color) {
    // printf(*color == black ? "IT IS BLACK\n" : "IT IS WHITE\n");
    switch(*type) {
        case pawn:      return *color == black ? 'p' : 'P';
        case king:      return *color == black ? 'k' : 'K';
        case queen:     return *color == black ? 'q' : 'Q';
        case rook:      return *color == black ? 'r' : 'R';
        case bishop:    return *color == black ? 'b' : 'B';
        case knight:    return *color == black ? 'n' : 'N';
        case empty:     return '.';
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    }
}


static void print_ChessBoard_CLI(struct square ***ChessBoard, enum color *user_side) {
    // printf_ascii_charactrers();
    switch (*user_side) {
        case white: {
            printf("    A B C D E F G H\n");
            for (u8 i = 0; i < 8; ++i) {
                printf("%d | ", 8 - i);
                for (u8 j = 0; j < 8; ++j) {
                    printf("%c ", get_fig_symbol(
                                &ChessBoard[i][j]->obj.type,
                                &ChessBoard[i][j]->obj.side));
                }
                printf("| %d\n", 8 - i);
            }
            printf("    A B C D E F G H\n");
            break;
        }
        case black: {
            printf("    H G F E D C B A\n");
            for (u8 i = 0; i < 8; ++i) {
                printf("%d | ", i + 1);
                for (u8 j = 8; j > 0; --j) {
                    printf("%c ", get_fig_symbol(
                                &ChessBoard[i][j - 1]->obj.type,
                                &ChessBoard[i][j - 1]->obj.side));
                }
                printf("| %d\n", i + 1);
            }
            printf("    H G F E D C B A\n");
            break;
        }
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    }
}


static unsigned char get_pos_value
                        (char *symbol, char *number, enum color *user_side) {
    return ((GET_STANDART_SYMBOL(*symbol) - 96))    + 
        // ((64 - *number) * 8) * *user_side == white  + 
        ((*number - 1) * 8);
}


static enum type_pieces get_pieces_type(unsigned char symb) {
    symb = GET_STANDART_SYMBOL(symb);
    switch (symb) {
        case 'n':   return knight;
        case 'q':   return queen;
        case 'k':   return king;
        case 'r':   return rook;
        case 'b':   return bishop;
        case 'p':   return pawn;
        default :   return empty;
    }
}


static char check_correct_input(char **pos) {
    if ((pos[0][1] && pos[1][1]) && ((pos[0][1] > '0' && pos[0][1] < '9') &&
                                        (pos[1][1] > '0' && pos[1][1] < '9')))
        return 0;
    return ERROR_INPUT_INCORRECT_SYMBOL;

}


static unsigned char input_proc(char *input, char **pos) {
    if (strcmp(input, "help") == 0){}
    else if (strcmp(input, "quit") == 0) {}
    else {
        char lpos = 0;
        for (unsigned char iter = 0; iter < strlen(input); ++iter) {
            if (lpos == 2) break;
            if ('a' <= GET_STANDART_SYMBOL(input[iter]) && 
                 GET_STANDART_SYMBOL(input[iter]) <= 'h') {
                pos[lpos++] = &input[iter];
            }
        }
        if (lpos < 2) return ERROR_INPUT_INCORRECT_LEN;
        if (check_correct_input(pos)) return ERROR_INPUT_INCORRECT_SYMBOL;
    }
    return 0;
}


static unsigned char user_move(
        struct ChessGame *global, unsigned char *opos, unsigned char *npos
        ) {
    // for a while just replace. In the future I add completly checking correct
    printf("opos - %d%d\n", (*opos - 1) / 8, (*opos - 1) % 8);
    printf("npos - %d%d\n", (*npos - 1) / 8, (*npos - 1) % 8);

    printf("opos type %c\n", get_fig_symbol(&global->ChessBoard[(*opos - 1) / 8]
                [(*opos - 1) % 8]->obj.type,
            &global->ChessBoard[(*opos - 1) / 8][(*opos - 1) % 8]->obj.side));

    printf("npos type %c\n", get_fig_symbol(&global->ChessBoard[(*npos - 1) / 8]
                [(*npos - 1) % 8]->obj.type,
            &global->ChessBoard[(*npos - 1) / 8][(*npos - 1) % 8]->obj.side));

    // 64 - A - 1 / 8

  
    // black
    global->ChessBoard[(*npos - 1) / 8][(*npos - 1) % 8]->obj.type =
            global->ChessBoard[(*opos - 1) / 8][(*opos - 1) % 8]->obj.type;
    global->ChessBoard[(*npos - 1) / 8][(*npos - 1) % 8]->obj.side =
            global->ChessBoard[(*opos - 1) / 8][(*opos - 1) % 8]->obj.side;

    global->ChessBoard[(*opos - 1) / 8][(*opos - 1) % 8]->obj.type = empty;
    global->ChessBoard[(*opos - 1) / 8][(*opos - 1) % 8]->obj.side = none;
    

    return 0;
}


extern ssize_t getline(char **lineptr, size_t *n, FILE *stream);

static void printf_debug(struct square ***ChessBoard) {
    // printf_ascii_charactrers();
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.side);
        }
        printf("\n");
    }
    printf("\n");

    // for (u8 i = 0; i < 8; ++i) {
    //     for (u8 j = 0; j < 8; ++j) {
    //         printf("%d ", ChessBoard[i][j]->side);
    //     }
    //     printf("\n");
    //     }
    printf("\n");

    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.type);
        }
        printf("\n");
    }
    printf("\n");
    // for (u8 i = 0; i < 8; ++i) {
    //     for (u8 j = 0; j < 8; ++j) {
    //         printf("%d ", ChessBoard[i][j]->pos);
    //     }
    //     printf("\n");
    // }
    printf("\n");
}

void run_session(struct ChessGame *global) {
    unsigned char status = GAME_STATUS_SESSION_ACTIVE;
    for (; status == GAME_STATUS_SESSION_ACTIVE; ) {
        // printf_debug(global->ChessBoard);
        print_ChessBoard_CLI(global->ChessBoard, &global->user_side);
        char *user_input = NULL;
        size_t len;
        if (getline(&user_input, &len, stdin)) {
            char **pos = (char **)malloc(sizeof(char *) * 2);
            unsigned char err = input_proc(user_input, pos);
            switch (err) {
                case 0: break;
                case ERROR_INPUT_INCORRECT_SYMBOL:
                        printf("Incorrect input\n");
                        exit(-1);
                        break;
                case ERROR_INPUT_INCORRECT_LEN:
                        printf("Incorrect number of pos. Check --help\n");
                        exit(-1);
                        break;
            }
            unsigned char opos = get_pos_value(&pos[0][0], &pos[0][1],
                    &global->user_side);
            if (global->ChessBoard[(opos - 1) / 8][(opos - 1) % 8]->obj.type ==
                empty) {
                printf("ERROR: EMPTY SQUARE MOVED\n");
                free(user_input); free(pos);
                continue;
            }
            if (global->ChessBoard[(opos - 1) / 8][(opos - 1) % 8]->obj.side != 
                                                        global->user_side) {

                printf("ERROR_MOVE_FIGURE_OF_OTHER_SIDE\n");
                free(user_input); free(pos);
                continue;
            }
            unsigned char npos = get_pos_value(&pos[1][0], &pos[1][1],
                    &global->user_side);

            printf("opos=%d\nnpos=%d\n", opos, npos);
            err = user_move(global, &opos, &npos);
            free(pos);
        }
        free(user_input);
  }

  switch (status) {

  }
}

