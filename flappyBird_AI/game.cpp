#include "game.h"
#include <iostream>
#include <cmath>
#include <random>

pairPipe_type Game::checkClosestPipe(pairPipes_type& pipes, Bird& bird) {
	float closest_x = 10000.0f; // initialize closest with very high number
	pairPipe_type closest_pipe;
	for (auto& pipe : pipes) {
		double gap = (pipe.first.getPosition().x + Game::pipes.getWidth() - bird.pos_x);
		if (gap >= 0 && gap < closest_x) {
			closest_x = gap;
			closest_pipe = pipe;
		}
	}
	return closest_pipe;
}

void Game::checkDeath(pairPipe_type& closest_pipe, std::vector<Bird>& birds, int& alive) {
	float first_x = closest_pipe.first.getPosition().x;
	float first_y = closest_pipe.first.getPosition().y;
	float first_height = closest_pipe.first.getSize().y;
	float second_x = closest_pipe.second.getPosition().x;
	float second_y = closest_pipe.second.getPosition().y;
	float second_height = closest_pipe.second.getSize().y;
	float pipe_width = static_cast<float>(pipes.getWidth());
	
	for (auto& bird : birds) {
		if (!bird.isDead()) {
			bool collide_x_first = (bird.pos_x < first_x + pipe_width) && (bird.pos_x + bird.size > first_x);
			bool collide_x_second = (bird.pos_x < second_x + pipe_width) && (bird.pos_x + bird.size > second_x);
			bool collide_y_first = (bird.pos_y < first_y + first_height) && (bird.pos_y + bird.size > first_y);
			bool collide_y_second = (bird.pos_y < second_y + second_height) && (bird.pos_y + bird.size > second_y);
			if ((collide_x_first && collide_y_first) || (collide_x_second && collide_y_second)) {
				bird.makeDead();
				--alive;
			}
			else if (bird.pos_y == setup.getWindowHeight() - bird.size) {
				bird.makeDead();
				--alive;
			}
			else if (bird.pos_y <= 1) {
				bird.makeDead();
				--alive;
			}
		}
	}
}

void Game::drawGap(rWindow& window, pairPipe_type& closest_pipe, std::vector<Bird>& birds) {
	sf::RectangleShape lower_gap;

	for (auto& bird : birds) {
		if (bird.isDead()) continue;
		double second_y = closest_pipe.second.getPosition().y - bird.pos_y - bird.size / 2;
		double second_gap = second_y;

		lower_gap.setPosition(sf::Vector2f(bird.pos_x + bird.size / 2, bird.pos_y + bird.size / 2));
		lower_gap.setFillColor(sf::Color::Red);
		lower_gap.setSize(sf::Vector2f(2, second_gap));
		window.draw(lower_gap);
	}
}

std::vector<std::vector<double>> Game::findInputs(pairPipe_type& closest_pipe, std::vector<Bird>& birds) {
	std::vector<std::vector<double>> inputs;
	inputs.reserve(birds.size());

	for (auto& bird : birds) {
		double second_y = closest_pipe.second.getPosition().y - bird.pos_y - bird.size / 2;
		double first_y = second_y - pipes.getGap();
		std::vector<double> input;
		input.push_back(bird.pos_y);
		input.push_back(second_y);
		input.push_back(first_y);
		inputs.push_back(input);
	}
	return inputs;
}

void Game::decide(pairPipe_type& closest_pipe, std::vector<Bird>& birds) {
	auto inputs = findInputs(closest_pipe, birds);
	for (int i = 0; i < birds.size(); ++i) {
		if (birds[i].isDead()) continue;
		birds[i].think(inputs[i]);
	}
}

void Game::countFitness(pairPipe_type& closest_pipe, std::vector<Bird>& birds) {
	for (auto& bird : birds) {
		if (!bird.isDead()) {
			if (bird.pos_y < closest_pipe.second.getPosition().y && bird.pos_y > closest_pipe.first.getPosition().y + pipes.getWidth()) {
				bird.fitness += 0.1;
			}
			bird.fitness += 0.1;
		}
	}
}

bool Game::allDead(const std::vector<Bird>& birds) {
	for (auto& bird : birds) {
		if (!bird.isDead()) return false;
	}
	return true;
}

