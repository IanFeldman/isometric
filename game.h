#pragma once
#include "math.h"
#include "renderer.h"
#include "spritecomponent.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
// CC DEBUG
#include "collisioncomponent.h"

class Game {
    
public:
    Game();
    bool Initialize();
    void LoadData();
    void RunLoop();
    void Update();
    void GenerateOutput();
    void End();
    void UnloadData();

    // adds actor to actor list here
    void AddActor(class Actor* actor);
    // removes actor from actor list here
    void RemoveActor(class Actor* actor);
    // get list of actors
    std::vector<class Actor*> GetActors() { return mActors; }
    
    // add sprite
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    // getters/setters
    Renderer* GetRenderer() { return mRenderer; }

    SDL_Texture* GetTexture(const char* fileName);

    Vector2 GetCamera() { return mCamera; }
    void SetCamera(Vector2 pos) { mCamera = pos; }

protected:
    bool mRunning;
    int mPrevTime;
    class Renderer* mRenderer;
    std::vector<class Actor*> mActors;
    std::vector<class SpriteComponent*> mSprites;
    std::unordered_map<std::string, SDL_Texture*> mTextureCache;

    // collision component debug
    std::vector<class CollisionComponent*> mColliders;

    class Player* mPlayer; // write class so I dont have to #include player.h
    class World* mWorld;

    Vector2 mCamera;
};
