#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <fstream>
#include <iostream>
#include "BlockManager.h"

int main()
{

	//Blocks
	BlockManager blocks;
	blocks.create_block("Nothing", 0x00000000);
	blocks.create_block("Void", 0x000000FF);

	blocks.create_block("Hell_Steel_Ore", 0xB54546FF);
	blocks.create_block("Malachite_Ore", 0x44CC83FF);
	blocks.create_block("Diamond_Ore", 0x49E0FBFF);
	blocks.create_block("Crystal_Ore", 0xA87DD8FF);
	blocks.create_block("Copper_Ore", 0xCA937BFF);
	blocks.create_block("Demon_Goo", 0x600000FF);
	blocks.create_block("Bedrock", 0x646466FF);
	blocks.create_block("Ruby_Ore", 0xE96665FF);
	blocks.create_block("Red_Wood", 0xA4593CFF);
	blocks.create_block("Iron_Ore", 0xAFAFAFFF);
	blocks.create_block("Void_Ore", 0x4DB999FF);
	blocks.create_block("Water", 0x32A6DFFF);
	blocks.create_block("Brick", 0x7C634AFF);
	blocks.create_block("Stone", 0x928C8CFF);
	blocks.create_block("Grass", 0x478142FF);
	blocks.create_block("Lava", 0xE45746FF);
	blocks.create_block("Dirt", 0x985E41FF);
	blocks.create_block("Dirt", 0x985E41FF);
	blocks.create_block("Wood", 0xB15E35FF);
	blocks.create_block("Leaf", 0x6E9C6AFF);
	blocks.create_block("Sand", 0xE8CD73FF);

	sf::Vector2u size;
	int current = 1;
	bool s = false;
	bool a = false;
	bool d = false;
	bool w = false;
	bool q = false;
	unsigned int scale = 1;
	std::cout << "Open: o, New: n\n";
	std::string str;
	std::cin >> str;
	sf::RectangleShape cursor;
	
	sf::Texture cursor_texture;
	sf::Image structure;
	sf::Image screen;

	if (str == "o")
	{
		std::cout << "Enter save name:\n";
		std::cin >> str;
		structure.loadFromFile("Structures/" + str + ".png");
		printf("Loaded\n");
		size = structure.getSize();
	}
	else
	{
		std::cout << "Enter Width:\n";
		std::cin >> size.x;
		std::cout << "Enter Height:\n";
		std::cin >> size.y;
		

		structure.create(size.x, size.y, blocks.get_by_name("Nothing").get_color());

	}

	scale = 800 / size.y;
	sf::RenderWindow window(sf::VideoMode(size.x * scale, size.y * scale), "Structure Designer", sf::Style::None);

	screen.create(size.x * 8, size.y * 8, sf::Color::Red);

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(8, 8));
	cursor.setSize(sf::Vector2f(scale, scale));
	cursor.setFillColor(sf::Color(255, 255, 255, 200));
	shape.setScale(sf::Vector2f(window.getSize().x / shape.getSize().x, window.getSize().y / shape.getSize().y));
	sf::Texture texture;
	shape.setPosition(0, 0);
	sf::Vector2i pos;

	while (window.isOpen())
	{

		pos = (sf::Vector2i)window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
		pos.x = pos.x / scale * scale;
		pos.y = pos.y / scale * scale;

		sf::Event event;
		while (window.pollEvent(event));
		{
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::S)
				{
					if (!s)
					{
						s = true;
						std::cout << "Enter save name:\n";
						std::cin >> str;
						structure.saveToFile("Structures/" + str + ".png");
						printf("Saved\n");
					}
				}
				else
				{
					s = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					if (!d)
					{
						d = true;
						current++;
						if (current >= blocks.get_counter())
						{
							current = 1;
						}
					}
				}
				else
				{
					d = false;
				}
				if (event.key.code == sf::Keyboard::A) {
					if (!a)
					{
						a = true;
						current--;
						if (current < 1)
						{
							current = blocks.get_counter() - 1;
						}
					}
				}
				else
				{
					a = false;
				}
				if (event.key.code == sf::Keyboard::W) {
					if (!w)
					{
						w = true;
						structure.setPixel(pos.x / scale, pos.y / scale, blocks.get_by_num(current).get_color());
					}
				}
				else
				{
					w = false;
				}
				if (event.key.code == sf::Keyboard::Q) {
					if (!q)
					{
						q = true;
						structure.setPixel(pos.x / scale, pos.y / scale, blocks.get_by_name("Nothing").get_color());
					}
				}
				else
				{
					q = false;
				}
			}
			else
			{
				s = false;
				d = false;
				a = false;
				w = false;
				q = false;
			}
		}

		for (size_t x = 0; x < size.x; x++)
		{
			for (size_t y = 0; y < size.y; y++)
			{
				screen.copy(blocks.get_by_color(structure.getPixel(x, y)).get_image(), x * 8, y * 8);
			}
		}

		cursor.setPosition((sf::Vector2f)pos);
		Block& current_block = blocks.get_by_num(current);
		cursor_texture.loadFromImage(current_block.get_image());
		cursor.setTexture(&cursor_texture);
		texture.loadFromImage(screen);
		shape.setTexture(&texture);

		window.clear();

		window.draw(shape);
		window.draw(cursor);

		window.display();
	}

	return 0;
}