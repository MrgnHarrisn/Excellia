#include "Server.h"

Server::Server()
{
	m_settings.update();
	printf("Port: ");
	std::cin >> m_port;

	if (m_listener.listen(m_port) != sf::Socket::Done) {
		printf("Cannot listen on port: %i\n", m_port);
	}

	/* Create world */
	m_wm.set_size(m_settings.get_world_size());
	m_wm.set_seed(573849);

	std::cout << m_wm.get_seed() << std::endl;

	m_wm.create();

}

void Server::start()
{
	std::thread connect_clients(&Server::connect_clients, this);
	std::thread recieve_packets(&Server::recieve_packets, this);

	update_clients();

	connect_clients.join();
	recieve_packets.join();

}

void Server::update_clients()
{
	// printf("Updating clients\n");
	while (true)
	{
		for (size_t i = 0; i < m_clients.size(); i++) {
			sf::Packet packet;
			packet << "updated_positions";
			packet << m_players.size();
			packet << i;
			for (size_t j = 0; j < m_players.size(); j++) {
				packet << m_players[j].get_position().x;
				packet << m_players[j].get_position().y;
			}
			send_packet(packet, m_clients[i]);
		}
	}
}

void Server::connect_clients()
{
	printf("Connecting clients\n");
	/* Connect new clients etc. */
	while (true)
	{
		sf::TcpSocket* new_client = new sf::TcpSocket();
		if (m_listener.accept(*new_client) == sf::Socket::Done) {
			/* if we connect to the client successfully add it to the list */
			new_client->setBlocking(false);
			{
				/* Stop other threads from touching the m_clients list */
				std::lock_guard<std::mutex> lock(m_client_mutex);

				m_clients.push_back(new_client);

				/* Send world information to the client */
				printf("Added new client\n");

				Player player;
				player.create(&m_wm);
				m_players.push_back(player);

				send_world(new_client);
				
			}
		}
		else {
			delete(new_client);
		}
	}

}

void Server::recieve_packets()
{
	printf("Recieving packets\n");
	/* Recieve packets here */
	while (true)
	{
		/* Itterate over list */
		if (m_clients.size() > 0) {
			/* if we have clients recieve their packets */

			/* Recieve packets */
			for (size_t i = 0; i < m_clients.size(); i++) {
				recieve_packet(m_clients[i], i);
			}

		}
	}
}

void Server::recieve_packet(sf::TcpSocket* client, size_t index)
{
	sf::Packet packet;
	sf::Socket::Status status = client->receive(packet);

	/* Error handeling */
	if (status != sf::Socket::Done) {
		if (status == sf::Socket::Disconnected) {
			/* Disconnect the client */
			disconnect_client(client, index);
		}
		return;
	}

	/* Sends the pakcet to be parsed */
	parse(packet, client, index);

	/* Clear the packet */
	packet.clear();
}

void Server::disconnect_client(sf::TcpSocket* client, size_t index)
{
	{
		/* Stop threads from being able to touch the m_clients */
		std::lock_guard<std::mutex> lock(m_client_mutex);

		/* Disconnect the client */
		client->disconnect();

		/* Clear the memory */
		delete(client);

		/* Remove from list */
		m_clients.erase(m_clients.begin() + index);
	}
}

void Server::parse(sf::Packet& packet, sf::TcpSocket* client, size_t index)
{
	/* parse the packet information */

	std::string data;
	packet >> data;
	std::cout << data << std::endl;
	if (data == "server_msg") {
		/* Recieved a message for the server */
		packet >> data;
		std::cout << data << std::endl;
		return;
	}
	else if (data == "player_pos") {
		/* Recieving player position */
		sf::Vector2f position;
		packet >> position.x;
		packet >> position.y;
		m_players[index].set_position(position);
	}

}

void Server::send_packet(sf::Packet& packet, sf::TcpSocket* client)
{
	sf::Socket::Status status = client->send(packet);
	if (status != sf::Socket::Done) {
		printf("Failed to send packet\n");
	}
}

void Server::send_world(sf::TcpSocket* target)
{
	sf::Packet packet;
	packet << "world_info";
	packet << m_wm.get_seed();
	packet << m_wm.get_size().x;
	packet << m_wm.get_size().y;
	send_packet(packet, target);
}
