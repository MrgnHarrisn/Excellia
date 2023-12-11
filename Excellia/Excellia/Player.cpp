#include "Player.h"



Player::Player(sf::Vector2f position)
{
	set_position(position);
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f{ 10.0, 30.0 });
	shape.setPosition(get_position());
	shape.setFillColor(sf::Color::Magenta);
	m_shape = shape;
}

void Player::update(float dt)
{
	// TODO
}

sf::Drawable& Player::render_shape()
{
	return m_shape;
}
