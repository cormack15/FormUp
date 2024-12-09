//point-smooth-beep.mp3				-> RibhavAgrawal [user_id:39286533] on pixabay.com
//8-bit-video-game-points.mp3		-> Make_More_Sound [user_id:35032787] on pixabay.com
//classic-game-action-negative.mp3	-> floraphonic [user_id:38928062] on pixabay.com

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "player.h"
#include "shape.h"
#include "game.h"

using namespace std;
using namespace sf;

//Class to run all events in the game
class Game {
public:
	Game();
	void Run();

private:
	void Load();
	void UpdateScoreText();
	void UpdateLivesText();
	void IntelligentSpawning();
	void Events(sf::RenderWindow& window);
	void Options(sf::RenderWindow& window);
	void Restart(sf::RenderWindow& window);
	void Update(sf::RenderWindow& window);
	void Render(sf::RenderWindow& window);
	sf::IntRect GetCorrespondingTargetSprite(const sf::IntRect& modifierSpriteRect);

	sf::RenderWindow window;
	
	//Game objects
	Player* player;
	std::vector<Target*> targets;
	std::vector<Modifier*> modifiers;

	//Textures & Sprites
	sf::Texture spritesheet;
	sf::Texture modifierSpritesheet;
	sf::Texture background;
	sf::Texture tutorial;
	sf::Texture pauseButton;
	sf::Texture pauseMenu;
	sf::Texture gameoverMenu;
	sf::Texture optionsMenu;
	sf::Sprite targetSprite;
	sf::Sprite modifierSprite;
	sf::Sprite backgroundSprite;
	sf::Sprite tutorialSprite;
	sf::Sprite pauseButtonSprite;
	sf::Sprite pauseMenuSprite;
	sf::Sprite gameoverMenuSprite;
	sf::Sprite optionsMenuSprite;

	//Audio
	sf::SoundBuffer modifierSFXBuffer;
	sf::SoundBuffer targetSFXBuffer;
	sf::SoundBuffer damageSFXBuffer;
	sf::Sound modifierSFX;
	sf::Sound targetSFX;
	sf::Sound damageSFX;

	//Fonts & Text
	sf::Font font;
	sf::Text scoreText;
	sf::Text livesText;

	//Game state variables
	int randXSpawnPoint;
	int randSpawnTimeframe = 1;
	int randTargetSpriteX;	int randModifierSpriteX;		//Stores the random X coord for targets and modifiers
	int randTargetSpriteY;	int randModifierSpriteY;		//Stores the random Y coord for targets and modifiers
	int targetIDCounter;	int modifierIDCounter;			//Stores the ID counter for targets and modifiers
	int targetExists;				//Stores how many targets currently exist
	int modifierExists;				//Stores how many modifiers currently exist
	int randTargetSprite;
	int score = 0;					//Stores the player's score
	int lives = 3;					//Stores the player's lives
	float prevDT;					//Stores the dt before the game was paused for Update()
	char spawnerFlag;				//Stores a flag to direct the spawner
	bool isGamePaused = false;		//Stores if the game is paused
	bool isGameover = false;		//Stores if the game is over
	bool isGameOptions = false;		//Stores if the options menu is open
	bool justResumed = false;		//Stores if the game was just resumed
	std::vector<std::pair<int, int>> toSpawnTargets;		//List of current targets to spawn

	sf::Clock spawnClock;
};

//UI & Variables
Game::Game()
	: window(sf::VideoMode(gameWidth, gameHeight), "FORM UP"),
	player(nullptr),
	score(0),
	lives(3),
	targetIDCounter(0),
	modifierIDCounter(0),
	targetExists(0),
	modifierExists(0),
	randSpawnTimeframe(1) {
		window.setFramerateLimit(60);
		srand(static_cast<unsigned>(time(0)));

		Load();
}

