#ifndef CLISSSION
#define CLISSSION

struct square;

#ifndef CHESSUTILITIES
enum color { none = 0, black = 1, white = 2 };
#endif

#define EXIT_CODE                   0xff
#define HELP_CODE                   0xfa
#define INCORRECT_COMMAND_CODE      0xfb

#define GET_STANDART_SYMBOL(symb) (32 * (symb < 97)) + symb

#include <stdint.h>
uint8_t CLI_run_session(struct ChessGame *global);

#endif
