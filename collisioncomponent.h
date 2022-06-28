#pragma once
#include "component.h"
#include "math.h"

class CollisionComponent : public Component
{
public:
    CollisionComponent(class Actor* owner, int width, int height);

    Vector2 GetMinOffset();
    Vector2 Collide(class CollisionComponent* otherCC);
    int GetWidth() { return mColliderWidth; }
    int GetHeight() { return mColliderHeight; }

    void Debug();

protected:
    int mColliderWidth;
    int mColliderHeight;
};

