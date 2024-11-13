#include "player.h"
using namespace sf;
using namespace std;

void Player::Update(double dt)
{
	//Define the movement speed for the player
	const float playerSpeed = 400;

	//Move in four directions based on keys
	Vector2f movement(0.0f, 0.0f);

	// Move Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movement.y -= playerSpeed * dt;
	}
	//Move Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movement.y += playerSpeed * dt;
	}
	//Move Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= playerSpeed * dt;
	}
	//Move Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += playerSpeed * dt;
	}

	move(movement);

	Entity::Update(dt);
}

Player::Player() : _speed(200.0f), Entity(make_unique<CircleShape>(25.f))
{
	_shape->setFillColor(Color::Black);
	_shape->setOrigin(sf::Vector2f(25.f, 25.f));
}

void Player::Render(sf::RenderWindow& window) const
{
	window.draw(*_shape);
}