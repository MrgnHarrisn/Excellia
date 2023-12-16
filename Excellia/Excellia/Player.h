#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <SFML/Window/Keyboard.hpp>

#include "WorldManager.h"
#include "BlockManager.h"
#include "Collision.hpp"
#include "Actor.h"
#include <string>
/// <summary>
/// Main character - type Actor
/// </summary>
class Player : public Actor {
public:

	/// <summary>
	/// Constructor for player
	/// </summary>
	/// <param name="position">Position the player starts at</param>
	Player(WorldManager& wm);

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

	/// <summary>
	/// Gets the world sprite
	/// </summary>
	/// <returns>World sprite</returns>
	sf::Sprite get_sprite();

	std::string get_name();

	void set_name(std::string name);

private:

	sf::RectangleShape m_shape;
	float m_move_speed = 15;
	float m_sprint_speed = 25;
	float m_speed = m_move_speed;
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	WorldManager& m_wm;

	std::string m_name;

};