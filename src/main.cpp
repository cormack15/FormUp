#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "player.h"
#include "entity.h"
#include "shape.h"
#include "game.h"

using namespace std;
using namespace sf;

//VARIABLES
int randXSpawnPoint;

//Unique pointer for the player
std::unique_ptr<Player> player;

//Pointer for targets
std::vector<Target*> targets;

//Defining textures
sf::Texture spritesheet;
sf::Sprite targetSprite;


void Load()
{	
	//Create player and place on the screen
	player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(gameWidth / 2.f, gameHeight / 1.2f));
	
	//Error handling for loading texture
	if (!spritesheet.loadFromFile("res/spritesheet.png")) {
		std::cerr << "Failed to load spritesheet" << std::endl;
	}

	//Load in sprite for a target
	targetSprite.setTexture(spritesheet);
	targetSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	//OLD, KEPT FOR A REMINDER: Create a target
	//Target* tar = new Target(sf::IntRect(Vector2i(150, 50), Vector2i(50, 50)), { 200,200 });
	//targets.push_back(tar);
}

void SpawnTargets() {
	//Define clock
	static sf::Clock clock;

	//Spawn targets at random locations every second
	if (clock.getElapsedTime().asSeconds() > 1) {
		for (int i = 0; i < 1; i++) {
			randXSpawnPoint = (rand() % 401) + 50;

			auto rect = IntRect(i * 50, 0, 50, 50);

			Vector2f position = Vector2f(randXSpawnPoint, 0);

			auto placedTarget = new Target(rect, position);
			targets.push_back(placedTarget);
		}

		clock.restart();
	}

	//Programatically spawn targets
	/*for (int i = 0; i < 5; i++) {
		randXSpawnPoint = (rand() % 450) + 50;

		auto rect = IntRect(i * 50, i * 50, 50, 50);

		Vector2f position = Vector2f(randXSpawnPoint, 0);

		auto placedTarget = new Target(rect, position);
		targets.push_back(placedTarget);
	}*/
}

void Render(sf::RenderWindow& window)
{
	//Render the player
	player->Render(window);

	//Render the targets
	for (const auto t : targets) {
		window.draw(*t);
	}
}

void Update(sf::RenderWindow& window)
{

	//Reset clock, recalculate deltatime
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	// Process events
	sf::Event event;
	while (window.pollEvent(event))
	{
		// Close the window when the close event is triggered
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		// Close window on pressing Escape
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}
	}

	//Update targets
	for (auto& s : targets) {
		s->Update(dt);
	};

	//Update player
	player->Update(dt);

}

int main() {
	//Create window & set frame rate
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "FORM UP");
	window.setFramerateLimit(60);

	//Seed random with time
	srand(time(0));

	//Load
	Load();

	//Clear, Update, Render, and Display
	while (window.isOpen()) {

		// Clear the window
		window.clear(sf::Color::White);

		SpawnTargets();

		// Update
		Update(window);

		// Draw game elements
		Render(window);

		// Display the rendered frame
		window.display();
	}

	return 0;
}