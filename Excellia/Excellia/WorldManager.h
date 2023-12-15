#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

#include <SFML/System/Vector2.hpp>

#include "TerrainGeneration.h"
#include "Collision.hpp"
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
	WorldManager(RenderWindow& window, Vector2u size = Vector2u(0, 0), long int seed = -1);

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
	/// <param name="w">RenderWindow to render to</param>
	/// <returns></returns>
	sf::Sprite get_render(RenderWindow& w);

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
	Vector2f screen_pos_to_world_pos(Vector2i mouse_pos);

	void break_block(RenderWindow& window, Vector2i mouse_pos);

	void place_block(Vector2i mouse_pos);

	sf::Sprite get_view_sprite();

private:
	Random m_random;
	std::vector<int> m_dirt_heights;	/* This is how much dirt should be on top */
	std::vector<int> m_heights;
	std::vector<sf::Vector2f> m_caves;
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	RenderWindow& m_window;
	int m_width = 800;
	int m_height = 800;
};