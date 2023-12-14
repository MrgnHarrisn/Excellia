#pragma once

#include <SFML/System/Vector2.hpp>
#include "Utils.h"
#include "Random.h"
#include <cmath>
using namespace sf;


class TerrainGeneration
{
public:

	/// <summary>
	/// Interpolates the values of a set of vectors
	/// </summary>
	/// <param name="heights">the Vectors to interpolate</param>
	/// <param name="max_height">Maximum possible height</param>
	/// <returns>vector of floats (heights)</returns>
	static std::vector<int> interpolate(std::vector<sf::Vector2f> heights, int max_height);

	/// <summary>
	/// Generates a set of heights for the terrain
	/// </summary>
	/// <param name="map_width">How many heights we wish to generate</param>
	/// <param name="epsilon">The largest possible offset of a float</param>
	/// <returns>a vector of floats representing the height</returns>
	static std::vector<int> generate_heights(int map_width, float epsilon, int height, Random& r);

	/// <summary>
	/// Generates random depths for dirt
	/// </summary>
	/// <param name="n">Number of columns</param>
	/// <param name="random">instance of Random to use</param>
	/// <returns>Vector of dirt depths</returns>
	static std::vector<int> generate_dirt(int n, Random& random);

	/// <summary>
	/// Generates caves at a random position
	/// </summary>
	/// <param name="min">Minimum position of the cave</param>
	/// <param name="max">Maximum position of the cave</param>
	/// <param name="r">instance of Random to use</param>
	/// <returns>A set of caves</returns>
	static std::vector<Vector2f> generate_caves(Vector2f min, Vector2f max, Random& r);

private:
};

