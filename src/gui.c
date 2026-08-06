#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_types.h>

#include <unistd.h>

#include "../include/gui.h"
#include "../include/util.h"
#include "../include/engine.h"


uint8_t gui_init(struct chess *global) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init error: %s\n", SDL_GetError());
    return 1;
	}

	SDL_Window *screen = SDL_CreateWindow(
    "Chess",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    640,
    480,
    SDL_WINDOW_OPENGL
	);
	printf("Window created");
	for (;;)
		sleep(1);
}
