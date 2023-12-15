#include "WorldManager.h"

WorldManager::WorldManager(RenderWindow& window, Vector2u size, long int seed) : m_window(window)
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
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);
	sf::Vector2f min, max;
	min.x = 0, min.y = 0;
	max.x = m_width, max.y = m_height;
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);

	for (int i = 0; i < m_heights.size(); i++) {
		/* Do dirt for column */
		for (int j = m_heights[i]; j <= m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Dirt));
		}

		for (int j = m_heights[i] + m_dirt_heights[i]; j < m_height; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Stone));
		}
	}

	m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);
}

sf::Sprite WorldManager::get_render(RenderWindow& w)
{

	get_view_sprite();

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

Vector2f  WorldManager::screen_pos_to_world_pos(Vector2i mouse_pos)

{
	
	sf::Vector2f world_pos = m_window.mapPixelToCoords(mouse_pos, m_window.getView());

	Vector2f image_pos(world_pos.x, world_pos.y); /* Not really needed */

	return image_pos;

}

void WorldManager::break_block(RenderWindow& window, Vector2i mouse_pos)
{
	Vector2f block = screen_pos_to_world_pos(mouse_pos);

	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {
		m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Void));

		/* Update image */
		m_texture.loadFromImage(m_image);
		m_sprite.setTexture(m_texture);
	}
}

void WorldManager::place_block(Vector2i mouse_pos)
{
	Vector2f block = screen_pos_to_world_pos(mouse_pos);

	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) == Block::Void) {
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Wood));

			/* Update image */
			m_texture.loadFromImage(m_image);
			m_sprite.setTexture(m_texture);
		}
	}
}

sf::Sprite WorldManager::get_view_sprite() {
	Vector2i half_size(m_window.getView().getSize().x / 2, m_window.getView().getSize().y / 2);
	Vector2i top_left = (Vector2i)(m_window.getView().getCenter()) - half_size - Vector2i(1, 0);

	sf::Image temp;
	temp.create(m_window.getView().getSize().x + 7, m_window.getView().getSize().y + 3);
	
	/* Get pixels in view of texture */
	int i_x = 0;

	int loop_max_x = (int)(top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (int)(top_left.y + half_size.y * 2) + 3;

	for (int x = (int)(top_left.x); x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = (int)(top_left.y); y < loop_max_y; y++) {
			// printf("X: %i Y: %i\n", i_x, i_y);
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

	sf::Texture tex;
	tex.loadFromImage(temp);
	sf::Sprite sprite;
	sprite.setTexture(tex);
	sprite.setPosition((Vector2f)top_left);
	m_window.draw(sprite);
	return sprite;
}