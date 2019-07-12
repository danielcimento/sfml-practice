#include "../Helpers.hpp"
#include <SFML/Window/VideoMode.hpp>

#define BLOCK_SIZE 20

// TODO: Joystick support

class Apple : public sf::RectangleShape {
	public:
		Apple(sf::Vector2f position) {
			setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			setFillColor(sf::Color::White);
			setPosition(position);
		}
};

class Snake : public sf::Drawable {
	public:
		std::vector<sf::RectangleShape> blocks;
		std::vector<sf::Vector2f> velocities;
		bool dead = false;

		Snake(sf::Vector2f initialPosition) {
			reset(initialPosition);
		}

		sf::RectangleShape *head() {
			return &blocks[0];
		}

		void reset(sf::Vector2f initialPosition) {
			dead = false;
			blocks.clear();
			addBlock();
			head()->setPosition(initialPosition);
		}

		void addBlock() {
			sf::RectangleShape shape(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			shape.setFillColor(sf::Color::White);
			if(!blocks.empty()) {
				shape.setPosition(blocks.back().getPosition());
			}
			blocks.push_back(shape);
			velocities.push_back(sf::Vector2f(0.0, 0.0));
		}

		void turn(sf::Vector2f direction) {
			// We don't want to allow turns in the complete opposite direction.
			if(
				!dead &&
				(direction.x != -velocities[0].x || direction.x == 0.0) &&
				(direction.y != -velocities[0].y || direction.y == 0.0)) {
				velocities[0] = sf::Vector2f(direction.x, direction.y);
			}
		}

		void update() {
			// Each block moves independently following its own "momentum"
			for (uint i = 0; i < blocks.size(); i++) {
				blocks[i].move(sf::Vector2f(velocities[i].x * BLOCK_SIZE, velocities[i].y * BLOCK_SIZE));
			}
			// Then, the block takes the velocity of the block in front of it.
			for (int i = blocks.size() - 1; i > 0; i--) {
				velocities[i] = velocities[i - 1];
			}
		}

		bool tryEat(Apple *apple) {
			if(apple->getGlobalBounds().intersects(head()->getGlobalBounds())) {
				addBlock();
				return true;
			}
			return false;
		}

		bool outOfBounds(float xBound, float yBound) {
			sf::Vector2f headPos = head()->getPosition();
			return headPos.x < 0.0 || headPos.y < 0.0 || headPos.x > xBound || headPos.y > yBound;
		}

		// We only check if the head collides with anything, because anything else would imply we've already collided.
		// It also allows us to skirt around the fact that newly created blocks start overlapping.
		bool collides() {
			if(blocks.size() < 5) {
				return false;
			}
			for (uint i = 1; i < blocks.size(); i++) {
				if(head()->getGlobalBounds().intersects(blocks[i].getGlobalBounds())) {
					return true;
				}
			}
			return false;
		}

		void kill() {
			dead = true;
			for(auto it = velocities.begin(); it != velocities.end(); it++) {
				*it = sf::Vector2f(0.0, 0.0);
			}
		}

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			for (auto it = blocks.begin(); it != blocks.end(); ++it) {
				target.draw(*it);
			}
		}
};

class Score : public sf::Drawable {
	private:
		sf::Font font;
		sf::Text text;
		int score;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			target.draw(text);
		}
	public:
		Score() {
			score = 0;
			font.loadFromFile("content/clacon.ttf");
			text.setString("Score: " + std::to_string(score));
			text.setFont(font);
			text.setCharacterSize(30);
			text.setFillColor(sf::Color::Yellow);
			text.move(sf::Vector2f(10.0, 0.0));
		}

		int getValue() {
			return score;
		}

		void increment() {
			score++;
			text.setString("Score: " + std::to_string(score));
		}

		void reset() {
			score = 0;
			text.setString("Score: " + std::to_string(score));
		}
};

class InfoText : public sf::Text {
	private:
		sf::Font font;
	public:
		InfoText(int horizontalBound) {
			font.loadFromFile("content/clacon.ttf");
			setString("Press <ESC> to exit.");
			setFont(font);
			setCharacterSize(30);
			setFillColor(sf::Color::White);

			sf::FloatRect bounds = getLocalBounds();
			setPosition(horizontalBound - bounds.width - 10, 0);
		}
};

