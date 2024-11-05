#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "setup.h"

typedef std::vector<double> weights_type;
typedef std::vector<weights_type> vecWeights_type;

class Bird {
	Setup setup;
private:
	sf::RectangleShape body;
	sf::Color color;
	const float jumpStrength = 2.5f;
	const float g = 9.8f;
	float v = 0;
	bool dead = false;

	weights_type dot(const weights_type& inputs, const vecWeights_type& weights);
	double activation_tanh(const double& input); // activation for a single neuron
	weights_type activation_tanh(const weights_type& inputs); // activation for vector of neuron
	vecWeights_type setInitiliazeWeights(const int& input_neurons, const int& output_neurons);
	std::vector<double> forward(weights_type& inputs);
public:
	vecWeights_type weights_input_hidden;
	vecWeights_type weights_hidden_output;
	double fitness = 0; 
	double size = 20;
	float pos_x = 0;
	float pos_y = 0;
	Bird();
	void makeDead();
	bool isDead() const;
	void draw(rWindow& window);
	void fall(const unsigned int& fps);
	void jump();
	void think(weights_type& inputs);
	void renew();
};
