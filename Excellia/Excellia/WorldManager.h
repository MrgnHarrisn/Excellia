#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

#include "TerrainGeneration.h"
#include "BlockManager.h"
#include "Structure.h"

/// <summary>
/// Manages a world
/// </summary>
class WorldManager
{
public:

	/// <summary>
	/// Constructor for WorldManager
	/// </summary>
	WorldManager(sf::RenderWindow& window, sf::Vector2u size, BlockManager& blocks, long int seed = -1);

	/// <summary>
	/// Creates a world with a random seed
	/// </summary>
	void create();

	/// <summary>
	/// Calcualtes a valid y position for player at given x position
	/// </summary>
	/// <param name="x">x position for player</param>
	/// <returns>y position for player to spawn at</returns>
	int place_player(int x);

	/// <summary>
	/// Get's block at specific position
	/// </summary>
	/// <param name="block">The position we want to check</param>
	/// <returns>the integer value of the block</returns>
	Block& get_block(sf::Vector2i block);

	/// <summary>
	/// Converts a screen position to a world position
	/// </summary>
	/// <param name="window">Window in use</param>
	/// <param name="view">View in use</param>
	/// <param name="mouse_pos">Mouse position we want to convert</param>
	/// <returns>world position</returns>
	sf::Vector2f screen_pos_to_world_pos(sf::Vector2i mouse_pos);

	/// <summary>
	/// Replaces block under mouse with void
	/// </summary>
	/// <param name="window">Current renderwindow</param>
	/// <param name="mouse_pos">Position of mouse</param>
	void break_block(sf::Vector2i mouse_pos);

	/// <summary>
	/// Replaces void with a block
	/// </summary>
	/// <param name="mouse_pos">Position of mouse</param>
	/// <param name="material">Block type to place</param>
	/// <param name="player_pos">Position of player</param>
	void place_block(Block& material, sf::Vector2i mouse_pos, sf::Vector2f player_pos);

	/// <summary>
	/// Forces a block to be placed at position
	/// </summary>
	/// <param name="pos">Position on texture</param>
	/// <param name="material">Block type to place</param>
	void force_place_block(Block& material, sf::Vector2i pos);

	/// <summary>
	/// Creates a sprite of the current view
	/// </summary>
	/// <returns>Sprite of the current view</returns>
	sf::Drawable& get_view_sprite();

	int find_highest_point();

	sf::Vector2i game_pos_to_screen_pos(sf::Vector2f pos);

	std::vector<std::string> ore_spawn_in_range(sf::Vector2i pos);

private:

	Random m_random;
	std::vector<int> m_dirt_heights;	/* This is how much dirt should be on top */
	std::vector<int> m_heights;
	std::vector<int> m_trees;
	std::vector<sf::Vector2i> m_ores;
	std::vector<sf::Vector2i> m_caves;
	sf::Image m_image;
	sf::RenderWindow& m_window;

	int m_width = 800;
	int m_height = 800;

	sf::Texture m_view_texture;
	sf::Sprite m_view_sprite;
	sf::Image m_view_image;

	// Blocks
	BlockManager* m_blocks;

	// Premade Structures
	Structure s_tree;
	Structure s_tree2;
	Structure s_palette;
};