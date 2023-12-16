
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
		velocity.x += m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocity.x -= m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		velocity.y -= m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		velocity.y += m_speed;
	}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        velocity.y -= m_move_speed + 30;
		
    }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		/* Move Up */
	}

    // Temp gravity
    velocity.y += 30;

    // Call collision
    sf::Vector2f pos = get_position();
	sf::Vector2f temp_v = sf::Vector2f(velocity.x *= 0.0001, velocity.y *= 0.0001);
	
	while (dt > 0.0001)
	{
		pos += can_move_pos(pos, temp_v);
		set_position(pos);
		dt -= 0.0001;
	}

	velocity.x *= dt;
	velocity.y *= dt;

	pos += can_move_pos(pos, velocity);
	set_position(pos);

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
	const float e = 0.0001;

	/* For X */
	if (velocity.x < 0) {
		if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 0.01)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 1)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 2)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 3-e)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
	}
	else if (velocity.x > 0) {
		if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 0.01)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 1)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 2)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 3-e)) + sf::Vector2f(velocity.x, 0))) != Block::Void) {
			velocity.x = 0;
			position.x = static_cast<int>(position.x + 0.5-e);
		}
	}

	/* For Y*/
	if (velocity.y > 0)
	{
		if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 0) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e) - e;
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 0) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e) - e;
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 3-e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e) - e;
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 3-e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e) - e;
		}
	}
	else if (velocity.y < 0)
	{
		if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 3-e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e);
		}
		else if (m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 3-e) + sf::Vector2f(0, velocity.y))) != Block::Void) {
			velocity.y = 0;
			position.y = static_cast<int>(position.y + 0.5-e);
		}
	}

    return velocity;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}
