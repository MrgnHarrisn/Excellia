#pragma once

#include <random>

/// <summary>
/// Stores seed and keeps track of random numbers
/// </summary>
class Random
{
public:

	/// <summary>
	/// Default constructor
	/// </summary>
	Random();

	/// <summary>
	/// Constructor with a specific seed
	/// </summary>
	/// <param name="seed">Seed we wish to use</param>
	Random(long int seed);

	/// <summary>
	/// Generates a random number with stored seed
	/// </summary>
	/// <param name="min">smallest output number</param>
	/// <param name="max">largest output number</param>
	/// <returns>random number</returns>
	float random(float min, float max);

private:

	long int m_seed;
	std::mt19937 m_gen;
};