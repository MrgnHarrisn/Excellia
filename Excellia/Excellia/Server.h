#pragma once

#include <SFML/Network.hpp>
#include "WorldManager.h"
#include "Actor.h"
#include "Player.h"
#include "Settings.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <map>

class Server
{
public:
	Server();
	void start();
	void run();
	void parse(std::string& data);
	void connect_clients();
	void recieve_packet(sf::TcpSocket* socket, size_t index);
	void disconnect_client(sf::TcpSocket* client, size_t index);
	void send_packet(sf::Packet& packet, sf::IpAddress addy, unsigned short port);

	void send_world(sf::TcpSocket* target);

private:
	sf::TcpListener m_listener;
	sf::TcpSocket m_client;
	sf::Socket::Status m_status;

	WorldManager m_wm;	// this will be the world everyone needs

	std::vector<Player> m_players;
	// std::map<std::string, std::unique_ptr<sf::TcpSocket>> m_clients;
	std::vector<sf::TcpSocket*> m_clients;
	unsigned short m_port;
	sf::SocketSelector m_selector;

};

