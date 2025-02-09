#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
class LTexture{
	public:
		LTexture();
		~LTexture();
		bool loadFromFile( std::string path );
		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect* clip = NULL );
		int getWidth();
		int getHeight();
	private:
		SDL_Texture* mTexture;
		int mWidth;
		int mHeight;
};
bool init();
bool loadMedia();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
LTexture gSpriteSheetTexture;
LTexture gBackgroundTexture;
LTexture::LTexture(){
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
LTexture::~LTexture()	{ free(); }

bool LTexture::loadFromFile( std::string path ){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		SDL_FreeSurface( loadedSurface );
	mTexture = newTexture;
	return mTexture != NULL;
}
void LTexture::free(){
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )	{	SDL_SetTextureColorMod( mTexture, red, green, blue );}
void LTexture::setBlendMode( SDL_BlendMode blending )	{	SDL_SetTextureBlendMode( mTexture, blending );}
void LTexture::setAlpha( Uint8 alpha )	{	SDL_SetTextureAlphaMod( mTexture, alpha );}
void LTexture::render( int x, int y, SDL_Rect* clip ){
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}
int LTexture::getWidth(){return mWidth;}
int LTexture::getHeight(){return mHeight;}
bool init(){
	bool success = true;
		SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
	return success;
}
bool loadMedia(){
	bool success = true;
	gSpriteSheetTexture.loadFromFile( "image/foo.png" ) ;
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w =  64;
		gSpriteClips[ 0 ].h = 205;

		gSpriteClips[ 1 ].x =  64;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w =  64;
		gSpriteClips[ 1 ].h = 205;
		
		gSpriteClips[ 2 ].x = 128;
		gSpriteClips[ 2 ].y =   0;
		gSpriteClips[ 2 ].w =  64;
		gSpriteClips[ 2 ].h = 205;

		gSpriteClips[ 3 ].x = 192;
		gSpriteClips[ 3 ].y =   0;
		gSpriteClips[ 3 ].w =  64;
		gSpriteClips[ 3 ].h = 205;
	if( !gBackgroundTexture.loadFromFile( "image/background.png" ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}
	return success;
}
void close(){
	gSpriteSheetTexture.free();
	gBackgroundTexture.free();
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
}
int main( int argc, char* args[] ){
		init();
		loadMedia();
			bool quit = false;
			SDL_Event e;
			int frame = 0,picture = 0;
			const int num = 7;
			while( !quit ){
				while( SDL_PollEvent( &e ) != 0 ){
					if( e.type == SDL_QUIT )quit = true;
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear( gRenderer );
				gBackgroundTexture.render(0,0);
				SDL_Rect* currentClip = &gSpriteClips[frame / num];
				gSpriteSheetTexture.render(((SCREEN_WIDTH - currentClip->w) - picture),(SCREEN_HEIGHT - currentClip->h), currentClip);
				SDL_RenderPresent( gRenderer );
				// SDL_RenderClear( gRenderer );
				frame++;
				++picture;
				if(picture >= SCREEN_WIDTH ) picture = 0;
				if( frame /num>= WALKING_ANIMATION_FRAMES ) frame = 0;
				SDL_RenderClear( gRenderer );
			}
	close();
	return 0;
}
