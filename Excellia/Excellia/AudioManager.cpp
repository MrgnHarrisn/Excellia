#include "AudioManager.h"

AudioManager::AudioManager()
{

}

sf::Sound& AudioManager::get_by_id(unsigned int id)
{
	return m_sounds[id];
}

void AudioManager::load_from_file(const std::string& file_name)
{
	std::fstream file;
	std::string name;
	unsigned int id = 0;
	file.open("Game/" + file_name + ".blk");

	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << file_name << ".blk\n";
		return;
	}

	while (file >> name)
	{
		std::cout << "Name: " << name << std::endl;
		if (!(file >> id)) {
			std::cerr << "Error reading in id\n";
			break;
		}
		sf::SoundBuffer sound_buffer;
		if (!sound_buffer.loadFromFile(base_path + name + ".wav")) {
			std::cerr << "Could not find sound file: " << name << ".wav\n";
			continue;
		}

		m_buffers[id] = sound_buffer;
		m_sounds[id].setBuffer(m_buffers[id]);

	}

	file.close();
}

void AudioManager::play_sound(unsigned int id)
{
	m_sounds[id].play();
}
