#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "player.h"
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

int randTargetSprite;

//Pointer for the player
Player* player;

//Pointer for targets
std::vector<Target*> targets;
std::vector<Modifier*> modifiers;

//Defining textures
sf::Texture spritesheet;
sf::Texture modifierSpritesheet;
sf::Sprite targetSprite;
sf::Sprite modifierSprite;

//Tracking the score
int score = 0;
//Tracking the lives
int lives = 3;

//List of current targets to spawn
std::vector<std::pair<int, int>> toSpawnTargets;

//Fonts
sf::Font font;
sf::Text scoreText;
sf::Text livesText;

void UpdateScoreText()
{
	//Set the score text to display the current scores
	scoreText.setString(std::to_string(score));
	//Position the score text at the top of the screen
	scoreText.setPosition((gameWidth * 0.8f) - (scoreText.getLocalBounds().width * 0.5f), 10.f);
}

void UpdateLivesText()
{
	livesText.setString("Lives: " + to_string(lives));
	livesText.setPosition((gameWidth * 0.2f) - (livesText.getLocalBounds().width * 0.5f), 10.f);
}

void Load()
{
	//Set variables
	targetIDCounter = 0;

	//Create player and place on the screen
	player = new Player();
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

	// Load font-face from res dir
	font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");

	// Set text element to use font
	scoreText.setFont(font);
	// set the character size to 24 pixels
	scoreText.setCharacterSize(24);
	// Set font color
	scoreText.setFillColor(sf::Color::Black);
	//Update the text to reflect initial scores
	UpdateScoreText();

	// Set text element to use font
	livesText.setFont(font);
	// set the character size to 24 pixels
	livesText.setCharacterSize(24);
	// Set font color
	livesText.setFillColor(sf::Color::Black);
	//Update the text to reflect initial scores
	UpdateLivesText();

}

void IntelligentSpawning()
{
	//Define clock
	static sf::Clock clock;

	int modifierThreshold = 2;			//Determines how many modifiers there should be on screen at once
	int targetThreshold = 2;			//Determines how many targets there should be on screen at once
	int tarOrMod;						//Determines whether a target or a modifier should be spawned

	if (clock.getElapsedTime().asSeconds() > randSpawnTimeframe) {						//Check if it's time to spawn something
		randXSpawnPoint = (rand() % 401) + 50;											//Create random spawn location along X axis
		Vector2f position = Vector2f(randXSpawnPoint, 0);								//Define the X,Y position to spawn at

		tarOrMod = (rand() % 10) + 1;													//Choose to spawn either a target or a modifier
		if (toSpawnTargets.size() <= 1) {												//If there is a low amount of targets set to spawn, weight towards spawning a modifier
			cout << " tst=" << toSpawnTargets.size();
			tarOrMod -= 2;
		}

		//if (modifierExists < modifierThreshold) {										//Check if there aren't enough modifiers
		if (toSpawnTargets.size() == 0 || tarOrMod < 5) {
			auto rect = IntRect(randModifierSpriteX * 70, randModifierSpriteY * 70, 70, 70);	//Define the sprite for the modifier
			auto placedModifier = new Modifier(rect, position);									//Create the modifier

			placedModifier->idColour = randModifierSpriteX + 49;								//Set the modifier's X, Y, and num ids
			placedModifier->idShape = randModifierSpriteY + 49;
			modifierIDCounter += 1;																//Increment modifierIDCounter
			placedModifier->idNum = modifierIDCounter;
			modifiers.push_back(placedModifier);												//Push modifier stack

			std::pair<int, int> justSpawnedMod = std::make_pair(randModifierSpriteX, randModifierSpriteY);
			toSpawnTargets.push_back(justSpawnedMod);

			randModifierSpriteX = (rand() % 5);													//Choose a random sprite X
			randModifierSpriteY = (rand() % 4);													//Choose a random sprite Y
			modifierExists += 1;																//Change how many modifiers there currently are
		}
		//else if (targetExists < targetThreshold) {									//Check if there aren't enough targets
		else if (tarOrMod >= 5) {
			int wildcard = (rand() % 5);												//Determine if a completely random target should be spawned

			if (wildcard == 0) {
				randTargetSpriteX = (rand() % 5);
				randTargetSpriteY = (rand() % 4);

				auto rect = IntRect(randTargetSpriteX * 50, randTargetSpriteY * 50, 50, 50);
				auto placedTarget = new Target(rect, position);

				placedTarget->idColour = randTargetSpriteX + 49;
				placedTarget->idShape = randTargetSpriteY + 49;
				targetIDCounter += 1;
				placedTarget->idNum = targetIDCounter;

				targets.push_back(placedTarget);
				targetExists += 1;
			}
			else if (toSpawnTargets.size() >= 0) {
				randTargetSprite = (rand() % toSpawnTargets.size());

				int tempX = toSpawnTargets.at(randTargetSprite).first;
				int tempY = toSpawnTargets.at(randTargetSprite).second;

				auto rect = IntRect(tempX * 50, tempY * 50, 50, 50);
				auto placedTarget = new Target(rect, position);

				placedTarget->idColour = tempX + 49;
				placedTarget->idColour = tempY + 49;
				targetIDCounter += 1;
				placedTarget->idNum = targetIDCounter;
				targets.push_back(placedTarget);

				targetExists += 1;

				toSpawnTargets.erase(toSpawnTargets.begin() + randTargetSprite);
			}
		}

		//cout << " tm=" << tarOrMod;

		clock.restart();																		//Restart the clock
		randSpawnTimeframe = (rand() % 2) + 1;													//Choose a random time until next target spawns
	}
}

