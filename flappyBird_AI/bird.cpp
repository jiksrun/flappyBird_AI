#include "bird.h"
#include <random>
#include <iostream>
#include <cmath> 

Bird::Bird() {
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, 255);
	color = sf::Color(dist(rd), dist(rd), dist(rd));

	// set position and size
	pos_x = (setup.getWindowWidth() - setup.getBoardWidth()) / 2 + setup.getBoardWidth() - size;
	pos_y = setup.getWindowHeight() / 2 + size;
	body.setPosition(sf::Vector2f(pos_x, pos_y));
	body.setSize(sf::Vector2f(size, size));
	body.setFillColor(color);

	int input_neurons = 3;
	int hidden_neurons = 2;
	int output_neurons = 1;

	// initialize random weights for input layer to hidden layer. 
	weights_input_hidden = setInitiliazeWeights(input_neurons, hidden_neurons);
	// initialize random weights for hidden layer to output layer. 
	weights_hidden_output = setInitiliazeWeights(hidden_neurons, output_neurons);
}

vecWeights_type Bird::setInitiliazeWeights(const int& input_neurons, const int& output_neurons) {
	vecWeights_type outputs;
	outputs.reserve(input_neurons); 

	for (int i = 0; i < input_neurons; ++i) {
		weights_type temp;
		for (int j = 0; j < output_neurons; ++j) {
			temp.push_back(setup.randomNumberGenerator());
		}
		outputs.push_back(temp);
	}
	return outputs;
}

double Bird::activation_tanh(const double& input) {
	return tanh(input);
}

weights_type Bird::activation_tanh(const weights_type& inputs) {
	weights_type outputs;
	outputs.reserve(inputs.size());
	for (double x : inputs) {
		outputs.push_back(activation_tanh(x));
	}
	return outputs;
}

weights_type Bird::dot(const weights_type& inputs, const vecWeights_type& weights) {
	weights_type outputs;
	outputs.reserve(weights[0].size());
	for (int i = 0; i < outputs.capacity(); ++i) {
		double result = 0;
		for (int j = 0; j < inputs.size(); ++j) {
			result += inputs[j] * weights[j][i];
		}
		outputs.push_back(result);
	}
	return outputs;
}

std::vector<double> Bird::forward(weights_type& inputs) {
	weights_type hidden_layer_neurons = activation_tanh(dot(inputs, weights_input_hidden));
	weights_type output_layer_neurons = activation_tanh(dot(hidden_layer_neurons, weights_hidden_output));
	return (output_layer_neurons);
}

void Bird::think(weights_type& inputs) {
	std::vector<double> decision = forward(inputs);
	if (decision[0] > 0.0) jump();
}

void Bird::makeDead() {
	body.setFillColor(sf::Color::Red);
	dead = true;
}

bool Bird::isDead() const {
	return dead;
}

void Bird::draw(rWindow& window) {
	window.draw(body);
}


void Bird::fall(const unsigned int& fps) {
	v += g * (1.0f / static_cast<float>(fps));

	if (pos_y + v < setup.getWindowHeight() - size) pos_y += v;
	else {
		pos_y = setup.getWindowHeight() - size;
		v = 0;
	}

	body.setPosition(sf::Vector2f(pos_x, pos_y));
}

void Bird::jump() {
	if (!dead) {
		v = -jumpStrength;
		pos_y += v;

		if (pos_y < 0) {
			pos_y = 0;
			v = 0;
		}
		body.setPosition(sf::Vector2f(pos_x, pos_y));
	}
}

void Bird::renew() {
	body.setFillColor(color);
	dead = false;
	fitness = 0;
	// position reset
	pos_x = (setup.getWindowWidth() - setup.getBoardWidth()) / 2 + setup.getBoardWidth() - size;
	pos_y = setup.getWindowHeight() / 2 + size;
	v = 0; // velocity reset
	body.setPosition(sf::Vector2f(pos_x, pos_y));
}