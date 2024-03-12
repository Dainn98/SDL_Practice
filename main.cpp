#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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
class LTimer
{
    public:
        LTimer();
        void start();
        void stop();
        void pause();
        void unpause();
        Uint32 getTicks();
        bool isStarted();
        bool isPaused();
    private:
        Uint32 mStartTicks;
        Uint32 mPausedTicks;
        bool mPaused;
        bool mStarted;
};

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}
void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}
void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}
void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}
void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}
Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}
bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}
bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}
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
LTexture gPausePromptTexture,
gTimeTextTexture,
gStartPromptTexture;
TTF_Font* gFont = NULL;
SDL_Joystick* gGameController = NULL;
LTexture gArrowTexture,
        gTimeTextTure,
        gPromptTextTexture;
LTexture gPromptTexture; //screen texture
Mix_Music *gMusic = NULL; //loadMUV
Mix_Chunk *gScratch = NULL; //LoadWAV
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

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

int main( int argc, char* args[] )
{
	freopen("output.txt","w",stdout);
	init();
	loadMedia();
	bool quit = false;
	SDL_Event e;
	LTexture* currentTexture = NULL;
    LTimer timer;
    SDL_Color textColor = { 0, 0, 0, 255 };
    //Current time start time
    Uint32 startTime = 0;
    std::vector<Pipe> pipes;
    std :: stringstream timeText;
	while( !quit ){
		while( SDL_PollEvent( &e ) != 0 ){
			if( e.type == SDL_QUIT )quit = true;	
            //Handle key press
            else if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    Mix_PlayChannel(-1, gHigh,0);
                    break;
                case SDLK_UP:
                    Mix_PlayChannel(-1,gMedium,0);
                    break;
                case SDLK_DOWN:
                    Mix_PlayChannel(-1,gScratch,0);
                    break;
                case SDLK_4:
                    Mix_PlayChannel(-1,gLow,0);
                case SDLK_9:
                        if(Mix_PausedMusic() == 0) Mix_PlayMusic(gMusic,-1);
                        else{
                            if(Mix_PausedMusic() == 1) Mix_ResumeMusic();
                            else Mix_PauseMusic();
                        } 
                    break;
                case SDLK_0:
                    Mix_HaltMusic();
                    break;
                default:
                    break;
                }
                if(e.key.keysym.sym == SDLK_RETURN) startTime = SDL_GetTicks();

                if( e.key.keysym.sym == SDLK_s ){
                        if( timer.isStarted() )  timer.stop();
                        else    timer.start();
                    }
                            //Pause/unpause
                    else if( e.key.keysym.sym == SDLK_p )
                            {
                                if( timer.isPaused() )  timer.unpause();
                                else timer.pause();
                                
                            }
                }
		}
         //Set text to be rendered
                timeText.str( "" );
                timeText << "Seconds since start time " << ( timer.getTicks() / 1000.f ) ; 

                //Render text
                if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
                {
                    printf( "Unable to render time texture!\n" );
                } 
        // gTimeTextTure.loadFromRenderedText(timeText.str().c_str(),textColor);
        // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF ); //set alpha white
		// SDL_RenderClear( gRenderer );
        // //
		// const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
		// if( currentKeyStates[ SDL_SCANCODE_UP ])currentTexture = &gUpTexture;		
		// else if( currentKeyStates[ SDL_SCANCODE_DOWN ] ) currentTexture = &gDownTexture;
		// else if( currentKeyStates[ SDL_SCANCODE_LEFT ] ) currentTexture = &gLeftTexture;
		// else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] ) currentTexture = &gRightTexture;
		// else if(currentKeyStates[SDL_SCANCODE_W] ){
		// 	currentTexture = &gPiranha1;
		// 	currentTexture->render(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
		// }
		// else if(currentKeyStates[SDL_SCANCODE_A] ) {
		// 	currentTexture = &gPiranha2;
		// 	currentTexture->render(SCREEN_WIDTH/3,SCREEN_HEIGHT/2);
		// }
		// else if(currentKeyStates[SDL_SCANCODE_S] ) currentTexture = &gPiranha3;
		// else currentTexture = &gPressTexture;
		// gPromptTextTexture.render(0,0);
        // gTimeTextTure.render(0,(SCREEN_HEIGHT-gPromptTextTexture.getHeight())/2);
        // gPromptTexture.render(0,0)	;
		// //clear the previous render.
		// currentTexture->render( 0, 0 ); // render texture.
		// SDL_RenderPresent( gRenderer );
        //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );

                //Render textures
                gStartPromptTexture.render( ( SCREEN_WIDTH - gStartPromptTexture.getWidth() ) / 2, 0 );
                gPausePromptTexture.render( ( SCREEN_WIDTH - gPausePromptTexture.getWidth() ) / 2, gStartPromptTexture.getHeight() );
                gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTimeTextTexture.getHeight() ) / 2 );

                //Update screen
                SDL_RenderPresent( gRenderer );
	}
	close();

	return 0;
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
                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}
	return success;
}
bool loadMedia(){
	bool success = true;
    gFont = TTF_OpenFont( "Sprites/arial.ttf", 28 );
	gArrowTexture.loadFromFile("Sprites/arrow.png");
	// gPressTexture.loadFromFile( "Sprites/press.png" );
	gUpTexture.loadFromFile( "Sprites/up.png" ) ;
	gDownTexture.loadFromFile( "Sprites/down.png" );
	gLeftTexture.loadFromFile( "Sprites/left.png" ) ;
	gRightTexture.loadFromFile( "Sprites/right.png" ) ;
	gPiranha1.loadFromFile( "Sprites/Piranha1.png" );
	gPiranha2.loadFromFile( "Sprites/Piranha2.png" );
	gPiranha3.loadFromFile("Sprites/Piranha3.png");
    // gPromptTexture.loadFromFile( "Sprites/prompt.png");
    SDL_Color textColor = {0x00,0x00,0x00,0xFF};
    gPromptTexture.loadFromRenderedText("Press Enter to Reset Start Time",textColor);
    gMusic = Mix_LoadMUS("Sprites/beat.wav");
    gScratch = Mix_LoadWAV("Sprites/scracth.wav");
    gHigh = Mix_LoadWAV("Sprites/high.wav");
    gMedium = Mix_LoadWAV( "Sprites/medium.wav" );
    gLow = Mix_LoadWAV( "Sprites/low.wav" );

	return success;
}
void close(){
    //Free loaded images
	gPromptTexture.free();
    gTimeTextTure.free();
    //Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;
	//Free the sound effects
	Mix_FreeChunk( gScratch );
	Mix_FreeChunk( gHigh );
	Mix_FreeChunk( gMedium );
	Mix_FreeChunk( gLow );
    gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;
	//Free the music
	Mix_FreeMusic( gMusic );
	gMusic = NULL;

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
    TTF_Quit();
    Mix_Quit();
	SDL_Quit();
}

