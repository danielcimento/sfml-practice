#ifndef KING_H
#define KING_H

class King : public sf::Drawable {
	public:
		King(sf::Vector2f screenDimensions);
		void handleKeypress(int keyCode);
		void handleKeyUp(int keyCode);
		void update(float timeDelta);
	private:
		sf::Sprite sprite;
		bool facingRight = true;
		bool jumping = false;
		int currentWalkTexture = 0;
		sf::Clock walkingTimer;
		sf::Clock jumpingTimer;
		std::vector<sf::Texture> walkingTextures;
		// Preparation, Upwards Movement, Downwards Movement, Landing
		std::vector<sf::Texture> jumpingTextures;
		sf::Vector2f targetVelocity;
		sf::Vector2f currentVelocity;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
			target.draw(sprite);
		}
		void move(float timeDelta);
		void updateTexture();
};

#endif