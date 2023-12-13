#include "WorldManager.h"

WorldManager::WorldManager()
{

}

void WorldManager::create()
{
	m_heights = TerrainGeneration::generate_heights(m_width, 0.001, m_height, m_random);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_heights, 800, m_random);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = 800, max.y = 800;
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);
}

void WorldManager::create(long int seed)
{

}