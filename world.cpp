#include "fastnoiselite.h"
#include "game.h"
#include "world.h"
#include <SDL2/SDL.h>

World::World(Game* game)
    :mGame(game)
{
    mTileset = mGame->GetTexture("assets/depth-tileset.png");
    mNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

void World::Generate(int playerX, int playerY) {
    // amount of tiles to generate in each direction around player
    int tileCount = 128;
    int tileSize = 4; // how big to draw the tiles

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

            // -1 to 1 -> 0 to 16
            height = (int)((height + 1) * 8.0f);
            if (height == 16) { height--; }

            int tilesetRow = (int)(height / 4);
            int tilesetColumn = (int)height % 4;
            // the 16s here come from the tileset sprite itself. Immutable!
            SDL_Rect sourceRect = { tilesetColumn * 16, tilesetRow * 16, 16, 16 };

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
    SDL_DestroyTexture(backgroundTex);
}

