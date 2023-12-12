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

// Base case for dot product, multiplying two numbers

/// <summary>
/// Base case for multiplying two Vectors to get dot product
/// </summary>
/// <typeparam name="T">Type of variable</typeparam>
/// <param name="a">Vector a</param>
/// <param name="b">Vector b</param>
/// <returns>resulting dot product</returns>
template<typename T>
float dot_product(const T& a, const T& b) {
	return (a.x * b.x + a.y * b.y);
}

// Recursive variadic template for dot product calculation

/// <summary>
/// Recursive template for dot product calculation
/// </summary>
/// <typeparam name="T"></typeparam>
/// <typeparam name="...Args"></typeparam>
/// <param name="a"></param>
/// <param name="...args"></param>
/// <returns></returns>
template<typename T, typename... Args>
float dot_product(const T& a, const Args&... args) {
	return a * dot_product(args...);
}

// Function to calculate dot product of vectors

/// <summary>
/// Function to calculate dot product of a series of Vectors
/// </summary>
/// <typeparam name="T">Type</typeparam>
/// <typeparam name="...Args">all Vectors to calculate</typeparam>
/// <param name="a">vector a</param>
/// <param name="...args">all other Vectors</param>
/// <returns>resulting dot product</returns>
template<typename T, typename... Args>
float dot_product_vectors(const T& a, const Args&... args) {
	return dot_product(a, args...);
}