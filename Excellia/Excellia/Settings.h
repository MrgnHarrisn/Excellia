#pragma once

#include <fstream>
#include <SFML/System/Vector2.hpp>

/// <summary>
/// Imports settings from file
/// screenw -> width of screen
/// screenh -> height of screen
/// </summary>

class Settings
{
public:
	sf::Vector2u getScreensize() { return m_screen_size; };
	void update();
private:
	sf::Vector2u m_screen_size = sf::Vector2u(800, 800);
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
	}
}