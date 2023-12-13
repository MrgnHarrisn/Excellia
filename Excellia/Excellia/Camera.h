#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include "Actor.h"

class Camera {
public:

	/// <summary>
	/// COnstructor for Camera
	/// </summary>
	/// <param name="position">Position of the camera</param>
	/// <param name="size">Size of the view/perspective</param>
	Camera(sf::Vector2f position, sf::Vector2u size, Actor& actor, float d_zoom);

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
	/// 
	/// </summary>
	/// <returns></returns>
	sf::View& get_view();

private:
	int m_zoom = 0;
	sf::View m_view;
	sf::Vector2f m_position;
	Actor& m_follow;
	float m_cam_speed = 100;
};