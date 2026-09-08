#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/gui.h"
#include "../include/util.h"
#include "../include/engine.h"
#include "../include/logging.h"


static SDL_bool is_under_mcursor(int32_t x_cursor, int32_t y_cursor,
			int32_t x_obj, int32_t y_obj, int32_t w_obj, int32_t h_obj) {
	return (x_cursor >= x_obj && x_cursor <= (x_obj + w_obj))
			&& (y_cursor >= y_obj && y_cursor <= (y_obj + h_obj));
}


static char *get_path_to_texture(char *file_name) {
	char *rp = get_rp_to_chess_dir();
	char *texturePath;
	asprintf(&texturePath, "%s/%s", rp, file_name);
	free(rp);
	return texturePath;
}


void draw_board(SDL_Renderer *renderer, struct guiChess *global) {
	SDL_Rect square;
	square.w = 125;
	square.h = 125;
	for (uint8_t i = 0; i < 8; ++i) { // only if I make move or holding piece?
		square.y = 125 * i;
		for (uint8_t j = 0; j < 8; ++j) {
			square.x = 125 * j;
			if (global->engine->board[i][j].side == white) {
				if (global->guiBoard[i][j].is_highlighted == SDL_TRUE)
					SDL_SetRenderDrawColor(renderer, 160, 216, 183, 255);
				else 
					SDL_SetRenderDrawColor(renderer, 240, 195, 128, 255);
			}
			else {
				if (global->guiBoard[i][j].is_highlighted == SDL_TRUE)
					SDL_SetRenderDrawColor(renderer, 182, 215, 168, 255);
				else
					SDL_SetRenderDrawColor(renderer, 109, 62, 23, 255);
			}
			SDL_RenderFillRect(renderer, &square);
			SDL_RenderCopy(renderer, global->guiBoard[i][j].texture, NULL, &global->guiBoard[i][j].pos);
		}
	}
}


static SDL_Texture *
get_figure_texture(SDL_Renderer *renderer, struct piece *fig)
{
	char *texturePath = NULL;
	switch (fig->type) {
		case pawn:
			texturePath = get_path_to_texture(fig->side == white ? "pawn.png" : "pawn1.png");
			break;
		case king:
			texturePath = get_path_to_texture(fig->side == white ? "king.png" : "king1.png");
			break;
		case knight:
			texturePath = get_path_to_texture(fig->side == white ? "knight.png" : "knight1.png");
			break;
		case queen:
			texturePath = get_path_to_texture(fig->side == white ? "queen.png" : "queen1.png");
			break;
		case rook:
			texturePath = get_path_to_texture(fig->side == white ? "rook.png" : "rook1.png");
			break;
		case empty:
			return NULL;
		case bishop:
			texturePath = get_path_to_texture(fig->side == white ? "bishop.png" : "bishop1.png");
			break;
	}
	SDL_Texture *buffer = IMG_LoadTexture(renderer, texturePath);
	free(texturePath);

	if (buffer == NULL) {
		printf("SDL_IMG_LoadTexture error: %s\n", SDL_GetError());
		return NULL;
	}
	return buffer;
}


