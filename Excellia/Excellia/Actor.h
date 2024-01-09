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

private:
	sf::CircleShape a_shape;
	sf::Vector2f m_position;
};