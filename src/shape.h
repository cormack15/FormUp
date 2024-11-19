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