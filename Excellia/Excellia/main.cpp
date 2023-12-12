
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"

int main()
{
	// Creates and loads settings
	Settings settings;
	settings.update();

	// Creates player
	sf::Vector2f position(100, 100);
	
	Player p(position);
	Camera cam(position, settings.getScreensize(), p);

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.getScreensize().x, settings.getScreensize().y), "Pixellia");

	// Main loop
	while (window.isOpen())
	{
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
		}
		p.update(0.005);
		window.setView(cam.get_view());
		// Reset and render
		cam.update();
		window.clear();
		window.draw(p.render_shape());
		window.display();

	}

	return 0;
}