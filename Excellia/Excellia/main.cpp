
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Player.h"
#include "Utils.h"

int main()
{
	Settings settings;
	settings.update();

	sf::Vector2f position(100, 100);
	Player p(position);

	sf::RenderWindow window(sf::VideoMode(settings.getScreensize().x, settings.getScreensize().y), "Excellia");

	printf("%f",f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				settings.update();
				window.setSize(sf::Vector2u(settings.getScreensize().x, settings.getScreensize().y));
			}
		}

		window.clear();
		window.draw(p.render_shape());
		window.display();

	}

	return 0;
}