
#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow& window, sf::Vector2u size, BlockManager& blocks, long int seed) : m_window(window)
{

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

	// Setup View Image
	m_view_image.create((unsigned int)(265.0f / window.getSize().y * window.getSize().x), 265U, sf::Color(0, 0, 0, 0));
	m_view_sprite.setOrigin(0, 0);
	m_view_sprite.setScale(0.125, 0.125);
	m_view_texture.loadFromImage(m_view_image);

	// Save blocks
	m_blocks = &blocks;

	// Create World
	create();
}

void WorldManager::create()
{
	sf::Clock clock;

	/* Create Structures */
	s_tree.Load_Image("Structures/Tree.png");
	s_tree.Set_Origin(2, 8);
	s_tree2.Load_Image("Structures/Tree2.png");
	s_tree2.Set_Origin(7, 14);

	// Create Blank World
	m_image.create(m_width, m_height, m_blocks->get_by_name("Void").get_color());

	// Place Stone
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);

	// Place Dirt
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);

	// Place Caves
	sf::Vector2i min = sf::Vector2i(0, (int)(find_highest_point() * 1.3 + 15));
	sf::Vector2i max = sf::Vector2i(m_width, m_height);
	m_caves = TerrainGeneration::generate_caves(min, max, m_random, m_heights);
	m_ores = TerrainGeneration::generate_ores(min, max, m_random);

	// Temp color
	sf::Color _color;

	// Loop Over Columns
	for (unsigned int i = 0; i < m_heights.size(); i++) {

		// Draw Stone
		_color = m_blocks->get_by_name("Stone").get_color();
		for (int j = m_heights[i] + m_dirt_heights[i]; j < m_height; j++) {
			m_image.setPixel(i, j, _color);
		}

		// Draw Dirt
		_color = m_blocks->get_by_name("Dirt").get_color();
		for (int j = m_heights[i]; j < m_heights[i] + m_dirt_heights[i]; j++) {
			m_image.setPixel(i, j, _color);
		}

		// Draw Grass
		_color = m_blocks->get_by_name("Grass").get_color();
		m_image.setPixel(i, m_heights[i], _color);

	}

	for (size_t i = 0; i < m_ores.size(); i++) {

		sf::Vector2i position_a = m_ores[i];
		
		std::vector<std::string> blocks = ore_spawn_in_range(position_a);
		if (blocks.size() == 0) { continue;  }
		int index = (int)m_random.random(0.0f, (float)blocks.size() - 0.01f);
		Block& block = m_blocks->get_by_name(blocks[index]);
		// Draw Square

		for (int i = 0; i < 5; i++)
		{
			for (int d_x = 0; d_x < 2; d_x++) {
				for (int d_y = 0; d_y < 2; d_y++) {
					force_place_block(block, position_a + sf::Vector2i(d_x, d_y));
				}
			}

			// Walk
			sf::Vector2i delta;
			delta.x = m_random.random(-1, 1) < 0 ? -1 : 1;
			delta.y = m_random.random(-1, 1) < 0 ? -1 : 1;
			position_a += delta;
		}
	}


	/* Generate caves with random walking */
	for (unsigned int i = 0; i < m_caves.size(); i++) {

		sf::Vector2i position_a = m_caves[i];
		int cave_size = (m_caves[i].y > m_height * 0.75 ? 1000 : 400);
		Block& _cave = m_blocks->get_by_name("Cave");

		// Loop Over Caves
		for (int j = 0; j < cave_size; j++) {

			// Draw Square
			for (int d_x = 0; d_x < 3; d_x++) {
				for (int d_y = 0; d_y < 3; d_y++) {
					if (position_a.y + d_y >= m_heights[(unsigned int)std::min(std::max((float)(position_a.x + d_x), 0.0f), (float)(m_width - 1))]) {
						force_place_block(_cave, position_a + sf::Vector2i(d_x, d_y));
					}
				}
			}

			// Walk
			sf::Vector2i delta;
			delta.x = m_random.random(-1, 1) < 0 ? -1 : 1;
			delta.y = m_random.random(-1, 1) < 0 ? -1 : 1;
			position_a += delta;
		}
	}


	/* Generate Water */
	std::vector<int> lakes;

	int lake_pos = 0;

	// Randomise position
	while (lake_pos < m_width - 15)
	{
		if (std::abs(lake_pos - m_width / 2) < 60)
		{
			lake_pos += (int)m_random.random(65, 100);
		}
		lakes.push_back(lake_pos);

		// Move along
		lake_pos += (int)m_random.random(65, 100);
	}

	// Walk water
	for (int i = 0; i < lakes.size(); i++)
	{
		sf::Vector2i pos;
		pos.x = lakes[i];
		pos.y = m_heights[lakes[i]] - 3;
		int counter = 1;

		while (pos.x > 0 && m_image.getPixel(pos.x - 1, pos.y) == m_blocks->get_by_name("Void").get_color())
		{
			pos.x--;
		}
		while (m_image.getPixel(pos.x + counter, pos.y) == m_blocks->get_by_name("Void").get_color())
		{
			counter++;
		}
		if (counter < 60 && counter > 10)
		{
			for (int i = -1; i < counter; i++)
			{
				int j = pos.y + 1;
				while (m_image.getPixel(pos.x + i, j) == m_blocks->get_by_name("Void").get_color())
				{
					m_image.setPixel(pos.x + i, j, m_blocks->get_by_name("Water").get_color());
					j++;
				}
				for (int k = 0; k < m_dirt_heights[pos.x + i] / 2; k++)
				{
					m_image.setPixel(pos.x + i, j + k, m_blocks->get_by_name("Sand").get_color());
				}
			}
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
		if (m_image.getPixel(pos.x, pos.y+1) == m_blocks->get_by_name("Grass").get_color() &&
			m_image.getPixel(pos.x, pos.y) != m_blocks->get_by_name("Water").get_color())
		{
			if (m_random.random(0, 5) >= 1) {
				s_tree.Build(m_image, pos);
			}
			else {
				s_tree2.Build(m_image, pos);
			}
			m_image.setPixel(pos.x, pos.y+1, m_blocks->get_by_name("Dirt").get_color());
		}
	}

	for (int i = 0; i < m_width; i++) {
		/* Draw bedrock */
		for (int j = m_height - 1; j > m_height - 1 - m_dirt_heights[i]; j--) {
			m_image.setPixel(i, j, m_blocks->get_by_name("Bedrock").get_color());
		}
		/* Draw Lava*/
		for (int j = m_height - 1; j > m_height - 13; j--) {
			place_block(m_blocks->get_by_name("Lava"), sf::Vector2i(i, j));
		}
	}
	
	printf("World Loaded in %.2fs", clock.restart().asSeconds());
}

