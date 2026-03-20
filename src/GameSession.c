#include "../include/GameSession.h"
#include <stdlib.h>
#include <stdio.h>

extern struct square ***table;

void run_session(){
    unsigned char status = GAME_STATUS_SESSION_ACTIVE;

    while (status == GAME_STATUS_SESSION_ACTIVE){
    char *user_input = (char *)(malloc(sizeof(char)*10));
    if (user_input)
        scanf("%9s", user_input);
    
  }

  switch (status){

  }
}

