
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

	return interpolate(heights, (int)(height * 0.5));
}

std::vector<int> TerrainGeneration::generate_dirt(int map_width, Random& r)
{
	std::vector<int> depths;

	// Loop over columns
	for (int i = 0; i < map_width; i++) {
		
		// Store random depth
		depths.push_back(static_cast<int>(r.random(7, 9)));
	}

	return depths;
}

std::vector<sf::Vector2i> TerrainGeneration::generate_caves(sf::Vector2i min, sf::Vector2i max, Random& r)
{
	std::vector<sf::Vector2i> caves;
	sf::Vector2i cave_pos;

	int num_caves = (int)((max.x - min.x) * (max.y - min.y) / 5000.0f);

	// Loop for number of caves
	for (int i = 0; i < num_caves; i++) {

		// Randomise position
		cave_pos.x = (int)r.random(min.x + 10.0f, max.x - 10.0f);
		cave_pos.y = (int)r.random(min.y + 10.0f, max.y - 10.0f);

		// Store positions
		caves.push_back(cave_pos);
	}

	return caves;
}

std::vector<int> TerrainGeneration::generate_trees(int map_width, Random& r)
{
	std::vector<int> trees;

	// Stops first tree from being on the edge
	int tree_pos = (int)r.random(5, 15);
	

	// Randomise position
	while (tree_pos < map_width - 5)
	{
		if (std::abs(tree_pos - map_width / 2) < 4)
		{
			tree_pos += (int)r.random(3, 10);
		}
			trees.push_back(tree_pos);
		
		// Move along
		tree_pos += (int)r.random(10, 50);
	}

	return trees;
}

std::vector<sf::Vector2i> TerrainGeneration::generate_ores(sf::Vector2i min, sf::Vector2i max, Random& r)
{
	std::vector<sf::Vector2i> ores;
	sf::Vector2i ore_pos;

	int num_caves = (int)((max.x - min.x) * (max.y - min.y) / 5000.0f);

	// Loop for number of caves
	for (int i = 0; i < num_caves; i++) {

		// Randomise position
		ore_pos.x = (int)r.random(min.x + 10.0f, max.x - 10.0f);
		ore_pos.y = (int)r.random(min.y + 10.0f, max.y - 10.0f);

		// Store positions
		ores.push_back(ore_pos);
	}

	return ores;
}