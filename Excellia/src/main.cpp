
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include "AudioManager.h"
#include "WorldManager.h"
#include "EventManager.h"
#include "Settings.h"
#include "Player.h"
#include "Camera.h"
#include "Skybox.h"
#include "UI.h"

int main()
{
	sf::Font font;
	if (!font.loadFromFile("assets/Fonts/Minecraft.ttf")) {
		printf("Error finding font\n");
	}
	Button button(sf::Vector2f(200, 50), "Click Me!", font);
	button.setPosition(50, 50);
	button.set_on_click([]() {
		printf("Hello World\n");
	});

	// Create Settings
	Settings settings;


	// Create Blocks
	BlockManager blocks("Blocks");
	Block current_block;
	
	AudioManager audios;
	audios.load_from_file("Sounds");

	// Create Clock
	sf::Clock clock;


	// Create Window
	sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);
	window.setMouseCursorVisible(false);


	// Create Logo
	sf::Image logo;
	logo.loadFromFile("assets/Pixellia.png");
	window.setIcon(logo.getSize().x, logo.getSize().y, logo.getPixelsPtr());
	

	// Create World
	WorldManager world(window, settings.get_world_size(), blocks, 573849); // 573849 test seed


	// Create Player
	Player player(sf::Vector2f((float)((int)settings.get_world_size().x / 2), (float)world.place_player((int)settings.get_world_size().x / 2)), world, audios);
	
	 
	// Create Camera
	Camera camera(player.get_position(), sf::Vector2u{1080 * settings.get_screen_size().x / settings.get_screen_size().y, 1080}, player, settings.get_camera_zoom());


	// Create Cursor
	sf::RectangleShape cursor({ 1, 1 });
	sf::Texture cursor_texture;
	sf::Vector2f mouse_pos;
	cursor.setFillColor(sf::Color(255, 255, 255, 150));
	cursor_texture.loadFromImage(current_block.get_image());
	cursor.setTexture(&cursor_texture);


	// Create Healthbar
	sf::RectangleShape health_bar({ 100, 20 });
	health_bar.setPosition(sf::Vector2f(settings.get_screen_size().x - 100, 0));
	health_bar.setFillColor(sf::Color(255, 0U, 0U, 255U));


	// Create Shader
	sf::Shader shader;
	shader.loadFromFile("assets/Shaders/vertex_shader.vert", "assets/Shaders/fragment_shader.frag");


	// Create Skybox
	Skybox skybox;


	// Create Event Manager
	bool is_placing_block = false;
	bool is_breaking_block = false;
	bool is_changing_block = false;
	EventManager ev_manager(is_placing_block, is_breaking_block, is_changing_block, current_block, camera, player, world);
	ev_manager.add_button(button);
	// Main Loop
	while (window.isOpen())
	{

		// Update Delta Time
		float dt = clock.restart().asSeconds();


		// Update Player
		player.update(dt);


		// Update Camera
		camera.update(dt);
		window.setView(camera.get_view());

		
		// Update Events
		ev_manager.poll_events();

		// Update Mouse
		mouse_pos = world.screen_pos_to_world_pos(sf::Mouse::getPosition(world.get_window()));
		

		// Break Block
		if (is_breaking_block && Utils::distance(player.get_position() - sf::Vector2f(-0.5f, 1.5f), mouse_pos) <= settings.get_arm_length()) {
			world.place_block(current_block, sf::Mouse::getPosition(window), player.get_position(), player.get_shape().getSize());
		}


		// Place Block
		if (is_placing_block && Utils::distance(player.get_position() - sf::Vector2f(-0.5f, 1.5f), mouse_pos) <= settings.get_arm_length()) {
			world.break_block(sf::Mouse::getPosition(window));
		}

		// Update Current Block
		if (is_changing_block) {
			 current_block = world.get_block((sf::Vector2i)mouse_pos);
			 if (current_block.get_name() != "Void") {
				 cursor_texture.loadFromImage(current_block.get_image());
				 cursor.setTexture(&cursor_texture);
			 }
		}
		

		// Update Cursor
		cursor.setPosition((sf::Vector2f)(sf::Vector2i)(mouse_pos));


		// Update Healthbar
		health_bar.setSize(sf::Vector2f(100.0f * (float)player.get_current_health() / (float)player.get_max_health(), 100 / 8));
		health_bar.setPosition(sf::Vector2f(world.game_pos_to_screen_pos(player.get_position())) - sf::Vector2f{30.f, 130.f});
		// health_bar.setPosition(static_cast<sf::Vector2i>(world.game_pos_to_screen_pos(player.get_position())));

		// Update Shader
		shader.setUniform("playerPosition", sf::Vector2f(player.get_position().x / settings.get_world_size().x, player.get_position().y / settings.get_world_size().y));


		// Update World
		world.update_view();


		// Update Skybox
		skybox.update(dt);


		// Clear Window
		window.clear(skybox.get_sky_color());


		// Draw View
		window.draw(world, &shader);


		// Draw Player
		window.draw(player);


		// Draw Cursor
		window.draw(cursor);
		
		
		// Reset View For UI
		window.setView(window.getDefaultView());


		// Draw Healthbar
		window.draw(health_bar);

		/* Comment this out in order to not draw the button */
		window.draw(button);


		// Display Window
		window.display();

	}

	return 0;
}