
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
	BlockManager blocks("Blocks");
	Block current_block;
	

	// Create Clock
	sf::Clock clock;


	// Create Window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);


	// Create World
	WorldManager world(window, settings.get_world_size(), blocks, 573849); // 573849 test seed


	// Create Player
	Player player(sf::Vector2f((float)((int)settings.get_world_size().x / 2), (float)world.place_player((int)settings.get_world_size().x / 2)), world);
	
	 
	// Create Camera
	Camera camera(player.get_position(), sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, settings.get_camera_zoom());


	// Create Cursor
	sf::RectangleShape cursor({ 1, 1 });
	sf::Texture cursor_texture;
	cursor.setFillColor(sf::Color(255, 255, 255, 150));


	// Create Shader
	sf::Shader shader;
	shader.loadFromFile("Shaders/vertex_sader.vert", "Shaders/fragment_shaer.frag");


	// Create Skybox
	Skybox skybox;


	// Create Event Manager
	bool is_placing_placed = false;
	bool is_breaking_block = false;
	EventManager ev_manager(is_placing_placed, is_breaking_block, current_block, camera, player, world);


	// Main Loop
	while (window.isOpen())
	{

		// Update Delta Time
		float dt = clock.restart().asSeconds();

		
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
		camera.update(dt);


		// Update View
		window.setView(camera.get_view());


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