#pragma once

#include "Actor.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

class Player : public Actor {
public:

	/// <summary>
	/// COnstructor for player
	/// </summary>
	/// <param name="position">Position the player starts at</param>
	Player(sf::Vector2f position);

	/// <summary>
	/// Override of the update function
	/// </summary>
	/// <param name="dt">Delta time</param>
	void update(float dt) override;

	/// <summary>
	/// Get's the render shape of the player
	/// </summary>
	/// <returns>sf::Drawable& of player's shape</returns>
	sf::Drawable& render_shape() override;

private:
	sf::RectangleShape m_shape;
	float m_move_speed = 15;
	float m_sprint_speed = 25;
	float m_speed = m_move_speed;
};