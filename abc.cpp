#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PIPE_WIDTH = 100;
const int LOWER_PIPE_OFFSET = 100;
const int UPPER_PIPE_OFFSET = 50;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

class LTexture {
public:
    LTexture();
    ~LTexture();
    bool loadFromFile(string path);
    void free();
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);
    void render(int x, int y, SDL_Rect* clip = nullptr);
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};

LTexture::LTexture() {
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(string path) {
    free();
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << endl;
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
        } else {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != nullptr;
}

void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip) {
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

LTexture gPipeTexture;

class Pipe {
public:
    int x, height, width, whichPipe;

    Pipe(int startX, int pipeHeight);
    void update();
    void render();
    SDL_Rect strikeUpperObstacle();
    SDL_Rect strikeLowerObstacle();
};

Pipe::Pipe(int startX, int pipeHeight) {
    x = startX;
    height = pipeHeight;
    width = PIPE_WIDTH;
    whichPipe = rand() % 3;
}

void Pipe::update() {
    x -= 5;  // Đổi giá trị vận tốc cho phù hợp
    if (x + PIPE_WIDTH < 0) {
        x = SCREEN_WIDTH;
        height = rand() % (SCREEN_HEIGHT - LOWER_PIPE_OFFSET - UPPER_PIPE_OFFSET) + UPPER_PIPE_OFFSET;
        whichPipe = rand() % 3;
    }
}

void Pipe::render() {
    SDL_Rect pipeRect = {x, 0, width, height};

    if (whichPipe == 0) {
        gPipeTexture.loadFromFile("Sprites/pipeRed.png");
    } else if (whichPipe == 1) {
        gPipeTexture.loadFromFile("Sprites/pipeBlue.png");
    } else {
        gPipeTexture.loadFromFile("Sprites/pipeGreen.png");
    }

    gPipeTexture.render(pipeRect.x, pipeRect.y, nullptr);

    pipeRect.y = height + LOWER_PIPE_OFFSET;
    gPipeTexture.render(pipeRect.x, pipeRect.y, nullptr);
}

SDL_Rect Pipe::strikeUpperObstacle() {
    return {x, 0, width, height};
}

SDL_Rect Pipe::strikeLowerObstacle() {
    return {x, height + LOWER_PIPE_OFFSET, width, SCREEN_HEIGHT - (height + LOWER_PIPE_OFFSET)};
}

void renderAllPipes(list<Pipe>& pipes) {
    for (auto& pipe : pipes) {
        pipe.render();
    }
}

int main(int argc, char* args[]) {
    srand(static_cast<unsigned>(time(0)));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    gWindow = SDL_CreateWindow("Pipe Rendering Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    if (!gPipeTexture.loadFromFile("Sprites/pipeRed.png")) {
        cerr << "Failed to load pipe texture!" << endl;
        return -1;
    }

    list<Pipe> pipes;

    SDL_Event e;

    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        for (auto& pipe : pipes) {
            pipe.update();
        }

        SDL_RenderClear(gRenderer);
        renderAllPipes(pipes);
        SDL_RenderPresent(gRenderer);

        SDL_Delay(20);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
