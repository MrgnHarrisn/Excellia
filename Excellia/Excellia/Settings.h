#pragma once

#include <SFML/System/Vector2.hpp>
#include <fstream>

/// <summary>
/// Imports settings from file
/// screenw -> width of screen
/// screenh -> height of screen
/// </summary>
class Settings
{
public:
	sf::Vector2u get_screen_size()	{ return m_screen_size; };
	sf::Vector2u get_world_size ()	{ return m_world_size ; };
	void update();
private:
	sf::Vector2u m_screen_size = sf::Vector2u(800, 800);
	sf::Vector2u m_world_size  = sf::Vector2u(800, 800);
};

/// <summary>
/// Updates the current settings
/// </summary>
void Settings::update()
{
	std::fstream settings_file;
	std::string setting_name;

	settings_file.open("Settings.conf");

	// Return if failed
	if (!settings_file.is_open()) return;

	// Read in all settings
	while (settings_file >> setting_name)
	{
		// Screen Width
		if (setting_name == "screenw")
		{
			settings_file >> m_screen_size.x;
		}

		// Screen Height
		else if (setting_name == "screenh")
		{
			settings_file >> m_screen_size.y;
		}
		else if (setting_name == "worldw") // Temp
		{
			settings_file >> m_world_size.x;
		}
		else if (setting_name == "worldh") // Temp
		{
			settings_file >> m_world_size.y;
		}
	}
}