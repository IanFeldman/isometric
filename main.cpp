#include "game.h"
#include <iostream>

int main(int argv, char** args) {
    Game game;
    if (game.Initialize()) {
        game.RunLoop();
    }
    else {
        std::cout<<"Failed to initialize";
    }
    game.End();

    return 0;
}
