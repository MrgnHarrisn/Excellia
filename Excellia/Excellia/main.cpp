
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
	
	
	sf::Clock clock;
	float dt = 0;

	WorldManager wm(settings.get_world_size(), 573849);
	/* 573849 test seed */
	/* Crashes with 83875675 */
	/* 42069 is a good seed */
	/* Spawning inside dirt 6911 */
	wm.create();

	sf::Vector2f position(settings.get_world_size().x / 2, wm.place_player(settings.get_world_size().x / 2));
	Player p(position, wm);
	

	Camera cam(position, settings.get_screen_size(), p, 10);

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);

	// Main loop
	while (window.isOpen())
	{

		// Delta time
		dt = clock.restart().asSeconds();

		// Events and inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				window.setSize(sf::Vector2u(settings.get_screen_size().x, settings.get_screen_size().y));
			}
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
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					wm.break_block(window, sf::Mouse::getPosition(window));
				}
			}
		}

		RectangleShape shape;
		Vector2i world_pos = static_cast<Vector2i>(wm.screen_pos_to_world_pos(window, sf::Mouse::getPosition(window)));
		shape.setPosition(static_cast<Vector2f>(world_pos));
		// shape.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(), cam.get_view()));
		shape.setSize({ 1, 1 });
		shape.setFillColor(sf::Color::White);

		p.update(dt);
		window.setView(cam.get_view());
		// Reset and render
		cam.update(dt);
		window.clear();
		wm.get_render(window);
		window.draw(p.render_shape());
		window.draw(shape);
		
		window.display();

		// fps and ms
		if (1 / dt > 1000) printf("fps: %.4f    ms: %.4f\n", 1 / dt, dt);
		else printf("fps: %.4f     ms: %.4f\n", 1 / dt, dt);
	}

	return 0;
}