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

std::string BlockManager::block_name(Block b)
{
	switch (b)
	{
	case Block::Void:
		return "Void";
	case Block::Water:
		return "Water";
	case Block::Lava:
		return "Lava";
	case Block::Dirt:
		return "Dirt";
	case Block::Diamond:
		return "Diamond";
	case Block::Grass:
		return "Grass";
	case Block::Stone:
		return "Stone";
	case Block::Wood:
		return "Wood";
	default:
		return "Unknown";
	}
}

Block BlockManager::block_value(std::string& name)
{
	if (name == "Stone") {
		return Block::Stone;
	}
	else if (name == "Dirt") {
		return Block::Dirt;
	}
	else if (name == "Wood") {
		return Block::Wood;
	}
	else if (name == "Diamond") {
		return Block::Diamond;
	}
	else if (name == "Grass") {
		return Block::Grass;
	}
	else if (name == "Water") {
		return Block::Water;
	}
	else if (name == "Lava") {
		return Block::Lava;
	}
	else {
		return Block::Void;
	}
}

Block BlockManager::color_to_block(sf::Color color)
{
	return static_cast<Block>(color_to_hex(color));
}