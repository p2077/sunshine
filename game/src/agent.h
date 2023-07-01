#pragma once

#include "raylib.h"
#include "Rigidbody.h"
#include <list>

class IBehaviour;

class Agent {
public:
    float _maxSpeed{50.0f};
    Vector2 _acceleration;
    Rigidbody* object;

    Agent();
    Agent(Vector2 pos, float radius, Color colour);

    virtual ~Agent() {};

    // update the agent and its behaviours
    virtual void Update(float deltaTime);
    
    virtual void Draw();

    // add a behaviour to the agent
    void AddBehaviour(IBehaviour* behaviour);

    void SetPosition(Vector2 pos);
    Vector2 GetPosition();

private:
    float _radius {5.0f};
    Color _colour{ BLUE };

    std::list<IBehaviour*> _behaviours;
};
