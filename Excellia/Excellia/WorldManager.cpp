
#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow& window, sf::Vector2u size, long int seed) : m_window(window)
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
	m_view_image.create(265.0f / window.getSize().y * window.getSize().x, 265U, sf::Color(0, 0, 0, 0));
	m_view_sprite.setOrigin(0, 0);
	m_view_sprite.setScale(0.125, 0.125);
	m_view_texture.loadFromImage(m_view_image);

	// Create World
	create();
}

void WorldManager::create()
{
	sf::Clock clock;

	/* Create Blocks */
	i_void.create(8, 8, sf::Color(0, 0, 0, 0));
	i_dirt.loadFromFile("Textures/Dirt.png");
	i_diamond_ore.loadFromFile("Textures/Diamond_Ore.png");
	i_grass.loadFromFile("Textures/Grass.png");
	i_stone.loadFromFile("Textures/Stone.png");
	i_lava.loadFromFile("Textures/Lava.png");
	i_wood.loadFromFile("Textures/Wood.png");
	i_water.loadFromFile("Textures/Water.png");
	i_leaf.loadFromFile("Textures/Leaf.png");
	i_bedrock.loadFromFile("Textures/Bedrock.png");
	i_brick.loadFromFile("Textures/Brick.png");
	i_red_wood.loadFromFile("Textures/Red_Wood.png");
	i_hell_steel_ore.loadFromFile("Textures/Hell_Steel_Ore.png");
	i_crystal_ore.loadFromFile("Textures/Crystal_Ore.png");
	i_malachite_ore.loadFromFile("Textures/Malachite_Ore.png");
	i_ruby_ore.loadFromFile("Textures/Ruby_Ore.png");
	i_iron_ore.loadFromFile("Textures/Iron_Ore.png");
	i_copper_ore.loadFromFile("Textures/Copper_Ore.png");
	i_void_ore.loadFromFile("Textures/Void_Ore.png");
	i_sand.loadFromFile("Textures/Sand.png");


	/* Create Structures */
	s_tree.Load_Image("Structures/Tree.png");
	s_tree.Set_Origin(2, 8);
	s_tree2.Load_Image("Structures/Tree2.png");
	s_tree2.Set_Origin(7, 14);

	// Create Blank World
	m_image.create(m_width, m_height, BlockManager::hex_to_color(Block::Void));

	// Place Stone
	m_heights = TerrainGeneration::generate_heights(m_width, 0.005f, m_height, m_random);

	// Place Dirt
	m_dirt_heights = TerrainGeneration::generate_dirt(m_width, m_random);

	// Place Caves
	sf::Vector2i min = sf::Vector2i(0, (int)(find_highest_point() * 1.3));
	sf::Vector2i max = sf::Vector2i(m_width, m_height);
	m_caves = TerrainGeneration::generate_caves(min, max, m_random, m_heights);
	m_ores = TerrainGeneration::generate_ores(min, max, m_random);

	// Loop Over Columns
	for (unsigned int i = 0; i < m_heights.size(); i++) {

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
	for (unsigned int i = 0; i < m_caves.size(); i++) {

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

	for (size_t i = 0; i < m_ores.size(); i++) {

		sf::Vector2i position_a = m_caves[i];
		int cave_size = (m_caves[i].y > m_height * 0.75 ? 1000 : 400);

		/* Define the type of ore */
		Block block;
		
		std::vector<Block> blocks = ore_spawn_in_range(position_a);
		if (blocks.size() == 0) { continue;  }
		int index = (int)m_random.random(0.0f, (float)blocks.size() - 1.0f);
		block = blocks[index];
		// Draw Square
		for (int d_x = 0; d_x < 3; d_x++) {
			for (int d_y = 0; d_y < 3; d_y++) {
				force_place_block(block, position_a + sf::Vector2i(d_x, d_y));
			}
		}
	}

	/* Generates trees */
	m_trees = TerrainGeneration::generate_trees(m_width, m_random);
	int halfx = m_width / 2;

	// For Each Tree
	for (size_t i = 0; i < m_trees.size(); i++)
	{
		sf::Vector2i pos;
		pos.x = m_trees[i];
		pos.y = m_heights[m_trees[i]] - 1;

		// Randomise Tree Type
		if (m_image.getPixel(pos.x, pos.y+1) == BlockManager::hex_to_color((unsigned int)Block::Grass))
		{
			if (m_random.random(0, 5) >= 1) {
				s_tree.Build(m_image, pos);
			}
			else {
				s_tree2.Build(m_image, pos);
			}
		}
	}

	for (int i = 0; i < m_width; i++) {
		/* Draw bedrock */
		for (int j = m_height - 1; j > m_height - 1 - m_dirt_heights[i]; j--) {
			m_image.setPixel(i, j, BlockManager::hex_to_color(Block::Bedrock));
		}
	}
	
	printf("World Loaded in %.2fs", clock.restart().asSeconds());
}

int WorldManager::place_player(int x)
{
	return m_heights[x];
}

Block WorldManager::get_block(sf::Vector2i pos)
{
	if (pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height)
	{
		return BlockManager::color_to_block(m_image.getPixel(pos.x, pos.y));
	}
	else
	{
		return Block::Void;
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

std::vector<Block> WorldManager::ore_spawn_in_range(sf::Vector2i pos)
{

	std::vector<Block> ores;
	/*
	Crystal
	Malachite
	Ruby
	Diamond
	Iron
	Copper
	*/

	if (pos.y > m_height * 0.6 && pos.y < m_height * 0.8 && m_heights[pos.x] < pos.y) { ores.push_back(Block::Diamond_Ore); }
	if (pos.y > m_height * 0.56 && pos.y < m_height * 0.9 && m_heights[pos.x] < pos.y) { ores.push_back(Block::Ruby_Ore); }
	
	return ores;
}

void WorldManager::break_block(sf::Vector2i mouse_pos)
{
	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);

	// Check world bounds
	if (block.x >= 0u && block.x <= (unsigned int)m_width && block.y >= 0u && block.y < (unsigned int)m_height) {

		// Check theres block to break
		if (BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) != Block::Void && BlockManager::color_to_hex(m_image.getPixel(block.x, block.y)) != Block::Bedrock) {

			// Update Image
			m_image.setPixel(block.x, block.y, BlockManager::hex_to_color(Block::Void));
		}
	}
}

void WorldManager::place_block(Block material, sf::Vector2i mouse_pos, sf::Vector2f player_pos)
{

	sf::Vector2u block = (sf::Vector2u)screen_pos_to_world_pos(mouse_pos);

	if (std::abs((float)block.x - player_pos.x) < 1.0f && std::abs((float)block.y - player_pos.y + 1.995f) < 1.995f)
	{
		return;
	}

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

sf::Drawable &WorldManager::get_view_sprite()
{
	/* The size of the actual view is being changed but we aren't */

	// Find screen location
	sf::Vector2i view_size = (sf::Vector2i)m_window.getView().getSize();
	sf::Vector2i half_size(view_size.x / 2, view_size.y / 2);
	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 1);
	
	// Get pixels in view of texture
	int loop_max_x = (top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (top_left.y + half_size.y * 2) + 3;

	sf::Image* _block = &i_void;

	// Loop over image
	int i_x = 0;
	for (int x = top_left.x; x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = top_left.y; y < loop_max_y; y++) {
			Block block = get_block({ x, y });

			switch (block)
			{
			case Stone:
				_block = &i_stone;
				break;
			case Dirt:
				_block = &i_dirt;
				break;
			case Wood:
				_block = &i_wood;
				break;
			case Diamond_Ore:
				_block = &i_diamond_ore;
				break;
			case Grass:
				_block = &i_grass;
				break;
			case Water:
				_block = &i_water;
				break;
			case Lava:
				_block = &i_lava;
				break;
			case Leaf:
				_block = &i_leaf;
				break;
			case Brick:
				_block = &i_brick;
				break;
			case Red_Wood:
				_block = &i_red_wood;
				break;
			case Hell_Steel_Ore:
				_block = &i_hell_steel_ore;
				break;
			case Crystal_Ore:
				_block = &i_crystal_ore;
				break;
			case Malachite_Ore:
				_block = &i_malachite_ore;
				break;
			case Ruby_Ore:
				_block = &i_ruby_ore;
				break;
			case Iron_Ore:
				_block = &i_iron_ore;
				break;
			case Copper_Ore:
				_block = &i_copper_ore;
				break;
			case Bedrock:
				_block = &i_bedrock;
				break;
			case Void_Ore:
				_block = &i_void_ore;
				break;
			case Sand:
				_block = &i_sand;
				break;
			default:
				_block = &i_void;
				break;
			}

			m_view_image.copy(*_block, i_x * 8, i_y * 8);
			i_y++;
		}
		i_x++;
	}
	
	m_view_texture.update(m_view_image, 0, 0);
	m_view_sprite.setTexture(m_view_texture);
	m_view_sprite.setPosition((sf::Vector2f)(top_left));
	return m_view_sprite;
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