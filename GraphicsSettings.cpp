#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stack>


class GraphicSettings {
private:

public:
	std::string title;
	sf::VideoMode resolution;
	bool fullscreen;
	bool vsync;
	unsigned frameRateLimit;
	sf::ContextSettings contextSettings;
	std::vector<sf::VideoMode> videoModes;

	GraphicSettings() {
        this->title = "DEFAULT";
        this->resolution = sf::VideoMode::getDesktopMode();
        this->fullscreen = false;
        this->vsync = false;
        this->frameRateLimit = 120;
        this->contextSettings.antialiasingLevel = 0;
        this->videoModes = sf::VideoMode::getFullscreenModes();
    }
void saveToFile(const std::string path) {
	std::ofstream ofs(path);

	if (ofs.is_open()) {
		ofs << this->title;
		ofs << this->resolution.width << " " << this->resolution.height;
		ofs << this->fullscreen;
		ofs << this->frameRateLimit;
		ofs << this->vsync;
		ofs << this->contextSettings.antialiasingLevel;
	}
	ofs.close();

}

void loadFromFile(const std::string path) {
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->frameRateLimit;
		ifs >> this->vsync;
		ifs >> this->contextSettings.antialiasingLevel;
	}
	ifs.close();

}
};