
#include "Player.h"

Player::Player(sf::Vector2f position)
{
	set_position(position);
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f{ 1, 3 });
	shape.setPosition(get_position());
	shape.setFillColor(sf::Color::Magenta);
	m_shape.setOrigin(0, m_shape.getSize().y);
	m_shape = shape;
}

void Player::update(float dt)
{
	// Check Sprinting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		m_speed = m_sprint_speed;
	} else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		/* Move Right */
		sf::Vector2f new_pos = get_position();
		new_pos.x += m_speed * dt;
		set_position(new_pos);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		/* Move Left */
		sf::Vector2f new_pos = get_position();
		new_pos.x -= m_speed * dt;
		set_position(new_pos);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		/* Move Up */
	}
	m_shape.setPosition(get_position());

}

sf::Drawable& Player::render_shape()
{
	return m_shape;
}
