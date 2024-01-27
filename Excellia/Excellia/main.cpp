
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include "WorldManager.h"
#include "EventManager.h"
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
	Block current_block;
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


	// Create Window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);


	// Create World
	WorldManager world(window, settings.get_world_size(), blocks, 573849); // 573849 test seed


	// Create Player
	Player player(sf::Vector2f((float)((int)settings.get_world_size().x / 2), (float)world.place_player((int)settings.get_world_size().x / 2)), world);
	
	 
	// Create Camera
	Camera cam(player.get_position(), sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, settings.get_camera_zoom());


	// Create Cursor
	sf::RectangleShape cursor({ 1, 1 });
	sf::Texture cursor_texture;
	cursor.setFillColor(sf::Color(255, 255, 255, 150));


	// Create Shader
	sf::Shader shader;
	if (!shader.loadFromFile("Shaders/vertex_shader.vert", "Shaders/fragment_shader.frag")) {
		return -1;
	}


	// Create Skybox
	Skybox skybox;


	// Create Event Manager
	bool is_placing_placed = false;
	bool is_breaking_block = false;
	EventManager ev_manager(is_placing_placed, is_breaking_block, current_block, cam, player, world);


	// Main Loop
	while (window.isOpen())
	{

		// Update Delta Time
		dt = clock.restart().asSeconds();

		
		// Update Events
		ev_manager.poll_events();


		// Break Block
		if (is_breaking_block) world.place_block(current_block, sf::Mouse::getPosition(window), player.get_position());


		// Place Block
		if (is_placing_placed) world.break_block(sf::Mouse::getPosition(window));


		// Update Current Block
		cursor_texture.loadFromImage(current_block.get_image());
		cursor.setTexture(&cursor_texture);
		

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
		window.draw(world.get_view_sprite(), &shader);


		// Draw Player
		window.draw(player.render_shape());


		// Draw Cursor
		window.draw(cursor);
		

		// Display Window
		window.display();

	}

	return 0;
}