#include "raylib.h"
#include "Agent.h"
#include "followMouse.h"
#include "seek.h"
#include "flee.h"

class Game
{
public:
    Game();
    
    void Initialise();
    
    void RunLoop();
    
    void Shutdown();

private:
    
    void ProcessInput();
    void UpdateGame(float deltaTime);
    void GenerateOutput();

    Vector2 _mousePosition = { 0.0f, 0.0f };
    Agent* player;
    FollowMouseBehaviour* followMouseBehaviour;

    Agent* firstEnemy;
    SeekBehaviour* seekBehaviour;

    Agent* secondEnemy;
    FleeBehaviour* fleeBehaviour;
};