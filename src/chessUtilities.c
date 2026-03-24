#include "../include/chessUtilities.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void printf_binary(char *value){
    for (int i = 7; i >= 0; --i)
        printf("%i", ((1 << i) & (*value)) >> i);
    printf("\n");
}


static void printf_ascii_charactrers(){
    PRINT_DASH
    printf("Number\tCharacter\tBinary\n");
    for (char iter = 'A'; iter < 'A' + 26; ++iter) {
    // for (unsigned char iter = 1; iter < 255; ++iter) {
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

static void printf_debug(struct square ***ChessBoard);


struct square ***create_ChessBoard
                        (struct square ***ChessBoard, enum color *user_side) {
    char *start_pos = get_start_pos_str(user_side);

    ChessBoard = (malloc(sizeof(struct square **) * 8));
    for (int i = 0; i < 8; ++i) {
        ChessBoard[i] = (malloc(sizeof(struct square *) * 8));
        for (int j = 0; j < 8; ++j) {
            ChessBoard[i][j] = (malloc(sizeof(struct square )));

            /* set color the board square */
            ChessBoard[i][j]->side = (2 - ((i ^ j) & 1));
            /* set piece color */
            ChessBoard[i][j]->obj.side =
                ((start_pos[i * 8 + j] ^ 'E') != 0) +
                                        ((start_pos[i * 8 + j] >> 5) & 1);
            /* set piece type */
            ChessBoard[i][j]->obj.type =
                (7 & start_pos[i * 8 + j]);
            /* set position square */
            ChessBoard[i][j]->pos = i * 8 + (8 - j);
        }
    }
    return ChessBoard;
}


static void printf_debug(struct square ***ChessBoard) {
    // printf_ascii_charactrers();
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.side);
        }
        printf("\n");
    }
    printf("\n");

    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->side);
        }
        printf("\n");
        }
    printf("\n");

    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->obj.type);
        }
        printf("\n");
    }
    printf("\n");
    for (u8 i = 0; i < 8; ++i) {
        for (u8 j = 0; j < 8; ++j) {
            printf("%d ", ChessBoard[i][j]->pos);
        }
        printf("\n");
    }
    printf("\n");
}
