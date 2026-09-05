#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_render.h>

struct chess;

#define WINDOW_WIDTH		800
#define WINDOW_HEIGHT		640

#define BUTTON_MENU_WIDTH		150
#define BUTTON_MENU_HEIGHT		40


#define CORRECT_GUI_EXIT_CODE		0xed

uint8_t gui_start_menu(struct chess *global);

struct button {
	SDL_Rect rect;
	SDL_bool isHighlighted;
};

#endif /* GUI_H */
