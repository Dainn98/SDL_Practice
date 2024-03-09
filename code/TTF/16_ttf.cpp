// #include "file.h/CommonFunc.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class LTexture{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(string path);
        bool loadFromRenderedText(string textureText, SDL_Color textColor);
        void free();
        void setColor(Uint8 red,Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha);
         void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        int getWidth();
        int getHeight();
    private:
        SDL_Texture* mTexture;
        int mWidth,mHeight;
};
bool init();
bool loadMedia();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL; 	//New
LTexture gTextTexture;

LTexture::LTexture(){
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture(){  free();}
bool  LTexture::loadFromFile(string path){
    free();
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL){
        cout << "Could not load image!SDL_image Error" << path.c_str() << IMG_GetError() << endl;
    }else{
        SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0xFF,0xFF,0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
        if(newTexture == NULL) cout << "Could not create texture!SDL Error:"<<path.c_str() << SDL_GetError();
        else{
            mWidth = loadedSurface ->w;
            mHeight = loadedSurface ->h;
        }
       SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(string textureText,SDL_Color textColor){
    free();
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(),textColor);
    if(textSurface == NULL) cout << "Unable to render text surface!SDL_TTF Error: "<<TTF_GetError();
    else{
        mTexture = SDL_CreateTextureFromSurface(gRenderer,textSurface);
        if(mTexture == NULL) cout << "UN not create texture from rendered text!SDL_Error: "<< SDL_GetError();
        else{
            mWidth = textSurface ->w;
            mHeight = textSurface ->h;
        }
        SDL_FreeSurface( textSurface );
    }
    return mTexture != NULL;
}
void LTexture::free(){
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}
void LTexture::setColor(Uint8 red,Uint8 green,Uint8 blue){
    SDL_SetTextureColorMod(mTexture,red,green,blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending){
    SDL_SetTextureBlendMode(mTexture,blending);
}
void LTexture::setAlpha( Uint8 alpha )
{
	SDL_SetTextureAlphaMod( mTexture, alpha );
}
// void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
// {
// 	//Set rendering space and render to screen
// 	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

// 	//Set clip rendering dimensions
// 	if( clip != NULL )
// 	{
// 		renderQuad.w = clip->w;
// 		renderQuad.h = clip->h;
// 	}

// 	//Render to screen
// 	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
// }
void LTexture :: render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFLip flip){
    SDL_Rect renderQuad = {x,y, mWidth, mHeight};
    if(clip != NULL){
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;    
    }
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
bool init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) printf( "Warning: Linear texture filtering not enabled!" );
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 ){
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;
	gFont = TTF_OpenFont( "Sprites/arial.ttf", 28 );
	if( gFont == NULL ){
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}else{
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}
	return success;
}
void close()
{
	gTextTexture.free();
	TTF_CloseFont( gFont );
	gFont = NULL;
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	if( !init() )printf( "Failed to initialize!\n" );
	else{
		if( !loadMedia() )printf( "Failed to load media!\n" );
		else{	
			bool quit = false;
			SDL_Event e;
			while( !quit )
			{
				while( SDL_PollEvent( &e ) != 0 )
                    if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
					    quit = true;
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				//Render current frame
				gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
