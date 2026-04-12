#ifndef CHESSLOGGING
#define CHESSLOGGING

#include <stdio.h>
#include <stdint.h>

enum logging_t {
    modern_move_logging, poor_move_logging, debugging
};

enum mstate_t {
    move,
    white_win,
    black_win
};

FILE *new_logging(enum logging_t type);
int8_t SIMPLE_new_record(FILE *stream, uint8_t *opos, uint8_t *npos);
int8_t PGN_new_record(FILE *stream, uint8_t *opos, uint8_t *npos, 
                                                            enum mstate_t);
int8_t new_debug_record(char *fmt, ...);
int32_t close_debug_stream();

#endif // CHESSLOGGING
