#pragma once

#include <glm/glm/vec2.hpp>

struct Mob
{
    Mob()
        : _died(false)
        , health(1)
        , speed(1)
        , target(0,0)
    {
    
	}

	int health;
	float speed;
	glm::vec2 target;

	bool _died; // TODO: To compose ?
};
