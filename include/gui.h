#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_types.h>

struct chess;


#define CORRECT_GUI_EXIT_CODE		0xed

uint8_t gui_start_menu(struct chess *global);

#endif /* GUI_H */