//Load assets
void Game::Load() {
	//Load & set textures
	if (!spritesheet.loadFromFile("res/spritesheet.png")) {							//Targets
		std::cerr << "Failed to load spritesheet" << std::endl;
	}
	targetSprite.setTexture(spritesheet);
	targetSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	if (!modifierSpritesheet.loadFromFile("res/modifierspritesheet.png")) {			//Modifiers
		std::cerr << "Failed to load modifier spritesheet" << std::endl;
	}
	modifierSprite.setTexture(modifierSpritesheet);
	modifierSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(70, 70)));

	if (!background.loadFromFile("res/bg.png")) {									//Background
		std::cerr << "Failed to load background" << std::endl;
	}
	backgroundSprite.setTexture(background);
	backgroundSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	if (!tutorial.loadFromFile("res/tutorial.png")) {								//Tutorial
		std::cerr << "Failed to load tutorial image" << std::endl;
	}
	tutorialSprite.setTexture(tutorial);
	tutorialSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	if (!pauseButton.loadFromFile("res/pausebutton.png")) {							//Pause button
		std::cerr << "Failed to load pause button image" << std::endl;
	}
	pauseButtonSprite.setTexture(pauseButton);
	pauseButtonSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(50, 50)));

	if (!pauseMenu.loadFromFile("res/pausemenu.png")) {								//Pause menu
		std::cerr << "Failed to load pause menu image" << std::endl;
	}
	pauseMenuSprite.setTexture(pauseMenu);
	pauseMenuSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	if (!optionsMenu.loadFromFile("res/optionsmenu.png")) {							//Options menu
		std::cerr << "Failed to load options menu image" << std::endl;
	}
	optionsMenuSprite.setTexture(optionsMenu);
	optionsMenuSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	if (!gameoverMenu.loadFromFile("res/gameover.png")) {							//Gameover menu
		std::cerr << "Failed to load gameover image" << std::endl;
	}
	gameoverMenuSprite.setTexture(gameoverMenu);
	gameoverMenuSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(500, 900)));

	//Create player with loaded spritesheet texture
	player = new Player(spritesheet);
	player->setPosition(sf::Vector2f(static_cast<float>(gameWidth) / 2.f, static_cast<float>(gameHeight) / 1.2f));
	player->idColour = 49;
	player->idShape = 49;


	//Load audio
	if (!targetSFXBuffer.loadFromFile("res/8-bit-video-game-points.mp3")) {			//Target SFX
		std::cerr << "Failed to load target SFX file" << std::endl;
	}
	targetSFX.setBuffer(targetSFXBuffer);

	if (!modifierSFXBuffer.loadFromFile("res/point-smooth-beep.mp3")) {				//Modifier SFX
		std::cerr << "Failed to load modifier SFX file" << std::endl;
	}
	modifierSFX.setBuffer(modifierSFXBuffer);

	if (!damageSFXBuffer.loadFromFile("res/classic-game-action-negative.mp3")) {	//Damage SFX
		std::cerr << "Failed to load damage SFX file" << std::endl;
	}
	damageSFX.setBuffer(damageSFXBuffer);


	//Load fonts & set up text
	if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {					//Font
		std::cerr << "Failed to load font" << std::endl;
	}

	scoreText.setFont(font);														//Score font
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::Black);
	UpdateScoreText();

	livesText.setFont(font);														//Lives font
	livesText.setCharacterSize(24);
	livesText.setFillColor(sf::Color::Black);
	UpdateLivesText();
}

//Set & position score text
void Game::UpdateScoreText() {
	scoreText.setString(std::to_string(score));
	scoreText.setPosition((gameWidth * 0.8f) - (scoreText.getLocalBounds().width * 0.5f), 10.f);
}

//Set & position lives text
void Game::UpdateLivesText() {
	livesText.setString("Lives: " + to_string(lives));
	livesText.setPosition((gameWidth * 0.2f) - (livesText.getLocalBounds().width * 0.5f), 10.f);
}

