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
		sf::Packet packet;
		packet << "player_pos";
		packet << m_player.get_position().x;
		packet << m_player.get_position().y;
		send_packet(packet);
	}
}

void Client::start()
{
	std::thread update_server(&Client::update_server, this);
	std::thread recieve_packets(&Client::recieve_packets, this);
	
	while (!m_is_world_setup)
	{
		// just sit and wait till the game is ready
	}

	game();
	m_is_running.store(false);

	update_server.join();
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

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		float delta_time = clock.restart().asSeconds();
		
		m_player.update(delta_time);

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
