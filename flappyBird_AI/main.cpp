#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h> // for _getch()
#include <vector>
#include "setup.h"
#include "bird.h"
#include "pipes.h"
#include "game.h"
#include "board.h"

void createPopulation(std::vector<Bird>& birds, const int& num_population);
void addPopulation(std::vector<Bird>& birds, const int& new_population);
void drawBirds(rWindow& window, Setup& setup, std::vector<Bird>& birds);
void fallBirds(std::vector<Bird>& birds, const unsigned int& fps);
void jumpBirds(std::vector<Bird>& birds);
void printParentsInfo(std::vector<Bird>& parents);
void countScore(pairPipe_type& closest_pipe, std::vector<Bird>& birds, int& score);
void displayInfo(rWindow& window, Board& board, const int& tot_gen, const int& bestScore, const int& score, const int& alive, const int& population);

void preface() {
	std::cout << "With this seed (123), the best generation is the 31th generation (as far as i know)." << std::endl;
	std::cout << "Press any button to continue...";
	_getch();
	std::cout << std::endl;
}

int main() {
	preface();
	int starting_population = 15;
	int alive = starting_population;
	int tot_gen = 1;
	int bestScore = 0;
	int score = 0;
	Setup setup;
	Pipes pipes;
	Game game;
	Board board;
	pairPipe_type closest_pipe;
	std::vector<Bird> birds;
	createPopulation(birds, starting_population);
	unsigned int fps = 120;

	rWindow window(vidMode(setup.getWindowWidth(), setup.getWindowHeight()), "Flappy Bird");
	window.setFramerateLimit(fps);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed()) window.close();
			if (event.type == sf::Event::KeyPressed) if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) window.close();
			//if (event.type == sf::Event::KeyPressed) if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) jumpBirds(birds);
		}
		window.clear();
		setup.drawBorder(window);
		pipes.draw(window);
		drawBirds(window, setup, birds);
		pipes.movePipes();
		fallBirds(birds, fps);
		closest_pipe = game.checkClosestPipe(pipes.pipes, birds[0]);
		game.checkDeath(closest_pipe, birds, alive);
		//game.drawGap(window, closest_pipe, birds);
		game.decide(closest_pipe, birds);
		game.countFitness(closest_pipe, birds);
		countScore(closest_pipe, birds, score);
		if (score > bestScore) bestScore = score;
		if (game.allDead(birds)) {
			game.newGen(birds);
			game.restart(birds, pipes);
			++tot_gen;
			addPopulation(birds, (tot_gen+9) / 10 * starting_population);
			score = 0;
			alive = birds.size();
		}
		displayInfo(window, board, tot_gen, bestScore, score, alive, birds.size());
		window.display();
	}
}

void createPopulation(std::vector<Bird>& birds, const int& num_population) {
	birds.reserve(num_population);
	for (int i = 0; i < num_population; ++i) {
		Bird bird;
		birds.push_back(bird);
	}
}

void addPopulation(std::vector<Bird>& birds, const int& new_population) {
	while (birds.size() < new_population) {
		Bird bird;
		birds.push_back(bird);
	}
}

void drawBirds(rWindow& window, Setup& setup, std::vector<Bird>& birds) {
	for (Bird& bird : birds) {
		if (bird.pos_y + bird.size < setup.getWindowHeight()) bird.draw(window);
	}
}

void fallBirds(std::vector<Bird>& birds, const unsigned int& fps) {
	// use Bird& instead just Bird because we dont wanna just make a copy, we want to actually make a change to the real memory
	for (Bird& bird : birds) { 
		bird.fall(fps);
	}
}

void jumpBirds(std::vector<Bird>& birds) {
	for (Bird& bird : birds) {
		bird.jump();
	}
}

void printParentsInfo(std::vector<Bird>& parents) {
	// parent 0 info
	Bird dad = parents[0];
	std::cout << "Parent 0:" << std::endl;
	std::cout << "Fitness: " << dad.fitness << std::endl;
	std::cout << "weights_input_hidden:" << std::endl;
	for (int i = 0; i < dad.weights_input_hidden.size(); ++i) {
		for (int j = 0; j < dad.weights_input_hidden[0].size(); ++j) {
			std::cout << i << " " << j << " : " << dad.weights_input_hidden[i][j] << std::endl;
		}
	}
	std::cout << "weights_hidden_output:" << std::endl;
	for (int i = 0; i < dad.weights_hidden_output.size(); ++i) {
		for (int j = 0; j < dad.weights_hidden_output[0].size(); ++j) {
			std::cout << i << " " << j << " : " << dad.weights_hidden_output[i][j] << std::endl;
		}
	}
	std::cout << std::endl;
	// parent 1 info
	Bird mom = parents[1];
	std::cout << "Parent 1:" << std::endl;
	std::cout << "Fitness: " << mom.fitness << std::endl;
	std::cout << "weights_input_hidden:" << std::endl;
	for (int i = 0; i < mom.weights_input_hidden.size(); ++i) {
		for (int j = 0; j < mom.weights_input_hidden[0].size(); ++j) {
			std::cout << i << " " << j << " : " << mom.weights_input_hidden[i][j] << std::endl;
		}
	}
	std::cout << "weights_hidden_output:" << std::endl;
	for (int i = 0; i < mom.weights_hidden_output.size(); ++i) {
		for (int j = 0; j < mom.weights_hidden_output[0].size(); ++j) {
			std::cout << i << " " << j << " : " << mom.weights_hidden_output[i][j] << std::endl;
		}
	}
}

void countScore(pairPipe_type& closest_pipe, std::vector<Bird>& birds, int& score) {
	for (auto& bird : birds) {
		if (!bird.isDead()) {
			if (bird.pos_x == closest_pipe.first.getPosition().x) {
				score += 1;
				return;
			}
		}
	}
}

void displayInfo(rWindow& window, Board& board, const int& tot_gen, const int& bestScore, const int& score, const int& alive, const int& population) {
	board.displayTotalGenerations(window, tot_gen);
	board.displayOverallBestScore(window, bestScore);
	board.displayCurrentBestScore(window, score);
	board.displayAlive(window, alive, population);
	board.displayPressToQuit(window);
}