//Spawn targets & modifiers
void Game::IntelligentSpawning() {
	if (spawnClock.getElapsedTime().asSeconds() > randSpawnTimeframe) {		//Check if it's time to spawn something
		int randXSpawnPoint = ((rand() % 401) + 50);						//Create random spawn location along X axis
		sf::Vector2f position(static_cast<float>(randXSpawnPoint), 0.f);	//Set the position

		int tarOrMod = (rand() % 10) + 1;									//Target or modifier decider

		if (toSpawnTargets.empty()) {										//If there are no targets set to spawn, weight towards spawning a modifier
			tarOrMod -= 2;
		}
		else if (toSpawnTargets.size() >= 5) {								//If there are too many targets queued up, erase the oldest
			toSpawnTargets.erase(toSpawnTargets.end() - 1);
		}

		//Set random coordinates for targets and modifiers
		int randModifierSpriteX = (rand() % 5);
		int randModifierSpriteY = (rand() % 4);
		int randTargetSpriteX = (rand() % 5);
		int randTargetSpriteY = (rand() % 4);

		if (toSpawnTargets.empty() || tarOrMod < 5) {						//Spawn a modifier if there are no targets to spawn
			sf::IntRect rect(randModifierSpriteX * 70, randModifierSpriteY * 70, 70, 70);
			auto placedModifier = new Modifier(rect, position, modifierSpritesheet);

			if (score >= 0 && score <= 100) {
				placedModifier->speed += (score / 10.0f);
			}
			else {
				placedModifier->speed += (score / 10.0f) * 2.0f;
			}

			placedModifier->idColour = randModifierSpriteX + 49;
			placedModifier->idShape = randModifierSpriteY + 49;
			modifierIDCounter++;
			placedModifier->idNum = modifierIDCounter;

			modifiers.push_back(placedModifier);
			toSpawnTargets.push_back(std::make_pair(randModifierSpriteX, randModifierSpriteY));
			modifierExists++;
		}
		else {																//Else spawn a target
			int wildcard = (rand() % 10);									//Decide if the spawned target should be from the list or random

			if (wildcard == 0) {											//Spawn random target
				auto rect = IntRect(randTargetSpriteX * 50, randTargetSpriteY * 50, 50, 50);
				auto placedTarget = new Target(rect, position, spritesheet);

				if (score >= 0 && score <= 100) {								//Adjust target's speed by score
					placedTarget->speed += (score / 10);
				}
				else {
					placedTarget->speed += (score / 10) * 2;
				}

				placedTarget->idColour = randTargetSpriteX + 49;
				placedTarget->idShape = randTargetSpriteY + 49;
				targetIDCounter++;
				placedTarget->idNum = targetIDCounter;

				targets.push_back(placedTarget);
				targetExists++;
			}
			else if (!toSpawnTargets.empty()) {								//Spawn a target from the queue
				int randTargetSpriteIndex = (rand() % toSpawnTargets.size());
				int tempX = toSpawnTargets.at(randTargetSpriteIndex).first;
				int tempY = toSpawnTargets.at(randTargetSpriteIndex).second;

				auto rect = IntRect(tempX * 50, tempY * 50, 50, 50);
				auto placedTarget = new Target(rect, position, spritesheet);

				if (score >= 0 && score <= 100) {								//Adjust target's speed by score
					placedTarget->speed += (score / 10);
				}
				else {
					placedTarget->speed += (score / 10) * 2;
				}

				placedTarget->idColour = tempX + 49;
				placedTarget->idShape = tempY + 49;
				targetIDCounter++;
				placedTarget->idNum = targetIDCounter;

				targets.push_back(placedTarget);
				targetExists++;

				toSpawnTargets.erase(toSpawnTargets.begin() + randTargetSpriteIndex);				//Remove the target from the list
			}
		}

		spawnClock.restart();
		randSpawnTimeframe = (rand() % 2) + 1;
	}
}

