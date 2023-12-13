#include "Random.h"

Random::Random() 
{
	std::random_device rd;
	m_seed = rd();
	m_gen.seed(m_seed);
};

Random::Random(long int seed)
{
	m_seed = seed;
	m_gen.seed(seed);
}

long int Random::random(int min, int max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(m_gen);
}

long int Random::random(int min, int max, long int seed)
{
	std::mt19937 gen(seed);
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}