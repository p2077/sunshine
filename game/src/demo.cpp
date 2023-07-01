#include "raylib.h"
#include "game.h"
int main()
{
    Game game;

    game.Initialise();
    game.RunLoop();
    game.Shutdown();

    return 0;
}