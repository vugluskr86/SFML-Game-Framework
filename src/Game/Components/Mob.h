#pragma once

#include <glm/glm/vec2.hpp>

struct Mob
{
    Mob()
        : _died(false)
    {
    
	}

	int health;
	float speed;
	glm::vec2 target;

	bool _died; // TODO: To compose ?
};
