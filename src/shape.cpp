#include "shape.h"
#include "game.h"

using namespace std;

Shape::Shape() {};

Shape::Shape(sf::IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Shape::Update(const float& dt) {}

Shape::~Shape() = default;