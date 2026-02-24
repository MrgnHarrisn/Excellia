#pragma once

#include <SFML/Graphics.hpp>
#include "BlockManager.h"
#include "WorldManager.h"
#include "Player.h"
#include "Camera.h"
#include "UI.h"

class EventManager
{
public:
	EventManager(bool& is_placing_block, bool& is_breaking_block, bool& is_changing_block, Block& current_block, Camera& camera, Player& player, WorldManager& world_manager);
	void poll_event(Button& button);
	void poll_events();
	void add_button(Button& btn);
private:
	std::vector<Button*> m_buttons;
	sf::Event m_event;
	bool& m_is_placing_block;
	bool& m_is_breaking_block;
	bool& m_is_changing_block;
	Block& m_current_block;
	Camera& m_camera;
	Player& m_player;
	WorldManager& m_wm;
};