//Get a target sprite
sf::IntRect Game::GetCorrespondingTargetSprite(const sf::IntRect& modifierSpriteRect) {
	// Map modifier rects to target rects (Example mapping)
	// Circles
	if (modifierSpriteRect == sf::IntRect(0, 0, 70, 70))          return sf::IntRect(0, 0, 50, 50);   // Red Circle
	else if (modifierSpriteRect == sf::IntRect(70, 0, 70, 70))    return sf::IntRect(50, 0, 50, 50);   // Orange Circle
	else if (modifierSpriteRect == sf::IntRect(140, 0, 70, 70))   return sf::IntRect(100, 0, 50, 50);   // Yellow Circle
	else if (modifierSpriteRect == sf::IntRect(210, 0, 70, 70))   return sf::IntRect(150, 0, 50, 50);   // Green Circle
	else if (modifierSpriteRect == sf::IntRect(280, 0, 70, 70))   return sf::IntRect(200, 0, 50, 50);   // Blue Circle

	// Squares
	else if (modifierSpriteRect == sf::IntRect(0, 70, 70, 70))    return sf::IntRect(0, 50, 50, 50);  // Red Square
	else if (modifierSpriteRect == sf::IntRect(70, 70, 70, 70))   return sf::IntRect(50, 50, 50, 50);  // Orange Square
	else if (modifierSpriteRect == sf::IntRect(140, 70, 70, 70))  return sf::IntRect(100, 50, 50, 50);  // Yellow Square
	else if (modifierSpriteRect == sf::IntRect(210, 70, 70, 70))  return sf::IntRect(150, 50, 50, 50);  // Green Square
	else if (modifierSpriteRect == sf::IntRect(280, 70, 70, 70))  return sf::IntRect(200, 50, 50, 50);  // Blue Square

	// Triangles
	else if (modifierSpriteRect == sf::IntRect(0, 140, 70, 70))   return sf::IntRect(0, 100, 50, 50); // Red Triangle
	else if (modifierSpriteRect == sf::IntRect(70, 140, 70, 70))  return sf::IntRect(50, 100, 50, 50); // Orange Triangle
	else if (modifierSpriteRect == sf::IntRect(140, 140, 70, 70)) return sf::IntRect(100, 100, 50, 50); // Yellow Triangle
	else if (modifierSpriteRect == sf::IntRect(210, 140, 70, 70)) return sf::IntRect(150, 100, 50, 50); // Green Triangle
	else if (modifierSpriteRect == sf::IntRect(280, 140, 70, 70)) return sf::IntRect(200, 100, 50, 50); // Blue Triangle

	// Octagons
	else if (modifierSpriteRect == sf::IntRect(0, 210, 70, 70))   return sf::IntRect(0, 150, 50, 50); // Red Octagon
	else if (modifierSpriteRect == sf::IntRect(70, 210, 70, 70))  return sf::IntRect(50, 150, 50, 50); // Orange Octagon
	else if (modifierSpriteRect == sf::IntRect(140, 210, 70, 70)) return sf::IntRect(100, 150, 50, 50); // Yellow Octagon
	else if (modifierSpriteRect == sf::IntRect(210, 210, 70, 70)) return sf::IntRect(150, 150, 50, 50); // Green Octagon
	else if (modifierSpriteRect == sf::IntRect(280, 210, 70, 70)) return sf::IntRect(200, 150, 50, 50); // Blue Octagon

	// Default fallback if no match found
	return sf::IntRect(0, 0, 50, 50);
}

//Events
void Game::Events(sf::RenderWindow& window) {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type = sf::Event::Closed) {
			window.close();
		}

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
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 309 && sf::Mouse::getPosition(window).y <= 386) && isGamePaused && !isGameOptions) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isGamePaused = false;
			justResumed = true;
		}
	}

	//Options button
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 431 && sf::Mouse::getPosition(window).y <= 508) && isGamePaused) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isGamePaused = true;
			isGameOptions = true;
		}
	}

	//Quit button
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 556 && sf::Mouse::getPosition(window).y <= 633) && isGamePaused && !isGameOptions) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			window.close();
		}
	}

	//Gameover check
	if (lives <= 0) {
		isGamePaused = true;
		isGameover = true;

		//Quit button
		if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 556 && sf::Mouse::getPosition(window).y <= 633)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				window.close();				//Close the game
			}
		}

		//Restart button
		if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 309 && sf::Mouse::getPosition(window).y <= 386)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				Restart(window);			//Restart the game
			}
		}
	}
}

//Options menu
void Game::Options(sf::RenderWindow& window) {
	window.draw(optionsMenuSprite);				//Render options menu sprite

	//Back button
	if ((sf::Mouse::getPosition(window).x >= 70 && sf::Mouse::getPosition(window).x <= 431) && (sf::Mouse::getPosition(window).y >= 758 && sf::Mouse::getPosition(window).y <= 836)) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			isGameOptions = false;
		}
	}

	//WASD button
	if ((sf::Mouse::getPosition(window).x >= 127 && sf::Mouse::getPosition(window).x <= 373) && (sf::Mouse::getPosition(window).y >= 277 && sf::Mouse::getPosition(window).y <= 344)) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			player->controlSetup = false;
		}
	}

	//Arrows button
	if ((sf::Mouse::getPosition(window).x >= 127 && sf::Mouse::getPosition(window).x <= 373) && (sf::Mouse::getPosition(window).y >= 399 && sf::Mouse::getPosition(window).y <= 466)) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			player->controlSetup = true;
		}
	}
}

