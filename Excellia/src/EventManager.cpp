#include "EventManager.h"

EventManager::EventManager(
							bool& is_placing_block, 
							bool& is_breaking_block,
							bool& is_changing_block,
							Block& current_block,
							Camera& camera,
							Player& player,
							WorldManager& wm) :
							m_is_placing_block(is_placing_block),
							m_is_breaking_block(is_breaking_block),
							m_is_changing_block(is_changing_block),
							m_current_block(current_block),
							m_camera(camera),
							m_player(player),
							m_wm(wm)
{}

void EventManager::poll_event(Button& button)
{
	button.update(m_event, m_wm.get_window());
}


void EventManager::poll_events()
{
	while (m_wm.get_window().pollEvent(m_event))
	{

		bool uiHandled = false;
		for (Button* b : m_buttons) {
			if (b->update(m_event, m_wm.get_window())) {
				uiHandled = true;
			}
		}

		if (uiHandled) {
			continue; // skip further processing if UI consumed the event
		}

		switch (m_event.type)
		{
		case sf::Event::Closed:
			m_wm.get_window().close();
			break;

			/* Mouse Scroll Wheel */
		case sf::Event::MouseWheelScrolled:
			if (m_event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
				if (m_event.mouseWheelScroll.delta > 0) {
					/* Zoom in */
					m_camera.zoom_in();
				}
				else if (m_event.mouseWheelScroll.delta < 0) {
					/* Zoom out */
					m_camera.zoom_out();
				}
			}
			break;

			/* Mouse Buttons Pressed */
		case sf::Event::MouseButtonPressed:
			switch (m_event.mouseButton.button) {
			case sf::Mouse::Left:
				m_is_placing_block = true;
				break;
			case sf::Mouse::Right:
				m_is_breaking_block = true;
				break;
			case sf::Mouse::Middle:
				m_is_changing_block = true;
				break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (m_event.mouseButton.button) {
			case sf::Mouse::Left:
				m_is_placing_block = false;
				break;
			case sf::Mouse::Right:
				m_is_breaking_block = false;
				break;
			case sf::Mouse::Middle:
				m_is_changing_block = false;
				break;
			}
			break;
		case sf::Event::KeyPressed:
			switch (m_event.key.code) {
			case sf::Keyboard::A:
				m_player.set_moving_left(true);
				break;
			case sf::Keyboard::D:
				m_player.set_moving_right(true);
				break;
			case sf::Keyboard::Space:
				m_player.set_jumping(true);
				break;
			case sf::Keyboard::LShift:
				m_player.set_sprinting(true);
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (m_event.key.code) {
			case sf::Keyboard::A:
				m_player.set_moving_left(false);
				break;
			case sf::Keyboard::D:
				m_player.set_moving_right(false);
				break;
			case sf::Keyboard::Space:
				m_player.set_jumping(false);
				break;
			case sf::Keyboard::LShift:
				m_player.set_sprinting(false);
				break;
			}
			break;
		default:
			break;
		}
	}
}

void EventManager::add_button(Button& btn)
{
	m_buttons.push_back(&btn);
}
