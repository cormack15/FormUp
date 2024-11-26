#include "shape.h"
#include "game.h"

//Defining shape
Shape::Shape() {};

Shape::Shape(sf::IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Shape::Update(const float& dt) {}
Shape::~Shape() = default;

//Defining target
Target::Target() : Shape() {};

Target::Target(sf::IntRect ir, sf::Vector2f pos) : Shape(ir) {
	setOrigin(sf::Vector2f(16.f, 16.f));;
	setPosition(pos);
}

//Target update
void Target::Update(const float& dt) {
	Shape::Update(dt);

	//Target movement
	move(sf::Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

	if ((direction && getPosition().x > gameWidth - 16) || (!direction && getPosition().x < 16)) {
		direction = !direction;
	}
}

//Defining target's direction and speed
bool Target::direction;
float Target::speed;