
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	m_shape.setSize(sf::Vector2f(1, 3));
	m_shape.setOrigin(0, 3);
	set_position(position);
	m_shape.setPosition(get_position());
	m_texture.loadFromFile("Textures/Frankly.png");
	m_shape.setTexture(&m_texture);
}

void Player::update(float dt)
{
	// Reset x velocity
	m_velocity.x = 0;

	// Check Sprinting
	if (m_sprinting) {
		m_speed = m_sprint_speed;
	}
	else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (m_moving_left && m_moving_right) {
		m_velocity.x = 0;
	}
	else if (m_moving_right) {
		m_velocity.x = m_speed;
		if (m_facing_right) {
			m_shape.setOrigin(0, 3);
			m_shape.setScale(1, 1);
			m_facing_right = false;
		}
	}
	else if (m_moving_left) {
		m_velocity.x = -m_speed;
		if (!m_facing_right) {
			m_shape.setOrigin(1, 3);
			m_shape.setScale(-1, 1);
			m_facing_right = true;
		}
	}

	// Jumping
	if (m_can_jump && m_jumping)
	{
		m_velocity.y = -m_jump_force;
	}

	// Apply Gravity With Drag
	m_velocity.y += (m_gravity - m_drag * m_velocity.y * m_velocity.y) * dt;

	// Call Collision
	sf::Vector2f pos = get_position();
	int loop_count = (int)(dt * 1000 + 1);
	sf::Vector2f test_displacement = sf::Vector2f(m_velocity.x * dt / loop_count, m_velocity.y * dt / loop_count);

	for (int i = 0; i < loop_count; i++)
	{
		sf::Vector2f displacement = can_move_pos(pos, sf::Vector2f{ test_displacement.x, test_displacement.y });

		if (displacement == sf::Vector2f(0, 0))
		{
			break;
		}
		if (pos.y + test_displacement.y < 3 && test_displacement.y < 0)
		{
			displacement.y = 0;
		}
		if (pos.x + test_displacement.x < 0 && test_displacement.x < 0)
		{
			displacement.x = 0;
		}

		pos.x += displacement.x;
		pos.y += displacement.y;
		set_position(pos);
	}

	// Update shape position
	m_shape.setPosition(get_position());
}

sf::Vector2f Player::can_move_pos(sf::Vector2f &position, sf::Vector2f displacement)
{
	const float e = 0.01f;
	const float E = 0.05f;
	m_can_jump = false;

	// Left
	if (displacement.x < 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x - e, -E))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x, -1.0f))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x, -2.0f))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x, e - 3.0f))).get_is_solid()) {
			displacement.x = 0.0f;
			position.x = std::floor(position.x + 0.5f - e);
		}
	}
	// Right
	else if (displacement.x > 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + e + 1.0f, -E))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + 1.0f, -1.0f))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + 1.0f, -2.0f))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + 1.0f, e - 3.0f))).get_is_solid()) {
			displacement.x = 0;
			position.x = std::floor(position.x + 0.5f - e);
		}
	}
	// Down
	if (displacement.y > 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(1.0f - E, displacement.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = m_speed;
			m_can_jump = true;
		}
		else
		{
			m_can_jump = false;
		}
	}
	// Up
	else if (displacement.y < 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y + e - 3.0f))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(1.0f - E, displacement.y + e - 3.0f))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = m_speed;
		}
	}

    return displacement;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}

void Player::set_moving_left(bool left)
{
	m_moving_left = left;
}

void Player::set_moving_right(bool right)
{
	m_moving_right = right;
}

void Player::set_jumping(bool jump)
{
	m_jumping = jump;
}

void Player::set_sprinting(bool sprint)
{
	m_sprinting = sprint;
}