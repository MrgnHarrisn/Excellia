#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

#include "TerrainGeneration.h"
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
	WorldManager();

	WorldManager(Vector2u size);

	/// <summary>
	/// Creates a world with a random seed
	/// </summary>
	void create();

	/// <summary>
	/// Creates a world with a specific seed
	/// </summary>
	/// <param name="seed">Selected seed</param>
	void create(long int seed);

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

	unsigned int get_block(sf::Vector2i);

private:
	Random m_random;
	std::vector<int> m_dirt_heights;	/* This is howm uch dirt should be on top */
	std::vector<int> m_heights;
	std::vector<sf::Vector2f> m_caves;
	sf::Image m_image;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	int m_width = 800;
	int m_height = 800;
};