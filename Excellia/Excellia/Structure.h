#pragma once

#include "SFML/Graphics/Image.hpp"
#include "SFML/System/Vector2.hpp"

class Structure
{
public:
	Structure();
	Structure(std::string file_name, int x, int y);
	Structure(std::string file_name, sf::Vector2i position);
	void Load_Image(std::string file_name);
	void Set_Origin(int x, int y);
	void Set_Origin(sf::Vector2i position);
	void Build(sf::Image& world, sf::Vector2i position);
private:
	sf::Image m_image;
	sf::Vector2i m_origin{ 0, 0 };
};