int WorldManager::place_player(int x)
{
	return m_heights[x];
}

Block& WorldManager::get_block(sf::Vector2i pos)
{
	if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height)
	{
		return m_blocks->get_by_color(m_image.getPixel(pos.x, pos.y));
	}
	else
	{
		return m_blocks->get_by_name("Void");
	}
}

sf::Vector2f WorldManager::screen_pos_to_world_pos(sf::Vector2i mouse_pos)
{
	return m_window.mapPixelToCoords(mouse_pos, m_window.getView());
}

sf::Vector2i WorldManager::game_pos_to_screen_pos(sf::Vector2f mouse_pos)
{
	return m_window.mapCoordsToPixel(mouse_pos, m_window.getView());
}

std::vector<std::string> WorldManager::ore_spawn_in_range(sf::Vector2i pos)
{

	std::vector<std::string> ores;
	if (pos.y > m_height * 0.20 && pos.y < m_height * 0.4 && m_heights[pos.x] < pos.y) { ores.push_back("Copper_Ore"); }
	if (pos.y > m_height * 0.40 && pos.y < m_height * 0.7 && m_heights[pos.x] < pos.y) { ores.push_back("Iron_Ore"); }
	if (pos.y > m_height * 0.50 && pos.y < m_height * 0.7 && m_heights[pos.x] < pos.y) { ores.push_back("Crystal_Ore"); }
	if (pos.y > m_height * 0.60 && pos.y < m_height * 0.8 && m_heights[pos.x] < pos.y) { ores.push_back("Malachite_Ore"); }
	if (pos.y > m_height * 0.60 && pos.y < m_height * 0.8 && m_heights[pos.x] < pos.y) { ores.push_back("Diamond_Ore"); }
	if (pos.y > m_height * 0.70 && pos.y < m_height * 0.9 && m_heights[pos.x] < pos.y) { ores.push_back("Ruby_Ore"); }
	if (pos.y > m_height * 0.95 && pos.y < m_height * 1.0 && m_heights[pos.x] < pos.y) { ores.push_back("Void_Ore"); }
	
	return ores;
}

