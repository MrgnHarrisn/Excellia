
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Player.h"
#include "Utils.h"

int main()
{
	// Creates and loads settings
	Settings settings;
	settings.update();

	// Creates player
	sf::Vector2f position(100, 100);
	Player p(position);

	// Creates window
	sf::RenderWindow window(sf::VideoMode(settings.getScreensize().x, settings.getScreensize().y), "Excellia");

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
		
		// Reset and render
		window.clear();
		window.draw(p.render_shape());
		window.display();

	}

	return 0;
}