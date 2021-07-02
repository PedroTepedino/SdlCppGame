#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "LTexture.h"
#include "LButton.h"

extern SDL_Renderer* gRenderer;
extern TTF_Font *gFont;
extern LTexture gButtonSpriteSheetTexture;
extern SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];

#endif