#include <raylib.h>
#include <cstdlib> 
#include <cmath>
#include <vector>


struct Circle
{
    Vector2 center;
    float radius;
};

struct GameObject
{
    Circle circle;
    bool isConsumed;
};

class Rigidbody
{
public:
    Vector2 velocity;
    Vector2 position;
};

// normalizing
Vector2 Vector2Normalizing(Vector2 vector)
{
    float length = sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length != 0)
    {
        vector.x /= length;
        vector.y /= length;
    }
    return vector;
}

Vector2 Vector2Scaling(Vector2 vector, float scale)
{
    vector.x *= scale;
    vector.y *= scale;
    return vector;
}

//addition
Vector2 Vector2Addition(Vector2 v1, Vector2 v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

// subtraction
Vector2 Vector2Subtraction(Vector2 v1, Vector2 v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}
// rotation of vector
Vector2 Vector2Rotate(Vector2 vector, float angle)
{
    float radians = angle * PI / 180.0f;
    float cosAngle = cosf(radians);
    float sinAngle = sinf(radians);

    float x = vector.x * cosAngle - vector.y * sinAngle;
    float y = vector.x * sinAngle + vector.y * cosAngle;

    vector.x = x;
    vector.y = y;

    return vector;
}

class Agent
{
public:

    Rigidbody rigidbody;
    Circle circle;
    float maxSpeed;
    float maxAcceleration;
    Vector2 rightWhisker;
    Vector2 leftWhisker;

    void obstacleAvoidance(const Circle& obstacle, float avoidanceForce)
    {

        Vector2 ahead = Vector2Addition(rigidbody.position, Vector2Scaling(Vector2Normalizing(rigidbody.velocity), 75.0f));
        Vector2 ahead2 = Vector2Addition(rigidbody.position, Vector2Scaling(Vector2Normalizing(rigidbody.velocity), 50.0f));

        Vector2 avoidance = { 0, 0 };
        bool collisionDetected = false;

        // collision check
        if (CheckCollisionCircles(obstacle.center, obstacle.radius, circle.center, circle.radius))
        {
            avoidance = Vector2Subtraction(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead.x, ahead.y, circle.radius, circle.radius }))
        {
            avoidance = Vector2Subtraction(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }
        else if (CheckCollisionCircleRec(obstacle.center, obstacle.radius, { ahead2.x, ahead2.y, circle.radius, circle.radius }))
        {
            avoidance = Vector2Subtraction(rigidbody.position, obstacle.center);
            collisionDetected = true;
        }

        if (collisionDetected)
        {
            // Calculate the angle to rotate the agent
            float angle = atan2(avoidance.y, avoidance.x) - atan2(rigidbody.velocity.y, rigidbody.velocity.x);
            angle = angle * 180.0f / PI;

            // Rotate the agent
            rigidbody.velocity = Vector2Rotate(rigidbody.velocity, angle);
            leftWhisker = Vector2Rotate(leftWhisker, angle);
            rightWhisker = Vector2Rotate(rightWhisker, angle);

            // Apply avoidance force
            avoidance = Vector2Normalizing(avoidance);
            avoidance = Vector2Scaling(avoidance, avoidanceForce);
            rigidbody.velocity = Vector2Addition(rigidbody.velocity, avoidance);

            // Limit the agent's velocity
            float speed = std::hypot(rigidbody.velocity.x, rigidbody.velocity.y);
            if (speed > maxSpeed)
            {
                rigidbody.velocity = Vector2Scaling(rigidbody.velocity, maxSpeed / speed);
            }
        }
    }


};

int main(void)
{
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Agent Avoidance");
    SetTargetFPS(60);

    Agent firstAgent;
    firstAgent.rigidbody.position = { SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f };
    firstAgent.rigidbody.velocity = { 30.0f, 30.0f };
    firstAgent.maxSpeed = 599.0f;
    firstAgent.maxAcceleration = 100.0f;
    firstAgent.circle = { firstAgent.rigidbody.position, 20.0f };

    firstAgent.leftWhisker = { firstAgent.rigidbody.velocity.x * std::cos(-45.0f) - firstAgent.rigidbody.velocity.y * std::sin(-45.0f),
                           firstAgent.rigidbody.velocity.x * std::sin(-45.0f) + firstAgent.rigidbody.velocity.y * std::cos(-45.0f) };

    firstAgent.rightWhisker = { firstAgent.rigidbody.velocity.x * std::cos(45.0f) - firstAgent.rigidbody.velocity.y * std::sin(45.0f),
                            firstAgent.rigidbody.velocity.x * std::sin(45.0f) + firstAgent.rigidbody.velocity.y * std::cos(45.0f) };

    Agent secondAgent;
    secondAgent.rigidbody.position = { SCREEN_WIDTH / 2.0f + 100, SCREEN_HEIGHT / 2.0f };
    secondAgent.rigidbody.velocity = { -30.0f, -30.0f };
    secondAgent.maxSpeed = 500.0f;
    secondAgent.maxAcceleration = 100.0f;
    secondAgent.circle = { secondAgent.rigidbody.position, 20.0f };

    secondAgent.rightWhisker = { secondAgent.rigidbody.velocity.x * std::cos(45.0f) - secondAgent.rigidbody.velocity.y * std::sin(45.0f),
                            secondAgent.rigidbody.velocity.x * std::sin(45.0f) + secondAgent.rigidbody.velocity.y * std::cos(45.0f) };

    secondAgent.leftWhisker = { secondAgent.rigidbody.velocity.x * std::cos(-45.0f) - secondAgent.rigidbody.velocity.y * std::sin(-45.0f),
                           secondAgent.rigidbody.velocity.x * std::sin(-45.0f) + secondAgent.rigidbody.velocity.y * std::cos(-45.0f) };

    std::vector<Circle> obstacles;
    std::vector<GameObject> gameObjects;
    Color foodColor = ORANGE;
    Color obstacleColor = PURPLE;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            float objectRadius = 10.0f;
            bool createFood = (rand() / (float)RAND_MAX) < 0.5;

            if (createFood)
            {
                GameObject gameObject = { { mousePos, objectRadius }, false };
                gameObjects.push_back(gameObject);
            }
            else
            {
                Circle obstacle = { mousePos, objectRadius };
                obstacles.push_back(obstacle);
            }
        }


        for (const Circle& obstacle : obstacles)
        {
            firstAgent.obstacleAvoidance(obstacle, 1.5f);
            secondAgent.obstacleAvoidance(obstacle, 1.5f);
        }

        for (GameObject& gameObject : gameObjects)
        {
            if (!gameObject.isConsumed)
            {
                firstAgent.obstacleAvoidance(gameObject.circle, 5.5f);
                secondAgent.obstacleAvoidance(gameObject.circle, 5.5f);

                // Check if an agent has consumed the object
                if (CheckCollisionCircles(firstAgent.circle.center, firstAgent.circle.radius, gameObject.circle.center, gameObject.circle.radius))
                {
                    gameObject.isConsumed = true;
                }
                else if (CheckCollisionCircles(secondAgent.circle.center, secondAgent.circle.radius, gameObject.circle.center, gameObject.circle.radius))
                {
                    gameObject.isConsumed = true;
                }
            }
        }


        firstAgent.rigidbody.position.x += firstAgent.rigidbody.velocity.x * deltaTime;
        firstAgent.rigidbody.position.y += firstAgent.rigidbody.velocity.y * deltaTime;
        firstAgent.circle.center = firstAgent.rigidbody.position;

        secondAgent.rigidbody.position.x += secondAgent.rigidbody.velocity.x * deltaTime;
        secondAgent.rigidbody.position.y += secondAgent.rigidbody.velocity.y * deltaTime;
        secondAgent.circle.center = secondAgent.rigidbody.position;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircleV(firstAgent.circle.center, firstAgent.circle.radius, DARKGREEN);
        DrawCircleV(secondAgent.circle.center, secondAgent.circle.radius, BLUE);

        // Draw whiskers
        DrawLine((int)firstAgent.rigidbody.position.x, (int)firstAgent.rigidbody.position.y,
            (int)(firstAgent.rigidbody.position.x + firstAgent.leftWhisker.x * 5),
            (int)(firstAgent.rigidbody.position.y + firstAgent.leftWhisker.y * 5), RED);

        DrawLine((int)firstAgent.rigidbody.position.x, (int)firstAgent.rigidbody.position.y,
            (int)(firstAgent.rigidbody.position.x + firstAgent.rightWhisker.x * 5),
            (int)(firstAgent.rigidbody.position.y + firstAgent.rightWhisker.y * 5), RED);

        DrawLine((int)secondAgent.rigidbody.position.x, (int)secondAgent.rigidbody.position.y,
            (int)(secondAgent.rigidbody.position.x + secondAgent.leftWhisker.x * 5),
            (int)(secondAgent.rigidbody.position.y + secondAgent.leftWhisker.y * 5), RED);

        DrawLine((int)secondAgent.rigidbody.position.x, (int)secondAgent.rigidbody.position.y,
            (int)(secondAgent.rigidbody.position.x + secondAgent.rightWhisker.x * 5),
            (int)(secondAgent.rigidbody.position.y + secondAgent.rightWhisker.y * 5), RED);

        //food
        for (const Circle& obstacle : obstacles)
        {
            DrawCircleV(obstacle.center, obstacle.radius, obstacleColor);
        }


        for (const GameObject& gameObject : gameObjects)
        {
            if (!gameObject.isConsumed)
            {
                DrawCircleV(gameObject.circle.center, gameObject.circle.radius, foodColor);
            }
        }


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
