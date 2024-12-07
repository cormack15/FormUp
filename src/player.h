#pragma once
#include "shape.h"

class Player : public Shape
{
public:
	char idColour;
	char idShape;
	Player();
	void Update(const float& dt) override;
};