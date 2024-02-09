#pragma once

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Color.hpp"

class Block
{
public:
	Block();
	Block(std::string name, unsigned int id, float toughness, bool is_solid = true);
	std::string get_name();
	bool get_is_solid();
	unsigned int get_id();
	float get_toughness();
	// Add tool
	sf::Image& get_image();
	sf::Color get_color();
private:
	std::string m_name;
	bool m_is_solid;
	unsigned int m_id;
	float m_toughness;
	// Add tool
	sf::Image m_image;
	sf::Color m_color;
};