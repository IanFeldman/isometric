#pragma once
#include "actor.h"
#include "math.h"

enum class Dir {
    Left,
    Right,
    Up,
    Down
};

class Player : public Actor
{

public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
    void GetInput();
    void LoadAnimations();

private:
    class SpriteComponent* mSpriteComponent;
    class AnimatedSprite* mASprite;
    class CollisionComponent* mCC;
    Dir mDir;
    Vector2 mVelocity;
    float mMoveSpeed;
};

