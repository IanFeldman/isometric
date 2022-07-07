#pragma once
#include "component.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner);
	~SpriteComponent();

	// Set the texture to draw for this sprite
	virtual void SetTexture(SDL_Texture* texture);

    SDL_Texture* GetTexture() { return mTexture; }

	// Get the width/height of the texture
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	bool IsVisible() const { return mIsVisible; }
	void SetIsVisible(bool visible) { mIsVisible = visible; }

protected:
	// Texture to draw
	SDL_Texture* mTexture;
	// Width/height
	int mTexWidth;
	int mTexHeight;
	bool mIsVisible = true;
};
