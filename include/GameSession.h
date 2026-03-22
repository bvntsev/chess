#ifndef GAMESSSION
#define GAMESSSION

#define USER_INPUT_LEN 10


#define GAME_STATUS_SESSION_ACTIVE          1
#define GAME_STATUS_END_CHECKMATE_BLACK     2
#define GAME_STATUS_END_CHECKMATE_WHITE     3
#define GAME_STATUS_END_STALEMATE           4

#define ERROR_INPUT_ABSENT_PIECES           1
#define ERROR_INPUT_DISTANATION_BUSY        2
#define ERROR_INPUT_INCORRECT_SYMBOL        3
#define ERROR_INPUT_INCORRECT_LEN           4

#define ERROR_MOVE_FIGURE_OF_OTHER_SIDE     1

#define GET_STANDART_SYMBOL(symb) (32 * (symb < 97)) + symb

void run_session(struct ChessGame *global);

#endif /* GAMESSSION */
