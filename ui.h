#pragma once
#include <SDL2/SDL.h>

class UI
{
public:
	UI(class Game* game);
    // overridable destructor
	virtual ~UI();

    bool IsVisible() { return mVisible; }
    // getters/setters
    void SetTexture(SDL_Texture* tex) { mTexture = tex; }
    SDL_Texture* GetTexture() { return mTexture; }
    void SetRect(SDL_Rect r) { mRect = r; }
    SDL_Rect GetRect() { return mRect; }

	// Click function called from Game (overridable)
	virtual void OnClick();

protected:
	class Game* mGame;

    SDL_Texture* mTexture;
    bool mVisible;

	// Transform
    SDL_Rect mRect;
	float mScale;
};
