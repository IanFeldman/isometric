#include "game.h"
#include "ui.h"
#include <SDL2/SDL.h>

UI::UI(Game* game)
    :mGame(game)
    ,mVisible(true)
{
    mGame->AddUI(this);
}

// remove actors from game list, and destroy components
UI::~UI()
{
    mGame->RemoveUI(this);
}

void UI::OnClick() {
}

