#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_events.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "../include/gui.h"
#include "../include/util.h"
#include "../include/engine.h"
#include "../include/logging.h"


static uint8_t gui_start_pvp_one_device(struct chess *global) {
	// TODO: switch-case of chess mode (local/one_device/vs_bot)
	
	{
		uint8_t gui_init_err;
		switch (gui_init_err) {
			case 0xff: {
				printf("window wasn't created. Error exit\n");
				exit(-1);
			}
			default: break;
		}
	}
	return 255;
}


static struct button create_button(int32_t x, int32_t y) {
	struct button bt;
	bt.isHighlighted = SDL_FALSE;
	bt.rect.x = x;
	bt.rect.y = y;
	bt.rect.w = BUTTON_MENU_WIDTH;
	bt.rect.h = BUTTON_MENU_HEIGHT;
	return bt;
}


static SDL_bool is_under_mcursor(int32_t x_cursor, int32_t y_cursor,
			int32_t x_obj, int32_t y_obj, int32_t w_obj, int32_t h_obj) {
	return (x_cursor >= x_obj && x_cursor <= (x_obj + w_obj))
			&& (y_cursor >= y_obj && y_cursor <= (y_obj + h_obj));
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
	    SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN
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
	/* 	SDL_Quit(); */
	/* 	return ERROR_GUI_SURFACE_CREATION; */
	/* } */
	struct button pvp_one_device_button =
									create_button(WINDOW_WIDTH / 2 - 50, 300);
	struct button pvp_local_button = create_button(WINDOW_WIDTH / 2 - 50, 350);
	struct button pvp_bot_button = create_button(WINDOW_WIDTH /  2 - 50, 450);
	/* struct button exit_button = create_button(WINDOW_WIDTH / 2 - 50, 500); */
	struct button exit_button = create_button(350, 500);
	/* struct button exit_button = create_button(WINDOW_WIDTH / 2 - BUTTON_MENU_WIDTH / 2, 300); mehh*/
	
	
	uint8_t is_running = 1;
	while (is_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type){
			case SDL_MOUSEBUTTONDOWN:
			// When I press at a piece when it just moved under cursor 
				if (is_under_mcursor(event.button.x, event.button.y,
					exit_button.rect.x, exit_button.rect.y,
					exit_button.rect.w, exit_button.rect.h)) {
						printf("Exit button. Session will be ended soon\n");
						is_running = 0;
					}
				break;
			case SDL_MOUSEBUTTONUP:
				printf("--%d\n", event.button.timestamp);
				break;
			case SDL_QUIT: {
				is_running = 0;
			}
		}
		/* SDL_FillRect( */
		/* SDL_Surface *surface = SDL_CreateRGBSurface(0, 300, 100, 32, 0, 0, 0, 0); */

		// Throughout surface
		/* SDL_Surface *surface = SDL_GetWindowSurface(window); */

		/* uint32_t rgb = SDL_MapRGB(surface->format, 255, 255, 255); */
		/* SDL_FillRect(surface, &exit_button.rect, rgb); */

		/* SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface); */
		/* SDL_UpdateWindowSurface(window); */
		/* SDL_FreeSurface(surface); */
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


		SDL_RenderFillRect(renderer, &exit_button.rect);
		
		/* SDL_RenderCopy(renderer, texture, NULL, &exit_button.rect); */
		/* SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); */

		SDL_RenderPresent(renderer);

		/* SDL_ShowWindow(window); */
		/* SDL_Delay(80); */
		}
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return CORRECT_GUI_EXIT_CODE;
}
