
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

void Actor::set_moving_left(bool left)
{
	m_moving_left = left;
}

bool Actor::get_moving_left()
{
	return m_moving_left;
}

void Actor::set_moving_right(bool right)
{
	m_moving_right = right;
}

bool Actor::get_moving_right()
{
	return m_moving_right;
}

void Actor::set_jumping(bool jump)
{
	m_jumping = jump;
}

bool Actor::get_jumping()
{
	return m_jumping;
}

void Actor::set_sprinting(bool sprint)
{
	m_sprinting = sprint;
}

bool Actor::get_sprinting()
{
	return m_sprinting;
}