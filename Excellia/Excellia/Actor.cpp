
#include "Actor.h"

Actor::Actor() {}

void Actor::update(float dt) {}

sf::RectangleShape& Actor::get_shape()
{
	return m_shape;
}

void Actor::load_texture(std::string file)
{
	m_texture.loadFromFile("Textures/Frankly.png");
	m_shape.setTexture(&m_texture);
}

void Actor::jump(float jump_force)
{
	if (m_can_jump && m_jumping) m_velocity.y = -jump_force;
}

void Actor::apply_gravity(float acceleration, float drag, float dt)
{
	m_velocity.y += (acceleration - drag * m_velocity.y * m_velocity.y) * dt;
}

void Actor::update_facing()
{
	if (m_moving_right) {
		if (m_facing_right) {
			m_shape.setOrigin(0, m_shape.getSize().y);
			m_shape.setScale(1, 1);
			m_facing_right = false;
		}
	}
	else if (m_moving_left) {
		if (!m_facing_right) {
			m_shape.setOrigin(m_shape.getSize().x, m_shape.getSize().y);
			m_shape.setScale(-1, 1);
			m_facing_right = true;
		}
	}
}

void Actor::update_velocity()
{
	if (m_moving_left && m_moving_right) {
		m_velocity.x = 0;
	}
	else if (m_moving_right)
	{
		m_velocity.x = m_speed;
	}
	else if (m_moving_left)
	{
		m_velocity.x = -m_speed;
	}
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

void Actor::set_velocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
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

void Actor::set_max_health(int max_health)
{
	m_max_health = std::max(max_health, 0);
}
int Actor::get_max_health()
{
	return m_max_health;
}

void Actor::set_current_health(int current_health)
{
	m_current_health = std::max(std::min(current_health, m_max_health), 0);
	m_dead = (m_current_health <= 0.0f);
}

void Actor::dammaged(float dammage)
{
	m_current_health -= std::max((int)(dammage - m_armour / 10.0f), 0);
	m_dead = (m_current_health <= 0);
}

void Actor::healed(float healing)
{
	m_current_health += healing;
	if (m_current_health > m_max_health) m_current_health = m_max_health;
	m_dead = (m_current_health <= 0);
}

int Actor::get_current_health()
{
	return m_current_health;
}

bool Actor::get_dead()
{
	return m_dead;
}

void Actor::set_armour(int armour)
{
	m_armour = armour;
}

int Actor::get_armour()
{
	return m_armour;
}

sf::Vector2f Actor::can_move(WorldManager& world, sf::Vector2f& position, sf::Vector2f displacement, sf::Vector2f size)
{
	const float e = 0.01f;
	const float E = 0.05f;
	m_can_jump = false;

	// Left
	if (displacement.x < 0.0f) {
		if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(displacement.x - e, -size.y + 2.0f * E))).get_is_solid()) {
			displacement.x = 0;
			position.x = std::floor(position.x + 0.5f - e);
		} else {
			for (float y = 0; y < size.y; y++) {
				if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(displacement.x + e, -y - E))).get_is_solid()) {
					displacement.x = 0;
					position.x = std::floor(position.x + 0.5f - e);
					break;
				}
			}
		}
	}
	// Right
	else if (displacement.x > 0.0f) {
		if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(displacement.x - e + size.x, -size.y + 2.0f * E))).get_is_solid()) {
			displacement.x = 0;
			position.x = std::floor(position.x + 0.5f - e);
		} else {
			for (float y = 0; y < size.y; y++) {
				if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(displacement.x - e + size.x, -y - E))).get_is_solid()) {
					displacement.x = 0;
					position.x = std::floor(position.x + 0.5f - e);
					break;
				}
			}
		}
	}
	// Down
	if (displacement.y > 0.0f) {
		m_can_jump = false;

		if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(size.x - E, displacement.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = 11;
			m_can_jump = true;
		}
		else {
			for (int x = 0; x < size.x; x++) {
				if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(x + E, displacement.y))).get_is_solid()) {
					displacement.y = 0.0f;
					position.y = std::floor(position.y + 0.5f - e) - e;
					m_velocity.y = 11;
					m_can_jump = true;
					break;
				}
			}
		}
	}
	// Up
	else if (displacement.y < 0.0f) {
		if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(size.x - E, displacement.y + e - size.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = 0.0f;
		} else {
			for (int x = 0; x < size.x; x++) {
				if (world.get_block((sf::Vector2i)(m_position + sf::Vector2f(x + E, displacement.y + e - size.y))).get_is_solid()) {
					displacement.y = 0.0f;
					position.y = std::floor(position.y + 0.5f - e) - e;
					m_velocity.y = 0.0f;
					break;
				}
			}
		}
	}

	return displacement;
}

void Actor::move_with_collision(WorldManager& wm, float dt, bool fall_dammage)
{
	sf::Vector2f pos = m_position;
	int loop_count = (int)(dt * 1000 + 1);
	sf::Vector2f test_displacement = sf::Vector2f(m_velocity.x * dt / loop_count, m_velocity.y * dt / loop_count);

	for (int i = 0; i < loop_count; i++)
	{
		float y_val = m_velocity.y;
		sf::Vector2f displacement = can_move(wm, pos, sf::Vector2f(test_displacement.x, test_displacement.y), m_shape.getSize());

		if (displacement.y == 0)
		{
			if (fall_dammage)
			{
				if (m_distance_fallen > 5) {
					dammaged(m_distance_fallen - 5 - m_can_jump);
				}
				m_distance_fallen = 0.0f;
			}
		}
		if (displacement == sf::Vector2f(0.0f, 0.0f))
		{
			break;
		}
		if (pos.y + test_displacement.y < m_shape.getSize().y && test_displacement.y < 0)
		{
			displacement.y = 0;
		}
		if (pos.x + test_displacement.x < 0 && test_displacement.x < 0)
		{
			displacement.x = 0;
		}
		if (pos.x + test_displacement.x > wm.get_size().x - m_shape.getSize().x && test_displacement.x > 0)
		{
			displacement.x = 0;
		}
		if (fall_dammage)
		{
			m_distance_fallen += displacement.y;
		}

		pos.x += displacement.x;
		pos.y += displacement.y;
		m_position = pos;

		
	}

	m_shape.setPosition(m_position);
}

void Actor::move_without_collision(float dt)
{
	m_position += m_velocity * dt;
	m_shape.setPosition(m_position);
}