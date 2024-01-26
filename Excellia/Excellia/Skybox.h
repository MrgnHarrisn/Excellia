#pragma once

#include "SFML/Graphics/Color.hpp"

class Skybox
{
public:
	void update(float dt);
	sf::Color get_sky_color() { return current_color; };
private:

	bool is_transitioning = true;
	float end_of_day_time = 5 * 60;
	float current_time = 0;
	sf::Color night_color = sf::Color(0, 0, 40);		// Dark blue for night sky
	sf::Color day_color = sf::Color(135, 206, 235);		// Light blue for daylight sky
	sf::Color current_color = day_color;

};