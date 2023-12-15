#pragma once

#include <SFML/System/Vector2.hpp>

#include <random>
#include <cmath>

/// <summary>
/// A bunch of utilities that are common
/// </summary>
class Utils
{
public:

	/// <summary>
	/// Clips a number to be one of two
	/// </summary>
	/// <param name="n">Input number</param>
	/// <param name="lower">Lowest possible number</param>
	/// <param name="upper">Largest possible number</param>
	/// <returns>Clipped number</returns>
	static float clip(float n, float lower, float upper);

	/// <summary>
	/// Get's dot product of 2 or 3 vector2f's
	/// </summary>
	/// <param name="a">Vector a</param>
	/// <param name="b">Vector b</param
	/// <param name="c">Vector c (optional)</param>
	/// <returns>Resulting float</returns>
	static float dot_product(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c = sf::Vector2f(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN()));
};


