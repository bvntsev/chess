#include <SDL2/SDL.h>
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


static SDL_Window *gui_init_window(struct chess *global) {

	new_debug_record("gui_init\n");
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	    printf("SDL_Init error: %s\n", SDL_GetError());
    	return NULL;
	}
	
	fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
	
	SDL_Window *screen = SDL_CreateWindow(
    	"bvchess",
	    SDL_WINDOWPOS_CENTERED,
    	SDL_WINDOWPOS_CENTERED,
	    800,
    	640,
	    SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
	);

	if (screen == NULL) {
    	printf("SDL_CreateWindow error: %s\n", SDL_GetError());
	    SDL_Quit();
    	return NULL;
	}
	new_debug_record("gui was inited\n");

	return screen;
}


static uint8_t gui_start_pvp_one_device(struct chess *global) {
	// TODO: switch-case of chess mode (local/one_device/vs_bot)
	
	{
		uint8_t gui_init_err;
		switch (gui_init_err) {
			case 0xff: {
				printf("screen wasn't created. Error exit\n");
				exit(-1);
			}
			default: break;
		}
	}
	return 255;
}


uint8_t gui_start_menu(struct chess *global) {
	SDL_Window *screen = gui_init_window(global);
	if (screen == NULL) {
		return ERROR_GUI_WINDOW_INIT;
	}
	for (;;) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
			SDL_DestroyWindow(screen);
			SDL_Quit();
			return CORRECT_GUI_EXIT_CODE;
		}
		/* SDL_FillRect( */
		SDL_ShowWindow(screen);
		/* SDL_Delay(80); */
		}
	}
	SDL_DestroyWindow(screen);
	SDL_Quit();
	return CORRECT_GUI_EXIT_CODE;
}
