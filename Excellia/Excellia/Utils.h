#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <random>

float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

/// <summary>
/// Generates a random number between 2 numbers
/// </summary>
/// <param name="min">smallest possible vlaue</param>
/// <param name="max">largest possible value</param>
/// <returns>resulting float</returns>
float random(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

/// <summary>
/// Get's dot product of 2 numbers
/// </summary>
/// <param name="a">Vector a</param>
/// <param name="b">Vector b</param>
/// <returns>Resulting float</returns>
float dot_product_2(sf::Vector2f a, sf::Vector2f b)
{
	return (a.x * b.x + a.y * b.y);
}

/// <summary>
/// Get's dot product of 3 numbers
/// </summary>
/// <param name="a">Vector a</param>
/// <param name="b">Vector b</param>
/// <param name="c">Vector c</param>
/// <returns>Resulting float</returns>
float dot_product_3(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
	return a.x * b.x * c.x + a.y * b.y * c.y;
}