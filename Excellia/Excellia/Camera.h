#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>

class Camera {
public:

	/// <summary>
	/// COnstructor for Camera
	/// </summary>
	/// <param name="position">Position of the camera</param>
	/// <param name="size">Size of the view/perspective</param>
	Camera(sf::Vector2f position, sf::Vector2i size);

	/// <summary>
	/// Zooms the camera in
	/// </summary>
	void zoom_in();

	/// <summary>
	/// Zooms the camera out
	/// </summary>
	void zoom_out();

	sf::View& get_view();

private:
	float m_zoom = 1.0f;
	sf::View m_view;
	sf::Vector2f m_position;
};