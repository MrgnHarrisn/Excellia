
#include "Blocks.h"

Block::Block()
{
	m_id = 0;
	m_color = sf::Color(0, 0, 0);
	m_image.create(8, 8, m_color);
}

Block::Block(std::string name, unsigned int id)
{
	m_id = id;
	m_image.loadFromFile("Textures/" + name + ".png");
	m_color = sf::Color(m_id >> 24, m_id >> 16 & 0xFF, m_id >> 8 & 0xFF, m_id & 0xFF);
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