void Game::Restart(sf::RenderWindow& window) {
	score = 0;																			//Reset score
	lives = 3;																			//Reset lives
	UpdateScoreText();
	UpdateLivesText();

	while (!modifiers.empty()) {														//Clear modifiers list
		modifiers.erase(modifiers.begin());
	}

	while (!targets.empty()) {															//Clear targets list
		targets.erase(targets.begin());
	}

	player->idColour = 49;																//Reset player idColour
	player->idShape = 49;																//Reset player idShape
	player->setTextureRect(IntRect(0, 0, 50, 50));										//Reset player sprite
	player->setPosition(sf::Vector2f(gameWidth / 2.f, gameHeight / 1.2f));				//Reset player position


	isGamePaused = false;
	isGameover = false;
}

//Update
void Game::Update(sf::RenderWindow& window) {
	float dt = 1.0f / 60.0f;		//60 FPS

	player->Update(dt);

	for (auto& t : targets) {		//Update targets
		t->Update(dt);
	}

	targets.erase(std::remove_if(targets.begin(), targets.end(), [&](Target* t) {			//Remove targets that go off-screen
		if (t->getPosition().y > gameHeight) {
			targetExists--;
			delete t;
			return true;
		}
		return false;
	}), targets.end());

	for (auto& m : modifiers) {		//Update modifiers
		m->Update(dt);
	}

	modifiers.erase(std::remove_if(modifiers.begin(), modifiers.end(), [&](Modifier* m) {	//Remove modifiers that go off-screen
		if (m->getPosition().y > gameHeight) {
			modifierExists--;
			delete m;
			return true;
		}
		return false;
	}), modifiers.end());

	for (int i = 0; i < (int)targets.size(); i++) {										//Player-target collision
		if (player->getGlobalBounds().intersects(targets[i]->getGlobalBounds())) {
			if (player->idColour == targets[i]->idColour && player->idShape == targets[i]->idShape) {
				targetSFX.play();
				score += 10;
				UpdateScoreText();
			}
			else {
				damageSFX.play();
				lives -= 1;
				UpdateLivesText();
			}

			targetExists--;
			delete targets[i];
			targets.erase(targets.begin() + i);
			i--;
		}
	}

	for (int i = 0; i < (int)modifiers.size(); i++) {										//Player-modifier collision
		if (player->getGlobalBounds().intersects(modifiers[i]->getGlobalBounds())) {
			modifierSFX.play();
			player->idColour = modifiers[i]->idColour;
			player->idShape = modifiers[i]->idShape;

			score += 2;
			UpdateScoreText();

			// Change the player's texture rect to reflect the new shape
			sf::IntRect newRect = GetCorrespondingTargetSprite(modifiers[i]->getTextureRect());
			player->setTextureRect(newRect);

			modifierExists--;
			delete modifiers[i];
			modifiers.erase(modifiers.begin() + i);
			i--;
		}
	}
}

//Render
void Game::Render(sf::RenderWindow& window) {
	window.draw(backgroundSprite);		//Render background
	window.draw(*player);				//Render player

	for (const auto& t : targets) {		//Render targets
		window.draw(*t);
	}

	for (const auto& m : modifiers) {	//Render modifiers
		window.draw(*m);
	}

	window.draw(scoreText);				//Render score text
	window.draw(livesText);				//Render lives text

	if (score == 0) {					//Render tutorial
		window.draw(tutorialSprite);
	}

	if (isGamePaused == false) {			//Render pause button
		window.draw(pauseButtonSprite);
	}
	else if (isGameover == true) {			//Render gameover menu
		window.draw(gameoverMenuSprite);
	}
	else if (isGameOptions == true) {		//Render options menu
		Options(window);
	}
	else {									//Render pause menu
		window.draw(pauseMenuSprite);
	}
}

//Run loop
void Game::Run() {
	while (window.isOpen()) {
		window.clear(sf::Color::White);

		Events(window);

		if (!isGamePaused) {			//If the game is paused, skip spawning and update
			IntelligentSpawning();
			Update(window);
		}

		Render(window);

		window.display();
	}

	delete player;				//Delete player
	player = nullptr;

	for (auto t : targets) {	//Delete targets
		delete t;
	}
	targets.clear();

	for (auto m : modifiers) {	//Delete modifiers
		delete m;
	}
	modifiers.clear();
}

//Main method
int main() {
	Game game;
	game.Run();
	return 0;
}