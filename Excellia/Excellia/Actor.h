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
class Actor : public sf::Drawable
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

	sf::RectangleShape& get_shape();

	void load_texture(std::string file);

	void jump(float jump_force);

	void apply_gravity(float acceleration, float drag, float dt);

	void update_facing();

	void update_velocity();

	void set_position(sf::Vector2f position);
	sf::Vector2f get_position();

	void set_moving_left(bool left);
	bool get_moving_left();

	void set_moving_right(bool right);
	bool get_moving_right();

	void set_jumping(bool jump);
	bool get_jumping();

	void set_velocity(sf::Vector2f velocity);
	sf::Vector2f get_velocity();

	void set_speed(float speed);
	float get_speed();

	void set_max_health(int max_health);
	int get_max_health();

	void set_current_health(int current_health);
	void dammaged(float dammage);
	void healed(float healing);
	int get_current_health();
	bool get_dead();

	void set_armour(int armour);
	int get_armour();

	/// <summary>
	/// Checks if a given block is free to move to
	/// </summary>
	/// <param name="position">Where the player is/param>
	/// <param name="velocity">Where the player is going</param>
	/// <returns>if the block is free</returns>
	sf::Vector2f can_move(WorldManager& world, sf::Vector2f& position, sf::Vector2f velocity, sf::Vector2f size);

	/// <summary>
	/// Moves the actor, checking for collision
	/// </summary>
	/// <param name="wm">World to check for collision</param>
	/// <param name="dt">delta time</param>
	void move_with_collision(WorldManager& wm, float dt, bool fall_dammage);

	/// <summary>
	/// Moves the actor, not checking for collision
	/// </summary>
	/// <param name="dt">delta time</param>
	void move_without_collision(float dt);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(m_shape, states);
	}

	sf::RectangleShape m_shape;
	sf::Texture m_texture;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity{ 0, 0 };
	float m_speed = 10;
	bool m_moving_left = false;
	bool m_moving_right = false;
	bool m_jumping = false;
	bool m_can_jump = true;
	bool m_facing_right = false;
	bool m_dead = false;
	float m_distance_fallen = 0.0f;

	// stats
	int m_max_health = 16;
	int m_current_health = m_max_health;
	int m_armour = 0;
};