//Take the modifier sprite and return the target sprite
IntRect GetCorrespondingTargetSprite(const IntRect& modifierSpriteRect)
{
	//Circles
	if (modifierSpriteRect == IntRect(0, 0, 70, 70))			//Modifier: Red Circle
		return IntRect(0, 0, 50, 50);							//Target: Red Circle
	else if (modifierSpriteRect == IntRect(70, 0, 70, 70))		//Modifier: Orange Circle
		return IntRect(50, 0, 50, 50);							//Target: Orange Circle
	else if (modifierSpriteRect == IntRect(140, 0, 70, 70))		//Modifier: Yellow Circle
		return IntRect(100, 0, 50, 50);							//Target: Yellow Circle
	else if (modifierSpriteRect == IntRect(210, 0, 70, 70))		//Modifier: Green Circle
		return IntRect(150, 0, 50, 50);							//Target: Green Circle
	else if (modifierSpriteRect == IntRect(280, 0, 70, 70))		//Modifier: Blue Circle
		return IntRect(200, 0, 50, 50);							//Target: Green Circle
	//Squares
	else if (modifierSpriteRect == IntRect(0, 70, 70, 70))		//Modifier: Red Square
		return IntRect(0, 50, 50, 50);							//Target: Red Square
	else if (modifierSpriteRect == IntRect(70, 70, 70, 70))		//Modifier: Orange Square
		return IntRect(50, 50, 50, 50);							//Target: Orange Square
	else if (modifierSpriteRect == IntRect(140, 70, 70, 70))	//Modifier: Yellow Square
		return IntRect(100, 50, 50, 50);						//Target: Yellow Square
	else if (modifierSpriteRect == IntRect(210, 70, 70, 70))	//Modifier: Green Square
		return IntRect(150, 50, 50, 50);						//Target: Green Square
	else if (modifierSpriteRect == IntRect(280, 70, 70, 70))	//Modifier: Blue Square
		return IntRect(200, 50, 50, 50);						//Target: Green Square
	//Triangles
	else if (modifierSpriteRect == IntRect(0, 140, 70, 70))		//Modifier: Red Triangle
		return IntRect(0, 100, 50, 50);							//Target: Red Triangle
	else if (modifierSpriteRect == IntRect(70, 140, 70, 70))	//Modifier: Orange Triangle
		return IntRect(50, 100, 50, 50);						//Target: Orange Triangle
	else if (modifierSpriteRect == IntRect(140, 140, 70, 70))	//Modifier: Yellow Triangle
		return IntRect(100, 100, 50, 50);						//Target: Yellow Triangle
	else if (modifierSpriteRect == IntRect(210, 140, 70, 70))	//Modifier: Green Triangle
		return IntRect(150, 100, 50, 50);						//Target: Green Triangle
	else if (modifierSpriteRect == IntRect(280, 140, 70, 70))	//Modifier: Blue Triangle
		return IntRect(200, 100, 50, 50);						//Target: Green Triangle
	//Octagons
	else if (modifierSpriteRect == IntRect(0, 210, 70, 70))		//Modifier: Red Octagon
		return IntRect(0, 150, 50, 50);							//Target: Red Octagon
	else if (modifierSpriteRect == IntRect(70, 210, 70, 70))	//Modifier: Orange Octagon
		return IntRect(50, 150, 50, 50);						//Target: Orange Octagon
	else if (modifierSpriteRect == IntRect(140, 210, 70, 70))	//Modifier: Yellow Octagon
		return IntRect(100, 150, 50, 50);						//Target: Yellow Octagon
	else if (modifierSpriteRect == IntRect(210, 210, 70, 70))	//Modifier: Green Octagon
		return IntRect(150, 150, 50, 50);						//Target: Green Octagon
	else if (modifierSpriteRect == IntRect(280, 210, 70, 70))	//Modifier: Blue Octagon
		return IntRect(200, 150, 50, 50);						//Target: Green Octagon
}


void Render(sf::RenderWindow& window)
{
	//Render the player
	window.draw(*player);

	//Render the targets
	for (const auto t : targets) {
		window.draw(*t);
	}

	//Render the modifiers
	for (const auto m : modifiers) {
		window.draw(*m);
	}

	window.draw(scoreText);
	window.draw(livesText);
}

void Update(sf::RenderWindow& window)
{
	//Reset clock, recalculate deltatime
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();

	//Process events
	sf::Event event;
	while (window.pollEvent(event))
	{
		//Close the window when the close event is triggered
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		//Close window on pressing Escape
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

	//Check for collisions with targets
	for (int i = 0; i < targets.size(); i++)
	{
		if (player->getGlobalBounds().intersects(targets[i]->getGlobalBounds()))
		{
			//Check if the player's sprite is in the same row as the target's sprite
			if (player->getTextureRect().top == targets[i]->getTextureRect().top)
			{
				score += 10; //Increment the score
				UpdateScoreText(); //Update the score display
			}
			else
			{
				lives -= 1;	 //Remove a life
				UpdateLivesText();	//Update the lives display
			}

			targets.erase(targets.begin() + i);
			targetExists--;
			i--;
		}
	}

	//Check for collisions with modifiers
	for (int i = 0; i < modifiers.size(); i++)
	{
		if (player->getGlobalBounds().intersects(modifiers[i]->getGlobalBounds()))
		{
			//Set the player sprite to the target sprite corresponding to the modifier sprite
			player->setTextureRect(GetCorrespondingTargetSprite(modifiers[i]->getTextureRect()));
			modifiers.erase(modifiers.begin() + i);
			modifierExists--;
			i--;
		}
	}

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

	//Memory for the player
	delete player;
	player = nullptr;

	//Memory for the targets
	for (auto t : targets)
	{
		delete t;
	}
	targets.clear();

	//Memory for the modifiers
	for (auto m : modifiers)
	{
		delete m;
	}
	modifiers.clear();

	return 0;
}