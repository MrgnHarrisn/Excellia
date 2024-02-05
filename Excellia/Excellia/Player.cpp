
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	m_shape.setSize(sf::Vector2f(1, 3));
	m_shape.setOrigin(0, m_shape.getSize().y);
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
	if (get_sprinting()) {
		m_speed = m_sprint_speed;
	}
	else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (get_moving_left() && get_moving_right()) {
		m_velocity.x = 0;
	}
	else if (get_moving_right()) {
		m_velocity.x = m_speed;
		if (m_facing_right) {
			m_shape.setOrigin(0, m_shape.getSize().y);
			m_shape.setScale(1, 1);
			m_facing_right = false;
		}
	}
	else if (get_moving_left()) {
		m_velocity.x = -m_speed;
		if (!m_facing_right) {
			m_shape.setOrigin(m_shape.getSize().x, m_shape.getSize().y);
			m_shape.setScale(-1, 1);
			m_facing_right = true;
		}
	}

	// Jumping
	if (m_can_jump && get_jumping())
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
		sf::Vector2f displacement = check_collision(pos, sf::Vector2f(test_displacement.x, test_displacement.y), m_shape.getSize());

		if (displacement == sf::Vector2f(0, 0))
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
		if (pos.x + test_displacement.x > m_wm.get_size().x - m_shape.getSize().x && test_displacement.x > 0)
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

sf::Vector2f Player::check_collision(sf::Vector2f &position, sf::Vector2f displacement, sf::Vector2f size)
{
	const float e = 0.01f;
	const float E = 0.05f;
	m_can_jump = false;

	// Left
	if (displacement.x < 0.0f) {
		for (float y = 0; y < size.y; y++) {
			if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + e, -y - E))).get_is_solid()) {
				displacement.x = 0;
				position.x = std::floor(position.x + 0.5f - e);
				break;
			}
		}
	}
	// Right
	else if (displacement.x > 0.0f) {
		for (float y = 0; y < size.y; y++) {
			if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(displacement.x + e + size.x, -y - E))).get_is_solid()) {
				displacement.x = 0;
				position.x = std::floor(position.x + 0.5f - e);
				break;
			}
		}
	}
	// Down
	if (displacement.y > 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(size.x - E, displacement.y))).get_is_solid()) {
			displacement.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = m_speed;
			m_can_jump = true;
		}
		else m_can_jump = false;
	}
	// Up
	else if (displacement.y < 0.0f) {
		if (m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, displacement.y + e - size.y))).get_is_solid() ||
			m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(size.x - E, displacement.y + e - size.y))).get_is_solid()) {
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