#ifndef COMMON_FUNC_H_
#define COMMON_FUNC_H_
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
// #include "CommonTexture.h"
using namespace std;

static SDL_Event* g_event;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* birdTexture = NULL;
SDL_Texture* pipeTexture = NULL;
TTF_Font* gFont = NULL; 	//New
// LTexture gTextTexture;
//SCREEN
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
const int BACKGROUND_WIDTH = 1280;
const int BACKGROUND_HEIGHT = 800;
const int BASE_WIDTH = 1182;
const int BASE_HEIGHT = 134;
const int BASE_QUARTER_WIDTH = BASE_WIDTH / 4;
const int UPPER_PIPE_OFFSET = -40;
const int LOWER_PIPE_OFFSET = 150;
const int LOWER_PIPE_HEIGHT_OFFSET = 134;
const int BACKGROUND_FRAME = 8;
const int PIRANHA_PLANT = 5;

//Bird
const int BIRD_WIDTH = 50;
const int BIRD_HEIGHT = 50;
//Pipe
const int PIPE_WIDTH = 100;
const int PIPE_HEIGHT = 300;
const int FLYING_ANIMATION_FRAMES_OF_BIRD = 5; 
//Feature 
const int DELAY = 20;
const int GRAVITY = 1;
const int BIRD_JUMP_VELOCITY = -15;
const int PIPE_VELOCITY = 15;
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;



// LTexture gBackgroundTexture;
// LTexture gStretchedSurface;
// LTexture gTextures[FLYING_ANIMATION_FRAMES_OF_BIRD];
// static SDL_Window* g_window = NULL;
// static SDL_Renderer* g_renderer = NULL;
// #include <SDL_mixer.h>
// #include <string>
// #include <iostream>
// #include <cstdlib>
// #include <ctime>
// #include <stdio.h>
#endif
