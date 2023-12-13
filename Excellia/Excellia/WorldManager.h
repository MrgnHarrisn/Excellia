#pragma once
#include "TerrainGeneration.h"

class WorldManager
{
public:
	WorldManager();
private:
	unsigned int seed;
	std::vector<int> dirt_heights;
	std::vector<int> heights;
};