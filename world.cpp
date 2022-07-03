#include "fastnoiselite.h"
#include "game.h"
#include "math.h"
#include "tree.h"
#include "world.h"
#include <algorithm>
#include <SDL2/SDL.h>

World::World(Game* game)
    :mGame(game)
    ,mNoiseFreq(0.001f)
    ,mTileCount(10)         // number of tiles to generate in each direction
    ,mTileSize(64)          // how large to render tiles
    ,mSpriteTileSize(16)    // size of each tile on spritesheet
    ,mWaterLine(1.0f)
{
    mTileset = mGame->GetTexture("assets/tileset.png");
    mNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    mNoise.SetFrequency(mNoiseFreq);
}

void World::Generate(int playerX, int playerY) {
    // find the grid positions of player
    int playerGridX = (int)(playerX / mTileSize) * mTileSize;
    int playerGridY = (int)(playerY / mTileSize) * mTileSize;
    // when negative, round to more negative grid line
    if (playerX < 0) {
        playerGridX -= mTileSize;
    }
    if (playerY < 0) {
        playerGridY -= mTileSize;
    }

    // create big texture
    int width = mTileSize * 2 * mTileCount;
    int height = mTileSize * 2 * mTileCount;
    SDL_Texture* backgroundTex = SDL_CreateTexture(
            mGame->GetRenderer()->GetSDLRenderer(),
            SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            width,
            height);
    // set as render target
    SDL_SetRenderTarget(mGame->GetRenderer()->GetSDLRenderer(), backgroundTex);

    // sample points around player
    for (int i=-mTileCount; i<mTileCount; i++) {
        for (int j=-mTileCount; j<mTileCount; j++) {
            int sampleX = playerGridX + i * mTileSize;
            int sampleY = playerGridY + j * mTileSize;
            float height = mNoise.GetNoise((float)sampleX, (float)sampleY);
            height++; // 0 to 2
            
            SDL_Rect sourceRect;
            float sourceRotation = 0.0f;
            // location of tile on spritesheet
            int tilesetX;
            int tilesetY;

            // water
            if (height <= mWaterLine) {
                // updates tilesetX, tilesetY, and sourceRotation
                DetWaterTex(height, sampleX, sampleY, &tilesetX, &tilesetY, &sourceRotation);
            }
            // grass
            else {
                int rand = (int)(height * 1000) % 8;
                tilesetX = rand;
                tilesetY = 0;

                // tree
                if ((int)(height * 1000) % 64 == 0) {
                    // check if there is already a tree there
                    Vector2 pos = Vector2(sampleX, sampleY);
                    bool treePresent = false;
                    for (Vector2 p : mTreePositions) {
                        if (p.x == pos.x && p.y == pos.y) {
                            treePresent = true;
                            break;
                        }
                    }
                    if (!treePresent) {
                        // create tree
                        Tree* tree = new Tree(mGame);
                        mGame->AddActor(tree);
                        tree->SetPosition(pos);
                        tree->SetScale(4.0f);
                        mTreePositions.push_back(pos);
                    }
                }
            }

            // render to background texture
            sourceRect = { tilesetX * mSpriteTileSize, tilesetY * mSpriteTileSize, mSpriteTileSize, mSpriteTileSize };
            SDL_Rect destRect = { (i + mTileCount) * mTileSize, (j + mTileCount) * mTileSize, mTileSize, mTileSize }; 
            SDL_RenderCopyEx(
                    mGame->GetRenderer()->GetSDLRenderer(),
                    mTileset,
                    &sourceRect,
                    &destRect,
                    sourceRotation,
                    nullptr,
                    SDL_FLIP_NONE);
        }
    }

    SDL_SetRenderTarget(mGame->GetRenderer()->GetSDLRenderer(), nullptr);
    int texX = playerGridX - mTileCount * mTileSize;
    int texY = playerGridY - mTileCount * mTileSize;
    mGame->GetRenderer()->DrawWorld(backgroundTex, texX, texY, width, height);
    SDL_DestroyTexture(backgroundTex);
}

