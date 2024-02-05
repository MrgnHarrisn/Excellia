
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	m_shape.setSize(sf::Vector2f(1, 3));
	m_shape.setOrigin(0, m_shape.getSize().y);
	set_position(position);
	set_speed(m_move_speed);
	m_shape.setPosition(get_position());
	m_texture.loadFromFile("Textures/Frankly.png");
	m_shape.setTexture(&m_texture);
}

void Player::update(float dt)
{
	// Reset x velocity
	set_velocity_x(0.0f);

	// Check Sprinting
	if (get_sprinting()) {
		set_speed(m_sprint_speed);
	}
	else {
		set_speed(m_move_speed);
	}

	// Check Movement
	if (get_moving_left() && get_moving_right()) {
		set_velocity_x(0.0f);
	}
	else if (get_moving_right()) {
		set_velocity_x(get_speed());
		if (m_facing_right) {
			m_shape.setOrigin(0, m_shape.getSize().y);
			m_shape.setScale(1, 1);
			m_facing_right = false;
		}
	}
	else if (get_moving_left()) {
		set_velocity_x(-get_speed());
		if (!m_facing_right) {
			m_shape.setOrigin(m_shape.getSize().x, m_shape.getSize().y);
			m_shape.setScale(-1, 1);
			m_facing_right = true;
		}
	}

	// Jumping
	if (get_can_jump() && get_jumping())
	{
		set_velocity_y(-m_jump_force);
	}

	// Apply Gravity With Drag
	set_velocity_y(get_velocity().y + (m_gravity - m_drag * get_velocity().y * get_velocity().y) * dt);

	// Call Collision
	sf::Vector2f pos = get_position();
	int loop_count = (int)(dt * 1000 + 1);
	sf::Vector2f test_displacement = sf::Vector2f(get_velocity().x * dt / loop_count, get_velocity().y * dt / loop_count);

	for (int i = 0; i < loop_count; i++)
	{
		sf::Vector2f displacement = check_collision(m_wm, pos, sf::Vector2f(test_displacement.x, test_displacement.y), m_shape.getSize());

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

sf::Drawable& Player::render_shape()
{
    return m_shape;
}