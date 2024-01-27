#pragma once

#include <SFML/Graphics.hpp>

class EventManager
{
public:
	EventManager(sf::RenderWindow& window);
	void poll_events();
private:
	sf::RenderWindow& m_window;
	sf::Event m_event;
};