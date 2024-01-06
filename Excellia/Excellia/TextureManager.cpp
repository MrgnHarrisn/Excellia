
#include "TextureManager.h"

TextureManager::TextureManager() {}

bool TextureManager::load_texture(const std::string name, const std::string& file_name)
{
	sf::Texture* texture = new sf::Texture;

	/* Check if we have a specified path or not */
	if (!texture->loadFromFile(m_folder == "-1" ? file_name : m_folder + file_name)) {
		return false;
	}
	m_textures[name] = texture;
	return true;
}

sf::Texture* TextureManager::get_texture(const std::string& name)
{
	return m_textures[name];
}

bool TextureManager::texture_exists(std::string& name)
{
	return m_textures.find(name) != m_textures.end();
}

bool TextureManager::find_by_type(Block block)
{
	return m_textures.find(BlockManager::block_name(block)) != m_textures.end();
}

sf::Texture* TextureManager::get_by_type(Block block)
{
	return m_textures[BlockManager::block_name(block)];
}

void TextureManager::set_path(const std::string& folder)
{
	m_folder = folder;
}

void TextureManager::clear_path()
{
	m_folder = "-1";
}

void TextureManager::display_items()
{
	for (auto pair : m_textures) {
		std::cout << pair.first << std::endl;
	}
}

