#pragma once
#include <SFML/Graphics.hpp>

class Shape : public sf::Sprite {
protected:
	sf::IntRect _sprite;
	Shape();
public:
	explicit Shape(sf::IntRect ir);
	virtual ~Shape() = 0;
	virtual void Update(const float &dt);
};

class Target : public Shape {
public:
	Target(sf::IntRect ir, sf::Vector2f pos);
	static bool direction;
	static float speed;
	char idX;
	char idY;
	Target();
	void Update(const float &dt);
};