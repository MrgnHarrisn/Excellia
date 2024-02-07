#pragma once

#include <SFML/System/Vector2.hpp>

#include <fstream>

/// <summary>
/// Imports settings from file
/// screenw -> width of screen
/// screenh -> height of screen
/// 
/// camzoom -> default zoom of camera
/// </summary>
class Settings
{
public:

	Settings() { update(); };
	void update();

	sf::Vector2u get_screen_size()	{ return m_screen_size; };

	sf::Vector2u get_world_size ()	{ return m_world_size ; };
	float get_camera_zoom() { return m_camera_zoom; };
	float get_arm_length() { return m_arm_length; };

private:
	// Player settings
	sf::Vector2u m_screen_size = sf::Vector2u(800, 800);

	// Game settings
	sf::Vector2u m_world_size  = sf::Vector2u(30000, 800);
	float m_camera_zoom = 35;
	float m_arm_length = 9;
};

/// <summary>
/// Updates the current settings
/// </summary>
void Settings::update()
{
	std::fstream settings_file;
	std::string setting_name;

	settings_file.open("Game/Settings.conf");

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

		// World Width
		else if (setting_name == "worldw") // Temp
		{
			settings_file >> m_world_size.x;
		}

		// World Height
		else if (setting_name == "worldh") // Temp
		{
			settings_file >> m_world_size.y;
		}
	}
}