class GameOverText : public sf::Text {
	private:
		sf::Font font;
	public:
		GameOverText(int score) {
			font.loadFromFile("content/clacon.ttf");
			setString("Game Over!\nFinal Score: " + std::to_string(score) + "\nPress <SPACE> to play again.");
			setFont(font);
			setCharacterSize(30);
			setFillColor(sf::Color::Yellow);
			setPosition(sf::Vector2f(10.0, 0.0));
		}
};

sf::Vector2f getRandomPosition(float maxX, float maxY) {
	int x, y;
	x = rand() % (int)maxX;
	x -= (x % BLOCK_SIZE);
	y = rand() % (int)maxY;
	y -= (y % BLOCK_SIZE);
	return sf::Vector2f(x, y);
}

void spacebar(Snake *snake, Score *score, sf::Clock *clock, GameOverText **gameOverText, sf::Vector2f *initialPosition) {
	if(snake->dead) {
		score->reset();
		snake->reset(*initialPosition);
		clock->restart();
		if(*gameOverText != NULL) {
			delete *gameOverText;
		}
	} else {
		snake->addBlock();
	}
}

void playSnake()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(vm.width, vm.height), "Snake", sf::Style::Fullscreen);
	window.setFramerateLimit(5);
// #ifdef SFML_SYSTEM_WINDOWS
// 	__windowsHelper.setIcon(window.getSystemHandle());
// #endif
	sf::Vector2f initialPosition = sf::Vector2f(vm.width / 2 - (vm.width % BLOCK_SIZE), vm.height / 2 - (vm.height % BLOCK_SIZE));
	Snake snake(initialPosition);
	Apple *apple = new Apple(getRandomPosition(vm.width, vm.height));
	GameOverText *gameOverText = NULL;
	Score score;
	InfoText iT(vm.width);

	sf::Clock clock;

	sf::Event event;

	while (window.isOpen()) {
		// Update
		while (window.pollEvent(event)) {
			// float joystickX = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
			// float joystickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
			// Past a certain sensitivity on the joystick, we move in that direction.
			// if(std::abs(joystickX) > 50.0 || std::abs(joystickY) > 50.0) {
			// 	if(std::abs(joystickX) > std::abs(joystickY)) {
			// 		snake.turn(sf::Vector2f(std::abs(joystickX) / joystickX, 0.0));
			// 	} else {
			// 		snake.turn(sf::Vector2f(0.0, std::abs(joystickY) / joystickY));
			// 	}
			// }
			// if(sf::Joystick::isButtonPressed(0, 0)) {
			// 	spacebar(&snake, &score, &clock, &gameOverText, &initialPosition);
			// } else if(sf::Joystick::isButtonPressed(0, 1)) {
			// 	window.close();
			// }
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					case sf::Keyboard::Right:
						snake.turn(sf::Vector2f(1.0, 0.0));
						break;
					case sf::Keyboard::Left:
						snake.turn(sf::Vector2f(-1.0, 0.0));
						break;
					case sf::Keyboard::Down:
						snake.turn(sf::Vector2f(0.0, 1.0));
						break;
					case sf::Keyboard::Up:
						snake.turn(sf::Vector2f(0.0, -1.0));
						break;
					case sf::Keyboard::Space:
						// TODO: Create a new apple when restarting the game.
						spacebar(&snake, &score, &clock, &gameOverText, &initialPosition);
						break;
					default:
						break;
				}
			}
		}

		snake.update();
		if(snake.outOfBounds(vm.width, vm.height) || snake.collides()) {
			snake.kill();
			gameOverText = new GameOverText(score.getValue());
		}
		// If the snake eats the apple, delete it and create a new one.
		if (snake.tryEat(apple)) {
			score.increment();
			delete apple;
			apple = new Apple(getRandomPosition(vm.width - BLOCK_SIZE, vm.height - BLOCK_SIZE));
		}

		// Render
		window.clear();
		window.draw(snake);
		window.draw(*apple);
		if(clock.getElapsedTime().asSeconds() < 5) {
			window.draw(iT);
		}
		if(snake.dead) {
			window.draw(*gameOverText);
		} else {
			window.draw(score);
		}
		window.display();
	}
}

