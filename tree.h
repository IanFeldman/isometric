#pragma once
#include "actor.h"
#include "math.h"

class Tree: public Actor
{

public:
	Tree(class Game* game);
	void OnUpdate(float deltaTime) override;

private:
    class SpriteComponent* mSpriteComponent;
    class CollisionComponent* mCC;
};

