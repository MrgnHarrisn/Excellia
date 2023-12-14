#pragma once

#include <SFML/Graphics/Color.hpp>
#include "Block.h"

class BlockManager
{
public:
	/// <summary>
	/// Converts SFML color to unsigned int
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	static unsigned int color_to_hex(sf::Color color);
	static Block color_to_block(sf::Color color);
	static sf::Color hex_to_color(unsigned int value);
	static bool can_move_through(Block b);
private:
};