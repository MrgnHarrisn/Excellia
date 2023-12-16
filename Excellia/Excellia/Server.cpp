#include "Server.h"

Server::Server()
{

    Settings settings;

    printf("Enter port to use: ");
    std::cin >> m_port;

	if (m_listener.listen(m_port) != sf::Socket::Done) {
		printf("Error listening\n");
	}

    /* Create world */
    m_wm.create();

    /* Send world information */

}

void Server::send_world(sf::TcpSocket* target)
{
    sf::Packet packet;
    const sf::Uint8* pixels = m_wm.get_pixels();
    
    packet << m_wm.get_size().x << m_wm.get_size().y;

    constexpr size_t chunk_size = 1024;
    /* Do we need the 4? x * y would be the total number of pixels no? */
    size_t total_pixels = m_wm.get_size().x * m_wm.get_size().y;
    size_t sent_pixels = 0;

    while (sent_pixels < total_pixels) {
        size_t remaining_pixels = total_pixels - sent_pixels;
        size_t pixels_to_send = std::min(remaining_pixels, chunk_size);

        sf::Packet packet;
        for (size_t i = 0; i < pixels_to_send; i++) {
            packet << pixels[sent_pixels + i];
        }

        sent_pixels += pixels_to_send;

        if (target->send(packet) != sf::Socket::Done) {
            printf("Could not send information!\n");
        }

        packet.clear();

    }

    /*for (size_t i = 0; i < m_wm.get_size().x * m_wm.get_size().y * 4; i++) {
        packet << pixels[i];
    }*/

    /*if (target->send(packet) != sf::Socket::Done) {
        printf("Could not send information!\n");
    }*/

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
        std::string data;
        packet >> data;
        packet.clear();


        packet << data;

        /* Send out changes */
        send_packet(packet, client->getRemoteAddress(), client->getRemotePort());

    }

}

void Server::disconnect_client(sf::TcpSocket* client, size_t index)
{
    client->disconnect();
    printf("Deleting the pointer\n");
    delete(client);
    printf("Erasing from vector\n");
    m_clients.erase(m_clients.begin() + index);
    printf("Disconnected successfully!\n");
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
            m_clients.push_back(new_client);
            std::cout << "Added new Client!" << std::endl;
            /* Send client world information */

            send_world(m_clients[m_clients.size()-1]);
            
        }
        else {
            delete(new_client);
            break;
        }
    }
}

void Server::parse(std::string& message)
{
    
}