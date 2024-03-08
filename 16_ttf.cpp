#include "CommonFunc.h"
class LTexture{
    public:
    LTexture();
    ~LTexture();
    bool loadFromFile(string path);
    void free();
    void setColor(Uint8 red,Uint8 green, Uint8 blue);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(Uint8 alpha);
    int getWidth();
    int getHeigt();
}