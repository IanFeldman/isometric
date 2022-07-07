#pragma once
#include "component.h"
#include "math.h"
#include <SDL2/SDL.h>

class CollisionComponent : public Component
{
public:
    CollisionComponent(class Actor* owner, SDL_Rect bounds);

    Vector2 GetMinOffset();
    Vector2 Collide(class CollisionComponent* otherCC);

    SDL_Rect GetBounds() { return mBounds; }

    void Debug();

protected:
    SDL_Rect mBounds;
};

