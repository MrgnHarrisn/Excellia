#include "Server.h"

Server::Server()
{

    Settings settings;
    settings.update();

    printf("Enter port to use: ");
    std::cin >> m_port;

	if (m_listener.listen(m_port) != sf::Socket::Done) {
		printf("Error listening\n");
	}

    /* Create world */
    m_wm.set_size(settings.get_world_size());
    m_wm.set_seed(2934786);
    m_wm.create();
    

    /* Send world information */

}

void Server::send_world(sf::TcpSocket* target)
{
    if (m_clients.size() > 0) {
        sf::Packet packet;
        const sf::Uint8* pixels = m_wm.get_pixels();
        printf("WIDTH: %i | HEIGHT: %i\n", m_wm.get_size().x, m_wm.get_size().y);
 
        packet << m_wm.get_size().x << m_wm.get_size().y;

        packet << m_wm.get_seed();

        if (target->send(packet) != sf::Socket::Done) {
            printf("Oopsie poopsie\n");
            if (target->send(packet) != sf::Socket::Done) {
                printf("Looks like it failed a second time!\n");
            }
        }

    }
}

void Server::start()
{
    std::thread connection_thread(&Server::connect_clients, this);

    run();

}

void Server::recieve_packet(sf::TcpSocket* client, size_t index)
{
    sf::Packet packet;

    /* Check if we are disconnecting from our client */
    if (client->receive(packet) == sf::Socket::Disconnected) {
        disconnect_client(client, index);
        printf("Player disconnected\n");
        return;
    }

    
    if (packet.getDataSize() > 0) {
        /* Check if the data is empty */
        parse(packet);
        /* Send out changes */
        // send_packet(packet, client->getRemoteAddress(), client->getRemotePort());

    }

}

void Server::disconnect_client(sf::TcpSocket* client, size_t index)
{
    {
        std::lock_guard<std::mutex> lock(m_client_mutex);
        client->disconnect();
        delete(client);
        m_clients.erase(m_clients.begin() + index);
    }
}

/* This will send a packet to everyone except the person who sent the changes */
void Server::send_packet(sf::Packet& packet, sf::IpAddress exclude_addy, unsigned short port)
{
    for (size_t i = 0; i < m_clients.size(); i++) {
        sf::TcpSocket* client = m_clients[i];
        if (client->getRemoteAddress() != exclude_addy || client->getRemotePort() != port) {
            if (client->send(packet) != sf::Socket::Done) {
                printf("Could not broadcast\n");
            }
        }
    }
}


void Server::run()
{
    while (true)
    {
        if (m_clients.size() > 0) {
            for (size_t i = 0; i < m_clients.size(); i++) {
                recieve_packet(m_clients[i], i);
            }

            /* Send all updated positions to clients */
            for (size_t i = 0; i < m_clients.size(); i++) {
                sf::Packet packet;
                /* Tell it how many players there are */
                packet << m_clients.size();
                /* Index of the player */
                packet << i;

                for (size_t j = 0; j < m_players.size(); j++) {
                    packet << m_players[j].get_position().x;
                    packet << m_players[j].get_position().y;
                }

                if (m_clients[i]->send(packet) != sf::Socket::Done) {
                    printf("Error sending player positions to client\n");
                }

                packet.clear();

            }
        }
    }
}

/* Must run on a thread of its own */
void Server::connect_clients()
{
    while (true)
    {
        sf::TcpSocket* new_client = new sf::TcpSocket();
        if (m_listener.accept(*new_client) == sf::Socket::Done) {
            new_client->setBlocking(false);
            {
                std::lock_guard<std::mutex> lock(m_client_mutex);
                m_clients.push_back(new_client);
                Player p;
                p.create(&m_wm);
                m_players.push_back(p);
            }
            
            std::cout << "Added new Client!" << std::endl;

            // Check if there are other clients before sending world info
            if (m_clients.size() >= 1) {
                send_world(m_clients.back());
            }
        }
        else {
            delete(new_client);
            break;
        }
    }
}

void Server::parse(sf::Packet& packet)
{
    std::string data;
    packet >> data;

    if (data == "player_pos") {
        printf("Getting new player position\n");
        sf::Vector2f updated_pos;
        std::string name;
        packet >> updated_pos.x >> updated_pos.y;
        packet >> name;
        for (Player& p : m_players) {
            if (p.get_name() == name) {
                p.set_position(updated_pos);
            }
        }
    }
}