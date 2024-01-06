
#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow& window, sf::Vector2u size, long int seed) : m_window(window)
{

	/* Create Blocks */
	i_dirt.loadFromFile("Textures/Dirt.png");
	i_diamond.loadFromFile("Textures/Diamond.png");
	i_grass.loadFromFile("Textures/Grass.png");
	i_stone.loadFromFile("Textures/Stone.png");
	i_lava.loadFromFile("Textures/Lava.png");
	i_wood.loadFromFile("Textures/Wood.png");
	i_water.loadFromFile("Textures/Water.png");
	i_leaf.loadFromFile("Textures/Leaf.png");

	/* Create Structures */
	s_tree.Load_Image("Structures/Tree.png");
	s_tree.Set_Origin(2, 8);
	s_tree2.Load_Image("Structures/Tree2.png");
	s_tree2.Set_Origin(7, 14);

	// Set World Size
	if (size.x != 0 && size.y != 0)
	{
		m_width = size.x;
		m_height = size.y;
	}

	// Set World Seed
	if (seed != -1)
	{
		Random r(seed);
		m_random = r;
	}

	// Create Blank World
	m_image.create(m_width, m_height, BlockManager::hex_to_color(Block::Void));
}

void WorldManager::create()
{
	// Place Stone
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);

	// Place Dirt
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);

	// Place Caves
	sf::Vector2i min = sf::Vector2i(0, (int)(find_highest_point() * 1.3));
	sf::Vector2i max = sf::Vector2i(m_width, m_height);
	m_caves = TerrainGeneration::generate_caves(min, max, m_random);

	// Loop Over Columns
	for (size_t i = 0; i < m_heights.size(); i++) {

		// Draw Stone
		for (int j = m_heights[i] + m_dirt_heights[i]; j < m_height; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Stone));
		}

		// Draw Dirt
		for (int j = m_heights[i]; j < m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Dirt));
		}

		// Draw Grass
		m_image.setPixel(i, m_heights[i], BlockManager::hex_to_color(Block::Grass));

	}

	/* Generate caves with random walking */
	for (size_t i = 0; i < m_caves.size(); i++) {

		sf::Vector2i position_a = m_caves[i];
		int cave_size = (m_caves[i].y > m_height * 0.75 ? 1000 : 400);

		// Loop Over Caves
		for (int j = 0; j < cave_size; j++) {

			// Draw Square
			for (int d_x = 0; d_x < 3; d_x++) {
				for (int d_y = 0; d_y < 3; d_y++) {
					force_place_block(Block::Void, position_a + sf::Vector2i(d_x, d_y));
				}
			}

			// Walk
			sf::Vector2i delta;
			delta.x = m_random.random(-1, 1) < 0 ? -1 : 1;
			delta.y = m_random.random(-1, 1) < 0 ? -1 : 1;
			position_a += delta;
		}
	}

	/* Generates trees */
	m_trees = TerrainGeneration::generate_trees(m_width, m_random);

	// For Each Tree
	for (size_t i = 0; i < m_trees.size(); i++)
	{
		sf::Vector2i pos;
		pos.x = m_trees[i];
		pos.y = m_heights[m_trees[i]] - 1;

		// Randomise Tree Type
		if (m_random.random(0, 5) >= 1) {
			s_tree.Build(m_image, pos);
		} else {
			s_tree2.Build(m_image, pos);
		}
	}
}

int WorldManager::place_player(int x)
{
	return m_heights[x - 1] - 1; // -1 is to avoid collision problems
}

Block WorldManager::get_block(sf::Vector2i pos)
{
	return BlockManager::color_to_block(m_image.getPixel(pos.x, pos.y));
}

sf::Vector2f WorldManager::screen_pos_to_world_pos(sf::Vector2i mouse_pos)
{
	return m_window.mapPixelToCoords(mouse_pos, m_window.getView());
}

sf::Vector2i WorldManager::game_pos_to_screen_pos(sf::Vector2f mouse_pos)
{
	return m_window.mapCoordsToPixel(mouse_pos, m_window.getView());
}

void WorldManager::break_block(sf::Vector2i mouse_pos)
{
	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0u && block.x <= (unsigned int)m_width && block.y >= 0u && block.y < (unsigned int)m_height) {

		// Check theres block to break
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) != Block::Void) {

			// Update Image
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Void));
		}
	}
}

void WorldManager::place_block(Block material, sf::Vector2i mouse_pos)
{
	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0 && block.x <= (unsigned int)m_width && block.y >= 0 && block.y < (unsigned int)m_height) {

		// Check there is nothing there
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) == Block::Void) {

			// Update Image
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(material));
		}
	}
}

void WorldManager::force_place_block(Block material, sf::Vector2i pos)
{
	// Check world bounds
	if (pos.x >= 0 && pos.x <= m_width && pos.y >= 0 && pos.y < m_height) {

		m_image.setPixel(pos.x, pos.y, BlockManager::hex_to_color(material));
	}
}

void WorldManager::get_view_sprite()
{
	/* The size of the actual view is being changed but we aren't */

	// Find screen location
	sf::Vector2i view_size = (sf::Vector2i)m_window.getView().getSize();
	sf::Vector2i half_size(view_size.x / 2, view_size.y / 2);
	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 1);
	
	// Get pixels in view of texture
	int loop_max_x = (top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (top_left.y + half_size.y * 2) + 3;
	sf::Image* image = nullptr;

	sf::Image _image;
	sf::Sprite _sprite;
	_image.create((loop_max_x - top_left.x) * 8, (loop_max_y - top_left.y) * 8, sf::Color(0,0,0,0));
	
	

	// Loop over image
	int i_x = 0;
	for (int x = top_left.x; x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = top_left.y; y < loop_max_y; y++) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				Block block = get_block({ x, y });
				if (block != Block::Void) {

					switch (block)
					{
					case Stone:
						image = &i_stone;
						break;
					case Dirt:
						image = &i_dirt;
						break;
					case Wood:
						image = &i_wood;
						break;
					case Diamond:
						image = &i_diamond;
						break;
					case Grass:
						image = &i_grass;
						break;
					case Water:
						image = &i_water;
						break;
					case Lava:
						image = &i_lava;
						break;
					case Leaf:
						image = &i_leaf;
						break;
					default:
						break;
					}

					for (int w = 0; w < 8; w++)
					{
						for (int z = 0; z < 8; z++)
						{
							_image.setPixel(i_x * 8 + w, i_y * 8 + z, image->getPixel(w,z));
						}
					}
				}
			}
			i_y++;
		}
		i_x++;
	}
	m_view_texture.loadFromImage(_image);

	_sprite.setTexture(m_view_texture);
	_sprite.setPosition((sf::Vector2f)(top_left));
	_sprite.setOrigin(0, 0);
	_sprite.setScale(0.125, 0.125);
	m_window.draw(_sprite);
}

int WorldManager::find_highest_point()
{
	int lowest = m_height;
	for (size_t i = 0; i < m_heights.size(); i++) {
		if (m_heights[i] < lowest) {
			lowest = m_heights[i];
		}
	}
	return lowest;
}