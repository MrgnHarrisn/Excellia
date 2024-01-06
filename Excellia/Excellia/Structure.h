#pragma once

#include "SFML/Graphics/Image.hpp"

#include "SFML/System/Vector2.hpp"

#include <string>

class Structure
{
public:

	/// <summary>
	/// Creates an empty structure
	/// </summary>
	Structure();

	/// <summary>
	/// Creates a structure
	/// </summary>
	/// <param name="file_name">File to read from</param>
	/// <param name="x">x coordinate of origin</param>
	/// <param name="y">y coordinate of origin</param>
	Structure(std::string file_name, int x, int y);

	/// <summary>
	/// Creates a structure
	/// </summary>
	/// <param name="file_name">File to read from</param>
	/// <param name="position">sf::Vector2i of origin</param>
	Structure(std::string file_name, sf::Vector2i position);

	/// <summary>
	/// Loads structure from a file
	/// </summary>
	/// <param name="file_name">File to read from</param>
	void Load_Image(std::string file_name);

	/// <summary>
	/// Set what pixel gets placed at the build location
	/// </summary>
	/// <param name="x">x coordinate</param>
	/// <param name="y">y coordinate</param>
	void Set_Origin(int x, int y);

	/// <summary>
	/// Set what pixel gets placed at the build location
	/// </summary>
	/// <param name="position">sf::Vector2i of position</param>
	void Set_Origin(sf::Vector2i position);

	/// <summary>
	/// Builds the structure
	/// </summary>
	/// <param name="world">World to place structure in</param>
	/// <param name="position">Position to place at (centred on origin)</param>
	void Build(sf::Image& world, sf::Vector2i position);

private:
	sf::Image m_image;
	sf::Vector2i m_origin{ 0, 0 };
};