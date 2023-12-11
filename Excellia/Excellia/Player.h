#pragma once

#include "Actor.h"

class Player : public Actor {

	Player(sf::Vector2f position);
	void update(float dt) override;
	sf::Drawable& render_shape() override;

};