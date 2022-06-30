#pragma once
#include "spritecomponent.h"
#include <string>
#include <unordered_map>
#include <vector>

class AnimatedSprite : public SpriteComponent
{
public:
    AnimatedSprite(class Actor*, SDL_Texture* spritesheet, int drawOrder = 100);

    // overriding component update
    void Update(float deltaTime) override;

    void AddAnimation(const std::string* name, const std::vector<SDL_Rect*>* frames);

    void SetAnimation(const std::string* name, bool resetTimer = false);

    const std::string* GetAnimation() { return &mAnimName; }

    void SetIsPaused(bool pause) { mIsPaused = pause; }

    void SetFPS(float fps) { mAnimFPS = fps; }

protected:
    // spritesheet with animation frames on it
    SDL_Texture* mSpritesheet;
	// Map of animation name to vector of textures corresponding to the animation
	std::unordered_map<std::string, std::vector<SDL_Texture*>> mAnims;
	
	// Name of current animation
	std::string mAnimName;
	
	// Whether or not the animation is paused (defaults to false)
	bool mIsPaused = false;
	
    // times frames
	float mAnimTimer = 0.0f;
	
	// The frames per second the animation should run at
	float mAnimFPS = 10.0f;

    // current frame the animation is on
    int mFrame = 0;
};
