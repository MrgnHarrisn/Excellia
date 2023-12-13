#include "WorldManager.h"

WorldManager::WorldManager()
{
	m_image.create(m_width, m_height, sf::Color::Black);
}

void WorldManager::create()
{
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_heights, 800, m_random);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = 800, max.y = 800;
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);

	for (int i = 0; i < m_heights.size(); i++) {
		/* Do dirt for column */
		for (int j = m_heights[i]; j <= m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, Color(150, 75, 0));
		}
		for (int j = 799; j > m_heights[i] + m_dirt_heights[i]; j--) {
			m_image.setPixel(i, j, sf::Color(100, 100, 100));
		}
	}

}

void WorldManager::create(long int seed)
{
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, seed);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_heights, 800, seed);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = 800, max.y = 800;
	m_caves = TerrainGeneration::generate_caves(min, max, seed);

	for (int i = 0; i < m_heights.size(); i++) {
		/* Do dirt for column */
		for (int j = m_heights[i]; j <= m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, Color(150, 75, 0));
		}
		for (int j = 799; j > m_heights[i] + m_dirt_heights[i]; j--) {
			m_image.setPixel(i, j, sf::Color(100, 100, 100));
		}
	}

}

sf::Sprite WorldManager::get_render(RenderWindow& w)
{

	

	sf::Texture texture;
	texture.loadFromImage(m_image);
	sf::Sprite sprite(texture);

	w.draw(sprite);

	return sprite;

}

int WorldManager::place_player(int x)
{
	return m_heights[x] - m_dirt_heights[x];
}