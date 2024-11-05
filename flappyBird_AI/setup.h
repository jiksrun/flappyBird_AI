#pragma once

#include <SFML/Graphics.hpp>

typedef sf::RenderWindow rWindow;
typedef sf::VideoMode vidMode;

class Setup {
private:
	unsigned int WINDOW_WIDTH = 1800, WINDOW_HEIGHT = 800;
	unsigned int BOARD_WIDTH = 600, BOARD_HEIGHT = WINDOW_HEIGHT;
public:
	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;
	unsigned int getBoardWidth() const;
	unsigned int getBoardHeight() const;
	void drawBorder(rWindow& window);
	double randomNumberGenerator(const double& lower = -1.0, const double& upper = 1.0);
};
