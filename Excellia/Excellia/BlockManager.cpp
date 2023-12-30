#include "BlockManager.h"

unsigned int BlockManager::color_to_hex(sf::Color color)
{
	return (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
}

sf::Color BlockManager::hex_to_color(unsigned int value)
{
	return sf::Color(value >> 24, value >> 16 & 0xFF, value >> 8 & 0xFF, value & 0xFF);
}

bool BlockManager::can_move_through(Block b)
{
	switch (b)
	{
	case Block::Void:
	case Block::Water:
	case Block::Lava:
		return true;
	default:
		return false;
	}
}

Block BlockManager::index_to_block(unsigned int index)
{

	switch (index)
	{
	case 1:
		return Block::Stone;
	case 2:
		return Block::Dirt;
	case 3:
		return Block::Wood;
	case 4:
		return Block::Water;
	case 5:
		return Block::Lava;
	case 6:
		return Block::Diamond;
	default:
		return Block::Void;
	}
}

Block BlockManager::color_to_block(sf::Color color)
{
	return static_cast<Block>(color_to_hex(color));
}