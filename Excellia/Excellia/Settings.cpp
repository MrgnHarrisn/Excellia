#include "Settings.h"

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