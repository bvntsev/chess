#include "../include/chessUtilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char *get_str_position(const u8 row, const u8 column) {
    char *str = malloc(sizeof(char) * 3);
    if (str){
    str[0] = 'A' + column;
    str[1] = '1' + row;
    str[2] = '\0';
    }

    return str;
}


u8 get_num_position(const char *str) {
    return (str[0]-'A'+1)*10 + (str[1]-'1'+1);
}


static void printf_binary(char *value){
    for (int i = 7; i >= 0; --i)
        printf("%i", ((1 << i) & (*value)) >> i);
    printf("\n");
}


static char *get_start_pos_str(enum color *user_side){
    char *start_pos;
    switch (*user_side) {
        case black: {
            start_pos =
            "dnbkqbndppppppppEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEPPPPPPPPDNBKQBND";
            break;
    }
        case white: {
            start_pos =
            "DNBKQBNDPPPPPPPPEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEppppppppdnbkqbnd";
            break;
        }
        case none: {
            printf("%s:%d none error\n", __FILE__, __LINE__);
            exit(-1);
        }
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
    return start_pos;
};


static void create_board_white(struct ChessGame *global, char *start_pos) {

    global->ChessBoard = (malloc(sizeof(struct square **) * 8));
    for (char i = 8; i > 0; --i) {
        global->ChessBoard[i - 1] = (malloc(sizeof(struct square *) * 8));
        for (int j = 8; j > 0; --j) {
            global->ChessBoard[i - 1][j - 1] = (malloc(sizeof(struct square )));

            /* set color the board square */
            global->ChessBoard[i - 1][j - 1]->side = (2 - ((i ^ j) & 1));
            /* set piece color */
            global->ChessBoard[i - 1][j - 1]->obj.side =
                ((start_pos[(i - 1) * 8 + j - 1] ^ 'E') != 0) +
                                    ((start_pos[(i - 1) * 8 + j - 1] >> 5) & 1);
            /* set piece type */
            global->ChessBoard[i - 1][j - 1]->obj.type =
                (7 & start_pos[(i - 1) * 8 + j - 1]);
            /* set position square */
            global->ChessBoard[i - 1][j - 1]->pos = (8 - i) * 8 + j; //--
        }
    }
}


static void create_board_black(struct ChessGame *global, char *start_pos) {
    global->ChessBoard = (malloc(sizeof(struct square **) * 8));
    for (int i = 0; i < 8; ++i) {
        global->ChessBoard[i] = (malloc(sizeof(struct square *) * 8));
        for (int j = 0; j < 8; ++j) {
            global->ChessBoard[i][j] = (malloc(sizeof(struct square )));

            /* set color the board square */
            global->ChessBoard[i][j]->side = (2 - ((i ^ j) & 1));
            /* set piece color */
            global->ChessBoard[i][j]->obj.side =
                ((start_pos[i * 8 + j] ^ 'E') != 0) +
                                        ((start_pos[i * 8 + j] >> 5) & 1);
            /* set piece type */
            global->ChessBoard[i][j]->obj.type =
                (7 & start_pos[i * 8 + j]);
            /* set position square */
            global->ChessBoard[i][j]->pos = i * 8 + (8 - j);
        }
    }
}
void create_board(struct ChessGame *global) {
    char *start_pos = get_start_pos_str(&global->user_side);
    switch (global->user_side) {
        case black: return create_board_black(global, start_pos);
        case white: return create_board_white(global, start_pos);
        case none: {
            printf("%s:%d none error\n", __FILE__, __LINE__);
            exit(-1);
        }
        default: {
            printf("%s:%d default error\n", __FILE__, __LINE__);
            exit(-1);
        }
    };
}


static void printf_ascii_charactrers(){
    PRINT_DASH
    printf("Number\tCharacter\tBinary\n");
    for (char iter = 'A'; iter < 'A' + 26; ++iter) {
        printf("%d\t\t%c\t\t\t", iter, iter);
        printf_binary(&iter);
    }
    PRINT_DASH
    for (char iter = 'a'; iter < 'a' + 26; ++iter) {
        printf("%d\t\t%c\t\t\t", iter, iter);
        printf_binary(&iter);
    }
    PRINT_DASH
};


void printf_board(struct square *** table) {
    // printf_ascii_charactrers();
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", table[i][j]->obj.side);
        }
        printf("\n");
    }
    printf("\n");

    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", table[i][j]->side);
        }
        printf("\n");
        }
    printf("\n");

    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", table[i][j]->obj.type);
        }
        printf("\n");
    }
    printf("\n");
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", table[i][j]->pos);
        }
        printf("\n");
    }
    printf("\n");
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%s ", get_str_position((table[i][j]->pos-1) / 8, (table[i][j]->pos-1) % 8));
        }
        printf("\n");
    }
}
