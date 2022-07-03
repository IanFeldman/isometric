#include "animatedsprite.h"
#include "collisioncomponent.h"
#include "game.h"
#include "math.h"
#include "player.h"
#include "world.h"
#include <string>

Player::Player(Game* game)
	:Actor(game)
    ,mMoveSpeed(300.0f)
{
    // create collision component
    mCC = new CollisionComponent(this, 12, 16);

    mASprite = new AnimatedSprite(this, mGame->GetTexture("assets/tileset.png"), 100);
    mASprite->SetFPS(10.0f);
    LoadAnimations();
    mDir = Dir::Down;
}

void Player::OnUpdate(float deltaTime)
{
    // updates velocity
    GetInput(); 
    
    // update position
    mPosition += mVelocity * deltaTime;
    // offset for collisions
    Vector2 offset = mCC->GetMinOffset();
    mPosition += offset; 

    // update camera
    // lineary interpolate camera position
    float fac = 0.15f; // from  0 to 1
    Vector2 camPos = (1.0f - fac) * mGame->GetCamera() + fac * mPosition;
    // snap cam to player when close enough
    if (std::abs(camPos.x - mPosition.x) + std::abs(camPos.y - mPosition.y) < 1.0f) {
        camPos = mPosition;
    }
    mGame->SetCamera(camPos);
}

void Player::GetInput() {
    Vector2 vel = Vector2(0.0f, 0.0f);

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    bool left = keyboardState[SDL_SCANCODE_A];
    bool right = keyboardState[SDL_SCANCODE_D];
    bool up = keyboardState[SDL_SCANCODE_W];
    bool down = keyboardState[SDL_SCANCODE_S];

    bool inWater = mGame->GetWorld()->IsWater((int)mPosition.x, (int)mPosition.y);

    std::string name;
    if (left) {
        mDir = Dir::Left;
        vel.x -= mMoveSpeed;
        if (inWater) {
            name = "swimL";
        }
        else {
            name = "moveL";
        }
    }
    if (right) {
        mDir = Dir::Right;
        vel.x += mMoveSpeed;
        if (inWater) {
            name = "swimR";
        }
        else {
            name = "moveR";
        }
    }
    if (up) {
        mDir = Dir::Up;
        vel.y -= mMoveSpeed;
        if (inWater) {
            name = "swimU";
        }
        else {
            name = "moveU";
        }
    }
    if (down) {
        mDir = Dir::Down;
        vel.y += mMoveSpeed;
        if (inWater) {
            name = "swimD";
        }
        else {
            name = "moveD";
        }
    }
    // not moving
    if (!left && !right && !up && !down) {
        if (mDir == Dir::Left) {
            if (inWater) {
                name = "swimL";
            }
            else {
                name = "idleL";
            }
        }
        else if (mDir == Dir::Right) {
            if (inWater) {
                name = "swimR";
            }
            else {
                name = "idleR";
            }
        }
        else if (mDir == Dir::Up) {
            if (inWater) {
                name = "swimU";
            }
            else {
                name = "idleU";
            }
        }
        else {
            if (inWater) {
                name = "swimD";
            }
            else {
                name = "idleD";
            }
        }
    }
    mASprite->SetAnimation(&name);
    mVelocity = vel;
}

void Player::LoadAnimations() {
    SDL_Rect frame1, frame2, frame3, frame4;

    // IDLES
    // idle left
    std::string name = "idleL";
    frame1 = { 0 * 16, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> idleL = { &frame1 };
    mASprite->AddAnimation(&name, &idleL);
    // idle right 
    name = "idleR";
    frame1 = { 4 * 16, 4 * 16, 16, 16 };
    std::vector<SDL_Rect*> idleR= { &frame1 };
    mASprite->AddAnimation(&name, &idleR);
    // idle up 
    name = "idleU";
    frame1 = { 0 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> idleU = { &frame1 };
    mASprite->AddAnimation(&name, &idleU);
    // idle down
    name = "idleD";
    frame1 = { 4 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> idleD = { &frame1 };
    mASprite->AddAnimation(&name, &idleD);

    // MOVES
    // move left
    name = "moveL";
    frame1 = { 1 * 16, 4 * 16, 16, 16 };
    frame2 = { 2 * 16, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> moveL = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &moveL);
    // move right
    name = "moveR";
    frame1 = { 5 * 16, 4 * 16, 16, 16 };
    frame2 = { 6 * 16, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> moveR = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &moveR);
    // move up
    name = "moveU";
    frame1 = { 1 * 16, 5 * 16, 16, 16 };
    frame2 = { 2 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> moveU = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &moveU);
    // move down
    name = "moveD";
    frame1 = { 5 * 16, 5 * 16, 16, 16 };
    frame2 = { 6 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> moveD = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &moveD);

    // SWIMS
    // swim left
    name = "swimL";
    frame1 = { 3 * 16, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> swimL = { &frame1 };
    mASprite->AddAnimation(&name, &swimL);
    // swim right 
    name = "swimR";
    frame1 = { 7 * 16, 4 * 16, 16, 16 };
    std::vector<SDL_Rect*> swimR= { &frame1 };
    mASprite->AddAnimation(&name, &swimR);
    // swim up 
    name = "swimU";
    frame1 = { 3 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> swimU = { &frame1 };
    mASprite->AddAnimation(&name, &swimU);
    // swim down
    name = "swimD";
    frame1 = { 7 * 16, 5 * 16, 16, 16 };
    std::vector<SDL_Rect*> swimD = { &frame1 };
    mASprite->AddAnimation(&name, &swimD);
    // set anim
    name = "idleL";
    mASprite->SetAnimation(&name);
	mASprite->SetIsPaused(false);
}
