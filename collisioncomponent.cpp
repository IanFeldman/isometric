#include "actor.h"
#include "collisioncomponent.h"
#include "game.h"
#include "math.h"
// CC Debug
#include <SDL2/SDL.h>
#include <iostream>

CollisionComponent::CollisionComponent(Actor* owner, SDL_Rect bounds)
    :Component(owner)
    ,mBounds(bounds)
{
}

Vector2 CollisionComponent::GetMinOffset() {
    Vector2 minOffset = Vector2(0.0f, 0.0f);

    // iterate over every actor
    for (Actor* actor : mOwner->GetGame()->GetActors()) {
        // skip itself
        if (actor == mOwner) {
            continue;
        }
        // check if it has a collision component
        CollisionComponent* otherCC = actor->GetComponent<CollisionComponent>();
        if (otherCC != nullptr) {
            // check collision
            minOffset += Collide(otherCC);
        }
    }

    return minOffset;
}

Vector2 CollisionComponent::Collide(CollisionComponent* otherCC) {
    // check collision
    // this
    int thisWidth = mBounds.w * mOwner->GetScale();
    int thisHeight = mBounds.h * mOwner->GetScale();
    // top left and right
    int thisX = mBounds.x * mOwner->GetScale() + mOwner->GetPosition().x;
    int thisY = mBounds.y * mOwner->GetScale() + mOwner->GetPosition().y;

    // other
    SDL_Rect otherBounds = otherCC->GetBounds();
    float otherScale = otherCC->GetOwner()->GetScale();
    int otherWidth = otherBounds.w * otherScale;
    int otherHeight = otherBounds.h * otherScale;
    // top left and right
    int otherX = otherBounds.x * otherScale + otherCC->GetOwner()->GetPosition().x; 
    int otherY = otherBounds.y * otherScale + otherCC->GetOwner()->GetPosition().y; 
    
    // actual distance
    int deltaX = thisX - otherX;
    // min distance it should be
    int expectedDistX;
    if (deltaX >= 0) {
        expectedDistX = otherWidth;
    }
    else {
        expectedDistX = thisWidth;
    }

    // same for y
    int deltaY = thisY - otherY;
    int expectedDistY;
    if (deltaY >= 0) {
        expectedDistY = otherHeight;
    }
    else {
        expectedDistY = thisHeight;
    }

    bool collision = std::abs(deltaX) <= expectedDistX && std::abs(deltaY) <= expectedDistY;

    if (collision) {
        int moveX = expectedDistX - std::abs(deltaX);
        int moveY = expectedDistY - std::abs(deltaY);
        // x move is smaller. default to x when equal
        if (moveX <= moveY) {
            if (deltaX < 0) {
                return Vector2(-moveX, 0.0f);
            }
            return Vector2(moveX, 0.0f);
        }
        // y move is smaller 
        if (deltaY < 0) {
            return Vector2(0.0f, -moveY);
        }
        return Vector2(0.0f, moveY);
    }
    return Vector2(0.0f, 0.0f);
}

void CollisionComponent::Debug() {
    // debug draw our own collider
    float scale = mOwner->GetScale();
    SDL_Rect r;
    r.w = mBounds.w * scale;
    r.h = mBounds.h * scale;
    r.x = mBounds.x * scale + mOwner->GetPosition().x;
    r.y = mBounds.y * scale + mOwner->GetPosition().y;
    mOwner->GetGame()->GetRenderer()->DrawRectangle(r);
}

