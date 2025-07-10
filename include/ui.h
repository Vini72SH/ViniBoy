#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void delay(uint32_t ms);

void ui_init();

void ui_handle_events();

void ui_exit();

#endif  // UI_H