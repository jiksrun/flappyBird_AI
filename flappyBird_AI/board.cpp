#include "board.h"
#include <iostream>
#include <string>

Board::Board() {
	loadFont();
}

void Board::loadFont() {
	if (!font.loadFromFile("PressStart2P.ttf")) {
		std::cerr << "Error loading font..." << std::endl;
	}
}

void Board::displayTotalGenerations(rWindow& window, const int& tot_gen) {
	sf::Text text;
	text.setFont(font);
	text.setString("# OF GENERATIONS: " + std::to_string(tot_gen));
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x_gap, setup.getBoardHeight() / gap_divider));
	text.setCharacterSize(size);
	window.draw(text);
}

void Board::displayOverallBestScore(rWindow& window, const int& bestScore) {
	sf::Text text;
	text.setFont(font);
	text.setString("BEST SCORE: " + std::to_string(bestScore));
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x_gap, setup.getBoardHeight() / gap_divider * 2));
	text.setCharacterSize(size);
	window.draw(text);
}

void Board::displayCurrentBestScore(rWindow& window, const int& bestScore) {
	sf::Text text;
	text.setFont(font);
	text.setString("CURRENT SCORE: " + std::to_string(bestScore));
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x_gap, setup.getBoardHeight() / gap_divider * 3));
	text.setCharacterSize(size);
	window.draw(text);
}

void Board::displayAlive(rWindow& window, const int& alive, const int& population) {
	sf::Text text;
	text.setFont(font);
	text.setString("ALIVE: " + std::to_string(alive) + "/" + std::to_string(population));
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x_gap, setup.getBoardHeight() / gap_divider * 4));
	text.setCharacterSize(size);
	window.draw(text);
}

void Board::displayPressToQuit(rWindow& window) {
	sf::Text text;
	text.setFont(font);
	text.setString("PRESS 'Q' TO QUIT");
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(x_gap, setup.getBoardHeight() / gap_divider * 5));
	text.setCharacterSize(size);
	window.draw(text);
}