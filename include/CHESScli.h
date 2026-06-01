#ifndef CLISSSION
#define CLISSSION

#include <stdint.h>

#define EXIT_CODE                   0xff
#define HELP_CODE                   0xfa
#define INVALID_OPTION              0xfb
#define RELOAD_CODE                 0xfc
#define CLEAR_CODE 					0xfd

#define GET_STANDART_SYMBOL(symb) (32 * (symb < 97)) + symb

struct chess;
uint8_t CLI_run_session_pvp(struct chess *global);

#endif
