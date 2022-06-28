#include "animatedsprite.h"
#include "actor.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
    float frameInterval = 1.0f / mAnimFPS;

    if(!mAnimName.empty()) {
        if(!mIsPaused) {
            mAnimTimer += deltaTime;

            // new frame
            if (mAnimTimer >= frameInterval) {
                mAnimTimer = 0.0f;
                mFrame = (mFrame + 1) % mAnims[mAnimName].size();
                SetTexture(mAnims[mAnimName][mFrame]);
            }
        }
    }
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}

