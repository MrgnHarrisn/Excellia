#include "Client.h"

Client::Client() : m_is_running(true)
{
	m_settings.update();
	printf("IP: ");
	std::cin >> m_ip_addrs;
	printf("Port: ");
	std::cin >> m_port;

	m_status = m_server.connect(m_ip_addrs, m_port);

	if (m_status != sf::Socket::Done) {
		printf("Failed to connect to server\n");
	}
	m_server.setBlocking(false);
	start();

	if (!m_shader.loadFromFile("vertex_shader.vert", "fragment_shader.frag")) {
		printf("Something went wrong!");
	}

}

void Client::send_player_pos()
{
	sf::Packet packet;
	packet << "player_pos";
	packet << m_player.get_position().x;
	packet << m_player.get_position().y;
	send_packet(packet);
}

void Client::recieve_packets()
{
	while (m_is_running.load())
	{
		sf::Packet packet;

		/* Try and recieve a packet */
		sf::Socket::Status status = m_server.receive(packet);

		/* Check if we recieve a packet */
		if (status != sf::Socket::Done) {
			/* Skip the rest of the code */
			continue;
		}

		/* parse the packet's information */
		parse(packet);

		packet.clear();
	}
}

void Client::update_server()
{
	while (m_is_running.load())
	{
		
	}
}

void Client::start()
{
	// std::thread update_server(&Client::update_server, this);
	std::thread recieve_packets(&Client::recieve_packets, this);
	
	while (!m_is_world_setup)
	{
		// just sit and wait till the game is ready
	}

	game();
	m_is_running.store(false);

	// update_server.join();
	recieve_packets.join();

}

void Client::game()
{
	m_camera.attach(&m_player);
	
	sf::RenderWindow window(sf::VideoMode(m_settings.get_screen_size().x, m_settings.get_screen_size().y), "Pixellia", sf::Style::None);
	m_wm.set_render_window(&window);
	sf::Clock clock;
	sf::Texture texture;
	texture.loadFromFile("player.png");
	sf::Vector2f prev_player_pos = m_player.get_position();

	bool is_block_placed = false;
	bool is_block_break = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Right) {
					is_block_placed = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_break = true;
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					
					// Get block at mouse position
					Block temp_block = static_cast<Block>(m_wm.get_block(static_cast<sf::Vector2i>(m_wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window)))));
					if (temp_block != Block::Void) m_current_block = temp_block;
					
				}
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0) {
						m_camera.zoom_in();
					}
					else if (event.mouseWheelScroll.delta < 0) {
						m_camera.zoom_out();
					}
				}
			}
			
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					is_block_break = false;
				}
				else if (event.mouseButton.button == sf::Mouse::Right) {
					is_block_placed = false;
				}
			}

			// Change current_block
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				m_current_block = Block::Stone;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				m_current_block = Block::Dirt;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				m_current_block = Block::Wood;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
				m_current_block = Block::Water;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
				m_current_block = Block::Lava;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
				m_current_block = Block::Diamond;
			}

		}

		// Break blocks
		if (is_block_break) {
			
			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2f block_pos = m_wm.break_block(mouse_pos, false);

			/* Send Information */
			sf::Packet packet;
			packet << "break_block";
			packet << block_pos.x;
			packet << block_pos.y;
			send_packet(packet);
		}

		// Place blocks
		if (is_block_placed) {

			sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2f block_pos = m_wm.place_block(m_current_block, sf::Mouse::getPosition(window), false);

			/* Send information */
			sf::Packet packet;
			packet << "place_block";
			packet << block_pos.x;
			packet << block_pos.y;
			packet << static_cast<unsigned int>(m_current_block);
			send_packet(packet);
		}

		// Create cursor
		sf::RectangleShape cursor;
		sf::Vector2i world_pos = static_cast<sf::Vector2i>(m_wm.screen_pos_to_world_pos(sf::Mouse::getPosition(window)));

		// Update cursor
		cursor.setPosition(static_cast<sf::Vector2f>(world_pos));
		cursor.setSize({ 1, 1 });
		cursor.setFillColor(sf::Color(255, 255, 255, 50));

		float delta_time = clock.restart().asSeconds();
		
		m_player.update(delta_time);

		if (m_player.get_position() != prev_player_pos) {
			send_player_pos();
			prev_player_pos = m_player.get_position();
		}

		m_camera.update(delta_time);
		
		window.setView(m_camera.get_view());

		window.clear(sf::Color::Black);
		window.draw(m_wm.get_view_sprite());

		for (sf::Vector2f position : m_player_positions) {
			sf::RectangleShape rs;
			rs.setFillColor(sf::Color::White);
			rs.setOrigin(0, 3);
			rs.setPosition(position);
			rs.setSize({ 1,3 });
			rs.setTexture(&texture);
			window.draw(rs);
		}
		window.draw(cursor);
		window.draw(m_player.render_shape());
		window.display();
	}
}

void Client::parse(sf::Packet& packet)
{
	std::string data;
	packet >> data;

	if (data == "world_info") {
		printf("Getting world info\n");
		sf::Vector2u world_size;
		int r_seed = 0;

		packet >> r_seed;
		packet >> world_size.x;
		packet >> world_size.y;

		m_wm.set_seed(r_seed);
		m_wm.set_size(world_size);

		m_wm.create();

		m_player.create(&m_wm);
		m_camera.create(m_player.get_position(), m_settings.get_screen_size(), &m_player, 10);

		m_is_world_setup = true; // our game is ready to play
	}
	else {

		/* We only want to run stuff if the world is ready */
		if (m_is_world_setup) {
			if (data != "updated_positions")
			{
				std::cout << data << std::endl;
			}
			/* read packet information */
			if (data == "msg") {
				packet >> data;
				std::cout << data << std::endl;
			}
			else if (data == "updated_positions") {
				m_player_positions.clear();
				size_t num_players;
				size_t client_index;

				packet >> num_players;
				packet >> client_index;

				for (size_t i = 0; i < num_players; i++) {
					sf::Vector2f pos;

					packet >> pos.x;
					packet >> pos.y;

					if (i == client_index) {
						// m_player.set_position(pos);
					}
					else {
						m_player_positions.push_back(pos);
					}
				}

			}
			else if (data == "break_block") {
				/* Will be stressful with lots of people changing stuff */
				sf::Vector2f position;
				packet >> position.x;
				packet >> position.y;
				
				m_wm.break_block(static_cast<sf::Vector2i>(position), true);
			}
			else if (data == "place_block") {
				printf("SOmeone else placed a block!\n");
				sf::Vector2f position;
				unsigned int block_type;
				packet >> position.x;
				packet >> position.y;
				packet >> block_type;
				m_wm.force_place_block(static_cast<Block>(block_type), static_cast<sf::Vector2i>(position), true);

				printf("X: %f Y:%f B:%u\n", position.x, position.y, block_type);
			}
		}
	}

}

void Client::send_packet(sf::Packet& packet)
{
	sf::Socket::Status status = m_server.send(packet);

	if (status != sf::Socket::Done) {
		// printf("Failed to send packet\n");
	}
}
