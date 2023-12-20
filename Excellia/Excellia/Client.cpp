#include "Client.h"

Client::Client()
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

	start();

}

void Client::recieve_packets()
{
	while (true)
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

	}
}

void Client::update_server()
{
	while (true)
	{

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

	update_server.join();
	recieve_packets.join();

}

void Client::game()
{
	while (true)
	{
		
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

		m_is_world_setup = true; // our game is ready to play
	}
	else {

		/* We only want to run stuff if the world is ready */
		if (m_is_world_setup) {
			/* read packet information */
		}
	}

}