#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow& window, sf::Vector2u size, long int seed) : m_window(window)
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

	m_image.create(m_width, m_height, BlockManager::hex_to_color(Block::Void));
}

void WorldManager::create()
{
	// Places stone
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);

	// Places dirt
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);

	// Places Caves
	sf::Vector2f min = sf::Vector2f(0, 0);
	sf::Vector2f max = sf::Vector2f(m_width, m_height);
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);

	// Loop over columns
	for (int i = 0; i < m_heights.size(); i++) {

		// Draw stone
		for (int j = m_heights[i] + m_dirt_heights[i]; j < m_height; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Stone));
		}

		// Draw dirt
		for (int j = m_heights[i]; j <= m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Dirt));
		}

	}

	// Store Map
	m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);

}

sf::Sprite WorldManager::get_render()
{
	get_view_sprite();
	return m_sprite;
}

int WorldManager::place_player(int x)
{
	return m_heights[x - 1]; // -1 is to avoid collision problems
}

unsigned int WorldManager::get_block(sf::Vector2i pos)
{
	return BlockManager::color_to_hex(m_image.getPixel(pos.x, pos.y));
}

sf::Vector2f WorldManager::screen_pos_to_world_pos(sf::Vector2i mouse_pos)
{
	return m_window.mapPixelToCoords(mouse_pos, m_window.getView());
}

void WorldManager::break_block(sf::Vector2i mouse_pos)
{
	sf::Vector2f block = screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {

		// Check theres block to break
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) != Block::Void) {

			// Update Image
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Void));
			m_texture.loadFromImage(m_image);
			m_sprite.setTexture(m_texture);
		}
	}
}

void WorldManager::place_block(Block material, sf::Vector2i mouse_pos)
{
	sf::Vector2f block = screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {

		// Check there is nothing there
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) == Block::Void) {

			// Update Image
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(material));
			m_texture.loadFromImage(m_image);
			m_sprite.setTexture(m_texture);
		}
	}
}

sf::Sprite WorldManager::get_view_sprite() {

	// Find screen location
	sf::Vector2i half_size(m_window.getView().getSize().x / 2, m_window.getView().getSize().y / 2);
	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 0);

	// Make image
	sf::Image temp;
	temp.create(m_window.getView().getSize().x + 7, m_window.getView().getSize().y + 3);
	
	// Get pixels in view of texture
	int loop_max_x = (int)(top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (int)(top_left.y + half_size.y * 2) + 3;

	// Loop over image
	int i_x = 0;
	for (int x = (int)(top_left.x); x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = (int)(top_left.y); y < loop_max_y; y++) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				temp.setPixel(i_x, i_y, m_image.getPixel(x, y));
			}
			else {
				temp.setPixel(i_x, i_y, BlockManager::hex_to_color(Block::Void));
			}
			i_y++;
		}
		i_x++;
	}
	
	// Load and draw sprite
	sf::Sprite sprite;
	sf::Texture texture;
	texture.loadFromImage(temp);
	sprite.setTexture(texture);
	sprite.setPosition((sf::Vector2f)top_left);
	m_window.draw(sprite);

	return sprite;
}