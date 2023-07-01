#include "game.h"
#include "raylib.h"

Game::Game()
{

}

void Game::Initialise()
{
    SetTargetFPS(60);
    InitWindow(800, 600, "Kinematics Simulation");

    player = new Agent({ 150, 150 }, 20.0f, PURPLE);

    followMouseBehaviour = new FollowMouseBehaviour();
    player->AddBehaviour(followMouseBehaviour);
    player->_maxSpeed = 100.0f;

    firstEnemy = new Agent({ 760, 20 }, 20.0f, ORANGE);
    seekBehaviour = new SeekBehaviour(player);
    firstEnemy->AddBehaviour(seekBehaviour);

    secondEnemy = new Agent({ 400,300 }, 20.0f, LIME);
    fleeBehaviour = new FleeBehaviour(player);
    secondEnemy->AddBehaviour(fleeBehaviour);
}

void Game::RunLoop()
{
    while (!WindowShouldClose())
    {
        ProcessInput();
        UpdateGame(GetFrameTime());
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    CloseWindow();
}

void Game::ProcessInput()
{
    //_mousePosition = GetMousePosition();
}

void Game::UpdateGame(float deltaTime)
{

    player->Update(deltaTime);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) == true)
        firstEnemy->Update(deltaTime);
    secondEnemy->Update(deltaTime);
}

void Game::GenerateOutput()
{
    BeginDrawing();

    
    ClearBackground(WHITE);

    player->Draw();
    firstEnemy->Draw();
    secondEnemy->Draw();

    EndDrawing();
}