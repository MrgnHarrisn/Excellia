#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>
#include "BlockManager.h"
#include <iostream>
#include "Block.h"
#include <string>

class TextureManager
{
public:
	TextureManager();
	bool load_texture(const std::string name, const std::string& file_name);
	sf::Texture* get_texture(const std::string& name);
	bool texture_exists(std::string& name);
	bool find_by_type(Block block);
	sf::Texture* get_by_type(Block block);
	void set_path(const std::string& folder);
	void clear_path();
	void display_items();
private:
	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::string m_folder = "-1";
};

