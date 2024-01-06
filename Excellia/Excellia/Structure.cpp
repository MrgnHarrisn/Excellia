
#include <string>

#include "Structure.h"

/// <summary>
/// Creates an empty structure
/// </summary>
Structure::Structure() {}

/// <summary>
/// Creates a structure
/// </summary>
/// <param name="file_name">File to read from</param>
/// <param name="x">x coordinate of origin</param>
/// <param name="y">y coordinate of origin</param>
Structure::Structure(std::string file_name, int x, int y)
{
	Load_Image(file_name);
	Set_Origin(x, y);
}

/// <summary>
/// Creates a structure
/// </summary>
/// <param name="file_name">File to read from</param>
/// <param name="position">sf::Vector2i of origin</param>
Structure::Structure(std::string file_name, sf::Vector2i position)
{
	Load_Image(file_name);
	Set_Origin(position);
}

/// <summary>
/// Loads structure from a file
/// </summary>
/// <param name="file_name">File to read from</param>
void Structure::Load_Image(std::string file_name)
{
	m_image.loadFromFile(file_name);
}

/// <summary>
/// Set what pixel gets placed at the build location
/// </summary>
/// <param name="x">x coordinate</param>
/// <param name="y">y coordinate</param>
void Structure::Set_Origin(int x, int y)
{
	m_origin.x = x;
	m_origin.y = y;
}

/// <summary>
/// Set what pixel gets placed at the build location
/// </summary>
/// <param name="position">sf::Vector2i of position</param>
void Structure::Set_Origin(sf::Vector2i position)
{
	m_origin = position;
}

/// <summary>
/// Builds the structure
/// </summary>
/// <param name="world">World to place structure in</param>
/// <param name="position">Position to place at (centred on origin)</param>
void Structure::Build(sf::Image& world, sf::Vector2i position)
{
	position -= m_origin;

	for (size_t x = 0; x < m_image.getSize().x; x++)
	{
		for (size_t y = 0; y < m_image.getSize().y; y++)
		{
			if (m_image.getPixel(x, y) != sf::Color(0U, 0U, 0U, 0U))
			{
				world.setPixel(position.x, position.y, m_image.getPixel(x, y));
			}
			position.y++;
		}
		position.y -= m_image.getSize().y;
		position.x++;
	}
}