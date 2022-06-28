#pragma once
#include "fastnoiselite.h"
#include <SDL2/SDL.h>

class World {

public:
    World(class Game* game);
    void Generate(int playerX, int playerY);

protected:
    class Game* mGame;
    FastNoiseLite mNoise;
    SDL_Texture* mTileset;
};

