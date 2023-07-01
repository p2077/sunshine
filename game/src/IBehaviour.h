#pragma once

#include "raylib.h"

class Agent;

class IBehaviour {
public:
    
    IBehaviour() {}
    ~IBehaviour() {}

   
    virtual void Update(Agent* agent, float deltaTime) = 0;

};