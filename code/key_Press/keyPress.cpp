#include <SDL2/SDL.h>
#include <iostream>
#include <string>
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//Key press surfaces constants
enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL,
};
//The window we'll be rendering to
SDL_Surface* loadSurface( string path );
SDL_Window* gWindow = NULL;
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;
//The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
//Current displayed image
SDL_Surface* gCurrentSurface = NULL;
//Starts up SDL and creates window
bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cout << "SDL coult not initialize! SDL Error: \n" << SDL_GetError();
        success = false;
    }else{
        gWindow = SDL_CreateWindow("TuanAnh",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            cout << "Window could not initialize! SDL Error: \n "<< SDL_GetError();
            success = false;
        }else gScreenSurface = SDL_GetWindowSurface(gWindow);

    }

}
//Loads media
bool loadMedia(){
    bool success = true;
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("image/press.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL){
        cout << "Failed to load default image!" <<endl;
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("image/up.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL){
        cout << "Failed to load up image!" <<endl;
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("image/down.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL){
        cout << "Failed to load down image!" <<endl;
        success = false;
    }
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("image/right.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL){
        cout << "Failed to load right image!" <<endl;
        success = false;
    }
     gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("image/left.bmp");
    if(gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL){
        cout << "Failed to load left image!" <<endl;
        success = false;
    }
    return success;
}
//Frees media and shuts down SDL

//Loads individual image
SDL_Surface* loadSurface( string path ){
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL ) cout << "Unable to load image ! SDL Error: \n"<<path.c_str() << SDL_GetError() ;
	return loadedSurface;
}
void close(){
    for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i ){
		SDL_FreeSurface(gKeyPressSurfaces[ i ]);
		gKeyPressSurfaces[i] = NULL;
	}
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}
int main(int argc, char* args[]){
    if(!init()) cout << "Failed to initialize";
    else{
        if(!loadMedia) cout <<"F2 load Media\n";
        else{
            bool quit = false;
            SDL_Event e;
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT) quit = true;
                    else if(e.type == SDL_KEYDOWN){
                        switch(e.key.keysym.sym){
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            default :
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;
                        }
                    }
                }
                SDL_UpperBlit(gCurrentSurface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();
    return 0;
}