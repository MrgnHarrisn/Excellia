
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
	set_position(position);
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f{ 1, 3 });
	shape.setOrigin(0, shape.getSize().y);
	shape.setPosition(get_position());
	shape.setFillColor(sf::Color::Magenta);
	m_shape = shape;
}

void Player::update(float dt)
{
	sf::Vector2f velocity(0, 0);

	// Check Sprinting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		m_speed = m_sprint_speed;
	} else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		/* Move Right */
		velocity.x += m_speed * dt;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		/* Move Left */
		velocity.x -= m_speed * dt;
	}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        /* Move Up */
        velocity.y -= (m_speed + 30) * dt;
    }

    velocity.y += 30 * dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		/* Move Up */
	}
	velocity = can_move_pos(velocity);
	set_position(get_position() + velocity);

	m_shape.setPosition(get_position());

}

Vector2f Player::can_move_pos(Vector2f velocity)
{
    Vector2f new_velocity = velocity;

    /* Get all 4 spots */
    Vector2f new_pos = get_position() + velocity;

    Vector2i bottom_left = static_cast<Vector2i>(new_pos);
    Vector2i top_left = bottom_left;
    top_left.y -= m_shape.getSize().y;
    Vector2i top_right = top_left;
    top_right.x += m_shape.getSize().x;
    Vector2i bottom_right = top_right;
    bottom_right.y += m_shape.getSize().y;

    // Check collision in y-axis directions
    if ((!BlockManager::can_move_through((Block)m_wm.get_block(bottom_left)) ||
        !BlockManager::can_move_through((Block)m_wm.get_block(bottom_right))) && velocity.y > 0)
    {
        new_velocity.y = 0; // Stop movement in the positive y direction (falling down)
    }

    if ((!BlockManager::can_move_through((Block)m_wm.get_block(top_left)) ||
        !BlockManager::can_move_through((Block)m_wm.get_block(top_right))) && velocity.y < 0)
    {
        new_velocity.y = 0; // Stop movement in the negative y direction (jumping)
    }

    if (static_cast<int>(10*new_pos.y) == 10*static_cast<int>(new_pos.y))
    {
        top_left.y--;
        top_right.y--;
        bottom_left.y--;
        bottom_right.y--;
    }

    // Check collision in x-axis directions
    if ((!BlockManager::can_move_through((Block)m_wm.get_block(bottom_left)) ||
        !BlockManager::can_move_through((Block)m_wm.get_block(top_left))) && velocity.x < 0)
    {
        new_velocity.x = 0; // Stop movement in the negative x direction
    }

    if ((!BlockManager::can_move_through((Block)m_wm.get_block(bottom_right)) ||
        !BlockManager::can_move_through((Block)m_wm.get_block(top_right))) && velocity.x > 0)
    {
        new_velocity.x = 0; // Stop movement in the positive x direction
    }

    return new_velocity;
}


sf::Drawable& Player::render_shape()
{
	return m_shape;
}
