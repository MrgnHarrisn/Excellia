#pragma once

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>

/// <summary>
/// Player/Enemy/NPC class
/// </summary>
class Actor
{
public:
	Actor();
	virtual void update(float dt);
	/// <summary>
	/// Creates a drawable shape for SFML
	/// </summary>
	/// <returns>Drawable shape for SFML</returns>
	virtual sf::Drawable render_shape();
private:
	sf::Vector2f m_position;

};