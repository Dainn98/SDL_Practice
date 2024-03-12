#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BIRD_WIDTH = 50;
const int BIRD_HEIGHT = 50;
const int PIPE_WIDTH = 100;
const int PIPE_HEIGHT = 300;
const int GRAVITY = 1;
const int JUMP_VELOCITY = -15;

SDL_Window* window = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* pipeTexture = nullptr;

class Bird {
public:
    int x, y, velocity;

    Bird() {
        x = SCREEN_WIDTH / 2;
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
        x -= 5;
        if (x + PIPE_WIDTH < 0) {
            x = SCREEN_WIDTH;
            height = rand() % (SCREEN_HEIGHT - 200) + 100;
        }
    }

    void render() {
        SDL_Rect upperPipeRect = { x, 0, PIPE_WIDTH, height };
        SDL_Rect lowerPipeRect = { x, height + 150, PIPE_WIDTH, SCREEN_HEIGHT - height - 150 };
        SDL_RenderCopy(gRenderer, pipeTexture, nullptr, &upperPipeRect);
        SDL_RenderCopy(gRenderer, pipeTexture, nullptr, &lowerPipeRect);
    }
};

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    // Load bird texture
    SDL_Surface* birdSurface = IMG_Load("image/bird.jpg");
    birdTexture = SDL_CreateTextureFromSurface(gRenderer, birdSurface);
    SDL_FreeSurface(birdSurface);

    // Load pipe texture
    SDL_Surface* pipeSurface = IMG_Load("image/pipe.png");
    pipeTexture = SDL_CreateTextureFromSurface(gRenderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);

    return true;
}

void close() {
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::cout << "Failed to initialize." << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    Bird bird;
    Pipe pipe(SCREEN_WIDTH, rand() % (SCREEN_HEIGHT - 200) + 100);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
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

        bird.render();
        pipe.render();

        SDL_RenderPresent(gRenderer);
        SDL_Delay(15);
    }

    close();

    return 0;
}
