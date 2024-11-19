#include "player.h"

using namespace std;
using namespace sf;

void Player::Update(double dt)
{

	//Move in four directions based on keys
	Vector2f movement(0.0f, 0.0f);

	// Move Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		movement.y -= _speed * dt;
	}
	//Move Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		movement.y += _speed * dt;
	}
	//Move Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		movement.x -= _speed * dt;
	}
	//Move Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		movement.x += _speed * dt;
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