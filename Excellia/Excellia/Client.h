#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include "Settings.h"
#include "Player.h"
#include "WorldManager.h"
#include "Camera.h"

class Client
{
public:
	Client();
	void connect();
	void run();
	// void recieve_packets();
	void parse(std::string& data);
private:
	sf::TcpSocket m_server;
	std::string m_ip_addrs;
	unsigned short m_port;

	Player m_player;
	Camera m_camera;
	std::vector<sf::Vector2f> player_positions;

	sf::Socket::Status m_status;
	std::string m_name;
	WorldManager m_wm;
};

