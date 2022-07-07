#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include <SDL2/SDL.h>

SpriteComponent::SpriteComponent(Actor* owner)
	:Component(owner)
	,mTexture(nullptr)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
