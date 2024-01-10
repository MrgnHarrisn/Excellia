
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include "WorldManager.h"
#include "Settings.h"
#include "Player.h"
#include "Camera.h"

int main()
{

	// Create Settings
	Settings settings;
	

	// Create Clock
	sf::Clock clock;
	float dt = 0;


	// Create window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);


	// Creates world
	WorldManager world(window, settings.get_world_size(), 573849); // 573849 test seed


	// Create Player
	Player player(sf::Vector2f((int)settings.get_world_size().x / 2, (float)world.place_player(settings.get_world_size().x / 2.0f)), world);
	
	 
	// Create Camera
	Camera cam(player.get_position(), sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, 35);


	// Create Cursor
	sf::RectangleShape cursor;
	cursor.setSize({ 1, 1 });
	cursor.setFillColor(sf::Color(255, 255, 255, 50));


	// Create Shader
	sf::Shader shader;
	if (!shader.loadFromFile("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag")) {
		printf("Something went wrong!");
		return -1;
	}


	// Set Day/Night Cycle Defaults
	bool is_transitioning = true;
	float end_of_day_time = 5 * 60;
	float current_time = 0;
	sf::Color night_color(0, 0, 40);		// Dark blue for night sky
	sf::Color day_color(135, 206, 235);		// Light blue for daylight sky
	sf::Color current_color = night_color;
	

	// Set Build/Destroy Defaults
	bool is_block_placed = false;
	bool is_placing_block = false;
	Block current_block = Block::Wood;


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

			// Scroll Input
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					// Zoom In
					if (event.mouseWheelScroll.delta > 0) {
						cam.zoom_in();
					}
					// Zoom Out
					else if (event.mouseWheelScroll.delta < 0) {
						cam.zoom_out();
					}
				}
			}

			// Mouse Input
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// Start Placing
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = true;
				}
				// Start Breaking
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = true;
				}
				// Select Block
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					Block temp_block = world.get_block(static_cast<sf::Vector2i>(world.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));
					if (temp_block != Block::Void) current_block = temp_block;
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				// Stop Placing
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_placed = false;
				}
				// Stop Breaking
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_placing_block = false;
				}
			}

			// Keyboard Input
			if (event.type == sf::Event::KeyPressed)
			{
				// Change Selected Block
				if (event.key.code == sf::Keyboard::Num1) {
					current_block = Block::Stone;
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					current_block = Block::Dirt;
				}
				else if (event.key.code == sf::Keyboard::Num3) {
					current_block = Block::Wood;
				}
				else if (event.key.code == sf::Keyboard::Num4) {
					current_block = Block::Water;
				}
				else if (event.key.code == sf::Keyboard::Num5) {
					current_block = Block::Lava;
				}
				else if (event.key.code == sf::Keyboard::Num6) {
					current_block = Block::Diamond_Ore;
				}
				else if (event.key.code == sf::Keyboard::Num7) {
					current_block = Block::Grass;
				}
				else if (event.key.code == sf::Keyboard::Num8) {
					current_block = Block::Leaf;
				}
				else if (event.key.code == sf::Keyboard::Num9) {
					current_block = Block::Brick;
				}
				else if (event.key.code == sf::Keyboard::Num0) {
					current_block = Block::Red_Wood;
				}
				else if (event.key.code == sf::Keyboard::F1) {
					current_block = Block::Hell_Steel_Ore;
				}
				else if (event.key.code == sf::Keyboard::F2) {
					current_block = Block::Crystal_Ore;
				}
				else if (event.key.code == sf::Keyboard::F3) {
					current_block = Block::Malachite_Ore;
				}
				else if (event.key.code == sf::Keyboard::F4) {
					current_block = Block::Ruby_Ore;
				}
				else if (event.key.code == sf::Keyboard::F5) {
					current_block = Block::Iron_Ore;
				}
				else if (event.key.code == sf::Keyboard::F6) {
					current_block = Block::Copper_Ore;
				}
				else if (event.key.code == sf::Keyboard::F7) {
					current_block = Block::Void_Ore;
				}
			}
		}


		// Place/Break Block
		if (is_block_placed) {
			world.break_block(sf::Mouse::getPosition(window));
		} else if (is_placing_block) {
			world.place_block(current_block, sf::Mouse::getPosition(window), player.get_position());
		}


		// Update Cursor
		cursor.setPosition((sf::Vector2f)(sf::Vector2i)(world.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));


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
		window.draw(world.get_view_sprite());


		// Draw Player
		window.draw(player.render_shape());


		// Draw Cursor
		window.draw(cursor);
		

		// Display Window
		window.display();

	}

	return 0;
}