void WorldManager::break_block(sf::Vector2i mouse_pos)
{
	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0u && block.x <= (unsigned int)m_width && block.y >= 0u && block.y < (unsigned int)m_height) {

		// Check theres block to break
		if (m_blocks->get_by_color(m_image.getPixel(block.x, block.y)).get_toughness() != -1) {
			// Update Image
			m_image.setPixel(block.x, block.y, m_blocks->get_by_name("Void").get_color());
		}
	}
}

void WorldManager::place_block(Block& material, sf::Vector2i mouse_pos, sf::Vector2f player_pos, sf::Vector2f player_size)
{

	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);
	float y_val = (player_size.y + 1) / 2 - 0.005;
	float x_val = (player_size.x + 1) / 2;

	// Check player bounds
	if (std::abs((float)block.x - player_pos.x - x_val + 1) < x_val && std::abs((float)block.y - player_pos.y + y_val) < y_val)
	{
		if (material.get_is_solid())
		{
			return;
		}
	}

	// Check world bounds
	if (block.x >= 0 && block.x <= (unsigned int)m_width && block.y >= 0 && block.y < (unsigned int)m_height) {

		// Check there is nothing there
		if (!m_blocks->get_by_color(m_image.getPixel(block.x, block.y)).get_is_solid() && material.get_name() != "null") {

			// Update Image
			m_image.setPixel(block.x, block.y, material.get_color());
		}
	}
}

void WorldManager::force_place_block(Block& material, sf::Vector2i pos)
{
	// Check world bounds
	if (pos.x >= 0 && pos.x <= m_width && pos.y >= 0 && pos.y < m_height) {

		m_image.setPixel(pos.x, pos.y, material.get_color());
	}
}

void WorldManager::update_view()
{
	/* The size of the actual view is being changed but we aren't */

	// Find screen location
	sf::Vector2i view_size = (sf::Vector2i)m_window.getView().getSize();
	sf::Vector2i half_size(view_size.x / 2, view_size.y / 2);
	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 1);
	
	// Get pixels in view of texture
	int loop_max_x = (top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (top_left.y + half_size.y * 2) + 3;

	// Loop over image
	int i_x = 0;
	for (int x = top_left.x; x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = top_left.y; y < loop_max_y; y++) {
			m_view_image.copy(get_block(sf::Vector2i(x, y)).get_image(), i_x * 8, i_y * 8);
			i_y++;
		}
		i_x++;
	}
	
	m_view_texture.update(m_view_image, 0, 0);
	m_view_sprite.setTexture(m_view_texture);
	m_view_sprite.setPosition((sf::Vector2f)(top_left));
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

sf::RenderWindow& WorldManager::get_window()
{
	return m_window;
}

sf::Vector2u WorldManager::get_size()
{
	return m_image.getSize();
}