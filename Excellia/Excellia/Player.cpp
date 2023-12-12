
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		/* Move Right */
		sf::Vector2f new_pos = get_position();
		new_pos.x += m_move_speed * dt;
		set_position(new_pos);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		/* Move Left */
		sf::Vector2f new_pos = get_position();
		new_pos.x -= m_move_speed * dt;
		set_position(new_pos);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		/* Move Up */
	}
}

sf::Drawable& Player::render_shape()
{
	m_shape.setPosition(get_position());
	m_shape.setOrigin(m_shape.getSize().x / 2, m_shape.getSize().y / 2);
	return m_shape;
}
