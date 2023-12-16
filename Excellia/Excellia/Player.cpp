
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
    sf::RectangleShape shape;

    // Set start position
	set_position(position);

    // Load player texture
    m_texture.loadFromFile("player.png");

    // Create shape
	shape.setSize(sf::Vector2f(1, 3));
	shape.setOrigin(0, 3);
	shape.setPosition(get_position());
    shape.setTexture(&m_texture);
    
    // Store shape
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
		velocity.x += m_speed * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x -= m_speed * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		velocity.y -= m_speed * dt;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		velocity.y += m_speed * dt;
	}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        velocity.y -= (m_move_speed + 30) * dt;
		
    }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		/* Move Up */
	}

    // Temp gravity
    velocity.y += 30 * dt;

    // Call temp collision
    sf::Vector2f pos = get_position();
	velocity = can_move_pos(pos, velocity);
	set_position(pos + velocity);

    // Update shape position
	m_shape.setPosition(get_position());
}

sf::Sprite Player::get_sprite()
{
    m_sprite.setTexture(m_texture);
    m_sprite.setPosition(get_position());
    return m_sprite;
}

sf::Vector2f Player::can_move_pos(sf::Vector2f &position, sf::Vector2f velocity)
{
	sf::Vector2f new_pos_x = get_position() + sf::Vector2f(velocity.x, 0);
	sf::Vector2f new_pos_y = get_position() + sf::Vector2f(0, velocity.y);

	/*printf("Position (View): %f, %f\n", m_wm.get_view_sprite().getPosition().x, m_wm.get_view_sprite().getPosition().y);
	printf("Position (Player): %f, %f\n", get_position().x, get_position().y);*/

	/* Could use AABB collision to make it better and use sf::Vector2f instead to be more precise */

	/* For X */
	if (velocity.x < 0) {
		if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 0)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 1)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 2)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
	}
	else if (velocity.x > 0) {
		if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 0)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 1)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 2)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
		}
	}

	/* For Y*/
	if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0, 0) + sf::Vector2f(0, velocity.y))) != Block::Void) {
		velocity.y = 0;
	}
	else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(-1, 0) + sf::Vector2f(0, velocity.y))) != Block::Void) {
		velocity.y = 0;
	}
	else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(-1, 3) + sf::Vector2f(0, velocity.y))) != Block::Void) {
		velocity.y = 0;
	} else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0, 3) + sf::Vector2f(0, velocity.y))) != Block::Void) {
		velocity.y = 0;
	}

    return velocity;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}
