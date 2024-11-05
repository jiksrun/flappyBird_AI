#pragma once

#include <SFML/Graphics.hpp>
#include "bird.h"
#include "setup.h"

class Board {
	Setup setup;
	Bird bird;
private:
	sf::Font font;
	float x_gap = 20;
	unsigned int size = 25;
	int gap_divider = 6;
	void loadFont();
public:
	Board();
	void displayTotalGenerations(rWindow& window, const int& tot_gen);
	void displayOverallBestScore(rWindow& window, const int& bestScore);
	void displayCurrentBestScore(rWindow& window, const int& bestScore);
	void displayAlive(rWindow& window, const int& alive, const int& population);
	void displayPressToQuit(rWindow& window);
};