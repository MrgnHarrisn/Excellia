#pragma once

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Color.hpp"

#include <unordered_map>

class Block
{
public:
	Block();
	Block(std::string name, unsigned int id);
	unsigned int get_id();
	sf::Image& get_image();
	sf::Color get_color();
private:
	unsigned int m_id;
	sf::Image m_image;
	sf::Color m_color;
};