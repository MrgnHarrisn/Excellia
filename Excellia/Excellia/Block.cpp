
#include "Block.h"

Block::Block()
{
	m_name = "null";
	m_id = 0x00000000;
	m_toughness = 0;
	m_is_solid = 0;
	m_color = sf::Color::Black;
	m_image.create(8, 8, m_color);
}

Block::Block(std::string name, unsigned int id, float toughness, bool is_solid)
{
	m_name = name;
	m_id = id;
	m_toughness = toughness;
	m_is_solid = is_solid;
	m_image.loadFromFile("Textures/" + m_name + ".png");
	m_color = sf::Color(m_id >> 24, m_id >> 16 & 0xFF, m_id >> 8 & 0xFF, m_id & 0xFF);
}

/// <summary>
/// Gets bane of block
/// </summary>
/// <returns>name</returns>
std::string Block::get_name()
{
	return m_name;
}

/// <summary>
/// Gets if a block is solid
/// </summary>
/// <returns>is solid</returns>
bool Block::get_is_solid()
{
	return m_is_solid;
}

/// <summary>
/// Gets id of block
/// </summary>
/// <returns>id</returns>
unsigned int Block::get_id()
{
	return m_id;
}

/// <summary>
/// Gets toughness of block
/// </summary>
/// <returns>toughness</returns>
float Block::get_toughness()
{
	return m_toughness;
}

// Add tool

/// <summary>
/// gets image of block
/// </summary>
/// <returns>image</returns>
sf::Image& Block::get_image()
{
	return m_image;
}

/// <summary>
/// gets color of block id
/// </summary>
/// <returns>color</returns>
sf::Color Block::get_color()
{
	return m_color;
}