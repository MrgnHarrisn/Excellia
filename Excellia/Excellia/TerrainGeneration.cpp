#include "TerrainGeneration.h"

std::vector<int> TerrainGeneration::interpolate(std::vector<sf::Vector2f> heights, int max_height)
{
	std::vector<int> new_heights;
	float sum;

	// Loop over columns
	for (size_t i = 0; i < heights.size(); i++) {

		// Check its not an edge
		if (i > 0 && i < heights.size() - 1) {
			sum = Utils::dot_product(heights[i], heights[i - 1], heights[i + 1]);
		}
		else if (i == 0) {
			sum = Utils::dot_product(heights[i], heights[i + 1]);
		}
		else {
			sum = Utils::dot_product(heights[i], heights[i - 1]);
		}

		// Save result
		new_heights.push_back(static_cast<int>(sum * max_height));
	}

	return new_heights;
}

std::vector<int> TerrainGeneration::generate_heights(int map_width, float epsilon, int height, Random& r)
{
	std::vector<sf::Vector2f> heights;

	// Get first column
	sf::Vector2f first(r.random(0, 1), r.random(0, 1));
	heights.push_back(first);

	// Loop over columns
	for (int i = 1; i < map_width; i++) {

		// Clamp based on previous and randomise
		float nx = r.random(Utils::clip(heights[i - 1].x - epsilon, 0.6f, 0.8f), Utils::clip(heights[i - 1].x + epsilon, 0.6f, 0.8f));
		float ny = r.random(Utils::clip(heights[i - 1].y - epsilon, 0.6f, 0.8f), Utils::clip(heights[i - 1].y + epsilon, 0.6f, 0.8f));
		sf::Vector2f nh(nx, ny);

		// Store height
		heights.push_back(nh);
	}

	return interpolate(heights, height * 0.5);
}

std::vector<int> TerrainGeneration::generate_dirt(int n, Random& random)
{
	std::vector<int> depths;

	// Loop over columns
	for (size_t i = 0; i < n; i++) {
		
		// Store random depth
		depths.push_back(static_cast<int>(random.random(3, 6)));
	}

	return depths;
}

std::vector<sf::Vector2f> TerrainGeneration::generate_caves(sf::Vector2f min, sf::Vector2f max, Random& r)
{
	std::vector<sf::Vector2f> caves;
	sf::Vector2f cave_pos;

	// Loop for number of caves
	for (int i = 0; i < 10; i++) {

		// Randomise position
		cave_pos.x = r.random(min.x, max.x); // maybe not needed
		cave_pos.y = r.random(min.y, max.y);

		// Repeat until positon is valid
		do {
			cave_pos.x = r.random(min.x, max.x);
			cave_pos.y = r.random(min.y, max.y);
		} while ((cave_pos.x < 10) || (cave_pos.x > max.x - 10) || (cave_pos.y < 10) || (cave_pos.y > max.y - 10));

		// Store positions
		caves.push_back(cave_pos);
	}

	return caves;
}