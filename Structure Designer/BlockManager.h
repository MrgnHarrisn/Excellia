#pragma once

#include "Blocks.h"

class BlockManager
{
public:
	BlockManager();
	void create_block(std::string name, unsigned int id);
	Block& get_by_id(unsigned int id);
	Block& get_by_name(std::string name);
	Block& get_by_color(sf::Color);
	Block& get_by_num(int n);
	int get_counter();

private:
	int counter = 0;
	std::unordered_map<unsigned int, Block> m_blocks;
	std::unordered_map<std::string, unsigned int> m_ids;
	std::unordered_map<unsigned int, int> nums;

};