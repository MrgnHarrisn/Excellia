#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Actor.h"

/// <summary>
/// Main character - type Actor
/// </summary>
class Player : public Actor {
public:

	/// <summary>
	/// Constructor for player
	/// </summary>
	/// <param name="position">Position the player starts at</param>
	Player(sf::Vector2f position, WorldManager& wm);

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
	float m_move_speed = 11;
	float m_slow_move_speed = 4;
	float m_sprint_speed = 16;
	float m_jump_force = 30;
	sf::Texture m_texture;
	WorldManager& m_wm;
	float m_gravity = 120.0f;
	float m_drag = 0.02f;
	bool m_facing_right = false;
};