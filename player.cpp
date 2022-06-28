#include "animatedsprite.h"
#include "collisioncomponent.h"
#include "game.h"
#include "player.h"
#include "math.h"

Player::Player(Game* game)
	:Actor(game)
    ,mMoveSpeed(300.0f)
{
    // create collision component
    mCC = new CollisionComponent(this, 32, 32);

    mSpriteComponent = new SpriteComponent(this, 100);
    mSpriteComponent->SetTexture(mGame->GetTexture("assets/blank.png"));
    /*
    mASprite = new AnimatedSprite(this, 100);
    mASprite->SetFPS(10.0f);
    // idle
	std::vector<SDL_Texture*> idleAnim{
		mGame->GetTexture("assets/player/idle/idle1.png"),
		mGame->GetTexture("assets/player/idle/idle1.png"),
		mGame->GetTexture("assets/player/idle/idle2.png"),
		mGame->GetTexture("assets/player/idle/idle2.png"),
		mGame->GetTexture("assets/player/idle/idle3.png"),
		mGame->GetTexture("assets/player/idle/idle3.png"),
		mGame->GetTexture("assets/player/idle/idle4.png"),
		mGame->GetTexture("assets/player/idle/idle4.png")};
    mASprite->AddAnimation("idle", idleAnim);
    // run right
	std::vector<SDL_Texture*> runRAnim{
		mGame->GetTexture("assets/player/run-r/run-r1.png"),
		mGame->GetTexture("assets/player/run-r/run-r2.png"),
		mGame->GetTexture("assets/player/run-r/run-r3.png"),
		mGame->GetTexture("assets/player/run-r/run-r4.png"),
		mGame->GetTexture("assets/player/run-r/run-r5.png"),
		mGame->GetTexture("assets/player/run-r/run-r6.png")};
    mASprite->AddAnimation("run-right", runRAnim);
    // run left 
	std::vector<SDL_Texture*> runLAnim{
		mGame->GetTexture("assets/player/run-l/run-l1.png"),
		mGame->GetTexture("assets/player/run-l/run-l2.png"),
		mGame->GetTexture("assets/player/run-l/run-l3.png"),
		mGame->GetTexture("assets/player/run-l/run-l4.png"),
		mGame->GetTexture("assets/player/run-l/run-l5.png"),
		mGame->GetTexture("assets/player/run-l/run-l6.png")};
    mASprite->AddAnimation("run-left", runLAnim);
    // set anim
    mASprite->SetAnimation("idle");
	mASprite->SetIsPaused(false);
    */
    // set move state
    mMoveState = MoveState::Idle;
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

