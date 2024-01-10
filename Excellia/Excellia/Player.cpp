
#include "Player.h"

Player::Player(sf::Vector2f position, WorldManager& wm) : m_wm(wm)
{
    sf::RectangleShape shape;

	set_position(position);

    m_texture.loadFromFile("Textures/Frankly.png");

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
	m_velocity.x = 0;

	// Check Sprinting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		m_speed = m_sprint_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
		m_speed = m_slow_move_speed;
	}
	else {
		m_speed = m_move_speed;
	}

	// Check Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_velocity.x = 0;
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			m_velocity.x = m_speed;
			if (facing_right) {
				m_shape.setOrigin(0, 3);
				m_shape.setScale(1, 1);
				facing_right = false;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			m_velocity.x = -m_speed;
			if (!facing_right) {
				m_shape.setOrigin(1, 3);
				m_shape.setScale(-1, 1);
				facing_right = true;
			}
		}
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
	m_velocity.y += (gravity - 0.02 * m_velocity.y * m_velocity.y) * dt;
	printf("%f\n", m_velocity.y);

	// Call Collision
	sf::Vector2f pos = get_position();
	int loop_count = dt * 1000 + 1;
	sf::Vector2f temp_v = sf::Vector2f(m_velocity.x * dt / loop_count, m_velocity.y * dt / loop_count);

	for (int i = 0; i < loop_count; i++)
	{
		sf::Vector2f temp_pos = can_move_pos(pos, sf::Vector2f{ temp_v.x, temp_v.y });

		if (temp_pos == sf::Vector2f(0, 0))
		{
			break;
		}
		if (pos.y + temp_v.y < 3 && temp_v.y < 0)
		{
			temp_pos.y = 0;
		}
		if (pos.x + temp_v.x < 0 && temp_v.x < 0)
		{
			temp_pos.x = 0;
		}

		pos.x += temp_pos.x;
		pos.y += temp_pos.y;
		set_position(pos);

		if (temp_pos.y == 0 && m_velocity.y > 0)
		{
			m_can_jump = true;
		}
	}

	// Update shape position
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
	const float E = 0.1;
	m_can_jump = false;

	// Left
	if (velocity.x < 0.0f) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x - e, -0.01f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x, -1.0f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x, -2.0f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x, e - 3.0f))))) {
			velocity.x = 0.0f;
			position.x = std::floor(position.x + 0.5f - e);
		}
	}
	// Right
	else if (velocity.x > 0.0f) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x + e + 1.0f, -0.01f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x + 1.0f, -1.0f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x + 1.0f, -2.0f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(velocity.x + 1.0f, e - 3.0f))))) { 
			velocity.x = 0;
			position.x = std::floor(position.x + 0.5f - e);
		}
	}
	// Down
	if (velocity.y > 0.0f) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, velocity.y)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(1.0f - E, velocity.y))))) {
			velocity.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = 0.0f;
			m_can_jump = true;
		}
	}
	// Up
	else if (velocity.y < 0.0f) {
		if (!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(E, velocity.y + e - 3.0f)))) ||
			!BlockManager::can_move_through(m_wm.get_block((sf::Vector2i)(get_position() + sf::Vector2f(1.0f - E, velocity.y + e - 3.0f))))) {
			velocity.y = 0.0f;
			position.y = std::floor(position.y + 0.5f - e) - e;
			m_velocity.y = 0.0f;
		}
	}

    return velocity;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}
