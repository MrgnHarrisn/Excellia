#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "BlockManager.h"
#include "Block.h"

#include <unordered_map>
#include <iostream>
#include <string>

class TextureManager
{
public:

	/// <summary>
	/// Creates a texture manager
	/// </summary>
	TextureManager();

	/// <summary>
	/// Loads a texture from a file
	/// </summary>
	/// <param name="name">Name of the texture</param>
	/// <param name="file_name">Name of the file</param>
	/// <returns>Did it load</returns>
	bool load_texture(const std::string name, const std::string& file_name);

	/// <summary>
	/// Gets texture from memory
	/// </summary>
	/// <param name="name">Name of texture</param>
	/// <returns>Texture</returns>
	sf::Texture* get_texture(const std::string& name);

	/// <summary>
	/// Checks if there is a texture with a given name
	/// </summary>
	/// <param name="name">Name to look for</param>
	/// <returns>If found</returns>
	bool texture_exists(std::string& name);

	/// <summary>
	/// Checks if a given block is loaded
	/// </summary>
	/// <param name="block">Block to look for</param>
	/// <returns>If found</returns>
	bool find_by_type(Block block);

	/// <summary>
	/// Gets texture by block
	/// </summary>
	/// <param name="block">Block to look for</param>
	/// <returns>Texture</returns>
	sf::Texture* get_by_type(Block block);

	/// <summary>
	/// Sets floder directory to use
	/// </summary>
	/// <param name="folder">Folder directory</param>
	void set_path(const std::string& folder);

	/// <summary>
	/// Resets folder path
	/// </summary>
	void clear_path();

	/// <summary>
	/// Prints out what has been loaded
	/// </summary>
	void display_items();

private:
	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::string m_folder = "-1";
};

