#include "Client.h"

Client::Client() {

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
        m_wm.create();
    }
    else {
        printf("Did not recieve packet\n");
        connect();
        // Handle initial packet receive error
    }

}

void Client::run()
{
    // std::thread reception(&Client::recieve_packets, this);

    Settings settings;
    settings.update();

    m_camera.attach(m_player);

    sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);
    
    window.setView(m_camera.get_view());

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

        window.clear(sf::Color::Magenta);
        window.draw(m_wm.get_view_sprite());
        window.display();
    }

}

void Client::parse(std::string& data)
{
    std::stringstream parser(data);
    std::string token;

    parser >> token;

    if (token == "world_info")
    {
        /* Something like this:
        *               SEED      W   H
            world_info 123456789 800 800
        */
        sf::Vector2u size;
        long int seed;
        parser >> seed;
        /* Set the seed */
        m_wm.set_seed(seed);
        parser >> size.x;
        parser >> size.y;

        /* Set the size */
        m_wm.set_size(size);

        /* Create the world */
        printf("Creating world!\n");
        m_wm.create();

        /* Add changes to the world */
    }
    else {
        std::cout << data << std::endl;
    }

}