
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	get_shape().setSize(sf::Vector2f(1, 3));
	get_shape().setOrigin(0, get_shape().getSize().y);
	set_position(position);
	set_speed(m_move_speed);
	get_shape().setPosition(position);
	m_texture.loadFromFile("Textures/Frankly.png");
	get_shape().setTexture(&m_texture);
}

void Player::update(float dt)
{
	// Reset x velocity
	set_velocity_x(0.0f);

	// Sprinting
	if (get_sprinting()) {
		set_speed(m_sprint_speed);
	}
	else {
		set_speed(m_move_speed);
	}

	// Rotation
	update_facing();

	// Velocity
	update_velocity();

	// Jumping
	jump(m_jump_force);

	// Gravity
	apply_gravity(m_gravity, m_drag, dt);

	// Collision (needs moving into the function)
	sf::Vector2f pos = get_position();
	int loop_count = (int)(dt * 1000 + 1);
	sf::Vector2f test_displacement = sf::Vector2f(get_velocity().x * dt / loop_count, get_velocity().y * dt / loop_count);

	for (int i = 0; i < loop_count; i++)
	{
		sf::Vector2f displacement = check_collision(m_wm, pos, sf::Vector2f(test_displacement.x, test_displacement.y), get_shape().getSize());

		if (displacement == sf::Vector2f(0, 0))
		{
			break;
		}
		if (pos.y + test_displacement.y < get_shape().getSize().y && test_displacement.y < 0)
		{
			displacement.y = 0;
		}
		if (pos.x + test_displacement.x < 0 && test_displacement.x < 0)
		{
			displacement.x = 0;
		}
		if (pos.x + test_displacement.x > m_wm.get_size().x - get_shape().getSize().x && test_displacement.x > 0)
		{
			displacement.x = 0;
		}

		pos.x += displacement.x;
		pos.y += displacement.y;
		set_position(pos);
	}

	// Update shape position
	get_shape().setPosition(get_position());
}