std::vector<Bird> Game::findParents(std::vector<Bird>& birds) {
	std::vector<Bird> parents;
	parents.reserve(2);

	// initialize with first two indices
	int highest_idx = 0;
	int secondHighest_idx = 1;

	// first pass to find highest
	for (int i = 1; i < birds.size(); ++i) {
		if (birds[i].fitness > birds[highest_idx].fitness) {
			highest_idx = i;
		}
	}

	// second pass to find second highest
	for (int i = 0; i < birds.size(); ++i) {
		if (i != highest_idx && birds[i].fitness > birds[secondHighest_idx].fitness) {
			secondHighest_idx = i;
		}
	}
	parents.push_back(birds[highest_idx]);
	parents.push_back(birds[secondHighest_idx]);
	return parents;
}

Bird Game::crossover(std::vector<Bird>& parents) {
	Bird bird;
	Bird dad = parents[0];
	Bird mom = parents[1];

	// inherit half weights_input_hidden from dad
	for (int i = 0; i < dad.weights_input_hidden[0].size() / 2; ++i) {
		for (int j = 0; j < bird.weights_input_hidden.size(); ++j) {
			bird.weights_input_hidden[j][i] = dad.weights_input_hidden[j][i];
		}
	}

	// antoher half from mom
	for (int i = 0; i < mom.weights_input_hidden[0].size() / 2; ++i) {
		for (int j = 0; j < bird.weights_input_hidden.size(); ++j) {
			bird.weights_input_hidden[j][mom.weights_input_hidden[0].size() / 2 + i] = mom.weights_input_hidden[j][mom.weights_input_hidden[0].size() / 2 + i];
		}
	}

	// inherit hal weights_hidden_output from dad
	for (int i = 0; i < dad.weights_hidden_output[0].size() / 2; ++i) {
		for (int j = 0; j < bird.weights_hidden_output.size(); ++j) {
			bird.weights_hidden_output[j][i] = dad.weights_hidden_output[j][i];
		}
	}

	// another half from mom
	for (int i = 0; i < mom.weights_hidden_output[0].size() / 2; ++i) {
		for (int j = 0; j < bird.weights_hidden_output.size(); ++j) {
			bird.weights_hidden_output[j][mom.weights_hidden_output[0].size() / 2 + i] = mom.weights_hidden_output[j][mom.weights_hidden_output[0].size() / 2 + i];
		}
	}

	return bird;
}

void Game::mutate(Bird& bird) {
	double mutation_rate = 0.2;
	double mutation_strength = 0.2;

	// mutate weights_input_hidden
	for (auto& weight_input_hidden : bird.weights_input_hidden) {
		for (auto& weight : weight_input_hidden) {
			if (abs(setup.randomNumberGenerator()) < mutation_rate) {
				weight += (setup.randomNumberGenerator() * mutation_strength);
			}
		}
	}
	// mutate weights_hidden_output
	for (auto& weight_hidden_output : bird.weights_hidden_output) {
		for (auto& weight : weight_hidden_output) {
			if (abs(setup.randomNumberGenerator()) < mutation_rate) {
				weight += (setup.randomNumberGenerator() * mutation_strength);
			}
		}
	}
}

void Game::newGen(std::vector<Bird>& birds) {
	std::vector<Bird> bestTwo = findParents(birds);
	Bird newBird = crossover(bestTwo);

	// elititsm
	birds[0].weights_input_hidden = bestTwo[0].weights_input_hidden;
	birds[0].weights_hidden_output = bestTwo[0].weights_hidden_output;
	birds[1].weights_input_hidden = bestTwo[1].weights_input_hidden;
	birds[1].weights_hidden_output = bestTwo[1].weights_hidden_output;

	// apply the crossover and mutation
	for (int i = 2; i < birds.size(); ++i) {
		birds[i].weights_input_hidden = newBird.weights_input_hidden;
		birds[i].weights_hidden_output = newBird.weights_hidden_output;
		mutate(birds[i]);
	}
}

void Game::restart(std::vector<Bird>& birds, Pipes& pipes) {
	Pipes newPipes;
	pipes.pipes = newPipes.pipes;
	for (auto& bird : birds) {
		bird.renew();
	}
}