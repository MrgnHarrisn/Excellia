#include "TerrainGeneration.h"

std::vector<int> TerrainGeneration::interpolate(std::vector<sf::Vector2f> heights, int max_height)
{
	std::vector<int> new_heights;
	for (int i = 0; i < heights.size(); i++) {
		if (i > 0 && i < heights.size() - 1) {
			float sum = Utils::dot_product(heights[i], heights[i - 1], heights[i + 1]);
			new_heights.push_back(static_cast<int>(sum * max_height));
		}
		else {
			/* Check for edge spot */
			if (i == 0) {
				float sum = Utils::dot_product(heights[i], heights[i + 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
			else {
				float sum = Utils::dot_product(heights[i], heights[i - 1]);
				new_heights.push_back(static_cast<int>(sum * max_height));
			}
		}
	}
	return new_heights;
}

std::vector<int> TerrainGeneration::generate_heights(int map_width, float epsilon, int height, Random& r)
{

	sf::Vector2f first(r.random(0.0, 1.0), r.random(0.0, 1.0));

	std::vector<sf::Vector2f> heights;
	heights.push_back(first);
	for (int i = 1; i < map_width; i++) {
		float nx = r.random(Utils::clip(heights[i - 1].x - epsilon, 0.0, 1.0), Utils::clip(heights[i - 1].x + epsilon, 0.0, 1.0));
		float ny = r.random(Utils::clip(heights[i - 1].y - epsilon, 0.0, 1.0), Utils::clip(heights[i - 1].y + epsilon, 0.0, 1.0));
		sf::Vector2f nh(nx, ny);
		heights.push_back(nh);
	}

	return interpolate(heights, 400);

}

std::vector<int> TerrainGeneration::generate_heights(int map_width, float epsilon, int height, Random& r, long int seed)
{

	sf::Vector2f first(r.random(0.0, 1.0), r.random(0.0, 1.0));

	std::vector<sf::Vector2f> heights;
	heights.push_back(first);
	for (int i = 1; i < map_width; i++) {
		float nx = r.random(Utils::clip(heights[i - 1].x - epsilon, 0.0, 1.0), Utils::clip(heights[i - 1].x + epsilon, 0.0, 1.0), seed);
		float ny = r.random(Utils::clip(heights[i - 1].y - epsilon, 0.0, 1.0), Utils::clip(heights[i - 1].y + epsilon, 0.0, 1.0), seed);
		sf::Vector2f nh(nx, ny);
		heights.push_back(nh);
	}

	return interpolate(heights, height);

}

std::vector<int> TerrainGeneration::generate_dirt(std::vector<int>& heights, int height_limit, Random& random)
{

	std::vector<int> output;
	for (int i = 0; i < heights.size(); i++) {
		int dirt_height = static_cast<int>(heights[i]);

		// Clip dirt_height to stay within the valid range of the image
		dirt_height = std::max(0, std::min(dirt_height, height_limit - 6));
		int dirt_range = random.random(3, 5);
		output.push_back(dirt_range);
	}

	return output;

}

std::vector<Vector2f> TerrainGeneration::generate_caves(Vector2f min, Vector2f max, Random& r)
{
	std::vector<Vector2f> caves;
	for (int i = 0; i < 10; i++) {
		Vector2f cave_pos;
		cave_pos.x = r.random(min.x, max.x);
		cave_pos.y = r.random(min.y, max.y);
		do {
			cave_pos.x = r.random(min.x, max.x);
			cave_pos.y = r.random(min.y, max.y);
		} while ((cave_pos.x < 10) || (cave_pos.x > max.x - 10) || (cave_pos.y < 10) || (cave_pos.y > max.y - 10));
		caves.push_back(cave_pos);
	}
	return caves;
}