#pragma once

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>

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
	/// Creates a drawable shape for SFML
	/// </summary>
	/// <returns>Drawable shape for SFML</returns>
	virtual sf::Drawable& render_shape();

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





private:
	sf::CircleShape a_shape;
	sf::Vector2f m_position;
	bool m_moving_left = false;
	bool m_moving_right = false;
	bool m_jumping = false;
	bool m_sprinting = false;

};