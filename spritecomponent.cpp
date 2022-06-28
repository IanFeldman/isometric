#include "actor.h"
#include "game.h"
#include "spritecomponent.h"
#include <SDL2/SDL.h>

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

/*
// we should be drawing in renderer
void SpriteComponent::Draw(Renderer* renderer) {
	Vector2 ownerPos = mOwner->GetPosition();
    int w = static_cast<int>(mTexWidth * mOwner->GetScale());
    int h = static_cast<int>(mTexHeight * mOwner->GetScale());
    // Center the rectangle around the position of the owner
    int x = static_cast<int>(ownerPos.x - w / 2);
    int y = static_cast<int>(ownerPos.y - h / 2 );

    renderer->DrawTexture(mTexture, w, h, x, y);
    
    OLD DRAWING
	SDL_Point windowSize = mOwner->GetGame()->GetRenderer()->GetWindowSize();

	// do not draw what is outside of the camera
	float halfWidth = (mTexWidth * mOwner->GetScale()) / 2.0f;
	float halfHeight = (mTexHeight * mOwner->GetScale()) / 2.0f;
	if (ownerPos.x < -halfWidth || ownerPos.y < -halfHeight || ownerPos.x > windowSize.x + halfWidth || ownerPos.y > windowSize.y + halfHeight)
		return;

	if (mTexture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(ownerPos.x - r.w / 2);
		r.y = static_cast<int>(ownerPos.y - r.h / 2 );

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(mOwner->GetGame()->GetRenderer(),
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}
*/

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
