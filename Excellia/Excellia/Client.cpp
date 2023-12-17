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

        packet >> image_size.x >> image_size.y;

        std::vector<sf::Uint8> pixels;
        // std::cout << image_size.x << " " << 

        size_t pixels_received = 0;
        size_t chunk_size = 256;
        size_t total_info = image_size.x * image_size.y * 4;
        while (pixels_received != total_info) {
            size_t expected_pixels = std::min(total_info - pixels_received, chunk_size);

            for (size_t i = 0; i < expected_pixels; i++) {
                sf::Uint8 color;
                packet >> color;
                pixels.push_back(color);
                pixels_received += 1;
            }

            if (m_server.receive(packet) != sf::Socket::Done) {
                printf("error getting packet\n");
                break;
            }
            else
            {
                printf("Receiving World Packet, %i/%i\n", pixels_received, total_info);
            }

            packet.clear();

        }

        printf("World Recieved!\n");

        sf::Image received_image;
        received_image.create(image_size.x, image_size.y, pixels.data());
        m_wm.set_world_image(received_image);
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

    sf::RenderWindow window(sf::VideoMode(settings.get_screen_size().x, settings.get_screen_size().y), "Pixellia", sf::Style::None);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::Magenta);
        window.draw(m_wm.get_render());
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