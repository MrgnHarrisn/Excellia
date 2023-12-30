#pragma once
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
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

	/// <summary>
	/// Constructor for Client
	/// </summary>
	Client();

	/// <summary>
	/// Starts the client
	/// </summary>
	void start();
	void game();
	void recieve_packets();
	void update_server();
	void parse(sf::Packet& packet);
	void send_packet(sf::Packet& packet);
	void send_player_pos();
private:
	sf::TcpSocket m_server;
	std::string m_ip_addrs;
	unsigned short m_port;
	sf::Socket::Status m_status;

	Camera m_camera;
	Player m_player;
	std::string m_name;
	
	std::vector<sf::Vector2f> m_player_positions;
	
	Settings m_settings;
	WorldManager m_wm;

	/* Make client wait till world is ready to be played in */
	bool m_is_world_setup = false;
	std::atomic<bool> m_is_running{ true };
	sf::Shader m_shader;
	Block m_current_block = Block::Wood;
};

