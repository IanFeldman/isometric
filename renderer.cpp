#include "actor.h"
#include "game.h"
#include "renderer.h"
#include "spritecomponent.h"
#include "UI.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// constructor
Renderer::Renderer(Game* game)
    :mGame(game)
    ,mWindowSize((SDL_Point){1024, 1024})
{ }

Renderer::~Renderer() {
    // Destroy window
    SDL_DestroyWindow(mWindow);
    // Destroy renderer
    SDL_DestroyRenderer(mSDLRenderer);
    // Clear textures
    for (auto it = mTextureCache.begin(); it != mTextureCache.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    mTextureCache.clear();
}

bool Renderer::Create() {
    // init video
    int initialized = SDL_Init(SDL_INIT_VIDEO);
    if (initialized != 0) {
        return false;
    }
    // init sdl_img
    int imgInitialized = IMG_Init(IMG_INIT_PNG);
    if ((imgInitialized & IMG_INIT_PNG) != IMG_INIT_PNG) {
        return false;
    }
    // create window
    mWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowSize.x, mWindowSize.y, 0);
    if (mWindow == NULL) {
        return false;
    }
    // create renderer
    mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (mSDLRenderer == NULL) {
        return false;
    }

    SDL_SetRenderDrawBlendMode(mSDLRenderer, SDL_BLENDMODE_BLEND);
    return true;
}

void Renderer::ClearScreen() {
    SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 0);
    SDL_RenderClear(mSDLRenderer);
}

void Renderer::Present() {
    SDL_RenderPresent(mSDLRenderer);
}

SDL_Texture* Renderer::LoadImage(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(mSDLRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return tempTexture;
}

SDL_Texture* Renderer::GetTexture(SDL_Rect rect) {
    // convert rect to string
    std::string key = std::to_string(rect.x) + std::to_string(rect.y) + std::to_string(rect.w) + std::to_string(rect.h);
    // look for texture
    std::unordered_map<std::string, SDL_Texture*>::const_iterator got = mTextureCache.find(key);
    // get it from map
    if (got != mTextureCache.end()) {
        return got->second;
    }
    else {
        SDL_Texture* tex = CreateTextureFromTexture(mGame->GetTileset(), rect.w, rect.h, &rect);
        mTextureCache.emplace(key, tex);
        return tex;
    }
}

SDL_Texture* Renderer::CreateTextureFromTexture(SDL_Texture* sourceTex, int width, int height, SDL_Rect* sourceRect) {
    SDL_Texture* tex = SDL_CreateTexture(mSDLRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetRenderTarget(mSDLRenderer, tex);
    SDL_RenderCopy(mSDLRenderer, sourceTex, sourceRect, nullptr);
    SDL_SetRenderTarget(mSDLRenderer, nullptr);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    return tex;
}

void Renderer::DrawSprite(SpriteComponent* sprite) {
    // cam worldspace
    float camX = mGame->GetCamera().x;
    float camY = mGame->GetCamera().y;
    // owner world pos -> cam pos
    float ownerPosX = sprite->GetOwner()->GetPosition().x - camX + mWindowSize.x / 2; // relative to cam position
    float ownerPosY = sprite->GetOwner()->GetPosition().y - camY + mWindowSize.y / 2; // relative to cam position
    float texW = sprite->GetTexWidth();
    float texH = sprite->GetTexHeight();
    float scale = sprite->GetOwner()->GetScale();

    // check if sprite is out of screen
    if (ownerPosX + (texW / 2.0f) * scale < 0 || ownerPosX - (texW / 2.0f) * scale > mWindowSize.x) {
        return;
    }

    // called by game in process update
    SDL_Rect r;
    r.w = static_cast<int>(texW * scale);
    r.h = static_cast<int>(texH * scale);
    r.x = static_cast<int>(ownerPosX - r.w / 2);
    r.y = static_cast<int>(ownerPosY - r.h / 2);

    SDL_RenderCopy(mSDLRenderer, sprite->GetTexture(), nullptr, &r);
}

void Renderer::DrawUI(UI* uiElement) {
    SDL_Rect r = uiElement->GetRect();
    SDL_RenderCopy(mSDLRenderer, uiElement->GetTexture(), nullptr, &r);  
}

void Renderer::DrawWorld(SDL_Texture* texture, int worldX, int worldY, int width, int height) {
    // cam worldspace
    float camX = mGame->GetCamera().x;
    float camY = mGame->GetCamera().y;
    SDL_Rect r = { (int)(worldX - camX + mWindowSize.x / 2), (int)(worldY - camY + mWindowSize.y / 2), width, height };
    SDL_RenderCopy(mSDLRenderer, texture, nullptr, &r);
}

// CC DEBUG
void Renderer::DrawRectangle(SDL_Rect rect) {
    // cam worldspace
    float camX = mGame->GetCamera().x;
    float camY = mGame->GetCamera().y;
    // convert to cam space
    rect.x += -camX + mWindowSize.x / 2;
    rect.y += -camY + mWindowSize.y / 2;
    SDL_SetRenderDrawColor(mSDLRenderer, 0, 255, 0, 255);
    SDL_RenderDrawRect(mSDLRenderer, &rect);
}

