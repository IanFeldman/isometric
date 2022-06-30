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

    mASprite = new AnimatedSprite(this, mGame->GetTexture("assets/test-anim.png"), 100);
    mASprite->SetFPS(2.0f);
    // base
    std::string name = "base";
    SDL_Rect frame1 = { 0, 0, 16, 16 };
    SDL_Rect frame2 = { 16, 0, 16, 16 };
	std::vector<SDL_Rect*> baseAnim = { &frame1, &frame2 };
    mASprite->AddAnimation(&name, &baseAnim);
    // set anim
    mASprite->SetAnimation(&name);
	mASprite->SetIsPaused(false);
    // set move state
    // mMoveState = MoveState::Idle;
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
    //mGame->SetCamera(mPosition);
    mGame->SetCamera(camPos);
}

void Player::GetInput() {
    Vector2 vel = Vector2(0.0f, 0.0f);

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    bool left = keyboardState[SDL_SCANCODE_A];
    bool right = keyboardState[SDL_SCANCODE_D];
    bool up = keyboardState[SDL_SCANCODE_W];
    bool down = keyboardState[SDL_SCANCODE_S];

    if (left) {
        vel.x -= mMoveSpeed;
    }
    if (right) {
        vel.x += mMoveSpeed;
    }
    if (up) {
        vel.y -= mMoveSpeed;
    }
    if (down) {
        vel.y += mMoveSpeed;
    }
    mVelocity = vel;

    /*
    switch(mMoveState) {
        case MoveState::Idle:
            // run right
            if (right && !left) {
                mVelocity.x = mMoveSpeed; 
                //mASprite->SetAnimation("run-right");
                mMoveState = MoveState::RunRight;
            }
            // run left
            else if (!right && left) {
                mVelocity.x = -mMoveSpeed;
                //mASprite->SetAnimation("run-left");
                mMoveState = MoveState::RunLeft;
            }
            // run up
            if (up && !down) {
                mVelocity.y = -mMoveSpeed;
                mMoveState = MoveState::RunUp;
            }
            // run down
            if (!up && down) {
                mVelocity.y = mMoveSpeed;
                mMoveState = MoveState::RunUp;
            }
            break;
        case MoveState::RunRight:
            // run left
            if (!right && left) {
                mVelocity.x = -mMoveSpeed;
                //mASprite->SetAnimation("run-left");
                mMoveState = MoveState::RunLeft;
            }
            // idle
            else if (!left && !right && !up && !down) {
                mVelocity = Vector2(0.0f, 0.0f);
                //mASprite->SetAnimation("idle");
                mMoveState = MoveState::Idle;
            }
            break;
        case MoveState::RunLeft:
            // run right
            if (right && !left) {
                mVelocity.x = mMoveSpeed;
                //mASprite->SetAnimation("run-right");
                mMoveState = MoveState::RunRight;
            }
            // idle
            else if ((!right && !left) || (left && right)) {
                mVelocity = Vector2(0.0f, 0.0f);
                //mASprite->SetAnimation("idle");
                mMoveState = MoveState::Idle;
            }
            break;
        case MoveState::RunUp:
            // run down 
            if (!up && down) {
                mVelocity.y = mMoveSpeed;
                //mASprite->SetAnimation("run-right");
                mMoveState = MoveState::RunDown;
            }
            // idle
            else if ((!right && !left) || (left && right)) {
                mVelocity = Vector2(0.0f, 0.0f);
                //mASprite->SetAnimation("idle");
                mMoveState = MoveState::Idle;
            }
            break;
        default:
            break;
    }
    */
}

