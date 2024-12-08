//point-smooth-beep.mp3				-> RibhavAgrawal [user_id:39286533] on pixabay.com
//8-bit-video-game-points.mp3		-> Make_More_Sound [user_id:35032787] on pixabay.com
//classic-game-action-negative.mp3	-> floraphonic [user_id:38928062] on pixabay.com

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
int randTargetSprite;

float prevDT;					//Stores the dt before the game was paused for Update()

char spawnerFlag;				//Stores a flag to direct the spawner

bool isGamePaused = false;		//Stores if the game is paused.
bool justResumed = false;		//Stores if the game was just resumed

//Pointer for the player
Player* player;

//Pointer for targets
std::vector<Target*> targets;
std::vector<Modifier*> modifiers;

//Defining textures
sf::Texture spritesheet;
sf::Texture modifierSpritesheet;
sf::Texture background;
sf::Texture tutorial;
sf::Texture pauseButton;
sf::Texture pauseMenu;
sf::Sprite targetSprite;
sf::Sprite modifierSprite;
sf::Sprite backgroundSprite;
sf::Sprite tutorialSprite;
sf::Sprite pauseButtonSprite;
sf::Sprite pauseMenuSprite;

//Defining sounds
sf::SoundBuffer modifierSFXBuffer;
sf::SoundBuffer targetSFXBuffer;
sf::SoundBuffer damageSFXBuffer;
sf::Sound modifierSFX;
sf::Sound targetSFX;
sf::Sound damageSFX;

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

void Load()
{
	//Set variables
	targetIDCounter = 0;

	//Create player and place on the screen
	player = new Player();
	player->setPosition(sf::Vector2f(gameWidth / 2.f, gameHeight / 1.2f));

	//Set player's initial colour and shape ids
	player->idColour = 49;
	player->idShape = 49;

	//Error handling for loading textures and sounds
	if (!spritesheet.loadFromFile("res/spritesheet.png")) {
		std::cerr << "Failed to load spritesheet" << std::endl;
	}
	if (!modifierSpritesheet.loadFromFile("res/modifierspritesheet.png")) {
		std::cerr << "Failed to load modifier spritesheet" << std::endl;
	}
	if (!background.loadFromFile("res/bg.png")) {
		std::cerr << "Failed to load background" << std::endl;
	}
	if (!tutorial.loadFromFile("res/tutorial.png")) {
		std::cerr << "Failed to load tutorial image" << std::endl;
	}
	if (!pauseButton.loadFromFile("res/pausebutton.png")) {
		std::cerr << "Failed to load pause button image" << std::endl;
	}
	if (!pauseMenu.loadFromFile("res/pausemenu.png")) {
		std::cerr << "Failed to load pause button image" << std::endl;
	}
	if (!modifierSFXBuffer.loadFromFile("res/point-smooth-beep.mp3")) {
		std::cerr << "Failed to load modifier SFX file" << std::endl;
	}
	if (!targetSFXBuffer.loadFromFile("res/8-bit-video-game-points.mp3")) {
		std::cerr << "Failed to load target SFX file" << std::endl;
	}
	if (!damageSFXBuffer.loadFromFile("res/classic-game-action-negative.mp3")) {
		std::cerr << "Failed to load damage SFX file" << std::endl;
	}

	//Load in sprite for a target
	targetSprite.setTexture(spritesheet);
	targetSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	//Load in sprite for a modifier
	modifierSprite.setTexture(modifierSpritesheet);
	modifierSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(70, 70)));

	//Load in background
	backgroundSprite.setTexture(background);
	backgroundSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	//Load in tutorial
	tutorialSprite.setTexture(tutorial);
	tutorialSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	//Load in pause button
	pauseButtonSprite.setTexture(pauseButton);
	pauseButtonSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	//Load in pause menu
	pauseMenuSprite.setTexture(pauseMenu);
	pauseMenuSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	//Load in audio
	modifierSFXBuffer.loadFromFile("res/point-smooth-beep.mp3");
	targetSFXBuffer.loadFromFile("res/8-bit-video-game-points.mp3");
	damageSFXBuffer.loadFromFile("res/classic-game-action-negative.mp3");
	modifierSFX.setBuffer(modifierSFXBuffer);
	targetSFX.setBuffer(targetSFXBuffer);
	damageSFX.setBuffer(damageSFXBuffer);

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

void Events(sf::RenderWindow& window) {
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

	//Pause button
	if ((sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x <= 50) && (sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).y <= 50) && !isGamePaused) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isGamePaused = true;
		}
	}

	//Resume button
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 309 && sf::Mouse::getPosition(window).y <= 386) && isGamePaused) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isGamePaused = false;
			justResumed = true;
		}
	}

	//Quit button
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 556 && sf::Mouse::getPosition(window).y <= 633) && isGamePaused) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			window.close();
		}
	}
}

