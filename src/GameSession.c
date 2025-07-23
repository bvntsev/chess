#include "../include/GameSession.h"

extern struct square ***table;

/* u8 check_correct_ */



u8 check_correct_move_user(char *startnig_pos, char *final_pos) {

  u8 starting_pos_num = get_num_position(startnig_pos);
  u8 final_pos_num = get_num_position(final_pos);

#define ROW_START starting_pos_num / 10
#define COLUMN_START starting_pos_num % 10

#define ROW_FINAL final_pos_num / 10
#define COLUMN_FINAL final_pos_num % 10

  if (table[ROW_START][COLUMN_START]->obj.type == empty){
    return ERROR_CODE_ABSENT_PIECES;
  }
  if (table[ROW_FINAL][COLUMN_FINAL]->obj.type != empty) {
    return ERROR_CODE_FINAL_POS_BUSY;
  }

  switch (table[ROW_START][COLUMN_START]->obj.type) {
  case pawn: {
  /*   if() */

  /* } */
    case bishop: {}
    case knight: {}
    case rook: {}
    case queen: {}
    case king: {}
    case empty: {
      return ERROR_CODE_ABSENT_PIECES;
    }
  }
#undef ROW_START
#undef ROW_FINAL
#undef COLUMN_START
#undef COLUMN_FINAL

  return 1;
  }
}
