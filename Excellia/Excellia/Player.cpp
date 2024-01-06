
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
    sf::RectangleShape shape;

	set_position(position);

    m_texture.loadFromFile("Textures/player.png");

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

	// Check Sprinting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		m_speed = m_sprint_speed;
	} else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_velocity.x = m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_velocity.x = -m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_velocity.y = -m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_velocity.y = m_speed;
	}

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_can_jump) {
		jump(dt);
	}

    // Apply Gravity
	m_velocity.y += gravity * dt;

	// Update Velocity
	// m_velocity *= dt;

    // Call Collision
    sf::Vector2f pos = get_position();
	pos += can_move_pos(pos, m_velocity * dt);
	
    // Update shape position
	set_position(pos);
	m_velocity.x = 0;
	m_shape.setPosition(get_position());
}

sf::Sprite Player::get_sprite()
{
    return m_sprite;
}

void Player::jump(float dt)
{
	/* Downward is positive, upwards is negative */
	m_velocity.y = -m_jump_force;
	m_jump_timer = 0.5;
	m_can_jump = false;
}

sf::Vector2f Player::can_move_pos(sf::Vector2f &position, sf::Vector2f velocity)
{
	const float e = 0.01f;

	/* For X */
	if (velocity.x < 0) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 0.01f)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f - e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 1)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 2)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(0, 3-e)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
	}
	else if (velocity.x > 0) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 0.01f)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 1)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 2)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)((get_position() - sf::Vector2f(-1, 3-e)) + sf::Vector2f(velocity.x, 0))))) {
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f-e);
		}
	}

	/* For Y */
	if (velocity.y > 0)
	{
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 0) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e) - e;
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 0) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e) - e;
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 3-e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e) - e;
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 3-e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e) - e;
		}
	}
	else if (velocity.y < 0)
	{
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(0-e, 3-e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e);
		}
		else if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() - sf::Vector2f(e-1, 3-e) + sf::Vector2f(0, velocity.y))))) {
			velocity.y = 0;
			position.y = std::floor(position.y + 0.5f-e);
		}
	}
	if (velocity.y == 0) {
		m_can_jump = true;
		m_velocity.y = 0;
	}
    return velocity;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}
