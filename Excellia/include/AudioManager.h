#pragma once

#include <iostream>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <fstream>
#include <iomanip>

class AudioManager
{
public:
	AudioManager();
	sf::Sound& get_by_id(unsigned int id);
	void load_from_file(const std::string& str);
	void play_sound(unsigned int id);
private:
	std::unordered_map<unsigned int, sf::SoundBuffer> m_buffers;
	std::unordered_map<unsigned int, sf::Sound> m_sounds;
	std::string base_path = "assets/Audio/";
};
