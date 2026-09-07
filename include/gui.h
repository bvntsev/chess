#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_render.h>


struct chess;

#define WINDOW_WIDTH		1000
#define WINDOW_HEIGHT		1000

#define BUTTON_MENU_WIDTH		200
#define BUTTON_MENU_HEIGHT		80


#define CORRECT_GUI_EXIT_CODE		0xed

uint8_t gui_start_menu(struct chess *global);

struct button {
	SDL_Rect rect;
	SDL_bool isHighlighted;
	SDL_Texture *idleButton;
	SDL_Texture *hgButton;
};

struct chess;
struct piece;

struct guiPiece {
	SDL_bool is_highlighted;
	SDL_Rect pos;
	SDL_Texture *texture;
};


struct active_figure {
	struct piece *obj;
	struct guiPiece *gui;
};


struct guiChess {
	struct chess *engine;
	struct guiPiece guiBoard[8][8];
	struct active_;
	/* time_t dtime; // timer? */
};

#endif /* GUI_H */
