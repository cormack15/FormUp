#include <iostream>
#include <stdio.h>
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

//Defining target's direction and speed
bool Target::direction = true;
float Target::speed = 100;

//Target update
void Target::Update(const float& dt) {
	Shape::Update(dt);
	
	//Target movement
	move(sf::Vector2f(0.f, dt * (direction ? 1.0f : -1.0f) * speed));
}