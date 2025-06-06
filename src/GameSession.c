#include "../include/GameSession.h"

extern struct square ***table;

/* u8 check_correct_ */

u8 check_correct_move_user(char *startnig_pos, char *final_pos) {
  u8 starting_pos_num = get_num_position(startnig_pos);
  u8 final_pos_num = get_num_position(final_pos);



#define ROW(pos) pos / 10
#define COLUMN(pos) pos % 10

  if (table[ROW(starting_pos_num)][COLUMN(starting_pos_num)]->obj.type == empty){
    return ERROR_CODE_ABSENT_PIECES;
  }
  if (table[ROW(final_pos_num)][COLUMN(final_pos_num)]->obj.type != empty) {
    return ERROR_CODE_FINAL_POS_BUSY;
  }

  switch (table[ROW(starting_pos_num)][COLUMN(starting_pos_num)]->obj.type) {
  case pawn: {}
    case bishop: {}
    case knight: {}
    case rook: {}
    case queen: {}
    case king: {}
    case empty: {
      return ERROR_CODE_ABSENT_PIECES;
    }
  }
#undef ROW
#undef COLUMN

  return 1;
  
}
