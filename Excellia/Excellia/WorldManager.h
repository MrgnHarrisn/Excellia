#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Color.hpp>

#include "TerrainGeneration.h"
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

	/// <summary>
	/// Creates a world with a random seed
	/// </summary>
	void create();

	/// <summary>
	/// Creates a world with a specific seed
	/// </summary>
	/// <param name="seed">Selected seed</param>
	void create(long int seed);

	sf::Sprite get_render(RenderWindow& w);

private:
	Random m_random;
	std::vector<int> m_dirt_heights;
	std::vector<int> m_heights;
	std::vector<sf::Vector2f> m_caves;
	int m_width = 800;
	int m_height = 800;
};