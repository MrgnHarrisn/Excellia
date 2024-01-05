#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "TerrainGeneration.h"
#include "TextureManager.h"
#include "BlockManager.h"
#include "Random.h"

/// <summary>
/// Manages a world
/// </summary>
class WorldManager
{
public:

	/// <summary>
	/// Constructor for WorldManager
	/// </summary>
	WorldManager(sf::RenderWindow& window, sf::Vector2u size = sf::Vector2u(0, 0), long int seed = -1);

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
	/// Returns the rendered image fo the world
	/// </summary>
	/// <returns></returns>
	sf::Sprite get_render();

	/// <summary>
	/// Get's block at specific position
	/// </summary>
	/// <param name="block">The position we want to check</param>
	/// <returns>the integer value of the block</returns>
	unsigned int get_block(sf::Vector2i block);

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
	void place_block(Block material, sf::Vector2i mouse_pos);

	/// <summary>
	/// Forces a block to be placed at position
	/// </summary>
	/// <param name="pos">Position on texture</param>
	/// <param name="material">Block type to place</param>
	void force_place_block(Block material, sf::Vector2i pos);

	/// <summary>
	/// Creates a sprite of the current view
	/// </summary>
	/// <returns>Sprite of the current view</returns>
	void get_view_sprite();

	int find_highest_point();

	sf::Vector2i game_pos_to_screen_pos(sf::Vector2f pos);

private:

	Random m_random;
	std::vector<int> m_dirt_heights;	/* This is how much dirt should be on top */
	std::vector<int> m_heights;
	std::vector<int> m_trees;
	std::vector<sf::Vector2i> m_caves;
	sf::Image m_image;
	/*sf::Texture m_texture;
	sf::Sprite m_sprite;*/
	sf::RenderWindow& m_window;

	sf::Texture m_perspective_tex;
	sf::Sprite m_perspective_sprite;
	TextureManager m_texture_manager;
	int m_width = 800;
	int m_height = 800;

	// Premade sprites
	sf::RectangleShape s_stone;
	sf::RectangleShape s_dirt;
	sf::RectangleShape s_wood;
	sf::RectangleShape s_diamond;
	sf::RectangleShape s_grass;
	sf::RectangleShape s_water;
	sf::RectangleShape s_lava;
	sf::RectangleShape s_void;
};