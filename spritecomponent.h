#pragma once
#include "component.h"
#include <SDL2/SDL.h>

class SpriteComponent : public Component
{
public:
	// Lower draw order corresponds with further back
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	// Draw this sprite
	//virtual void Draw(Renderer* renderer);
	// Set the texture to draw for this sprite
	virtual void SetTexture(SDL_Texture* texture);

    SDL_Texture* GetTexture() { return mTexture; }

	// Get the draw order for this sprite
	int GetDrawOrder() const { return mDrawOrder; }
	// Get the width/height of the texture
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	bool IsVisible() const { return mIsVisible; }
	void SetIsVisible(bool visible) { mIsVisible = visible; }

protected:
	// Texture to draw
	SDL_Texture* mTexture;
	// Draw order
	int mDrawOrder;
	// Width/height
	int mTexWidth;
	int mTexHeight;
	bool mIsVisible = true;
};
