
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
    sf::Sprite player_sprite;
    player_sprite.setTexture(m_texture);
    player_sprite.setPosition(get_position());
    return player_sprite;
}

sf::Vector2f Player::can_move_pos(sf::Vector2f &position, sf::Vector2f velocity)
{
    sf::Vector2f new_pos = get_position() + velocity;

	sf::Sprite view = m_wm.get_view_sprite();
	sf::Sprite player_sprite = get_sprite();

	if (Collision::PixelPerfectTest(player_sprite, view, 255)) {
		printf("Colliding\n");
	}

    return velocity;
}


sf::Drawable& Player::render_shape()
{
    return m_shape;
}
