#include "WorldManager.h"

WorldManager::WorldManager(sf::RenderWindow& window, sf::Vector2u size, long int seed) : m_window(window)
{

	m_texture_manager.set_path("textures/");

	/* Load textures */
	m_texture_manager.load_texture("Dirt", "Dirt.png");
	m_texture_manager.load_texture("Diamond", "Diamond.png");
	m_texture_manager.load_texture("Grass", "Grass.png");
	m_texture_manager.load_texture("Stone", "Stone.png");
	m_texture_manager.load_texture("Lava", "Lava.png");
	m_texture_manager.load_texture("Wood", "Wood.png");
	m_texture_manager.load_texture("Water", "Water.png");

	m_texture_manager.display_items();

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
	sf::Vector2i min = sf::Vector2i(0, find_highest_point() * 1.3);
	sf::Vector2i max = sf::Vector2i(m_width, m_height);



	m_caves = TerrainGeneration::generate_caves(min, max, m_random);
	m_trees = TerrainGeneration::generate_trees(m_width, m_random);

	// Loop over columns
	for (int i = 0; i < m_heights.size(); i++) {

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
	for (int i = 0; i < m_caves.size(); i++) {
		sf::Vector2i position_a = (sf::Vector2i)m_caves[i];
		int cave_size = (m_caves[i].y > m_height * 0.75 ? 400 : 200);
		for (int j = 0; j < cave_size; j++) {
			
			for (int d_x = 0; d_x < 3; d_x++) {
				for (int d_y = 0; d_y < 3; d_y++) {
					force_place_block(Block::Void, position_a + sf::Vector2i(d_x, d_y));
				}
			}

			sf::Vector2i delta;
			delta.x = m_random.random(-1, 1) < 0 ? -1 : 1;
			delta.y = m_random.random(-1, 1) < 0 ? -1 : 1;

			position_a += delta;
		}
	}

	/* Generates trees */
	for (int i = 0; i < m_trees.size(); i++)
	{
		sf::Vector2i pos;
		pos.x = m_trees[i];
		pos.y = m_heights[m_trees[i]] - 1;
		force_place_block(Block::Wood, pos);

		// Temp tree
		for (int i = 0; i < 5; i++) {
			pos.y--;
			force_place_block(Block::Wood, pos);
		}
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 2, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 2, pos.y));
		pos.y--;
		force_place_block(Block::Grass, sf::Vector2i(pos.x, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 2, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 2, pos.y));
		pos.y--;
		force_place_block(Block::Grass, sf::Vector2i(pos.x, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 2, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 2, pos.y));
		pos.y--;
		force_place_block(Block::Grass, sf::Vector2i(pos.x, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x - 1, pos.y));
		force_place_block(Block::Grass, sf::Vector2i(pos.x + 1, pos.y));
	}

	// Store Map
	/*m_texture.loadFromImage(m_image);
	m_sprite.setTexture(m_texture);*/

}

sf::Sprite WorldManager::get_render()
{
	// return get_view_sprite();
}

int WorldManager::place_player(int x)
{
	return m_heights[x - 1] - 1; // -1 is to avoid collision problems
}

unsigned int WorldManager::get_block(sf::Vector2i pos)
{
	return BlockManager::color_to_hex(m_image.getPixel(pos.x, pos.y));
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
	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {

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
	if (block.x >= 0 && block.x <= m_width && block.y >= 0 && block.y < m_height) {

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

//sf::Sprite WorldManager::get_view_sprite() {
//
//
//	/*
//	The size of the actual view is being changed but we aren't
//	*/
//
//	// Find screen location
//	sf::Vector2f view_size = m_window.getView().getSize();
//	sf::Vector2i half_size(view_size.x / 2, view_size.y / 2);
//	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 1);
//
//	// Make image
//	sf::Image temp;
//	temp.create((unsigned int)m_window.getView().getSize().x + 7, (unsigned int)m_window.getView().getSize().y + 3);
//
//	// Get pixels in view of texture
//	int loop_max_x = (int)(top_left.x + half_size.x * 2) + 3;
//	int loop_max_y = (int)(top_left.y + half_size.y * 2) + 3;
//
//	// Loop over image
//	int i_x = 0;
//	// printf("Loop Max: %i, %i\n", loop_max_x - top_left.x, loop_max_y - top_left.y);
//	for (int x = (int)(top_left.x); x < loop_max_x; x++) {
//		int i_y = 0;
//		for (int y = (int)(top_left.y); y < loop_max_y; y++) {
//			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
//				temp.setPixel(i_x, i_y, m_image.getPixel(x, y));
//			}
//			else {
//				temp.setPixel(i_x, i_y, BlockManager::hex_to_color(Block::Void));
//			}
//			i_y++;
//		}
//		i_x++;
//	}
//
//	// Load and draw sprite
//	m_perspective_tex.loadFromImage(temp);
//	m_perspective_sprite.setTexture(m_perspective_tex);
//	m_perspective_sprite.setPosition((sf::Vector2f)(top_left));
//	// m_window.draw(sprite);
//
//	return m_perspective_sprite;
//}

void WorldManager::get_view_sprite() {


	/*
	The size of the actual view is being changed but we aren't
	*/

	// Find screen location
	sf::Vector2f view_size = m_window.getView().getSize();
	sf::Vector2i half_size(view_size.x / 2, view_size.y / 2);
	sf::Vector2i top_left = (sf::Vector2i)(m_window.getView().getCenter()) - half_size - sf::Vector2i(1, 1);
	
	// Make image
	/*sf::Image temp;
	temp.create((unsigned int)m_window.getView().getSize().x + 7, (unsigned int)m_window.getView().getSize().y + 3);*/
	
	// Get pixels in view of texture
	int loop_max_x = (int)(top_left.x + half_size.x * 2) + 3;
	int loop_max_y = (int)(top_left.y + half_size.y * 2) + 3;

	// Loop over image
	int i_x = 0;
	// printf("Loop Max: %i, %i\n", loop_max_x - top_left.x, loop_max_y - top_left.y);
	for (int x = (int)(top_left.x); x < loop_max_x; x++) {
		int i_y = 0;
		for (int y = (int)(top_left.y); y < loop_max_y; y++) {
			if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
				// temp.setPixel(i_x, i_y, m_image.getPixel(x, y));
				Block block = (Block)get_block({ x, y });
				if (block != Block::Void) {
					sf::Vector2f current_pos;
					current_pos.x = top_left.x + i_x;
					current_pos.y = top_left.y + i_y;
					sf::RectangleShape sprite;
					sprite.setSize({ 1, 1 });
					sprite.setTexture(m_texture_manager.get_by_type(block));
					sprite.setPosition(current_pos);
					m_window.draw(sprite);
				}
			}
			i_y++;
		}
		i_x++;
	}
	
	

	// return m_perspective_sprite;
}

int WorldManager::find_highest_point()
{
	int lowest = m_height;
	for (int i = 0; i < m_heights.size(); i++) {
		if (m_heights[i] < lowest) {
			lowest = m_heights[i];
		}
	}
	return lowest;
}