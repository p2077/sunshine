#include "rlImGui.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    InitAudioDevice();
    Vector2 ballPosition = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };
    Vector2 mousePosition = { 0 };

    SetTargetFPS(60);


    SetExitKey(KEY_ESCAPE); 

    Texture2D texture = LoadTexture("C:\\Users\\prabh\\source\\repos\\p2077\\sunshine\\game\\assets\\textures\\erehh.png");

    Music music = LoadMusicStream("C:\\Users\\prabh\\source\\repos\\p2077\\sunshine\\game\\assets\\audio\\my name is jeff.mp3");
    music.looping = false;
    float pitch = 1.0f;

    PlayMusicStream(music);


    float timePlayed = 0.0f;
    bool pause = false;
    Color color1;
    Color color2;
    Color color3 = YELLOW; 

    while (!WindowShouldClose())
    {
        //music
        UpdateMusicStream(music);

        // Restart music playing (stop and play)
        if (IsKeyPressed(KEY_SPACE))
        {
            StopMusicStream(music);
            PlayMusicStream(music);
        }

        // Pause/Resume music playing
        if (IsKeyPressed(KEY_P))
        {
            pause = !pause;

            if (pause) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }

        if (IsKeyDown(KEY_S)) pitch -= 0.01f;
        else if (IsKeyDown(KEY_W)) pitch += 0.01f;

        SetMusicPitch(music, pitch);

        //time played

        timePlayed = GetMusicTimePlayed(music);


        //fullscreen
        if (IsKeyPressed(KEY_F))
            ToggleFullscreen();

        //hide cursor
        if (IsKeyPressed(KEY_H))
            HideCursor();
     

        // motion of the objects using arrow keys and cursor

        mousePosition = GetMousePosition(); // Get the mouse position every frame

        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;

        //collison

        if (CheckCollisionCircles(ballPosition, 50, mousePosition, 35))
        {
            color1 = PURPLE;
            color2 = LIME;
        }
        else
        {
            color1 = BLACK;
            color2 = DARKBLUE;
        }

        // color change of ball

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) color3 = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) color3 = BEIGE;

 
        //drawing

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //texts

        DrawText("Hello World!", 16, 9, 20, RED);

        DrawText("Press esc key to exit window", 200,5, 12, BLACK);

        DrawText("Press w key to increase the pitch of the audio", 200, 20, 12, BLACK);

        DrawText("Press s key to decrease the pitch of the audio", 200, 35, 12, BLACK);

        DrawText("Press space key to play the audio", 900, 20, 12, BLACK);

        DrawText("Press p key to pause/resume the audio", 900, 35, 12, BLACK);

        DrawText("Press left mouse button to change the color of the stationary circle to maroon", 10, 650, 12, BLACK);

        DrawText("Press right mouse button to change the color of the stationary circle to beige", 10, 665, 12, BLACK);

        DrawText("Press h key to hide cursor", 10, 680, 12, BLACK);

        DrawText(TextFormat("Time Played: %.2f sec", timePlayed), 10, 600, 20, DARKGRAY);

        //objects
 
        // line 
        DrawLine(10, 50, 130, 50, BLUE);

        // rectangle
        DrawRectangle(80, 80, 80, 100, VIOLET);

        // ball
        DrawCircleV(ballPosition, 50, color1);

        // texture
        DrawTexture(texture, 350,0, WHITE);

        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, BLACK);

        // stationary circle
        DrawCircle(300, 300, 50,color3);

        // controlled circle
        DrawCircleV(mousePosition, 35, color2);

       


        
        EndDrawing();
    }

    UnloadTexture(texture);
    UnloadMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
    return 0;
}



/* func1.exitkey function useful to exit the application by using just one key
   func2. getfps function can be used to get fps
   func3. iskeypressed function used to check if a key has been pressed once
   func4. setmusicpitch function used to set the pitch of audio played
   func5. pausemusicstream function used to pause the audio played
   func6. resumemusicstream funton used to resume the audio from where it was left
   func7. getmusictimeplayed funtion used to determine how long the music is being played for
   func8. ismousebutton pressed used to check if a mouse button has been pressed once
   func9. togglefullscreen funtion used to chnage window from windowed to fullscreen and visa versa
   func10. hidecursor used to hide cursor
 */