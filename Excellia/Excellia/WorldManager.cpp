#include "WorldManager.h"

WorldManager::WorldManager()
{

}

void WorldManager::create()
{
	m_heights = TerrainGeneration::generate_heights(m_width, 0.001f, m_height, m_random);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_heights, 800, m_random);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = 800, max.y = 800;
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);
}

void WorldManager::create(long int seed)
{

}

sf::Sprite WorldManager::get_render()
{
	sf::Image image;
	image.create(m_width, m_height, sf::Color::White);

	for (int i = 0; i < m_height; i++) {
		int dirt_height = static_cast<int>(m_heights[i]);

		// Clip dirt_height to stay within the valid range of the image
		dirt_height = std::max(0, std::min(dirt_height, m_height - 6));
		int dirt_range = m_random.random(3, 5);

		// Set the dirt color pixel at (i, dirt_height)
		for (int j = dirt_height; j <= dirt_height + dirt_range; j++)
		{
			image.setPixel(i, j, Color(150, 75, 0));
		}

		// Set other pixels above dirt_height
		for (int j = 799; j > dirt_height + dirt_range; j--) {
			image.setPixel(i, j, sf::Color(100, 100, 100));
		}

		/*for (int i = 0; i < caves.size(); i++) {
			int x = static_cast<int>(caves[i].x);
			int y = static_cast<int>(caves[i].y);

			drawCircle(image, x, y, 10, sf::Color::White);
		}*/
	}

	sf::Texture texture;
	texture.loadFromImage(image);
	sf::Sprite sprite(texture);

	return sprite;

}