static uint8_t
gui_start_pvp_one_device(struct chess *engine, SDL_Window *window,
						SDL_Renderer *renderer)
{
	// TODO: switch-case of chess mode (local/one_device/vs_bot)
	/* SDL_RenderClear(renderer); */
	uint8_t is_running = 1;
	struct guiChess global;
	global.engine = engine;
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			global.guiBoard[i][j].is_highlighted = SDL_FALSE;
			global.guiBoard[i][j].texture = get_figure_texture(renderer,
												&engine->board[i][j].obj);
			global.guiBoard[i][j].pos.w = 125;
			global.guiBoard[i][j].pos.h = 125;
			global.guiBoard[i][j].pos.x = j * 125; // BUG: If I change window resolution I get glitches 
			global.guiBoard[i][j].pos.y = i * 125;
		}
	}


	SDL_RenderClear(renderer);
	draw_board(renderer, &global);
	SDL_RenderPresent(renderer);
	SDL_bool is_mouse_holding = SDL_FALSE;
	struct active_figure oldPos;
	struct active_figure active;
	active.gui = NULL;
	active.obj = NULL;
	
	while (is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_q:
						case SDLK_ESCAPE:
							if (oldPos.gui != NULL) {
								free(oldPos.gui);
								free(oldPos.obj);
							}
							return 1;
						case SDLK_r:
							if (oldPos.gui != NULL) {
								free(oldPos.gui);
								free(oldPos.obj);
							}
							return 2;
					}
					break;
				case SDL_MOUSEMOTION:
					switch (event.button.button) {
						case 1: {// Left button
							if (!is_mouse_holding || active.gui == NULL) break;
							active.gui->pos.x = event.motion.x - 62;
							active.gui->pos.y = event.motion.y - 62;
							
							SDL_RenderClear(renderer);
							draw_board(renderer, &global);
							SDL_RenderCopy(renderer, active.gui->texture, NULL, &active.gui->pos);
							SDL_RenderPresent(renderer);
						}
						default: break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					is_mouse_holding = SDL_TRUE;
					for (uint8_t i = 0; i < 8; ++i)
						for (uint8_t j = 0; j < 8; ++j) {
#define ELEMENT_BELOW global.guiBoard[i][j].pos
							if (global.engine->board[i][j].obj.type != empty && is_under_mcursor(event.button.x, event.button.y,
								ELEMENT_BELOW.x, ELEMENT_BELOW.y,
								ELEMENT_BELOW.w, ELEMENT_BELOW.h)) {
							oldPos.gui = (struct guiPiece *)(malloc(sizeof(struct guiPiece)));
							oldPos.obj = (struct piece *)(malloc(sizeof(struct piece)));	


							// TODO: REWRITE THIS SHIT CODE!!
							oldPos.gui->is_highlighted = global.guiBoard[i][j].is_highlighted;
							oldPos.gui->pos = global.guiBoard[i][j].pos;
							oldPos.gui->texture = global.guiBoard[i][j].texture;

							oldPos.obj->side = global.engine->board[i][j].obj.side;
							oldPos.obj->type = global.engine->board[i][j].obj.type;

							active.gui = &global.guiBoard[i][j];
							active.obj = &global.engine->board[i][j].obj;

							global.guiBoard[i][j].is_highlighted = SDL_TRUE;
							goto found;
							}
						}
found:
					/* struct piece *obj = find_obj( */
					break;
				case SDL_MOUSEBUTTONUP: // BUG: If I move figure on friend figure piece doesn't return back to start position
					printf("SDL_MOUSEBUTTONUP\n");
					switch (event.button.button) {
						case 1: {// Left button
						for (uint8_t i = 0; i < 8; ++i) // If I move active figure below cursor I get literally it
							for (uint8_t j = 0; j < 8; ++j) {							
								if (global.engine->board[i][j].obj.type != empty && is_under_mcursor(event.button.x, event.button.y,
									ELEMENT_BELOW.x, ELEMENT_BELOW.y,
									ELEMENT_BELOW.w, ELEMENT_BELOW.h) &&
									&global.engine->board[i][j].obj != active.obj
									|| event.button.x < 0 || event.button.x > 1000 ||
									event.button.y < 0 || event.button.y > 1000) {

									
									/* printf("%d %d\n", event.button.x, event.button.y); */
									/* print_square_info(&global.engine->board[i][j]); */
									
									printf("ELEMENT_BELOW\n");
									if (event.button.x < 0 || event.button.x > 1000 ||
									event.button.y < 0 || event.button.y > 1000
									|| engine->board[i][j].obj.side == oldPos.obj->side) {

									active.gui->is_highlighted = oldPos.gui->is_highlighted;
									active.gui->pos = oldPos.gui->pos;
									active.gui->texture = oldPos.gui->texture;
	
									active.obj->side = oldPos.obj->side;
									active.obj->type = oldPos.obj->type;

									active.gui->is_highlighted = SDL_FALSE;
									active.gui = NULL;
									active.obj = NULL;
									printf("=========================\n");									
									printf("End section\n");
									
									free(oldPos.gui);
									free(oldPos.obj);
									oldPos.gui = NULL;
									oldPos.obj = NULL;
									
									SDL_RenderClear(renderer);
									draw_board(renderer, &global);
									SDL_RenderPresent(renderer);
									goto mbutton_up_quit; // I wrote really shit code. But it'll for a while
									}
								}
							}
						}
					}
mbutton_up_quit:
					printf("end case\n");
					is_mouse_holding = SDL_FALSE;
					break;
				case SDL_QUIT:
					is_running = 0;
					break;
			}
		}
		/* SDL_RenderPresent(renderer); ???*/
		SDL_Delay(25);
	}
	free(oldPos.gui);
	free(oldPos.obj);
	return 0;
}


static struct button create_button(int32_t x, int32_t y,
				SDL_Renderer *renderer, char *file_name) {
	struct button bt;
	bt.isHighlighted = SDL_FALSE;
	bt.rect.x = x;
	bt.rect.y = y;
	bt.rect.w = BUTTON_MENU_WIDTH;
	bt.rect.h = BUTTON_MENU_HEIGHT;

	char *texturePath = get_path_to_texture(file_name);
	bt.idleButton = IMG_LoadTexture(renderer, texturePath);
	free(texturePath);

	if (bt.idleButton == NULL) {
		printf("SDL_IMG_LoadTexture error: %s\n", SDL_GetError());
	}
	/* bt.hgButton = IMG_LoadTexture(renderer, path_to_hg); */

	return bt;
}


