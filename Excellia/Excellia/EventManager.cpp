#include "EventManager.h"

EventManager::EventManager(sf::RenderWindow& window) : m_window(window)
{

}

void EventManager::poll_events()
{
	while (m_window.pollEvent(m_event))
	{
		switch (m_event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;

		/* Mouse Scroll Wheel */
		case sf::Event::MouseWheelScrolled:
			if (m_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				if (m_event.mouseWheelScroll.delta > 0) {
					/* Zoom in */
				}
				else if (m_event.mouseWheelScroll.delta < 0) {
					/* Zoom out */
				}
			}
			break;

		/* Mouse Buttons Pressed */
		case sf::Event::MouseButtonPressed:
			switch (m_event.mouseButton.button) {
			case sf::Mouse::Left:
				break;
			case sf::Mouse::Right:
				break;
			case sf::Mouse::Middle:
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (m_event.mouseButton.button) {
			case sf::Mouse::Left:
					break;
			case sf::Mouse::Right:
					break;
			}
			break;
		case sf::Event::KeyPressed:
			switch (m_event.key.code) {
			case sf::Keyboard::A:
				break;
			case sf::Keyboard::D:
				break;
			case sf::Keyboard::Space:
				break;
			case sf::Keyboard::LShift:
				break;
			}
			break;
		}
	}
}