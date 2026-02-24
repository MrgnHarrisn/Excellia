
#include "Skybox.h"

void Skybox::update(float dt)
{
	current_time += dt;

	float transition_percentage = current_time / end_of_day_time;

	if (transition_percentage > 1.0f) {
		transition_percentage = 1.0f;
	}

	if (current_time <= end_of_day_time / 2) {
		current_color.r = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.r + transition_percentage * day_color.r);
		current_color.g = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.g + transition_percentage * day_color.g);
		current_color.b = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.b + transition_percentage * day_color.b);
	}
	else {
		current_color.r = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.r + transition_percentage * night_color.r);
		current_color.g = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.g + transition_percentage * night_color.g);
		current_color.b = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.b + transition_percentage * night_color.b);
	}

	if (current_time >= end_of_day_time) {
		current_time = 0.0f;
	}
}