void IntelligentSpawning()
{
	static sf::Clock clock;				//Define clock

	int tarOrMod;						//Determines whether a target or a modifier should be spawned

	if (clock.getElapsedTime().asSeconds() > randSpawnTimeframe) {						//Check if it's time to spawn something
		randXSpawnPoint = (rand() % 401) + 50;											//Create random spawn location along X axis
		Vector2f position = Vector2f(randXSpawnPoint, 0);								//Define the X,Y position to spawn at

		tarOrMod = (rand() % 10) + 1;													//Choose to spawn either a target or a modifier
		if (toSpawnTargets.size() <= 1) {												//If there is a low amount of targets set to spawn, weight towards spawning a modifier
			tarOrMod -= 2;
		}
		else if (toSpawnTargets.size() >= 5) {											//If there are too many targets queued up, erase the oldest in the queue
			toSpawnTargets.erase(toSpawnTargets.end() - 1);
		}

		if (toSpawnTargets.size() == 0 || tarOrMod < 5) {								//Spawn a modifier if there is no targets to spawn or if chosen randomly
			auto rect = IntRect(randModifierSpriteX * 70, randModifierSpriteY * 70, 70, 70);	//Define the sprite for the modifier
			auto placedModifier = new Modifier(rect, position);									//Create the modifier

			if (score >= 0 && score <= 100) {													//Adjust modifier's speed by score. Speed adjustment increases after 100 score
				placedModifier->speed += (score/10);
			}
			else {
				placedModifier->speed += (score/10) * 2;
			}

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
		else if (tarOrMod >= 5) {														//Spawn a target if chosen randomly
			int wildcard = (rand() % 10);												//Determine if a completely random target should be spawned

			if (wildcard == 0) {														//If 0 is picked, spawn and set a random target
				randTargetSpriteX = (rand() % 5);
				randTargetSpriteY = (rand() % 4);

				auto rect = IntRect(randTargetSpriteX * 50, randTargetSpriteY * 50, 50, 50);
				auto placedTarget = new Target(rect, position);

				if (score >= 0 && score <= 100) {													//Adjust target's speed by score
					placedTarget->speed += (score/10);
				}
				else {
					placedTarget->speed += (score/10) * 2;
				}

				placedTarget->idColour = randTargetSpriteX + 49;
				placedTarget->idShape = randTargetSpriteY + 49;
				targetIDCounter += 1;
				placedTarget->idNum = targetIDCounter;

				targets.push_back(placedTarget);
				targetExists += 1;
			}
			else if (toSpawnTargets.size() >= 0) {										//If not a random one, if there is a target to be spawned, spawn it
				randTargetSprite = (rand() % toSpawnTargets.size());

				int tempX = toSpawnTargets.at(randTargetSprite).first;
				int tempY = toSpawnTargets.at(randTargetSprite).second;

				auto rect = IntRect(tempX * 50, tempY * 50, 50, 50);
				auto placedTarget = new Target(rect, position);

				if (score >= 0 && score <= 100) {												//Adjust target's speed by score
					placedTarget->speed += (score / 10) * 2;
				}
				else {
					placedTarget->speed += (score / 10);
				}

				placedTarget->idColour = tempX + 49;
				placedTarget->idShape = tempY + 49;
				targetIDCounter += 1;
				placedTarget->idNum = targetIDCounter;
				targets.push_back(placedTarget);
				targetExists += 1;

				toSpawnTargets.erase(toSpawnTargets.begin() + randTargetSprite);				//Remove the target from the list
			}
		}

		clock.restart();																		//Restart the clock
		randSpawnTimeframe = ((rand() % 2) + 1);												//Choose a random time until next target spawns
	}
}

void Update(sf::RenderWindow& window)
{
	//Reset clock, recalculate deltatime
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	
	if (justResumed) {							//If the game was just resumed, reset dt to what it was before the game was paused
		dt = prevDT;
	}

	prevDT = dt;								//Store what dt is to reset back to it after resuming

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
			if (player->idColour == targets[i]->idColour && player->idShape == targets[i]->idShape)		//Check if the player's colour and shape matches the target's
			{
				targetSFX.play();	//Play target collection SFX
				score += 10;		//Increment the score
				UpdateScoreText();	//Update the score display
			}
			else
			{
				damageSFX.play();	//Play damage SFX
				lives -= 1;			//Remove a life
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
			modifierSFX.play();								//Play modifier collection SFX
			
			player->idColour = modifiers[i]->idColour;		//Set the player's colour and shape id to the modifier collected
			player->idShape = modifiers[i]->idShape;

			score += 2;
			UpdateScoreText();

			//Set the player sprite to the target sprite corresponding to the modifier sprite
			player->setTextureRect(GetCorrespondingTargetSprite(modifiers[i]->getTextureRect()));
			modifiers.erase(modifiers.begin() + i);
			modifierExists--;
			i--;
		}
	}
}

void Render(sf::RenderWindow& window)
{
	//Render the background
	window.draw(backgroundSprite);

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

	//Render the score and lives text
	window.draw(scoreText);
	window.draw(livesText);

	//Render the tutorial
	if (score == 0) {
		window.draw(tutorialSprite);
	}

	//Render the pause button
	if (isGamePaused == false) {
		window.draw(pauseButtonSprite);
	}
	else {
		window.draw(pauseMenuSprite);
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

		//Process events
		Events(window);

		if (isGamePaused == false) {
			//Spawn Target
			IntelligentSpawning();

			//Update
			Update(window);
		}

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