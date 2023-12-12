#pragma once

#include <SFML/System/Vector2.hpp>
#include "Utils.h"

/// <summary>
/// Interpolates the values of a set of vectors
/// </summary>
/// <param name="heights">the Vectors to interpolate</param>
/// <param name="max_height">Maximum possible height</param>
/// <returns>vector of floats (heights)</returns>
std::vector<float> interpolate(std::vector<sf::Vector2f> heights, int max_height)
{
	std::vector<float> new_heights;
	for (int i = 0; i < heights.size(); i++) {
		if (i > 0 && i < heights.size() - 1) {
			float sum = dot_product_3(heights[i], heights[i - 1], heights[i + 1]);
			new_heights.push_back(static_cast<int>(sum * max_height));
		}
		else {
			/* Check for edge spot */
			if (i == 0) {
				float sum = dot_product_2(heights[i], heights[i + 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
			else {
				float sum = dot_product_2(heights[i], heights[i - 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
		}
	}
	return new_heights;
}

/// <summary>
/// Generates a set of heights for the terrain
/// </summary>
/// <param name="map_width">How many heights we wish to generate</param>
/// <param name="epsilon">The largest possible offset of a flaot</param>
/// <returns>a vector of floats representing the height</returns>
std::vector<float> generate_heights(int map_width, float epsilon)
{

	sf::Vector2f first(random(0.0, 1.0), random(0.0, 1.0));

	std::vector<sf::Vector2f> heights;
	heights.push_back(first);
	for (int i = 1; i < map_width; i++) {
		float nx = random(clip(heights[i - 1].x - epsilon, 0.0, 1.0), clip(heights[i - 1].x + epsilon, 0.0, 1.0));
		float ny = random(clip(heights[i - 1].y - epsilon, 0.0, 1.0), clip(heights[i - 1].y + epsilon, 0.0, 1.0));
		sf::Vector2f nh(nx, ny);
		heights.push_back(nh);
	}

	return interpolate(heights, 400);

}