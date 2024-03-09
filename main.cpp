#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int JOYSTICK_DEAD_ZONE = 8000;
class LTexture{
	public:
		LTexture();
		~LTexture();
		bool loadFromFile( std::string path );	
		#if defined(SDL_TTF_MAJOR_VERSION)
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif	
		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );	
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
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
//Scene textures
LTexture gPressTexture,
		gUpTexture,
		gDownTexture,
		gLeftTexture,
		gRightTexture;
LTexture gPiranha1,
		gPiranha2,
		gPiranha3;
SDL_Joystick* gGameController = NULL;
LTexture gArrowTexture;
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()	{	free();	}

bool LTexture::loadFromFile( std::string path ){
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL ) printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	else{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )	printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		else{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif
void LTexture::free(){
	if( mTexture != NULL ){
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ){
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha ){
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

bool init(){
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		if( SDL_NumJoysticks() < 1 ) printf("Warning: No joysticks connected!\n");
		else{
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL ) printf("arning: Unable to open game controller! SDL Error:\n",SDL_GetError());
		}
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags )){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}
bool loadMedia(){
	bool success = true;
	gArrowTexture.loadFromFile("Sprites/arrow.png");
	gPressTexture.loadFromFile( "Sprites/press.png" );
	gUpTexture.loadFromFile( "Sprites/up.png" ) ;
	gDownTexture.loadFromFile( "Sprites/down.png" );
	gLeftTexture.loadFromFile( "Sprites/left.png" ) ;
	gRightTexture.loadFromFile( "Sprites/right.png" ) ;
	gPiranha1.loadFromFile( "Sprites/Piranha1.png" );
	gPiranha2.loadFromFile( "Sprites/Piranha2.png" );
	gPiranha3.loadFromFile("Sprites/Piranha3.png");
	return success;
}
void close(){
	//Free loaded images
	gArrowTexture.free();
	gPressTexture.free();
	gUpTexture.free();
	gDownTexture.free();
	gLeftTexture.free();
	gRightTexture.free();
	gPiranha1.free();
	gPiranha2.free();
	gPiranha3.free();
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();

	SDL_Quit();
}sdad


int main( int argc, char* args[] )
{
	freopen("output.txt","w",stdout);
	init();
	loadMedia();
	bool quit = false;
	int xDir = 0;
	int yDir = 0;
	SDL_Event e;
	LTexture* currentTexture = NULL;
	while( !quit ){
		while( SDL_PollEvent( &e ) != 0 ){
			if( e.type == SDL_QUIT )quit = true;	
			else if(e.type = SDL_JOYAXISMOTION){
				if(e.jaxis.which == 0){
					if(e.jaxis.axis == 0){
						if(e.jaxis.value < - JOYSTICK_DEAD_ZONE) xDir = -1; // left of dead zone
						else if(e.jaxis.value > JOYSTICK_DEAD_ZONE) xDir = 1; // right of dead zone
						else xDir = 0;
					}
				else if(e.jaxis.which == 1){
					if(e.jaxis.value < - JOYSTICK_DEAD_ZONE) yDir = -1; // top of dead zone
					else if(e.jaxis.value > JOYSTICK_DEAD_ZONE) yDir = 1; // bottom of dead zone
					else yDir = 0;
				}
				}
			}
		}
		// SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF ); //set alpha white
		// SDL_RenderClear( gRenderer );	
		const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		if( currentKeyStates[ SDL_SCANCODE_UP ])currentTexture = &gUpTexture;		
		else if( currentKeyStates[ SDL_SCANCODE_DOWN ] ) currentTexture = &gDownTexture;
		else if( currentKeyStates[ SDL_SCANCODE_LEFT ] ) currentTexture = &gLeftTexture;
		else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] ) currentTexture = &gRightTexture;
		else if(currentKeyStates[SDL_SCANCODE_W] ){
			currentTexture = &gPiranha1;
			currentTexture->render(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
		}
		else if(currentKeyStates[SDL_SCANCODE_A] ) {
			currentTexture = &gPiranha2;
			currentTexture->render(SCREEN_WIDTH/3,SCREEN_HEIGHT/2);
		}
		else if(currentKeyStates[SDL_SCANCODE_S] ) currentTexture = &gPiranha3;
		else currentTexture = &gPressTexture;
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF ); //set alpha white
		SDL_RenderClear( gRenderer );	
		double joystickAngle = atan2( (double)yDir, (double)xDir ) * ( 180.0 / M_PI );
		if( xDir == 0 && yDir == 0 ) joystickAngle = 0; // check correct angle
		gArrowTexture.render( ( SCREEN_WIDTH - gArrowTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gArrowTexture.getHeight() ) / 2, NULL, joystickAngle );				
		//clear the previous render.
		currentTexture->render( 0, 0 ); // render texture.
		SDL_RenderPresent( gRenderer );
	}
	close();

	return 0;
}
