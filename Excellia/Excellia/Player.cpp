
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm, AudioManager& audio_manager) : m_wm(wm)
{
	// Create shape
	get_shape().setSize(sf::Vector2f(1, 3));
	get_shape().setOrigin(0, get_shape().getSize().y);
	set_position(position);
	get_shape().setPosition(position);
	load_texture("Textures/Frankly.png");
	m_spawn_point = position;

	// Set Stats
	set_speed(m_move_speed);
	set_max_health(16);
	set_current_health(get_max_health());
	set_audio_manager(audio_manager);
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
	move_with_collision(m_wm, dt, true);


	// Respawn
	if (get_dead())
	{
		set_current_health(get_max_health());
		set_position(m_spawn_point);
		set_velocity(sf::Vector2f(0.0f, 0.0f));
	}
}

void Player::set_sprinting(bool sprint)
{
	m_sprinting = sprint;
}