
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"
#include "WorldManager.h"

int main()
{
	// Creates and loads settings
	Settings settings;
	settings.update();
	
	
	sf::Clock clock;
	float dt = 0;

	WorldManager wm(settings.get_world_size());
	/* 573849 test seed */
	wm.create(573849);

	sf::Vector2f position(settings.get_world_size().x / 2, wm.place_player(settings.get_world_size().x / 2));
	Player p(position);
	

	Camera cam(position, settings.get_screen_size(), p, 10);

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia");

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
		}
		p.update(dt);
		window.setView(cam.get_view());
		// Reset and render
		cam.update(dt);
		window.clear();
		wm.get_render(window);
		window.draw(p.render_shape());
		
		window.display();

		// fps and ms
		if (1 / dt > 1000) printf("fps: %.4f    ms: %.4f\n", 1 / dt, dt);
		else printf("fps: %.4f     ms: %.4f\n", 1 / dt, dt);
	}

	return 0;
}