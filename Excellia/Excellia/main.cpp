
#include <SFML/Graphics.hpp>
#include "Player.h"

int main()
{
	sf::Vector2f position(10, 10);
	Player p(position);

	sf::RenderWindow window(sf::VideoMode(800, 800), "Excellia");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();
		window.draw(p.render_shape());
		window.display();

	}

	return 0;
}