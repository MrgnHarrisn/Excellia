
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

void Actor::set_velocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

void Actor::set_velocity_x(float x)
{
	m_velocity.x = x;
}

void Actor::set_velocity_y(float y)
{
	m_velocity.y = y;
}

sf::Vector2f Actor::get_velocity()
{
	return m_velocity;
}

void Actor::set_speed(float speed)
{
	m_speed = speed;
}

float Actor::get_speed()
{
	return m_speed;
}

void Actor::set_can_jump(bool can_jump)
{
	m_can_jump = can_jump;
}

bool Actor::get_can_jump()
{
	return m_can_jump;
}

sf::Vector2f Actor::check_collision(WorldManager& world, sf::Vector2f& position, sf::Vector2f displacement, sf::Vector2f size)
{
	const float e = 0.01f;
	const float E = 0.05f;
	m_can_jump = false;

	// Left
	if (displacement.x < 0.0f) {
		for (float y = 0; y < size.y; y++) {
			if (world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + e, -y - E))).get_is_solid()) {
				displacement.x = 0;
				position.x = std::floor(position.x + 0.5f - e);
				break;
			}
		}
	}
	// Right
	else if (displacement.x > 0.0f) {
		for (float y = 0; y < size.y; y++) {
			if (world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + e + size.x, -y - E))).get_is_solid()) {
				displacement.x = 0;
				position.x = std::floor(position.x + 0.5f - e);
				break;
			}
		}
	}
	// Down
	if (displacement.y > 0.0f) {
		if (world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y))).get_is_solid() ||
			world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(size.x - E, displacement.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = m_speed;
			m_can_jump = true;
		}
		else m_can_jump = false;
	}
	// Up
	else if (displacement.y < 0.0f) {
		if (world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y + e - size.y))).get_is_solid() ||
			world.get_block((sf::Vector2i)(get_position() + sf::Vector2f(size.x - E, displacement.y + e - size.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = m_speed;
		}
	}

	return displacement;
}