#pragma once
#include "actor.h"
#include "math.h"

enum class MoveState {
    Idle,
    RunRight,
    RunLeft,
    RunUp,
    RunDown,
};

class Player : public Actor
{

public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
    void GetInput();

private:
    class SpriteComponent* mSpriteComponent;
    class AnimatedSprite* mASprite;
    class CollisionComponent* mCC;
    MoveState mMoveState;
    Vector2 mVelocity;
    float mMoveSpeed;
};

