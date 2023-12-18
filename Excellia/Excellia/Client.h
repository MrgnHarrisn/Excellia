#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "Settings.h"
#include "Player.h"
#include "Utils.h"
#include "Camera.h"
#include "WorldManager.h"
#include "BlockManager.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

class Client
{
public:
	Client();
	void connect();
	void run();
	void recieve_packets();
	void update();
	void parse(sf::Packet& packet);
private:
	sf::TcpSocket m_server;
	std::string m_ip_addrs;
	unsigned short m_port;

	Player m_player;
	Camera m_camera;
	std::vector<sf::Vector2f> m_player_positions;
	Settings m_settings;
	sf::Socket::Status m_status;
	std::string m_name;
	WorldManager m_wm;
};

