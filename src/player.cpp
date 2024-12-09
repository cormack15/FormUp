#include "player.h"
#include "shape.h"
#include "game.h"

using namespace std;


Player::Player(const sf::Texture& texture) : Shape(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(50, 50))) {
	setTexture(texture);
}

bool Player::controlSetup = true;

void Player::Update(const float& dt) {

	Shape::Update(dt);

	//Move in four directions based on keys

	const float playerSpeed = 400.f;


	//If true, use arrows. Else use WASD
	if (controlSetup) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {		//Up
			move(0.f, -playerSpeed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {	//Down
			move(0.f, playerSpeed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {	//Left
			move(-playerSpeed * dt, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {	//Right
			move(playerSpeed * dt, 0.f);
		}
	}
	else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {	//Up
			move(0.f, -playerSpeed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {	//Down
			move(0.f, playerSpeed * dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {	//Left
			move(-playerSpeed * dt, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {	//Right
			move(playerSpeed * dt, 0.f);
		}
	}

	// Check for boundaries
	// Left boundary: if the player's position goes beyond the left edge (x < 0)
	if (getPosition().x < 0.f) {
		move(sf::Vector2f(-getPosition().x, 0.0f));
	}
	// Right boundary: if the player's position goes beyond the right edge (x > gameWidth - shape width) 
	if (getPosition().x > gameWidth - 50.f) {
		move(sf::Vector2f(gameWidth - 50.f - getPosition().x, 0.f));
	}
	// Upper boundary: if the player's position goes beyond the top edge (y < 0)
	if (getPosition().y < 0.f) {
		move(sf::Vector2f(0.f, -getPosition().y));
	}
	// Lower boundary: if the player's position goes beyond the bottom edge (y > gameHeight - shape height)
	if (getPosition().y > gameHeight - 50.f) {
		move(sf::Vector2f(0.f, gameHeight - 50.f - getPosition().y));
	}
}