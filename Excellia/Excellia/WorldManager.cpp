#include "WorldManager.h"

WorldManager::WorldManager(Vector2u size, long int seed)
{
	if (size.x != 0 && size.y != 0)
	{
		m_width = size.x;
		m_height = size.y;
	}

	if (seed != -1)
	{
		Random r(seed);
		m_random = r;
	}

	m_image.create(m_width, m_height, sf::Color::Black);
}

void WorldManager::create()
{
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = m_width, max.y = m_height;
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);

	for (int i = 0; i < m_heights.size(); i++) {
		/* Do dirt for column */
		for (int j = m_heights[i]; j <= m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, Color(150, 75, 0));
		}

		for (int j = m_heights[i] + m_dirt_heights[i]; j < m_height; j++) {
			m_image.setPixel(i, j, Color(100, 100, 100));
		}
	}

	m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);
}

sf::Sprite WorldManager::get_render(RenderWindow& w)
{

	w.draw(m_sprite);

	return m_sprite;

}

int WorldManager::place_player(int x)
{
	return m_heights[x - 1]; // Height of ground + height of player
}

unsigned int WorldManager::get_block(sf::Vector2i pos)
{
	return BlockManager::color_to_hex(m_image.getPixel(pos.x, pos.y));
}

Vector2f  WorldManager::screen_pos_to_world_pos(RenderWindow& window, Vector2i mouse_pos)

{
	
	sf::Vector2f world_pos = window.mapPixelToCoords(mouse_pos, window.getView());

	Vector2f image_pos(world_pos.x, world_pos.y); /* Not really needed */

	return image_pos;

}

void WorldManager::break_block(RenderWindow& window, Vector2i mouse_pos)
{
	Vector2f block = screen_pos_to_world_pos(window, mouse_pos);

	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {
		m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Void));

		/* Update image */
		m_texture.loadFromImage(m_image);
		m_sprite.setTexture(m_texture);
	}
}