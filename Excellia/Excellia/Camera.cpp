#include "Camera.h"

Camera::Camera(sf::Vector2f position, sf::Vector2u size, Actor& actor) : m_follow(actor) {
	m_position = position;
	m_view.setSize(static_cast<sf::Vector2f>(size));
	m_view.zoom(m_zoom);
}



void Camera::attach(Actor& actor)
{
	m_follow = actor;
}

void Camera::update()
{

	sf::Vector2f newPosition = m_position + (m_follow.get_position() - m_position) * m_cam_speed;

	m_position = newPosition;
	m_view.zoom(m_zoom);
	m_view.setCenter(m_position);
}

void Camera::zoom_out()
{
	m_view.zoom(1.2);
}

void Camera::zoom_in()
{
	m_view.zoom(1 / 1.2);
}

sf::View& Camera::get_view()
{
	return m_view;
}