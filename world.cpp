#include "fastnoiselite.h"
#include "game.h"
#include "world.h"
#include <SDL2/SDL.h>

World::World(Game* game)
    :mGame(game)
{
    mTileset = mGame->GetTexture("assets/tileset.png");
    mNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

void World::Generate(int playerX, int playerY) {
    // amount of tiles to generate in each direction around player
    int tileCount = 3;
    int tileSize = 256;

    // find the grid positions of player
    int playerGridX = (int)(playerX / tileSize) * tileSize;
    int playerGridY = (int)(playerY / tileSize) * tileSize;

    // create big texture
    SDL_Texture* backgroundTex = SDL_CreateTexture(mGame->GetRenderer()->GetSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tileCount * 2 * tileSize, tileCount * 2 * tileSize);
    // set as render target
    SDL_SetRenderTarget(mGame->GetRenderer()->GetSDLRenderer(), backgroundTex);

    // sample points around player
    for (int i=-tileCount; i<tileCount; i++) {
        for (int j=-tileCount; j<tileCount; j++) {
            int sampleX = playerGridX + i * tileSize;
            int sampleY = playerGridY + j * tileSize;
            float height = mNoise.GetNoise((float)sampleX, (float)sampleY);

            SDL_Rect sourceRect;
            if (height < -0.5f) {
               sourceRect = {48, 0, 16, 16}; 
            }
            else if (height < 0.0f) {
               sourceRect = {32, 0, 16, 16}; 
            }
            else if (height < 0.5f) {
               sourceRect = {16, 0, 16, 16}; 
            }
            else{
               sourceRect = {0, 0, 16, 16}; 
            }

            SDL_Rect destRect = { (i + tileCount) * tileSize, (j + tileCount) * tileSize, tileSize, tileSize }; 
            SDL_RenderCopy(mGame->GetRenderer()->GetSDLRenderer(), mTileset, &sourceRect, &destRect);
        }
    }

    SDL_SetRenderTarget(mGame->GetRenderer()->GetSDLRenderer(), nullptr);
    int texX = playerGridX - tileCount * tileSize;
    int texY = playerGridY - tileCount * tileSize;
    int width = tileSize * 2 * tileCount;
    int height = tileSize * 2 * tileCount;
    mGame->GetRenderer()->DrawWorld(backgroundTex, texX, texY, width, height);
}

