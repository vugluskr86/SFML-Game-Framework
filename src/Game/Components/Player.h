#pragma once

// #include <vector>

/*
struct PlayerInputConfig
{
	std::vector<unsigned int> topKeys;
	std::vector<unsigned int> bottomKeys;
	std::vector<unsigned int> leftKeys;
	std::vector<unsigned int> rigthsKeys;
	std::vector<unsigned int> actionKeys;
	std::vector<unsigned int> alternateActionKeys;
	std::vector<unsigned int> selectKeys;
};
*/

struct Player 
{ 
	// TODO: Check template specialization for empty struct (SFINAE)
	unsigned int health;
	float speed;
	// PlayerInputConfig inputConfig;
};
