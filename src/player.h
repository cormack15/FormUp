#pragma once
#include "shape.h"

class Player : public Shape
{
public:
	Player();
	void Update(const float& dt) override;
};