void World::DetWaterTex(float height, int sampleX, int sampleY, int* tilesetX, int* tilesetY, float* sourceRotation) {
    // *pointer updates value at memory address of pointer
    // check if water is nearby
    // cardinal
    bool left = mNoise.GetNoise((float)sampleX - mTileSize, (float)sampleY) + 1 > mWaterLine;
    bool right = mNoise.GetNoise((float)sampleX + mTileSize, (float)sampleY) + 1 > mWaterLine;
    bool up = mNoise.GetNoise((float)sampleX, (float)sampleY - mTileSize) + 1 > mWaterLine;
    bool down = mNoise.GetNoise((float)sampleX, (float)sampleY + mTileSize) + 1 > mWaterLine;
    // diags
    bool upLeft = mNoise.GetNoise((float)sampleX - mTileSize, (float)sampleY - mTileSize) + 1 > mWaterLine;
    bool upRight = mNoise.GetNoise((float)sampleX + mTileSize, (float)sampleY - mTileSize) + 1 > mWaterLine;
    bool downLeft = mNoise.GetNoise((float)sampleX - mTileSize, (float)sampleY + mTileSize) + 1 > mWaterLine;
    bool downRight = mNoise.GetNoise((float)sampleX + mTileSize, (float)sampleY + mTileSize) + 1 > mWaterLine;
    *tilesetX = 0;
    *tilesetY = 1;

    // regular water
    if (!left && !right && !up && !down && !upLeft && !upRight && !downLeft && !downRight) {
        // random water tile
        int rand = (int)(height * 1000) % 3;
        *tilesetX = rand;
        *tilesetY = 3;
    }
    // CARDINALS
    // only left
    else if (left && !right && !up && !down) {
        *sourceRotation = 270.0f;
    }
    // only right
    else if (!left && right && !up && !down) {
        *sourceRotation = 90.0f;
    }
    // only up
    else if (!left && !right && up && !down) {
    }
    // only down
    else if (!left && !right && !up && down) {
        *sourceRotation = 180.0f;
    }
    // left + up
    else if (left && !right && up && !down) {
        *tilesetX = 1;
    }
    // left + down
    else if (left && !right && !up && down) {
        *tilesetX = 1;
        *sourceRotation = 270.0f;
    }
    // right + up 
    else if (!left && right && up && !down) {
        *tilesetX = 1;
        *sourceRotation = 90.0f;
    }
    // right + down 
    else if (!left && right && !up && down) {
        *tilesetX = 1;
        *sourceRotation = 180.0f;
    }
    // left + right 
    else if (left && right && !up && !down) {
        *tilesetX = 2;
        *sourceRotation = 90.0f;
    }
    // up + down 
    else if (!left && !right && up && down) {
        *tilesetX = 2;
    }
    // left + up + right 
    else if (left && right && up && !down) {
        *tilesetX = 3;
        *sourceRotation = 90.0f;
    }
    // up + right + down
    else if (!left && right && up && down) {
        *tilesetX = 3;
        *sourceRotation = 180.0f;
    }
    // right + down + left 
    else if (left && right && !up && down) {
        *tilesetX = 3;
        *sourceRotation = 270.0f;
    }
    // down + left + up  
    else if (left && !right && up && down) {
        *tilesetX = 3;
    }
    // left + right + up + down
    else if (left && right && up && down) {
        *tilesetX = 4;
    }

    // DIAGONALS
    // upleft
    else if (upLeft && !upRight && !downLeft && !downRight) {
        *tilesetY = 2;
    }
    // upright
    else if (!upLeft && upRight && !downLeft && !downRight) {
        *tilesetY = 2;
        *sourceRotation = 90.0f;
    }
    // downleft
    else if (!upLeft && !upRight && downLeft && !downRight) {
        *tilesetY = 2;
        *sourceRotation = 270.0f; 
    }
    // downright 
    else if (!upLeft && !upRight && !downLeft && downRight) {
        *tilesetY = 2;
        *sourceRotation = 180.0f;
    }
    // upleft + upright
    else if (upLeft && upRight && !downLeft && !downRight) {
        *tilesetX = 1;
        *tilesetY = 2;
    }
    // upright + downright
    else if (!upLeft && upRight && !downLeft && downRight) {
        *tilesetX = 1;
        *tilesetY = 2;
        *sourceRotation = 90.0f;
    }
    // downleft + downright
    else if (!upLeft && !upRight && downLeft && downRight) {
        *tilesetX = 1;
        *tilesetY = 2;
        *sourceRotation = 180.0f;
    }
    // upleft + downleft
    else if (upLeft && !upRight && downLeft && !downRight) {
        *tilesetX = 1;
        *tilesetY = 2;
        *sourceRotation = 270.0f;
    }
    // upleft + downright
    else if (upLeft && !upRight && !downLeft && downRight) {
        *tilesetX = 2;
        *tilesetY = 2;
    }
    // upright + downleft
    else if (!upLeft && upRight && downLeft && !downRight) {
        *tilesetX = 2;
        *tilesetY = 2;
        *sourceRotation = 90.0f;
    }
    // upleft + upright + downright
    else if (upLeft && upRight && !downLeft && downRight) {
        *tilesetX = 3;
        *tilesetY = 2;
    }
    // upright + downright + downleft
    else if (!upLeft && upRight && downLeft && downRight) {
        *tilesetX = 3;
        *tilesetY = 2;
        *sourceRotation = 90.0f;
    }
    // downright + downleft + upleft
    else if (upLeft && !upRight && downLeft && downRight) {
        *tilesetX = 3;
        *tilesetY = 2;
        *sourceRotation = 180.0f;
    }
    // downleft + upleft + upright
    else if (upLeft && upRight && downLeft && !downRight) {
        *tilesetX = 3;
        *tilesetY = 2;
        *sourceRotation = 270.0f;
    }
    // upleft + upright + downleft + downright
    else if (upLeft && upRight && downLeft && downRight) {
        *tilesetX = 4;
        *tilesetY = 2;
    }
}

bool World::IsWater(int x, int y) {
    // get the grid positions of player
    int gridX = (int)(x / mTileSize) * mTileSize;
    int gridY = (int)(y / mTileSize) * mTileSize;
    // when negative, round to more negative gridline
    if (x < 0) {
        gridX -= mTileSize;
    }
    if (y < 0) {
        gridY -= mTileSize;
    }

    float height = mNoise.GetNoise((float)gridX, (float)gridY);
    height++;

    return height <= mWaterLine;
}

