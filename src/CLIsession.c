/* parsing input, display, output */
#include "../include/CLIsession.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static u8 find_obj(const char *str, const char obj) {
  for (u32 i = 0; i < strlen(str); ++i)
    if(str[i] == obj)
      return 1;
  return 0;
}

char **parsing_user_move(const char *user_move) {

  u8 beats = 0;

  for (u32 i = 0; i < strlen(user_move); ++i)
    {
      if ((find_obj("abcdefgh+#=QRBNK12345678O-", user_move[i]))) {
          if (user_move[i] == 'x') beats = 1;
        }
      else
        return NULL;
    }
  char *start_pos;
  char *final_pos;


  

  /* example */
  /* pawn */
  /*   d5 */
  /*   cxd4 */
  /*   bxc1=Q+ */

  /* Rfb5; */
  /* Rfxb5; */
  /* Rfxb5+ */
  /* Rfxb5# */
  /* Rbxc1+ */
  /* if (find_obj(user_move, 'x')) */


  
}

void runtime(){
  u8 status = GAME_STATUS_SESSION_ACTIVE;


  while (status == GAME_STATUS_SESSION_ACTIVE){
    char *user_input = (char *)(malloc(sizeof(char)*10));
    if (user_input)
      scanf("%9s", user_input);
    
  }

  switch (status){

  }
}

void parsing_input(const char *inp){

}
