#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "WorldManager.h"
#include "BlockManager.h"
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

	/// <summary>
	/// Checks if a given block is free to move to
	/// </summary>
	/// <param name="position">Where the player is/param>
	/// <param name="velocity">Where the player is going</param>
	/// <returns>if the block is free</returns>
	sf::Vector2f can_move_pos(sf::Vector2f &position, sf::Vector2f velocity);

	void set_moving_left(bool left);

	void set_moving_right(bool right);

	void set_jumping(bool jump);
	
	void set_sprinting(bool sprint);

private:

	sf::RectangleShape m_shape;
	float m_move_speed = 13;
	float m_slow_move_speed = 4;
	float m_sprint_speed = 25;
	float m_speed = m_move_speed;
	float m_jump_force = 30;
	sf::Texture m_texture;
	WorldManager& m_wm;
	float m_gravity = 120;
	float m_drag = 0.02;
	bool m_can_jump = true;
	sf::Vector2f m_velocity{ 0,0 };
	bool m_facing_right = false;
	bool m_moving_left = false;
	bool m_moving_right = false;
	bool m_jumping = false;
	bool m_sprinting = false;
};