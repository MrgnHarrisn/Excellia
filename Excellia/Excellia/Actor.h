#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "WorldManager.h"
#include "Block.h"

/// <summary>
/// Player/Enemy/NPC class
/// </summary>
class Actor
{
public:

	/// <summary>
	/// Constructor for Actor
	/// </summary>
	Actor();

	/// <summary>
	/// Update function for an actor
	/// </summary>
	/// <param name="dt"Delta time></param>
	virtual void update(float dt);

	/// <summary>
	/// moves the actor
	/// </summary>
	/// <param name="position">Where to move it</param>
	void set_position(sf::Vector2f position);

	/// <summary>
	/// Gets position
	/// </summary>
	/// <returns>position</returns>
	sf::Vector2f get_position();

	void set_moving_left(bool left);
	bool get_moving_left();

	void set_moving_right(bool right);
	bool get_moving_right();

	void set_jumping(bool jump);
	bool get_jumping();

	void set_sprinting(bool sprint);
	bool get_sprinting();

	void set_velocity(sf::Vector2f velocity);
	void set_velocity_x(float x);
	void set_velocity_y(float y);
	sf::Vector2f get_velocity();

	void set_speed(float speed);
	float get_speed();

	void set_can_jump(bool can_jump);
	bool get_can_jump();

	void set_facing_right(bool facing_right);
	bool get_facing_right();

	/// <summary>
	/// Checks if a given block is free to move to
	/// </summary>
	/// <param name="position">Where the player is/param>
	/// <param name="velocity">Where the player is going</param>
	/// <returns>if the block is free</returns>
	sf::Vector2f check_collision(WorldManager& world, sf::Vector2f& position, sf::Vector2f velocity, sf::Vector2f size);



private:
	sf::Vector2f m_position;
	float m_speed = 10;
	sf::Vector2f m_velocity{ 0,0 };
	bool m_moving_left = false;
	bool m_moving_right = false;
	bool m_jumping = false;
	bool m_sprinting = false;
	bool m_can_jump = true;
	bool m_facing_right = false;
};