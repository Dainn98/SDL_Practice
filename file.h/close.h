#ifndef CLOSE_H
#define CLOSE_H
#include "declaration.h"
#include "LTexture.h"
void close() {
    for (int i = 0; i < 4; ++i) gTextures[i].free(); // => free loaded image
    SDL_DestroyTexture(birdTexture);
    gBackgroundTexture.free();
    // gSpriteSheetTexture.free();
    gBaseSurface.free();
   //destroy window,texture,render
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow= NULL;
    //quit SDL subsystem
    IMG_Quit();
    SDL_Quit();
}

#endif
