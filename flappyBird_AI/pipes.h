#pragma once

#include <SFML/Graphics.hpp>
#include <queue>
#include <utility>
#include "setup.h"
#include "bird.h"

typedef std::pair<sf::RectangleShape, sf::RectangleShape> pairPipe_type;
typedef std::vector<pairPipe_type> pairPipes_type;

class Pipes {
	Setup setup;
	Bird bird;
private:
	float v = 2;
	int tot_pipes = 6;
	int gap = bird.size * 4;
	int distance = bird.size * 18;
	int width = bird.size + bird.size / 2;
	double lowerRange = 150;
	double upperRange = setup.getWindowHeight()-lowerRange;
	double getRandomY();
	void addPipes();
public:
	pairPipes_type pipes;
	Pipes();
	void movePipes();
	void draw(rWindow& window);
	int getGap() const { return gap; };
	int getWidth() const { return width; };
};