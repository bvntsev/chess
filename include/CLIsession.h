#ifndef CLISSSION
#define CLISSSION

#include "GameSession.h"

char **parsing_user_move(const char *user_move);

#define GAME_STATUS_SESSION_ACTIVE 1
#define GAME_STATUS_END_CHECKMATE_BLACK 2
#define GAME_STATUS_END_CHECKMATE_WHITE 3
#define GAME_STATUS_END_STALEMATE 4


#endif /* CLISSSION */
