
#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"
#include "WorldManager.h"
#include "BlockManager.h"

int main()
{
	// Creates and loads settings
	Settings settings;
	settings.update();
	
	// Delta Time
	sf::Clock clock;
	float dt = 0;

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);

	/* 573849 test seed */
	/* Crashes with 83875675 */
	/* 42069 is a good seed */
	/* Spawning inside dirt 6911 */

	// Creates world
	WorldManager wm(window, settings.get_world_size(), 573849);
	wm.create();

	// Spawn player
	sf::Vector2f position(settings.get_world_size().x / 2, wm.place_player(settings.get_world_size().x / 2));
	Player p(position, wm);
	
	// Create camera attached to player
	Camera cam(position, settings.get_screen_size(), p, 10);

	// Set mouse bools
	bool is_block_placed = false;
	bool is_placing_block = false;

	// Main loop
	while (window.isOpen())
	{
		// Delta time
		dt = clock.restart().asSeconds();
		
		// Events and inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// Zoom in and out
			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0) {
						cam.zoom_in();
					}
					else if (event.mouseWheelScroll.delta < 0) {
						cam.zoom_out();
					}
				}
			}

			// Place and break start
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = true;
				}
			}

			// Place and break stop
			 if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = false;
				}
			}
		}

		// Break blocks
		if (is_block_placed) {
			wm.break_block(window, sf::Mouse::getPosition(window));
		}

		// Place blocks
		if (is_placing_block) {
			wm.place_block(sf::Mouse::getPosition(window));
		}

		// Create cursor
		sf::RectangleShape cursor;
		sf::Vector2i world_pos = static_cast<sf::Vector2i>(wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window)));

		// Update cursor
		cursor.setPosition(static_cast<sf::Vector2f>(world_pos));
		cursor.setSize({ 1, 1 });
		cursor.setFillColor(sf::Color(255, 255, 255, 50));

		// Update player
		p.update(dt);

		// Update view
		cam.update(dt);
		window.setView(cam.get_view());

		// Clear and draw
		window.clear();
		wm.get_render();
		window.draw(p.render_shape());
		window.draw(cursor);
		
		window.display();

		// fps and ms
		if (1 / dt > 1000) printf("fps: %.4f    ms: %.4f\n", 1 / dt, dt);
		else printf("fps: %.4f     ms: %.4f\n", 1 / dt, dt);
	}

	return 0;
}