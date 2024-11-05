#include "setup.h"
#include <random>

unsigned int Setup::getWindowWidth() const {
	return WINDOW_WIDTH;
}

unsigned int Setup::getWindowHeight() const {
	return WINDOW_HEIGHT;
}

unsigned int Setup::getBoardWidth() const {
	return BOARD_WIDTH;
}

unsigned int Setup::getBoardHeight() const {
	return BOARD_HEIGHT;
}

void Setup::drawBorder(rWindow& window) {
	sf::RectangleShape rect;
	rect.setPosition(sf::Vector2f(BOARD_WIDTH, 0));
	rect.setSize(sf::Vector2f(1, BOARD_HEIGHT));
	window.draw(rect);
}

double Setup::randomNumberGenerator(const double& lower, const double& upper) {
	// use seed to always get the same distribution every time we execute the code
	const unsigned int seed = 123; 
	static std::mt19937 gen(seed); // initialize the random number generator with the seed
	std::uniform_real_distribution<double> dist(lower, upper);
	return dist(gen);
}