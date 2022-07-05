#include "actor.h"
#include "math.h"
#include "game.h"
#include "player.h"
#include "renderer.h"
#include "spritecomponent.h"
#include "world.h"
#include <algorithm>
#include <SDL2/SDL.h>

Game::Game()
    :mRunning(true)
    ,mCamera(Vector2(0.0f, 0.0f))
{
}

bool Game::Initialize() {
    // create renderer
    mRenderer = new Renderer(this); // new creates the object and returns reference to it
    if (!mRenderer->Create())
        return false;
    
    // set previous time
    mPrevTime = SDL_GetTicks();
    
    LoadData();
    return true;
}

void Game::LoadData() {
    mTileset = mRenderer->LoadImage("assets/tileset.png");

    mPlayer = new Player(this);
    mPlayer->SetPosition(Vector2(400, 0));
    mPlayer->SetScale(4.0f);

    mWorld = new World(this);
}

void Game::RunLoop() {
    while (mRunning){
        // poll for exit
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    mRunning = false;
                    break;
                default:
                    break;
            }
        }

        Update();
        GenerateOutput();
    }
}

void Game::Update() {
    // Setting fps
    while ((int)SDL_GetTicks() < mPrevTime + 16) {}
    int currentTime = SDL_GetTicks();
    float deltaTime = (float)(currentTime - mPrevTime) / 1000;
    mPrevTime = currentTime;

    // update actors
    // calls onupdate and updates all components
    std::vector<Actor*> actors = mActors;
    for (Actor* a : actors) {
        a->Update(deltaTime);
    }

    // destroy dead actors
    std::vector<Actor*> tempActors;
    for (Actor* a : mActors) {
        if (a->GetState() == ActorState::Destroy) {
            tempActors.emplace_back(a);
        }
    }

    if (!tempActors.empty()) {
        for (Actor* tempActor : tempActors) {
            delete tempActor; // calls actor destructor
        }
    }
}

void Game::GenerateOutput() {
    // clear screen
    mRenderer->ClearScreen();

    // generates and renders terrain
    mWorld->Generate((int)mPlayer->GetPosition().x, (int)mPlayer->GetPosition().y);

    // loop over sprites and draw them
    for (SpriteComponent* sprite : mSprites) {
        if (sprite->IsVisible()) {
            mRenderer->DrawSprite(sprite);
        }
    }

    // CC DEBUG
    for (Actor* a : mActors) {
        CollisionComponent* cc = a->GetComponent<CollisionComponent>();
        if (cc != nullptr) {
            cc->Debug();
        }
    }

    // render present
    mRenderer->Present();
}

void Game::End() {
    UnloadData();
    SDL_Quit();
}

void Game::UnloadData() {
    // destructor kills window and renderer
    // deletes textures as well
    delete mRenderer;

    // delete all actors
    // erase takes out of vector and deletes
    if (!mActors.empty()) {
        mActors.erase(mActors.begin(), mActors.end());
    }

    // destroy textures
    SDL_DestroyTexture(mTileset);
}

// add/remove actors
void Game::AddActor(Actor* actor) {
    mActors.push_back(actor);
}

void Game::RemoveActor(Actor* actor) {
    // create iterator
    std::vector<Actor*>::iterator it;
    // find actor
    it = std::find(mActors.begin(), mActors.end(), actor);
    // remove it from actor vector
    if (it != mActors.end()) {
        mActors.erase(it);
    }
}

// add/remove sprites
void Game::AddSprite(SpriteComponent* sprite) {
    mSprites.push_back(sprite);
    std::sort(mSprites.begin(), mSprites.end(),
        [](SpriteComponent* a, SpriteComponent* b) {
            return a->GetDrawOrder() < b->GetDrawOrder();
        });
}

void Game::RemoveSprite(SpriteComponent* sprite) {
    std::vector<SpriteComponent*>::iterator it;
    it = std::find(mSprites.begin(), mSprites.end(), sprite);

    if (it != mSprites.end()) {
        mSprites.erase(it);
    }
}

