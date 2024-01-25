#include "BlockManager.h"

BlockManager::BlockManager()
{
	m_blocks.max_load_factor(0.001f);
	m_ids.max_load_factor(0.001f);
}

/// <summary>
/// Adds a block with the given properties
/// </summary>
/// <param name="name">name of block</param>
/// <param name="id">color identifier</param>
/// <param name="toughness">how hard it is to break</param>
/// <param name="is_solid">if it has collision</param>
void BlockManager::create_block(std::string name, unsigned int id, float toughness, bool is_solid)
{
	Block block(name, id, toughness, is_solid);
	m_blocks[id] = block;
	m_ids[name] = id;
}

/// <summary>
/// Gets a block by its color id
/// </summary>
/// <param name="id">id to look for</param>
/// <returns>block found</returns>
Block& BlockManager::get_by_id(unsigned int id)
{
	return m_blocks[id];
}

/// <summary>
/// Gets a block by its name
/// </summary>
/// <param name="name">name to look for</param>
/// <returns>block found</returns>
Block& BlockManager::get_by_name(std::string name)
{
	return m_blocks[m_ids[name]];
}

Block& BlockManager::get_by_color(sf::Color color)
{
	return m_blocks[(color.r << 24) + (color.g << 16) + (color.b << 8) + (color.a)];
}