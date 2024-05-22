#include "../include/game.h"

int main()
{
    Options::initialize();
    ResourceManager::initialize();

    Game game;
    game.run();

    return 0;
}