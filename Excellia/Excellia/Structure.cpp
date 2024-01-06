
#include "Structure.h"

Structure::Structure() {}

Structure::Structure(std::string file_name, int x, int y)
{
	Load_Image(file_name);
	Set_Origin(x, y);
}

Structure::Structure(std::string file_name, sf::Vector2i position)
{
	Load_Image(file_name);
	Set_Origin(position);
}

void Structure::Load_Image(std::string file_name)
{
	m_image.loadFromFile(file_name);
}

void Structure::Set_Origin(int x, int y)
{
	m_origin.x = x;
	m_origin.y = y;
}

void Structure::Set_Origin(sf::Vector2i position)
{
	m_origin = position;
}

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