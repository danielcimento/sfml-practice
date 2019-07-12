#ifndef KING_H
#define KING_H

#include "Camera.hpp"

class King : public CameraDrawable {
	public:
		King(sf::Vector2f screenDimensions, Camera *pCamera);
		void handleKeypress(int keyCode);
		void handleKeyUp(int keyCode);
		void update(float timeDelta);
		virtual void draw(sf::RenderTarget &target) {
			sprite.move(-camera->getCameraTransform());
			target.draw(sprite);
			sprite.move(camera->getCameraTransform());
		}
		virtual sf::Vector2f getPosition() {
			return sprite.getPosition();
		}
		sf::Sprite sprite;
		bool followVertically();
	private:
		bool facingRight = true;
		bool jumping = false;
		int currentWalkTexture = 0;
		Camera *camera;
		sf::Clock walkingTimer;
		sf::Clock jumpingTimer;
		std::vector<sf::Texture> walkingTextures;
		// Preparation, Upwards Movement, Downwards Movement, Landing
		std::vector<sf::Texture> jumpingTextures;
		sf::Vector2f targetVelocity;
		sf::Vector2f currentVelocity;
		void move(float timeDelta);
		void updateTexture();
};

#endif