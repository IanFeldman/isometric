#pragma once
#include "spritecomponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>

class Renderer {

public:
    Renderer(class Game* game);
    ~Renderer();

    bool Create();
    void ClearScreen();
    void Present();

    // creates texture out of image file
    SDL_Texture* LoadImage(const char* fileName);
    // fetches texture from tileset
    SDL_Texture* GetTexture(SDL_Rect rect);
    // creates texture from portion of another texture
    SDL_Texture* CreateTextureFromTexture(SDL_Texture* sourceTex, int width, int height, SDL_Rect* sourceRect);

    void DrawSprite(SpriteComponent* sprite);
    void DrawWorld(SDL_Texture* texture, int worldX, int worldY, int width, int height);

    // CC DEBUG
    void DrawRectangle(SDL_Rect rect);


    // getters/setters
    SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }
    SDL_Point GetWindowSize() { return mWindowSize; }

protected:
    class Game* mGame;
    SDL_Point mWindowSize;
    SDL_Window* mWindow;
    SDL_Renderer* mSDLRenderer;

    std::unordered_map<std::string, SDL_Texture*> mTextureCache;
};

