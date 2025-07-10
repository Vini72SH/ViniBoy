#include "../include/ui.h"

#include "../include/emu.h"

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

void delay(uint32_t ms) { return SDL_Delay(ms); }

void ui_init() {
    SDL_Init(SDL_INIT_VIDEO);
    DEBUG_PRINT("SDL INIT\n");
    TTF_Init();
    DEBUG_PRINT("TTF INIT\n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow,
                                &sdlRenderer);
}

void ui_handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0) {
        if (e.type == SDL_WINDOWEVENT &&
            e.window.event == SDL_WINDOWEVENT_CLOSE) {
            emu_get_context()->stop = true;
        }
    }
}

void ui_exit() {
    SDL_Quit();
    DEBUG_PRINT("SDL QUIT\n");
    TTF_Quit();
    DEBUG_PRINT("TTF INIT\n");
}