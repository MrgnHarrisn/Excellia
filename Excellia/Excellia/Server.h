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
#include <mutex>

class Server
{
public:
	/// <summary>
	/// Initialization of the server
	/// </summary>
	Server();

	/// <summary>
	/// Starts the server
	/// </summary>
	void start();


	/// <summary>
	/// Parses and acts on packet information
	/// </summary>
	/// <param name="packet"></param>
	void parse(sf::Packet& packet, sf::TcpSocket* client);


	/// <summary>
	/// Sends updated information to all the clients
	/// </summary>
	void update_clients();

	/// <summary>
	/// Adds new clients to the list of clients (runs in own thread)
	/// </summary>
	void connect_clients();

	/// <summary>
	/// Recieve packets from alll clients (runs in own thread)
	/// </summary>
	void recieve_packets();

	/// <summary>
	/// Recieves a packet from a speicifc client and sned packet to parse
	/// </summary>
	/// <param name="socket">Socket to recieve from</param>
	/// <param name="index">index of the socket in the list</param>
	void recieve_packet(sf::TcpSocket* socket, size_t index);


	/// <summary>
	/// Disconnects a client
	/// </summary>
	/// <param name="client">Client that disconnected</param>
	/// <param name="index">index in the lsit of clients</param>
	void disconnect_client(sf::TcpSocket* client, size_t index);

	/// <summary>
	/// Sends a packet to a client
	/// </summary>
	/// <param name="packet">Packet to send</param>
	/// <param name="client">Client to send to packet</param>
	void send_packet(sf::Packet& packet, sf::TcpSocket* client);

	// void send_packet(sf::Packet& packet, sf::IpAddress addy, unsigned short port);

	void send_world(sf::TcpSocket* target);

private:
	sf::TcpListener m_listener;
	sf::Socket::Status m_status;
	std::mutex m_client_mutex;
	Settings m_settings;

	WorldManager m_wm;	// this will be the world everyone needs

	std::vector<Player> m_players;
	// std::map<std::string, std::unique_ptr<sf::TcpSocket>> m_clients;
	std::vector<sf::TcpSocket*> m_clients;
	unsigned short m_port;

};

