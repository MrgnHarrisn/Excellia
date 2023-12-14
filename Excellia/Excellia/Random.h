#pragma once
#include <random>

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

	/// <summary>
	/// Generates a random number with a given seed (not stored)
	/// </summary>
	/// <param name="min">smallest number</param>
	/// <param name="max">largest number</param>
	/// <param name="seed">seed to use</param>
	/// <returns>random number</returns>
	float random(float min, float max, long int seed);

private:
	long int m_seed;
	std::mt19937 m_gen;
};