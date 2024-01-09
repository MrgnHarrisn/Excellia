
#include "Actor.h"

Actor::Actor() {}

void Actor::update(float dt) {}

sf::Drawable& Actor::render_shape()
{
	return a_shape;
}

void Actor::set_position(sf::Vector2f position)
{
	this->m_position = position;
}

sf::Vector2f Actor::get_position()
{
	return m_position;
}