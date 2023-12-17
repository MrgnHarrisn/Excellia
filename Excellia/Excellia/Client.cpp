
#include <SFML/Graphics.hpp>

#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"
#include "WorldManager.h"
#include "BlockManager.h"
#include "Client.h"

Client::Client() {

    // Initialize client socket info
    printf("IP Add: ");
    std::cin >> m_ip_addrs;
    printf("Port number: ");
    std::cin >> m_port;

    m_status = m_server.connect(m_ip_addrs, m_port);
    if (m_status != sf::Socket::Done) {
        printf("Failed to connect to client\n");
    }
    connect();
    run();

}

void Client::connect()
{
    sf::Packet packet;

    if (m_server.receive(packet) == sf::Socket::Done) {
        printf("Receiving World!\n");
        sf::Vector2u image_size;

        packet >> image_size.x >> image_size.y;

        std::vector<sf::Uint8> pixels;
        // std::cout << image_size.x << " " << 

        size_t pixels_received = 0;
        size_t chunk_size = 256;
        size_t total_info = image_size.x * image_size.y * 4;
        while (pixels_received != total_info) {
            size_t expected_pixels = std::min(total_info - pixels_received, chunk_size);

            for (size_t i = 0; i < expected_pixels; i++) {
                sf::Uint8 color;
                packet >> color;
                pixels.push_back(color);
                pixels_received += 1;
            }

            if (m_server.receive(packet) != sf::Socket::Done) {
                printf("error getting packet\n");
                break;
            }
            else
            {
                printf("Receiving World Packet, %i/%i\n", pixels_received, total_info);
            }

            packet.clear();

        }

        printf("World Recieved!\n");

        sf::Image received_image;
        received_image.create(image_size.x, image_size.y, pixels.data());
        m_wm.set_world_image(received_image);
    }
    else {
        printf("Did not recieve packet\n");
        connect();
        // Handle initial packet receive error
    }

}

void Client::run()
{
    sf::Shader shader;
	if (!shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag")) {
		printf("Something went wrong!");
		return;
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
	sf::Vector2f position = wm.place_player();
	Player p(wm);

	// Create camera attached to player
	Camera cam(position, settings.get_screen_size(), p, 10);

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

void Client::parse(std::string& data)
{
    std::stringstream parser(data);
    std::string token;

    parser >> token;

    if (token == "world_info")
    {
        /* Something like this:
        *               SEED      W   H
            world_info 123456789 800 800
        */
        sf::Vector2u size;
        long int seed;
        parser >> seed;
        /* Set the seed */
        m_wm.set_seed(seed);
        parser >> size.x;
        parser >> size.y;

        /* Set the size */
        m_wm.set_size(size);

        /* Create the world */
        printf("Creating world!\n");
        m_wm.create();

        /* Add changes to the world */
    }
    else {
        std::cout << data << std::endl;
    }

}