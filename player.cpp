#include "animatedsprite.h"
#include "collisioncomponent.h"
#include "game.h"
#include "math.h"
#include "player.h"
#include <string>

Player::Player(Game* game)
	:Actor(game)
    ,mMoveSpeed(300.0f)
{
    // create collision component
    mCC = new CollisionComponent(this, 32, 32);

    mASprite = new AnimatedSprite(this, mGame->GetTexture("assets/tileset.png"), 100);
    mASprite->SetFPS(10.0f);
    // ANIMATIONS
    // idle
    std::string name = "idle";
    SDL_Rect frame1 = { 0, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> idleAnim = { &frame1 };
    mASprite->AddAnimation(&name, &idleAnim);
    // walk
    name = "walk";
    frame1 = { 1 * 16, 4 * 16, 16, 16 };
    SDL_Rect frame2 = { 2 * 16, 4 * 16, 16, 16 };
	std::vector<SDL_Rect*> walkAnim = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &walkAnim);
    // set anim
    mASprite->SetAnimation(&name);
	mASprite->SetIsPaused(false);
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

    std::string name = "idle";
    if (left) {
        vel.x -= mMoveSpeed;
        name = "walk";
    }
    if (right) {
        vel.x += mMoveSpeed;
        name = "walk";
    }
    if (up) {
        vel.y -= mMoveSpeed;
        name = "walk";
    }
    if (down) {
        vel.y += mMoveSpeed;
        name = "walk";
    }
    mASprite->SetAnimation(&name);
    mVelocity = vel;
}

