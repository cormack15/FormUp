#include "player.h"
#include "shape.h"
#include "game.h"

using namespace std;


Player::Player() : Shape(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(50, 50)))
{
	setTexture(spritesheet);
}

void Player::Update(const float& dt)
{

	Shape::Update(dt);

	//Move in four directions based on keys

	const float playerSpeed = 400.f;

	// Move Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//movement.y -= _speed * dt;
		move(0.f, -playerSpeed * dt);
	}
	//Move Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		//movement.y += _speed * dt;
		move(0.f, playerSpeed * dt);
	}
	//Move Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		//movement.x -= _speed * dt;
		move(-playerSpeed * dt, 0.f);
	}
	//Move Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		//movement.x += _speed * dt;
		move(playerSpeed * dt, 0.f);
	}

	// Check for boundaries
	// Left boundary: if the player's position goes beyond the left edge (x < 0)
	if (getPosition().x < 0.f)
	{
		move(sf::Vector2f(-getPosition().x, 0.0f));
	}
	// Right boundary: if the player's position goes beyond the right edge (x > gameWidth - shape width) 
	if (getPosition().x > gameWidth - 50.f)
	{
		move(sf::Vector2f(gameWidth - 50.f - getPosition().x, 0.f));
	}
	// Upper boundary: if the player's position goes beyond the top edge (y < 0)
	if (getPosition().y < 0.f)
	{
		move(sf::Vector2f(0.f, -getPosition().y));
	}
	// Lower boundary: if the player's position goes beyond the bottom edge (y > gameHeight - shape height)
	if (getPosition().y > gameHeight - 50.f)
	{
		move(sf::Vector2f(0.f, gameHeight - 50.f - getPosition().y));
	}
}