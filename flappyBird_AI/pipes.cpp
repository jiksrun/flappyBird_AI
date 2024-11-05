#include "pipes.h"
#include <random>
#include <iostream>

Pipes::Pipes() {
	sf::RectangleShape upper_pipe;
	sf::RectangleShape lower_pipe;
	int lower_y = getRandomY();
	int lower_height = setup.getWindowHeight() - lower_y; // calculate lower pipe's height by substracing the window's height by the lower pipe y pos
	int upper_height = lower_y - gap; // calculate upper pipe's height by substracing lower pipe y pos by gap between pipe

	lower_pipe.setFillColor(sf::Color::Green);
	upper_pipe.setFillColor(sf::Color::Green);

	lower_pipe.setSize(sf::Vector2f(width, lower_height));
	upper_pipe.setSize(sf::Vector2f(width, upper_height));

	lower_pipe.setPosition(sf::Vector2f(setup.getWindowWidth(), lower_y)); // x is at the end of the window and y is random
	upper_pipe.setPosition(sf::Vector2f(setup.getWindowWidth(), 0)); // x is at the end of the window and y is 0
	pairPipe_type pairPipe(upper_pipe, lower_pipe);
	pipes.push_back(pairPipe);

	for (int i = 0; i < tot_pipes-1; ++i) { // tot_pipes - 1 bcs we already make one 
		addPipes();
	}
}

double Pipes::getRandomY() {
	return setup.randomNumberGenerator(lowerRange, upperRange);
}

void Pipes::addPipes() {
	sf::RectangleShape upper_pipe;
	sf::RectangleShape lower_pipe;
	int lower_y = getRandomY();
	int lower_height = setup.getWindowHeight() - lower_y; // the window's height minus the random y we got from rng
	int upper_height = lower_y - gap; // the upper pipe should be lower y minus gap
	int upper_lower_x = pipes[pipes.size()-1].first.getPosition().x + distance;

	lower_pipe.setSize(sf::Vector2f(width, lower_height));
	upper_pipe.setSize(sf::Vector2f(width, upper_height));

	lower_pipe.setFillColor(sf::Color::Green);
	upper_pipe.setFillColor(sf::Color::Green);

	lower_pipe.setPosition(sf::Vector2f(upper_lower_x, lower_y)); // x is at the end of the window and y is random
	upper_pipe.setPosition(sf::Vector2f(upper_lower_x, 0)); // x is at the end of the window and y is 0
	pairPipe_type pairPipe(upper_pipe, lower_pipe);
	pipes.push_back(pairPipe);
}

void Pipes::movePipes() {
	for (int i = 0; i < pipes.size(); ++i) {
		pairPipe_type& pipe = pipes[i];
		auto new_x = pipe.first.getPosition().x - v;
		auto first_y = pipe.first.getPosition().y;
		auto second_y = pipe.second.getPosition().y;
		if (new_x < setup.getBoardWidth()) {
			int lower_y = getRandomY();
			int upper_height = lower_y - gap;
			int lower_height = setup.getWindowHeight() - lower_y; 
			first_y = 0; 
			second_y = lower_y;

			pipe.first.setSize(sf::Vector2f(width, upper_height));
			pipe.second.setSize(sf::Vector2f(width, lower_height));

			new_x = pipes[i == 0 ? pipes.size() - 1 : i - 1].first.getPosition().x + distance;
		}
		pipe.first.setPosition(sf::Vector2f(new_x, first_y));
		pipe.second.setPosition(sf::Vector2f(new_x, second_y));
	}
}

void Pipes::draw(rWindow& window) {
	for (pairPipe_type& pipe : pipes) {
		window.draw(pipe.first);
		window.draw(pipe.second);
	}
}
