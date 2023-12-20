#include "Server.h"
#include "Client.h"
#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"
#include "WorldManager.h"
#include "BlockManager.h"
#include <iostream>

int main()
{
	
	char a;
	std::cin >> a;

	if (a == 's') {
		Server server;
		server.start();
	}
	else if (a == 'l') {
		sf::Shader shader;
		if (!shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag")) {
			printf("Something went wrong!");
			return -1;
		}

		// Creates and loads settings
		Settings settings;
		settings.update();

		// Delta Time
		sf::Clock clock;
		float dt = 0;

		// Creates window
		sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);
		// window.setFramerateLimit(10);

		/* 573849 test seed */
		/* 42069 is a good seed */

		// Creates world
		WorldManager wm(window, settings.get_world_size(), 573849);
		wm.create();

		// Spawn player
		Player p(&wm);

		// Create camera attached to player
		Camera cam(p.get_position(), settings.get_screen_size(), &p, 10);

		// Set build/destroy defaults
		bool is_block_placed = false;
		bool is_placing_block = false;
		Block current_block = Block::Wood;

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
					else if (event.mouseButton.button == sf::Mouse::Middle)
					{
						Block temp_block = static_cast<Block>(wm.get_block(static_cast<sf::Vector2i>(wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window)))));
						if (temp_block != Block::Void) current_block = temp_block;
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

				// Change current_block
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
					current_block = Block::Stone;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
					current_block = Block::Dirt;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
					current_block = Block::Wood;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
					current_block = Block::Water;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
					current_block = Block::Lava;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
					current_block = Block::Diamond;
				}
			}

			// Break blocks
			if (is_block_placed) {
				wm.break_block(sf::Mouse::getPosition(window));
			}

			// Place blocks
			if (is_placing_block) {
				wm.place_block(current_block, sf::Mouse::getPosition(window));
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

			shader.setUniform("playerPosition", sf::Vector2f(p.get_position().x / settings.get_world_size().x, p.get_position().y / settings.get_world_size().y));

			// Update view
			cam.update(dt);
			window.setView(cam.get_view());

			// Clear and draw
			window.clear();
			window.draw(wm.get_render(), &shader);
			// window.draw(wm.get_render());
			window.draw(p.render_shape());
			window.draw(cursor);

			window.display();

			// fps and ms
			/*if (1 / dt > 1000) printf("fps: %.4f    ms: %.4f\n", 1 / dt, dt);
			else printf("fps: %.4f     ms: %.4f\n", 1 / dt, dt);*/
		}
	}
	else {
		Client client;
	}

	return 0;
}