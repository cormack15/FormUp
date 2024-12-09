#pragma once
#include "shape.h"
#include <SFML/Graphics.hpp>

class Player : public Shape
{
public:
	static bool controlSetup;
	char idColour;
	char idShape;
	Player(const sf::Texture& texture);
	void Update(const float& dt) override;
};