// #include "../Helpers.hpp"
#include "King.hpp"
#include <SFML/Window/VideoMode.hpp>

// TODO: Add backdrop + platforms + enemies(?)

void playPlatformer()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(vm.width, vm.height), "Platformer", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
// #ifdef SFML_SYSTEM_WINDOWS
// 	__windowsHelper.setIcon(window.getSystemHandle());
// #endif
	sf::Event event;
	sf::Clock gameClock;

	King king(sf::Vector2f(vm.width, vm.height));

	sf::Time lastTime = gameClock.getElapsedTime();
	while (window.isOpen()) {
		sf::Time current = gameClock.getElapsedTime();
		sf::Time elapsed = current - lastTime;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				king.handleKeypress(event.key.code);
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					default:
						break;
				}
			} else if (event.type == sf::Event::KeyReleased) {
				king.handleKeyUp(event.key.code);
			}
		}

		// Update
		king.update(elapsed.asSeconds());

		// Render
		window.clear();
		window.draw(king);
		window.display();

		lastTime = current;
	}
}

