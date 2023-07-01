#include "agent.h"
#include "raymath.h"
#include "IBehaviour.h"
#include <stdio.h>

Agent::Agent()
{
    object = new Rigidbody();
}

Agent::Agent(Vector2 pos, float radius, Color colour) :
    _radius{ radius },
    _colour{ colour }
{
    object = new Rigidbody(pos, { 0.0f, 0.0f });
    object->_position = pos;
}

void Agent::SetPosition(Vector2 pos)
{
    object->_position = pos;
}

Vector2 Agent::GetPosition()
{
    return object->_position;
}

void Agent::Update(float deltaTime)
{
    // for each behaviour in the behavious list
    for (auto it = _behaviours.begin(); it != _behaviours.end(); it++)
    {
        (*it)->Update(this, deltaTime);
    }

    object->_velocity.x += _acceleration.x;
    object->_velocity.y += _acceleration.y;

    // check if the magitude of the velocity
    if (Vector2Length(object->_velocity) > _maxSpeed)
    {
        object->_velocity = Vector2Scale(Vector2Normalize(object->_velocity), _maxSpeed);
    }

    object->_position.x += object->_velocity.x * deltaTime;
    object->_position.y += object->_velocity.y * deltaTime;

    _acceleration = { 0.0f, 0.0f };
}

void Agent::Draw()
{
    // draw the ball of the agent with the position values
    DrawCircleV(object->_position, _radius, _colour);
}

void Agent::AddBehaviour(IBehaviour* behaviour)
{
    _behaviours.push_back(behaviour);
}

