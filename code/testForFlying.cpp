
#include "file.h/CommonFunc.h"
//Texture wrapper class
#include "file.h/CommonTexture.h"
//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();
// const int FLYING_ANIMATION_FRAMES_OF_BIRD = 3;
LTexture gTextures[FLYING_ANIMATION_FRAMES_OF_BIRD],
		gBackground;
bool init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load sprite sheet texture
	// gTextures[0].loadFromFile("Sprites/bluebird2.png");
    // gTextures[1].loadFromFile("Sprites/bluebird1.png");
    // gTextures[2].loadFromFile("Sprites/bluebird3.png");
	gBackground.loadFromFile("Sprites/background.jpg");
	gTextures[0].loadFromFile("Sprites/piranha1.png");
    gTextures[1].loadFromFile("Sprites/piranha2.png");
    gTextures[2].loadFromFile("Sprites/piranha3.png");
	gTextures[3].loadFromFile("Sprites/piranha4.png");
	gTextures[4].loadFromFile("Sprites/piranha5.png");
    // for(int i = 0; i < FLYING_ANIMATION_FRAMES_OF_BIRD; i++) if( gTextures[i] == NULL) success = false;
	
	
	return success;
}

void close()
{
    for (int i = 0; i < 5; ++i) {
        gTextures[i].free();
    }

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Current animation frame
			int frame = 0;
            int currentTexture = 0;
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current frame
				gBackground.render(0,0);
				gTextures[currentTexture].render(SCREEN_WIDTH/2  - currentTexture*45, SCREEN_HEIGHT/3);
				

				//Update screen
				SDL_RenderPresent( gRenderer );
                if(currentTexture != 5){
					currentTexture = (currentTexture + 1) % 5;
				}
                SDL_Delay(100);
				//Go to next frame
				// ++frame;

				// //Cycle animation
				// if( frame / 20 >= FLYING_ANIMATION_FRAMES_OF_BIRD )
				// {
				// 	frame = 0;
				// }
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
