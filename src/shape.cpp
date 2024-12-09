#include <iostream>
#include <stdio.h>
#include "shape.h"
#include "game.h"

// -------------------
// Shape Class
// -------------------
Shape::Shape() {};

Shape::Shape(sf::IntRect ir) : sf::Sprite(), _sprite(ir) {
	setTextureRect(_sprite);
};

Shape::~Shape() = default;

void Shape::Update(const float& dt) {}


// -------------------
// Target Class
// -------------------
Target::Target() : Shape() {};

//Defining target's direction and speed
bool Target::direction = true;
float Target::speed = 100.0f;

Target::Target(sf::IntRect ir, sf::Vector2f pos, const sf::Texture& texture) : Shape(ir) {
	setOrigin(16.f, 16.f);
	setTexture(texture);
	setPosition(pos);
}

//Target update
void Target::Update(const float& dt) {
	Shape::Update(dt);
	move(sf::Vector2f(0.f, dt * (direction ? 1.0f : -1.0f) * speed));		//Target movement
}


// -------------------
// Modifier Class
// -------------------
Modifier::Modifier() : Shape() {};

//Defining modifier's direction and speed
bool Modifier::direction = true;
float Modifier::speed = 100;

Modifier::Modifier(sf::IntRect ir, sf::Vector2f pos, const sf::Texture& texture) : Shape(ir) {
	setOrigin(16.f, 16.f);
	setTexture(texture);
	setPosition(pos);
}

//Modifier update
void Modifier::Update(const float& dt) {
	Shape::Update(dt);
	move(sf::Vector2f(0.f, dt * (direction ? 1.0f : -1.0f) * speed));		//Modifier movement
 }