static void load_board(SDL_Window *window) {
	
}


uint8_t gui_start_menu(struct chess *global) {

	new_debug_record("gui_init\n");
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	    printf("SDL_Init error: %s\n", SDL_GetError());
    	return ERROR_GUI_WINDOW_INIT;
	}
	
	SDL_Window *window = SDL_CreateWindow(
    	"bvchess",
	    SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED,
	    WINDOW_WIDTH,
    	WINDOW_HEIGHT,
	    SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN | SDL_WINDOW_BORDERLESS
	);

	if (window == NULL) {
    	printf("SDL_CreateWindow error: %s\n", SDL_GetError());
	    SDL_Quit();
    	return ERROR_GUI_WINDOW_CREATION;
	}
	new_debug_record("gui was inited\n");
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
					
	if (!renderer) {
    	printf("SDL_Renderer error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
	    SDL_Quit();
		return ERROR_GUI_RENDERER_CREATION;
	}
	/* SDL_Surface *surface = SDL_CreateRGBSurface(0, 200, 150, 8, 0, 0, 0, 0); */
	/* SDL_Surface *surface = SDL_GetWindowSurface(window); */
	SDL_ShowWindow(window);
	/* if (!surface) { */
	/* 	printf("SDL_Surface error: %s\n", SDL_GetError()); */
	/* 	SDL_DestroyWindow(window); */

	/* 	return ERROR_GUI_SURFACE_CREATION; */
	/* } */
	struct button pvp_one_device_button = create_button(
			WINDOW_WIDTH / 2 - 100, 500, renderer, "pvp_1d.png");
	struct button pvp_local_button = create_button(WINDOW_WIDTH / 2 - 100, 600,
			renderer, "pvp_2p.png");
	struct button pvp_bot_button = create_button(WINDOW_WIDTH /  2 - 100, 700,
			renderer, "pvp_ai.png");
	/* struct button exit_button = create_button(WINDOW_WIDTH / 2 - 50, 500); */
	struct button exit_button = create_button(WINDOW_WIDTH / 2 - 100, 800,
			renderer, "exit.png");
	/* struct button exit_button = create_button(WINDOW_WIDTH / 2 - BUTTON_MENU_WIDTH / 2, 300); mehh*/

	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	uint8_t is_running = 1;
	while (is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type){
				case SDL_MOUSEBUTTONUP:
			// When I press at a piece when it just moved under cursor 
					if (is_under_mcursor(event.button.x, event.button.y,
						exit_button.rect.x, exit_button.rect.y,
						exit_button.rect.w, exit_button.rect.h)) {
						printf("Exit button. Session will be ended soon\n");
						is_running = 0;
					}
					else if (is_under_mcursor(event.button.x, event.button.y,
				pvp_one_device_button.rect.x, pvp_one_device_button.rect.y,
				pvp_one_device_button.rect.w, pvp_one_device_button.rect.h))
					{
						/* SDL_DestroyTexture(pvp_bot_button.idleButton); */
						/* SDL_DestroyTexture(exit_button.idleButton); */
						/* SDL_DestroyTexture(pvp_local_button.idleButton); */
						/* SDL_DestroyTexture(pvp_local_button.idleButton); */
						SDL_RenderClear(renderer);
						SDL_RenderPresent(renderer);
						++is_running; // HARD-CODE crutch!!!
						while (is_running == 2) {// TODO: it's crutch CHANGE !!!
							is_running = gui_start_pvp_one_device
													(global, window, renderer);
						}
					}
					if (!is_running)
						goto quit;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
						case SDLK_q:
						case SDLK_ESCAPE:
							is_running = 0;
						default:
							break;
					break; 
				case SDL_QUIT: {
					is_running = 0;
					break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 213, 189, 175, 255);
		SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, exit_button.idleButton, NULL, &exit_button.rect);
		SDL_RenderCopy(renderer, pvp_bot_button.idleButton, NULL, &pvp_bot_button.rect);
		SDL_RenderCopy(renderer, pvp_local_button.idleButton, NULL, &pvp_local_button.rect);
		SDL_RenderCopy(renderer, pvp_one_device_button.idleButton, NULL, &pvp_one_device_button.rect);
		
		/* SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */


		/* SDL_RenderFillRect(renderer, &exit_button.rect); */
		
		/* sdl_RenderCopy(renderer, texture, NULL, &exit_button.rect); */
		/* SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */

		SDL_RenderPresent(renderer);

		/* SDL_ShowWindow(window); */
		/* SDL_Delay(80); */
		}
	}
	// TODO: think how to realize more optimaze option to calling the function
	// and in the case of exit to menu return to menu_loop without next call menu
quit:
	SDL_DestroyWindow(window);
	SDL_Quit();
	return CORRECT_GUI_EXIT_CODE;
}
