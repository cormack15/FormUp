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
int randSpawnTimeframe = 1;
int randTargetSpriteX;	int randModifierSpriteX;		//Stores the random X coord for targets and modifiers
int randTargetSpriteY;	int randModifierSpriteY;		//Stores the random Y coord for targets and modifiers
int targetIDCounter;	int modifierIDCounter;			//Stores the ID counter for targets and modifiers
int targetExists;				//Stores how many targets currently exist
int modifierExists;				//Stores how many modifiers currently exist
char spawnerFlag;				//Stores a flag to direct the spawner

//Unique pointer for the player
std::unique_ptr<Player> player;

//Pointer for targets
std::vector<Target*> targets;
std::vector<Modifier*> modifiers;

//Defining textures
sf::Texture spritesheet;
sf::Texture modifierSpritesheet;
sf::Sprite targetSprite;
sf::Sprite modifierSprite;


void Load()
{	
	//Set variables
	targetIDCounter = 0;
	
	//Create player and place on the screen
	player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(gameWidth / 2.f, gameHeight / 1.2f));
	
	//Error handling for loading textures
	if (!spritesheet.loadFromFile("res/spritesheet.png")) {
		std::cerr << "Failed to load spritesheet" << std::endl;
	}
	if (!modifierSpritesheet.loadFromFile("res/modifierspritesheet.png")) {
		std::cerr << "Failed to load modifier spritesheet" << std::endl;
	}

	//Load in sprite for a target
	targetSprite.setTexture(spritesheet);
	targetSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	//Load in sprite for a modifier
	modifierSprite.setTexture(modifierSpritesheet);
	modifierSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(70, 70)));

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
		placedTarget->idColour = randTargetSpriteX + 49;
		placedTarget->idShape = randTargetSpriteY + 49;
		targetIDCounter += 1;															//Increment targetIDCounter
		placedTarget->idNum = targetIDCounter;											//Set the targetIDCounter

		targets.push_back(placedTarget);												//Push target stack

		clock.restart();																//Restart the clock

		randSpawnTimeframe = (rand() % 3) + 1;											//Choose a random time until next target spawns
		randTargetSpriteX = (rand() % 5);												//Choose a random sprite X
		randTargetSpriteY = (rand() % 4);												//Choose a random sprite Y

		targetExists += 1;																//Change how many targets there currently are
	}
}


void IntelligentSpawning()
{
	//Define clock
	static sf::Clock clock;

	int modifierThreshold = 5;			//Determines how many modifiers there should be on screen at once
	int targetThreshold = 5;			//Determines how many targets there should be on screen at once

	if (clock.getElapsedTime().asSeconds() > randSpawnTimeframe) {						//Check if it's time to spawn something
		randXSpawnPoint = (rand() % 401) + 50;											//Create random spawn location along X axis
		Vector2f position = Vector2f(randXSpawnPoint, 0);								//Define the X,Y position to spawn at

		if (modifierExists < modifierThreshold) {										//Check if there aren't enough modifiers
			auto rect = IntRect(randModifierSpriteX * 70, randModifierSpriteY * 70, 70, 70);	//Define the sprite for the modifier
			auto placedModifier = new Modifier(rect, position);									//Create the modifier

			placedModifier->idColour = randModifierSpriteX + 49;								//Set the modifier's X, Y, and num ids
			placedModifier->idShape = randModifierSpriteY + 49;
			modifierIDCounter += 1;																//Increment modifierIDCounter
			placedModifier->idNum = modifierIDCounter;

			modifiers.push_back(placedModifier);												//Push target stack
			randModifierSpriteX = (rand() % 5);													//Choose a random sprite X
			randModifierSpriteY = (rand() % 4);													//Choose a random sprite Y
			modifierExists += 1;																//Change how many targets there currently are
		}
		else if (targetExists < targetThreshold) {										//Check if there aren't enough targets
			auto rect = IntRect(randTargetSpriteX * 50, randTargetSpriteY * 50, 50, 50);		//Define the sprite for the target
			auto placedTarget = new Target(rect, position);										//Create the target

			placedTarget->idColour = randTargetSpriteX + 49;									//Set the modifier's X, Y, and num ids
			placedTarget->idShape = randTargetSpriteY + 49;
			targetIDCounter += 1;																//Increment targetIDCounter
			placedTarget->idNum = targetIDCounter;

			targets.push_back(placedTarget);													//Push target stack
			randTargetSpriteX = (rand() % 5);													//Choose a random sprite X
			randTargetSpriteY = (rand() % 4);													//Choose a random sprite Y
			targetExists += 1;																	//Change how many targets there currently are
		}

		clock.restart();																		//Restart the clock
		randSpawnTimeframe = (rand() % 3) + 1;													//Choose a random time until next target spawns
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

	//Render the modifiers
	for (const auto m : modifiers) {
		window.draw(*m);
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
	for (auto& t : targets) {
		t->Update(dt);

		//Remove targets when they reach the bottom of the screen
		if (t->getPosition().y > gameHeight) {
			targetExists -= 1;										//Change how many targets there currently are
			targets.erase(targets.begin());
		}
	};

	//Update modifiers
	for (auto& m : modifiers) {
		m->Update(dt);

		//Remove modifiers when they reach the bottom of the screen
		if (m->getPosition().y > gameHeight) {
			modifierExists -= 1;									//Change how many modifiers there currently are
			modifiers.erase(modifiers.begin());
		}
	}

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
		//SpawnTargets();
		IntelligentSpawning();

		//Update
		Update(window);

		//Draw game elements
		Render(window);

		//Display the rendered frame
		window.display();
	}
	return 0;
}