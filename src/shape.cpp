#include "shape.h"
#include "game.h"


Shape::Shape() {};

Shape::Shape(sf::IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Shape::Update(const float& dt) {}

Shape::~Shape() = default;


Target::Target() : Shape() {};

Target::Target(sf::IntRect ir, sf::Vector2f pos) : Shape(ir) {
	setOrigin(sf::Vector2f(16.f, 16.f));;
	setPosition(pos);
}

void Target::Update(const float& dt) {
	Shape::Update(dt);
}