#include <SFML/Graphics.hpp>
#include <iostream>

//VARIABLES
//Game width
const int gameWidth = 800;
//Game height
const int gameHeight = 600;

void Load()
{


}

void Render(sf::RenderWindow& window)
{

	
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

}



int main() {
	//Window
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "SPACE INVADERS");
	window.setFramerateLimit(60);
	//Load
	Load();

	//Clear, Update, Render, and Display
	while (window.isOpen()) {

		// Clear the window
		window.clear();

		// Update
		Update(window);

		// Draw game elements
		Render(window);

		// Display the rendered frame
		window.display();
	}

	return 0;
}