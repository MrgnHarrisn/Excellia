
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	get_shape().setSize(sf::Vector2f(1, 3));
	get_shape().setOrigin(0, get_shape().getSize().y);
	set_position(position);
	set_speed(m_move_speed);
	get_shape().setPosition(position);
	load_texture("Textures/Frankly.png");
}

void Player::update(float dt)
{

	// Reset x velocity
	set_velocity(sf::Vector2f(0.0f, get_velocity().y));


	// Sprinting
	set_speed(m_sprinting ? m_sprint_speed : m_move_speed);


	// Rotation
	update_facing();


	// Velocity
	update_velocity();


	// Jumping
	jump(m_jump_force);


	// Gravity
	apply_gravity(m_gravity, m_drag, dt);


	// Move
	move_with_collision(m_wm, dt);
}

void Player::set_sprinting(bool sprint)
{
	m_sprinting = sprint;
}