#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include "WorldManager.h"
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include "Settings.h"

class Client
{
public:
	Client();
	void connect();
	void run();
	// void recieve_packets();
	void parse(std::string& data);
	void recieve_packet();
private:
	sf::TcpSocket m_server;
	std::string m_ip_addrs;
	unsigned short m_port;

	sf::Socket::Status m_status;
	std::string m_name;
	WorldManager m_wm;
};

