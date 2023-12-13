
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

	// Creates player
	
	
	
	
	sf::Clock clock;
	float dt = 0;

	WorldManager wm;
	wm.create(573849);

	sf::Vector2f position(400, wm.place_player(400));
	Player p(position);
	

	Camera cam(position, settings.getScreensize(), p, 10);

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.getScreensize().x, settings.getScreensize().y), "Pixellia");

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
				window.setSize(sf::Vector2u(settings.getScreensize().x, settings.getScreensize().y));
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