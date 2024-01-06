
#include <SFML/Graphics.hpp>

#include "WorldManager.h"
#include "BlockManager.h"
#include "Settings.h"
#include "Player.h"
#include "Camera.h"
#include "Utils.h"

int main()
{

	// Load Shader
	sf::Shader shader;
	if (!shader.loadFromFile("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag")) {
		printf("Something went wrong!");
		return -1;
	}


	// Load Settings
	Settings settings;
	settings.update();
	

	// Delta Time
	sf::Clock clock;
	float dt = 0;


	// Day/Night Cycle
	float end_of_day_time = 5 * 60;
	float current_time = 0;

	sf::Color night_color(0, 0, 40);		// Dark blue for night sky
	sf::Color day_color(135, 206, 235);		// Light blue for daylight sky
	sf::Color current_color = night_color;
	bool is_transitioning = true;


	// Create window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);
	// window.setFramerateLimit(30);

	// Creates world
	WorldManager wm(window, settings.get_world_size(), 573849); // 573849 test seed, 42069 is a good seed
	wm.create();


	// Create Player
	sf::Vector2f position((float)settings.get_world_size().x / 2, (float)wm.place_player(settings.get_world_size().x / 2));
	Player player(position, wm);
	

	// Create Camera
	Camera cam(position, sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, 20);


	// Set Build/Destroy Defaults
	bool is_block_placed = false;
	bool is_placing_block = false;
	Block current_block = Block::Wood;


	// Create Cursor
	sf::RectangleShape cursor;


	// Main loop
	while (window.isOpen())
	{

		// Reset Delta Time
		dt = clock.restart().asSeconds();
		

		// Events And Inputs
		sf::Event event;
		while (window.pollEvent(event))
		{

			// Close Window
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// Zoom In/Out
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

			// Start Placing/Breaking
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					Block temp_block = wm.get_block(static_cast<sf::Vector2i>(wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));
					if (temp_block != Block::Void) current_block = temp_block;
				}
			}

			// Stop Placing/Breaking
			 if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = false;
				}
			}

			// Change Selected Block
			 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				 current_block = Block::Stone;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				 current_block = Block::Dirt;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				 current_block = Block::Wood;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				 current_block = Block::Water;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				 current_block = Block::Lava;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
				 current_block = Block::Diamond;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
				 current_block = Block::Grass;
			 } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
				 current_block = Block::Leaf;
			 }

		}


		// Place/Break Block
		if (is_block_placed) {
			wm.break_block(sf::Mouse::getPosition(window));
		} else if (is_placing_block) {
			wm.place_block(current_block, sf::Mouse::getPosition(window));
		}


		// Update Cursor
		cursor.setPosition((sf::Vector2f)(sf::Vector2i)(wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));
		cursor.setSize({ 1, 1 });
		cursor.setFillColor(sf::Color(255, 255, 255, 50));


		// Update Player
		player.update(dt);


		// Update Shader
		shader.setUniform("playerPosition", sf::Vector2f(player.get_position().x / settings.get_world_size().x, player.get_position().y / settings.get_world_size().y));


		// Update Camera
		cam.update(dt);
		window.setView(cam.get_view());


		// Update Skybox
		current_time += dt;
		
		float transition_percentage = current_time / end_of_day_time;

		if (transition_percentage > 1.0f) {
			transition_percentage = 1.0f;
		}

		if (current_time <= end_of_day_time / 2) {
			current_color.r = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.r + transition_percentage * day_color.r);
			current_color.g = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.g + transition_percentage * day_color.g);
			current_color.b = static_cast<sf::Uint8>((1.0f - transition_percentage) * night_color.b + transition_percentage * day_color.b);
		}
		else {
			current_color.r = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.r + transition_percentage * night_color.r);
			current_color.g = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.g + transition_percentage * night_color.g);
			current_color.b = static_cast<sf::Uint8>((1.0f - transition_percentage) * day_color.b + transition_percentage * night_color.b);
		}

		if (current_time >= end_of_day_time) {
			current_time = 0.0f;
		}


		// Clear Window
		window.clear(current_color);


		// Render View
		wm.get_view_sprite();


		// Draw Player
		window.draw(player.render_shape());


		// Draw Cursor
		window.draw(cursor);
		

		// Display Window
		window.display();


		// FPS And MS
		/*if (1 / dt > 1000) printf("fps: %.4f    ms: %.4f\n", 1 / dt, dt);
		else printf("fps: %.4f     ms: %.4f\n", 1 / dt, dt);*/

	}

	return 0;
}