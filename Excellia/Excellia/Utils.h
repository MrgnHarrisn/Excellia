#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <random>

/// <summary>
/// Clips a number to be one of two
/// </summary>
/// <param name="n">Input number</param>
/// <param name="lower">Lowest possible number</param>
/// <param name="upper">Largest possible number</param>
/// <returns>Clipped number</returns>
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
/// Get's dot product of 2 or 3 vector2f's
/// </summary>
/// <param name="a">Vector a</param>
/// <param name="b">Vector b</param
/// <param name="c">Vector c (optional)</param>
/// <returns>Resulting float</returns>
float dot_product(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c = sf::Vector2f(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()))
{
	return (std::isnan(c.x) || std::isnan(c.y)) ? (a.x * b.x + a.y * b.y) : (a.x * b.x * c.x + a.y * b.y * c.y);
}