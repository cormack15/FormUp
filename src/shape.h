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
	char idColour;
	char idShape;
	int idNum;										//Unique identifier for targets
	Target();
	~Target() {};
	void Update(const float &dt);
};

class Modifier : public Shape {
public:
	Modifier(sf::IntRect ir, sf::Vector2f pos);
	static bool direction;
	static float speed;
	char idColour;
	char idShape;
	int idNum;										//Unique identifier for modifiers
	Modifier();
	~Modifier() {};
	void Update(const float &dt);
};