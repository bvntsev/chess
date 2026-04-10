#ifndef GAMESSSION
#define GAMESSSION

#include <stdint.h>

#define USER_INPUT_LEN 10

#define GAME_STATUS_SESSION_ACTIVE          0x01
#define GAME_STATUS_END_CHECKMATE_BLACK     0x02
#define GAME_STATUS_END_CHECKMATE_WHITE     0x03
#define GAME_STATUS_END_STALEMATE           0x04

#define ERROR_INPUT_ABSENT_PIECES           0x05
#define ERROR_INPUT_DISTANATION_BUSY        0x06
#define ERROR_INPUT_INCORRECT_SYMBOL        0x07
#define ERROR_INPUT_INCORRECT_LEN           0x08
#define ERROR_INPUT_FRIENDLY_ATTACK         0x09

#define ERROR_MOVE_FIGURE_OF_OTHER_SIDE     0x0a
#define ERROR_MINOR_MOVE                    0x0b // a1:a1

#define OPOS_X  (opos - 1)  / 8
#define OPOS_Y  (opos - 1)  % 8
#define OPOS_XP (*opos - 1) / 8
#define OPOS_YP (*opos - 1) % 8


#define NPOS_X  (npos - 1)  / 8
#define NPOS_Y  (npos - 1)  % 8
#define NPOS_XP (*npos - 1) / 8
#define NPOS_YP (*npos - 1) % 8

struct ChessGame;
void user_move(
        struct ChessGame *global, unsigned char *opos, unsigned char *npos
        );

int32_t check_correct_of_movement(
    struct ChessGame *global, uint8_t *opos, uint8_t *npos);
#endif
