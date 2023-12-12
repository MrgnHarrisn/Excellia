#include "Camera.h"

Camera::Camera(sf::Vector2f position, sf::Vector2i size)
{
	m_position = position;
	m_view.setSize(position);
}

void Camera::zoom_in()
{
	m_zoom += 0.5;
}

void Camera::zoom_out()
{
	m_zoom -= 0.5;
	if (m_zoom <= 0) {
		m_zoom = 0.05;
	}
}

sf::View& Camera::get_view()
{
	return m_view;
}