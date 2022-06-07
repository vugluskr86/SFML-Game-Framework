#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm/vec2.hpp>

struct Mob
{
	// TODO: Check template specialization for empty struct (SFINAE)
	int health;
	float speed;
	glm::vec2 target;
};
