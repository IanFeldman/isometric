#pragma once
#include "spritecomponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Renderer {

public:
    Renderer(class Game* game);
    ~Renderer();

    bool Create();
    void ClearScreen();
    void Present();

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
};

