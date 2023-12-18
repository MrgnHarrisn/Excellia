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

	sf::Vector2u get_screen_size() {
		return m_screen_size; 
	};
	sf::Vector2u get_world_size() { return m_world_size; };
	void update();

private:

	sf::Vector2u m_screen_size = sf::Vector2u(800, 800);
	sf::Vector2u m_world_size = sf::Vector2u(800, 800);
};