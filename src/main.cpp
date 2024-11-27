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
int randSpawnTimeframe;
int randTargetSpriteX;
int randTargetSpriteY;

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

void SpawnTargets()
{
	//Define clock
	static sf::Clock clock;

	//Spawn targets at random locations at random intervals
	if (clock.getElapsedTime().asSeconds() > randSpawnTimeframe) {
		randXSpawnPoint = (rand() % 401) + 50;											//Create random spawn location along X axis

		auto rect = IntRect(randTargetSpriteX * 50, randTargetSpriteY * 50, 50, 50);	//Define the sprite for the target
		Vector2f position = Vector2f(randXSpawnPoint, 0);								//Define the X,Y position for the target to spawn at

		auto placedTarget = new Target(rect, position);									//Create the target

		//Set the target's X and Y ids. This is done by taking the randTargetSprite X and Y and adding 49 to get the correct value in ASCII, which the then stored as a char
		placedTarget->idX = randTargetSpriteX + 49;
		placedTarget->idY = randTargetSpriteY + 49;

		targets.push_back(placedTarget);												//Push target stack

		clock.restart();																//Restart the clock

		randSpawnTimeframe = (rand() % 3) + 1;											//Choose a random time until next target spawns
		randTargetSpriteX = (rand() % 5);												//Choose a random sprite X
		randTargetSpriteY = (rand() % 4);												//Choose a random sprite Y
	}
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

		//Clear the window
		window.clear(sf::Color::White);

		//Spawn Target
		SpawnTargets();

		//Update
		Update(window);

		//Draw game elements
		Render(window);

		//Display the rendered frame
		window.display();
	}

	return 0;
}