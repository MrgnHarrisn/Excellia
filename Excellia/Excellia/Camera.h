#pragma once

#include <SFML/Graphics/View.hpp>

#include <SFML/System/Vector2.hpp>

#include "Actor.h"

class Camera {
public:

	Camera();

	/// <summary>
	/// COnstructor for Camera
	/// </summary>
	/// <param name="position">Position of the camera</param>
	/// <param name="size">Size of the view/perspective</param>
	Camera(sf::Vector2f position, sf::Vector2u size, Actor& actor, float d_zoom);

	void create(sf::Vector2f position, sf::Vector2u size, Actor& actor, float d_zoom);

	/// <summary>
	/// Updates the camera
	/// </summary>
	void update(float dt);

	/// <summary>
	/// Sets an Actor for the camera to follow
	/// </summary>
	/// <param name="actor">Actor to follow</param>
	void attach(Actor& actor);

	/// <summary>
	/// Zooms the camera in
	/// </summary>
	void zoom_in();

	/// <summary>
	/// Zooms the camera out
	/// </summary>
	void zoom_out();

	/// <summary>
	/// Returns a reference to the camera view
	/// </summary>
	/// <returns>Returns camera view</returns>
	sf::View& get_view();

private:

	int m_zoom = 0;
	sf::View m_view;
	sf::Vector2f m_position;
	Actor& m_follow;
	float m_cam_speed = 10;
};