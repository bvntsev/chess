#ifndef CLISSSION
#define CLISSSION

#ifndef GAMESESSION
#include "GameSession.h"
#endif GAMESESSION
#ifndef CHESSUTILITIES
#include "chessUtilities.h"
#endif

char **parsing_user_move(const char *user_move);

void print_ChessBoard_CLI(struct square ***ChessBoard, enum color *user_side);

#endif /* CLISSSION */
