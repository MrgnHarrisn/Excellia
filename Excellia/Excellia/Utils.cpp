
#include "Utils.h"

float Utils::clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

float Utils::dot_product(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c) {
	return (std::isnan(c.x) || std::isnan(c.y)) ? (a.x * b.x + a.y * b.y) : (a.x * b.x * c.x + a.y * b.y * c.y);
}