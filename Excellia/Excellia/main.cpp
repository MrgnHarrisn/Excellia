
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include "WorldManager.h"
#include "Settings.h"
#include "Player.h"
#include "Camera.h"
#include "Skybox.h"

int main()
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
	blocks.create_block("Demon_Goo", 0x600000FF, 1, false);
	blocks.create_block("Bedrock", 0x646466FF, -1, true);
	blocks.create_block("Ruby_Ore", 0xE96665FF, 1, true);
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
	Camera cam(player.get_position(), sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, settings.get_camera_zoom());


	// Create Cursor
	sf::RectangleShape cursor;
	sf::Texture cursor_texture;
	cursor.setSize({ 1, 1 });
	cursor.setFillColor(sf::Color(255, 255, 255, 150));


	// Create Shader
	sf::Shader shader;
	if (!shader.loadFromFile("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag")) {
		return -1;
	}


	// Create Skybox
	Skybox skybox;
	

	// Set Build/Destroy Defaults
	bool is_placing_placed = false;
	bool is_breaking_block = false;
	Block current_block;


	// Main loop
	while (window.isOpen())
	{

		// Reset Delta Time
		dt = clock.restart().asSeconds();
		

		// Show blocks on cursor
		cursor_texture.loadFromImage(current_block.get_image());
		cursor.setTexture(&cursor_texture);


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
					if (event.mouseWheelScroll.delta > 0)
					{
						cam.zoom_in();
					}
					// Zoom Out
					else if (event.mouseWheelScroll.delta < 0)
					{
						cam.zoom_out();
					}
				}
			}

			// Mouse Input
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// Start Placing
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					is_placing_placed = true;
				}
				// Start Breaking
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					is_breaking_block = true;
				}
				// Select Block
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					Block& temp_block = world.get_block(static_cast<sf::Vector2i>(world.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));
					if (temp_block.get_name() != "Void") current_block = temp_block;
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				// Stop Placing
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_placing_placed = false;
				}
				// Stop Breaking
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_breaking_block = false;
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
					current_block = blocks.get_by_name("Demon_Goo");
				}
				else if (event.key.code == sf::Keyboard::Num3) {
					current_block = blocks.get_by_name("Brick");
				}
				else if (event.key.code == sf::Keyboard::Num4) {
					current_block = blocks.get_by_name("Red_Wood");
				}
				else if (event.key.code == sf::Keyboard::Num5) {
					current_block = blocks.get_by_name("Hell_Steel_Ore");
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
		if (is_breaking_block)
		{
			world.place_block(current_block, sf::Mouse::getPosition(window), player.get_position());
		}
		if (is_placing_placed)
		{
			world.break_block(sf::Mouse::getPosition(window));
		} 
		

		// Update Cursor
		cursor.setPosition((sf::Vector2f)(sf::Vector2i)(world.screen_pos_to_world_pos(sf::Mouse::getPosition(window))));


		// Update Player
		player.update(dt);


		// Update Shader
		shader.setUniform("playerPosition", sf::Vector2f(player.get_position().x / settings.get_world_size().x, player.get_position().y / settings.get_world_size().y));


		// Update Camera
		cam.update(dt);


		// Update View
		window.setView(cam.get_view());


		// Update Skybox
		skybox.update(dt);


		// Clear Window
		window.clear(skybox.get_sky_color());


		// Draw View
		// window.draw(world.get_view_sprite(), &shader);
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