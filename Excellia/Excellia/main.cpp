
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include "WorldManager.h"
#include "Settings.h"
#include "Player.h"
#include "Camera.h"

int WinMain()
{

	// Create Settings
	Settings settings;


	// Create Blocks
	BlockManager blocks;
	blocks.create_block("Hell_Steel_Ore", 0xB54546FF, 1, true);
	blocks.create_block("Malachite_Ore", 0x44CC83FF, 1, true);
	blocks.create_block("Diamond_Ore", 0x49E0FBFF, 1, true);
	blocks.create_block("Crystal_Ore", 0xA87DD8FF, 1, true);
	blocks.create_block("Copper_Ore", 0xCA937BFF, 1, true);
	blocks.create_block("Bedrock", 0x646466FF, -1, true);
	blocks.create_block("Red_Wood", 0xA4593CFF, 1, true);
	blocks.create_block("Red_Wood", 0xA4593CFF, 1, true);
	blocks.create_block("Iron_Ore", 0xAFAFAFFF, 1, true);
	blocks.create_block("Void_Ore", 0x4DB999FF, 1, true);
	blocks.create_block("Void", 0x000000FF, -1, false);
	blocks.create_block("Water", 0x32A6DFFF, 1, false);
	blocks.create_block("Brick", 0x7C634AFF, 1, true);
	blocks.create_block("Stone", 0x928C8CFF, 1, true);
	blocks.create_block("Grass", 0x478142FF, 1, true);
	blocks.create_block("Lava", 0xE45746FF, 1, false);
	blocks.create_block("Dirt", 0x985E41FF, 1, true);
	blocks.create_block("Dirt", 0x985E41FF, 1, true);
	blocks.create_block("Wood", 0xB15E35FF, 1, true);
	blocks.create_block("Leaf", 0x6E9C6AFF, 1, true);
	blocks.create_block("Sand", 0xE8CD73FF, 1, true);
	

	// Create Clock
	sf::Clock clock;
	float dt = 0;


	// Create window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);


	// Creates world
	WorldManager world(window, settings.get_world_size(), blocks, 573849); // 573849 test seed


	// Create Player
	Player player(sf::Vector2f((float)((int)settings.get_world_size().x / 2), (float)world.place_player((int)settings.get_world_size().x / 2)), world);
	
	 
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
	Block current_block = blocks.get_by_name("Wood");


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
					if (temp_block.get_name() != "Void") current_block = temp_block;
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
				// Player Movement
				if (event.key.code == sf::Keyboard::A)
				{
					player.set_moving_left(true);
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					player.set_moving_right(true);
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					player.set_jumping(true);
				}
				else if (event.key.code == sf::Keyboard::LShift)
				{
					player.set_sprinting(true);
				}


				// Change Selected Block
				else if (event.key.code == sf::Keyboard::Num1) {
					current_block = blocks.get_by_name("Sand");
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					current_block = blocks.get_by_name("Dirt");
				}
				else if (event.key.code == sf::Keyboard::Num3) {
					current_block = blocks.get_by_name("Wood");
				}
				else if (event.key.code == sf::Keyboard::Num4) {
					current_block = blocks.get_by_name("Water");
				}
				else if (event.key.code == sf::Keyboard::Num5) {
					current_block = blocks.get_by_name("Lava");
				}
				else if (event.key.code == sf::Keyboard::Num6) {
					current_block = blocks.get_by_name("Diamond_Ore");
				}
				else if (event.key.code == sf::Keyboard::Num7) {
					current_block = blocks.get_by_name("Grass");
				}
				else if (event.key.code == sf::Keyboard::Num8) {
					current_block = blocks.get_by_name("Leaf");
				}
				else if (event.key.code == sf::Keyboard::Num9) {
					current_block = blocks.get_by_name("Brick");
				}
				else if (event.key.code == sf::Keyboard::Num0) {
					current_block = blocks.get_by_name("Red_Wood");
				}
				else if (event.key.code == sf::Keyboard::F1) {
					current_block = blocks.get_by_name("Hell_Steel_Ore");
				}
				else if (event.key.code == sf::Keyboard::F2) {
					current_block = blocks.get_by_name("Crystal_Ore");
				}
				else if (event.key.code == sf::Keyboard::F3) {
					current_block = blocks.get_by_name("Malachite_Ore");
				}
				else if (event.key.code == sf::Keyboard::F4) {
					current_block = blocks.get_by_name("Ruby_Ore");

				}
				else if (event.key.code == sf::Keyboard::F5) {
					current_block = blocks.get_by_name("Iron_Ore");

				}
				else if (event.key.code == sf::Keyboard::F6) {
					current_block = blocks.get_by_name("Copper_Ore");

				}
				else if (event.key.code == sf::Keyboard::F7) {
					current_block = blocks.get_by_name("Void_Ore");

				}
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				// Player Movement
				if (event.key.code == sf::Keyboard::A)
				{
					player.set_moving_left(false);
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					player.set_moving_right(false);
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					player.set_jumping(false);
				}
				else if (event.key.code == sf::Keyboard::LShift)
				{
					player.set_sprinting(false);
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