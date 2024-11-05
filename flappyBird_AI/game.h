#pragma once

#include "setup.h"
#include "bird.h"
#include "pipes.h"

class Game {
	Setup setup;
	Pipes pipes;
private:
	std::vector<std::vector<double>> findInputs(pairPipe_type& closest_pipe, std::vector<Bird>& birds);
	Bird crossover(std::vector<Bird>& parents);
	void mutate(Bird& bird);
public:
	pairPipe_type checkClosestPipe(pairPipes_type& pipes, Bird& bird);
	void checkDeath(pairPipe_type& closest_pipe, std::vector<Bird>& birds, int& alive);
	void drawGap(rWindow& window, pairPipe_type& closest_pipe, std::vector<Bird>& birds);
	void decide(pairPipe_type& closest_pipe, std::vector<Bird>& birds);
	void countFitness(pairPipe_type& closest_pipe, std::vector<Bird>& birds);
	bool allDead(const std::vector<Bird>& birds);
	std::vector<Bird> findParents(std::vector<Bird>& birds);
	void newGen(std::vector<Bird>& birds);
	void restart(std::vector<Bird>& birds, Pipes& pipes);

};