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
	/// Sets if the player is sprinting
	/// </summary>
	/// <param name="sprint">is the player sprinting</param>
	void set_sprinting(bool sprint);

private:
	WorldManager& m_wm;
	bool m_sprinting = false;
	float m_slow_move_speed = 4;
	float m_sprint_speed = 16;
	float m_move_speed = 11;
	float m_jump_force = 30;
	float m_gravity = 120.0f;
	float m_drag = 0.02f;
};