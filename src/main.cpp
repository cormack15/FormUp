#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "game.h"

//VARIABLES 


//Unique pointer for the player
std::unique_ptr<Player> player;

void Load()
{
	player = std::make_unique<Player>();
	player->setPosition(sf::Vector2f(gameWidth / 2.f, gameHeight / 1.2f));

}

void Render(sf::RenderWindow& window)
{
	player->Render(window);
	
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

	player->Update(dt);

}



int main() {
	//Window
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "FORM UP");
	window.setFramerateLimit(60);
	//Load
	Load();

	//Clear, Update, Render, and Display
	while (window.isOpen()) {

		// Clear the window
		window.clear(sf::Color::White);

		// Update
		Update(window);

		// Draw game elements
		Render(window);

		// Display the rendered frame
		window.display();
	}

	return 0;
}