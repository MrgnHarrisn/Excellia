#include "Client.h"

Client::Client() {

    m_settings.update();

    // Initialize client socket info
    printf("IP Add: ");
    std::cin >> m_ip_addrs;
    printf("Port number: ");
    std::cin >> m_port;

    m_status = m_server.connect(m_ip_addrs, m_port);
    if (m_status != sf::Socket::Done) {
        printf("Failed to connect to client\n");
    }
    connect();
    std::thread update(&Client::update, this);
    std::thread reception(&Client::recieve_packets, this);
    run();

}

void Client::connect()
{
    sf::Packet packet;

    if (m_server.receive(packet) == sf::Socket::Done) {
        printf("Receiving World!\n");
        sf::Vector2u image_size;
        int r_seed;

        packet >> image_size.x >> image_size.y;
        packet >> r_seed;
        
        
        m_wm.set_size(image_size);
        m_wm.set_seed(r_seed);
        printf("%i\n", static_cast<int>(m_wm.get_seed()));
        m_wm.create();

        m_player.create(&m_wm);
        m_camera.create(m_wm.place_player(), m_settings.get_screen_size(), &m_player, 10);
    }
    else {
        printf("Did not recieve packet\n");
        connect();
        // Handle initial packet receive error
    }
    printf("Done\n");
}

void Client::run()
{
    printf("Running\n");

    m_camera.attach(&m_player);

    sf::RenderWindow window(sf::VideoMode(m_settings.get_screen_size().x, m_settings.get_screen_size().y), "Pixellia", sf::Style::None);
    m_wm.set_render_window(&window);
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        float delta_time = clock.restart().asSeconds();

        m_player.update(delta_time);
        m_camera.update(delta_time);

        window.setView(m_camera.get_view());

        window.clear(sf::Color::Black);
        window.draw(m_wm.get_view_sprite());

        for (sf::Vector2f position : m_player_positions) {
            Player p;
            p.set_position(position);
            window.draw(p.render_shape());
        }

        window.draw(m_player.render_shape());
        window.display();
    }

}

void Client::recieve_packets()
{
    sf::Packet packet;
    while (true)
    {
        printf("Checking for packets\n");
        /* if we recieved a packet */
        if (m_server.receive(packet) == sf::Socket::Done) {
            printf("Recieved a packet\n");
            parse(packet);
        }
        packet.clear();
    }
}

void Client::update()
{
    sf::Packet p;
    while (true)
    {
        p << "player_pos";
        p << m_player.get_position().x << m_player.get_position().y;
        p << m_player.get_name();
        if (m_server.send(p) != sf::Socket::Done) {
            printf("Could not send packet\n");
            if (m_server.send(p) != sf::Socket::Done) {
                printf("Could not send packet again\n");
            }
        }
        p.clear();
    }
}

void Client::parse(sf::Packet& packet)
{
    std::string data;
    packet >> data;
    std::cout << data << std::endl;
    if (data == "updated_positions") {
        printf("getting updated positions\n");
        m_player_positions.clear();
        size_t num_clients;
        size_t client_index;
        packet >> num_clients;
        packet >> client_index;

        for (size_t i = 0; i < num_clients; i++) {
            sf::Vector2f pos;
            packet >> pos.x;
            packet >> pos.y;

            /* Check if it is out position or someone else's */
            if (i == client_index) {
                m_player.set_position(pos);
            }
            else {
                m_player_positions.push_back(pos);
            }
        }

    }

}