#include "actor.h"
#include "component.h"
#include "game.h"
#include "math.h"
#include <algorithm>

Actor::Actor(Game* game)
    :mGame(game)
    ,mState(ActorState::Active)
    ,mPosition(Vector2(0.0f, 0.0f))
    ,mScale(1.0f)
    ,mRotation(0.0f)
{
    mGame->AddActor(this);
}

// remove actors from game list, and destroy components
Actor::~Actor()
{
    mGame->RemoveActor(this);

    if (!mComponents.empty()) {
        for (Component* c : mComponents)
            delete c;
    }

    mComponents.clear();
}

// run through components and update them all
// actor specific update
void Actor::Update(float deltaTime)
{
    if (GetState() == ActorState::Active) {
        for (Component* c : mComponents)
            c->Update(deltaTime);

        OnUpdate(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime)
{
}

// add component and sort it based on update order
void Actor::AddComponent(Component* c)
{
    mComponents.push_back(c);
    std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) -> bool {
        return a->GetUpdateOrder() < b->GetUpdateOrder();
    });
}
