#pragma once

#include "Block.h"

#include <fstream>
#include <iomanip>

class BlockManager
{
public:
	BlockManager();
	BlockManager(std::string file_name);
	void load_from_file(std::string filename);
	void create_block(std::string name, unsigned int id, float toughness, bool is_solid);
	Block& get_by_id(unsigned int id);
	Block& get_by_name(std::string name);
	Block& get_by_color(sf::Color);

private:

	std::unordered_map<unsigned int, Block> m_blocks;
	std::unordered_map<std::string, unsigned int> m_ids;

};