
#include "file.h/CommonFunc.h"
#include "file.h/CommonTexture.h"

// const int SCREEN_WIDTH = 900;
// const int SCREEN_HEIGHT = 634;
// const int BIRD_WIDTH = 50;
// const int BIRD_HEIGHT = 50;
// const int PIPE_WIDTH = 100;
// const int PIPE_HEIGHT = 300;
// const int GRAVITY = 1;
// const int JUMP_VELOCITY = -15;
// const int PIPE_VELOCITY = 40;

// SDL_Window* gWindow = nullptr;
// // SDL_Renderer* gRenderer = nullptr;
// SDL_Texture* birdTexture = nullptr;
// SDL_Texture* pipeTexture = nullptr;
// SDL_Renderer* gRenderer = NULL;

// class LTexture
// {
// 	public:
// 		//Initializes variables
// 		LTexture();

// 		//Deallocates memory
// 		~LTexture();

// 		//Loads image at specified path
// 		bool loadFromFile( std::string path );

// 		//Deallocates texture
// 		void free();

// 		//Renders texture at given point
// 		void render( int x, int y );

// 		//Gets image dimensions
// 		int getWidth();
// 		int getHeight();

// 	private:
// 		//The actual hardware texture
// 		SDL_Texture* mTexture;

// 		//Image dimensions
// 		int mWidth;
// 		int mHeight;
// };
// LTexture::LTexture()
// {
// 	//Initialize
// 	mTexture = NULL;
// 	mWidth = 0;
// 	mHeight = 0;
// }

// LTexture::~LTexture()
// {
// 	//Deallocate
// 	free();
// }

// bool LTexture::loadFromFile( std::string path )
// {
// 	//Get rid of preexisting texture
// 	free();

// 	//The final texture
// 	SDL_Texture* newTexture = NULL;

// 	//Load image at specified path
// 	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
// 	if( loadedSurface == NULL )
// 	{
// 		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
// 	}
// 	else
// 	{
// 		//Color key image
// 		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

// 		//Create texture from surface pixels
//         newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
// 		if( newTexture == NULL )
// 		{
// 			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
// 		}
// 		else
// 		{
// 			//Get image dimensions
// 			mWidth = loadedSurface->w;
// 			mHeight = loadedSurface->h;
// 		}

// 		//Get rid of old loaded surface
// 		SDL_FreeSurface( loadedSurface );
// 	}

// 	//Return success
// 	mTexture = newTexture;
// 	return mTexture != NULL;
// }

// void LTexture::free()
// {
// 	//Free texture if it exists
// 	if( mTexture != NULL )
// 	{
// 		SDL_DestroyTexture( mTexture );
// 		mTexture = NULL;
// 		mWidth = 0;
// 		mHeight = 0;
// 	}
// }

// void LTexture::render( int x, int y )
// {
// 	//Set rendering space and render to screen
// 	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
// 	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
// }

// int LTexture::getWidth()
// {
// 	return mWidth;
// }

// int LTexture::getHeight()
// {
// 	return mHeight;
// }
LTexture gBackgroundTexture;
LTexture gStretchedSurface;
LTexture gTextures[FLYING_ANIMATION_FRAMES_OF_BIRD];
bool loadMedia()
{
	//Loading success flag
	bool success = true;    
	//Load background texture
	if( !gBackgroundTexture.loadFromFile( "sprites/backgroundForFlappyBird.png" ) )
	{
		printf( "Failed to load background texture image!\n" );
		success = false;
	}
    if(!gStretchedSurface.loadFromFile( "sprites/basePlus.png" )){
		printf( "Failed to load base image!\n" );
		success = false;
	}
    for(int i = 0; i <FLYING_ANIMATION_FRAMES_OF_BIRD;i++){
        string imagePath = "Sprites/bluebird" + to_string(i + 1) + ".png";
        if (!gTextures[i].loadFromFile(imagePath))  success = false;
    }
	return success;
}

class Bird {
public:
    int x, y, velocity;

    Bird() {
        x = SCREEN_WIDTH / 4;
        y = SCREEN_HEIGHT / 2;
        velocity = 0;
    }

    void jump() {
        velocity = JUMP_VELOCITY;
    }

    void update() {
        velocity += GRAVITY;
        y += velocity;
    }

    void render() {
        SDL_Rect birdRect = { x, y, BIRD_WIDTH, BIRD_HEIGHT };
        SDL_RenderCopy(gRenderer, birdTexture, nullptr, &birdRect);
    }
};

class Pipe {
public:
    int x, height;

    Pipe(int startX, int pipeHeight) {
        x = startX;
        height = pipeHeight;
    }

    void update() { 
        x -= PIPE_VELOCITY;
        if (x + PIPE_WIDTH < 0) {
            x = SCREEN_WIDTH;
            height = rand() % (SCREEN_HEIGHT - 200) ;
        }
    }

    void render() {
        SDL_Rect upperPipeRect = { x, -25, PIPE_WIDTH, height };
        SDL_Rect lowerPipeRect = { x, height + 150, PIPE_WIDTH, SCREEN_HEIGHT - height - 150 -134};
        SDL_RenderCopy(gRenderer, pipeTexture, nullptr, &upperPipeRect);
        SDL_RenderCopy(gRenderer, pipeTexture, nullptr, &lowerPipeRect);
    }
};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    // Load bird texture
    SDL_Surface* birdSurface = IMG_Load("sprites/redbird-midflap.png");
    birdTexture = SDL_CreateTextureFromSurface(gRenderer, birdSurface);
    SDL_FreeSurface(birdSurface);

    // Load pipe texture
    SDL_Surface* pipeSurface = IMG_Load("sprites/pipe-red.png");
    pipeTexture = SDL_CreateTextureFromSurface(gRenderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);

    return true;
}

void close() {
    for (int i = 0; i < 4; ++i) gTextures[i].free();
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyRenderer(gRenderer);
    gBackgroundTexture.free();
    gStretchedSurface.free();
    gRenderer = NULL;
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}


int main(int argc, char* args[]) {
    if (!init()) {
        std::cout << "Failed to initialize." << std::endl;
        return -1;
    }
    loadMedia();
    bool quit = false;
    SDL_Event e;

    Bird bird;
    Pipe pipe(SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - 200) + 100);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)  {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    bird.jump();
                }
            }
        }

        bird.update();
        pipe.update();

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);

        gBackgroundTexture.render( 0, 0 );
        gStretchedSurface.render(0,504);

        bird.render();
        pipe.render();
        SDL_RenderPresent(gRenderer);
        SDL_RenderPresent(gRenderer);
        SDL_Delay(16);
    }

    close();

    return 0;
}
