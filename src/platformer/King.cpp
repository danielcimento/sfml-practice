#include "King.hpp"
#include "MathHelpers.hpp"
#define KING_SCALE 3.0f
#define KING_VELOCITY 200.0f
#define KING_ACCELERATION 250.0f
#define TERMINAL_SPEED 600.0f
#define GRAVITY 400.0f
#define JUMP_HEIGHT 500.0f

King::King(sf::Vector2f screenDimensions) {
	currentVelocity = targetVelocity = sf::Vector2f(0.0, 0.0);

	for(int i = 0; i < 4; i++) {
			sf::Texture tex;
			if(!tex.loadFromFile("content/characters.png", sf::IntRect(17*i, 0, 17, 22))) {
				// TODO: Error handling
			}
			walkingTextures.push_back(tex);
		}
		for(int i = 0; i < 4; i++) {
			sf::Texture tex;
			if(!tex.loadFromFile("content/characters.png", sf::IntRect(17*(i + 4), 0, 17, 22))) {
				// TODO: Error handling
			}
			jumpingTextures.push_back(tex);
		}

	sprite.setTexture(walkingTextures[0]);
	sprite.setScale(sf::Vector2f(KING_SCALE, KING_SCALE));
	std::cout << sprite.getGlobalBounds().height << std::endl;
	sprite.setPosition(sf::Vector2f(screenDimensions.x / 2, screenDimensions.y - sprite.getGlobalBounds().height));
	sprite.setOrigin(8.5f, 0.0f);
	std::cout << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
}

void King::move(float timeDelta) {
	// Update position based on the current velocity with a few exceptions:
	//   -Don't move horizontally when charging a jump
	//   -Don't move vertically if it would take you below the screen
	sprite.setPosition(sf::Vector2f(
		jumping ? sprite.getPosition().x : sprite.getPosition().x + currentVelocity.x * timeDelta,
		std::min(sprite.getPosition().y + currentVelocity.y * timeDelta, 1014.0f)
	));

	// Update the velocity based on lerping the acceleration to the current velocity.
	currentVelocity = sf::Vector2f(
		approach(targetVelocity.x, currentVelocity.x, timeDelta * KING_ACCELERATION),
		approach(targetVelocity.y, currentVelocity.y, timeDelta * GRAVITY)
	);

	// If we're at the bottom of the screen, kill the vertical velocity (this allows us to jump)
	if(std::abs(sprite.getPosition().y - 1014.0f) < 0.001f) {
		currentVelocity.y = 0.0;
	}

	// If we're past the "charge" of the jump, we set our current velocity
	if(jumping && jumpingTimer.getElapsedTime().asSeconds() > 0.25f) {
		currentVelocity = sf::Vector2f(currentVelocity.x, -JUMP_HEIGHT);
		targetVelocity = sf::Vector2f(targetVelocity.x, TERMINAL_SPEED);
		jumping = false;
	}

}

void King::updateTexture() {
	// When "charging" the jump, switch to the jumping textures
	if(jumping && jumpingTimer.getElapsedTime().asSeconds() < 0.25f) {
		sprite.setTexture(jumpingTextures[0]);
	// When "rising", switch to the rising animation
	} else if(currentVelocity.y < 0.0){
		sprite.setTexture(jumpingTextures[1]);
	// When "falling", even when not jumping, change to the falling animation.
	} else if(currentVelocity.y > 0.0) {
		sprite.setTexture(jumpingTextures[2]);
	// If we aren't moving vertically, but we are moving horizontally, cycle the walk animation.
	} else if(currentVelocity.x != 0.0) {
		if(walkingTimer.getElapsedTime().asSeconds() > 0.25f) {
			currentWalkTexture = (currentWalkTexture + 1) % 4;
			sprite.setTexture(walkingTextures[currentWalkTexture]);
			walkingTimer.restart();
		}
	} else {
		sprite.setTexture(walkingTextures[currentWalkTexture = 0]);
	}
}

void King::update(float timeDelta) {
	move(timeDelta);
	updateTexture();
}

void King::handleKeypress(int keyCode) {
	switch(keyCode) {
		case sf::Keyboard::Right:
			if(!facingRight) {
				facingRight = true;
				sprite.scale(-1, 1);
			}
			targetVelocity = sf::Vector2f(KING_VELOCITY, targetVelocity.y);
			break;
		case sf::Keyboard::Left:
			if(facingRight) {
				facingRight = false;
				sprite.scale(-1, 1);
			}
			targetVelocity = sf::Vector2f(-KING_VELOCITY, targetVelocity.y);
			break;
		case sf::Keyboard::Down:
			break;
		case sf::Keyboard::Up:
			// if we aren't falling or charging a jump, start the jump cycle
			if(!jumping && std::abs(currentVelocity.y) < 0.001f) {
				jumping = true;
				jumpingTimer.restart();
			}
			break;
		case sf::Keyboard::Space:
			break;
		default:
			break;
	}
}

void King::handleKeyUp(int keyCode) {
	switch (keyCode) {
		case sf::Keyboard::Right:
			targetVelocity = sf::Vector2f(0.0f, targetVelocity.y);
			break;
		case sf::Keyboard::Left:
			targetVelocity = sf::Vector2f(0.0f, targetVelocity.y);
			break;
		case sf::Keyboard::Down:
			break;
		case sf::Keyboard::Up:
			if(!jumping) {
				currentVelocity = sf::Vector2f(currentVelocity.x, std::max(0.0f, currentVelocity.y));
			} else {
				jumping = false;
			}
			break;
		case sf::Keyboard::Space:
			break;
		default:
			break;
	}
}