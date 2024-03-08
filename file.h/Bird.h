// #ifndef BIRD_H_
// #define BIRD_H_
// #include "CommonFunc.h"

// class Bird {
// public:
//     int x, y, velocity, currentFrame;
//     Bird() {
//         x = SCREEN_WIDTH / 4;
//         y = SCREEN_HEIGHT / 2;
//         velocity = 0;
//         currentFrame = 0;
//     }
//     void jump() {
//         velocity = JUMP_VELOCITY;
//     }
//     void update() {
//         velocity += GRAVITY;
//         y += velocity;
//         currentFrame = (currentFrame + 1) % FLYING_ANIMATION_FRAMES_OF_BIRD;
//     }
//     void render() {
//         gTextures[currentFrame].render(x, y);
//     }
// };
// #endif