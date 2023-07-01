#include "flee.h"
#include "agent.h"
#include "raymath.h"


FleeBehaviour::FleeBehaviour(Agent* target) :
    _target(target)
{

}

FleeBehaviour::~FleeBehaviour()
{

}

void FleeBehaviour::Update(Agent* agent, float deltaTime)
{
    float distance = Vector2Distance(_target->GetPosition(), agent->GetPosition());

    
    if (distance < 125.0f)
    {
        Vector2 direction = Vector2Subtract(agent->GetPosition(), _target->GetPosition());
        agent->_acceleration = Vector2Scale(Vector2Normalize(direction), (agent->_maxSpeed * deltaTime));
    }
    else
    {
        
        agent->_acceleration.x -= (agent->object)->_velocity.x * deltaTime;
        agent->_acceleration.y -= (agent->object)->_velocity.y * deltaTime;
    }
}