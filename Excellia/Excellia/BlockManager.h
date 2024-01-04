#pragma once

#include <SFML/Graphics/Color.hpp>
#include "Block.h"
#include <string>

class BlockManager
{
public:

	/// <summary>
	/// Converts SFML color to unsigned int
	/// </summary>
	/// <param name="color">sf::Color to convert</param>
	/// <returns>uint32 hex</returns>
	static unsigned int color_to_hex(sf::Color color);

	/// <summary>
	/// Coverts a sf::Color to an enum
	/// </summary>
	/// <param name="color">sf::Color to convert</param>
	/// <returns>Enum block</returns>
	static Block color_to_block(sf::Color color);

	/// <summary>
	/// Converts a uint to a sf::Color
	/// </summary>
	/// <param name="value">Hex value</param>
	/// <returns>sf::Color of the hex</returns>
	static sf::Color hex_to_color(unsigned int value);

	/// <summary>
	/// Checks if a block is solid or not
	/// </summary>
	/// <param name="b">Block to check</param>
	/// <returns>Is it non solid?</returns>
	static bool can_move_through(Block b);

	static std::string block_name(Block b);

	static Block block_value(std::string& name);

};