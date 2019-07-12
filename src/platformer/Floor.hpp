#include "Camera.hpp"

class Floor : public CameraDrawable {
	private:
		Camera *camera;
		sf::RectangleShape shape;
		sf::Texture texture;
	public:
		Floor(Camera *pCamera);
		virtual void draw(sf::RenderTarget &target);
		virtual sf::Vector2f getPosition();
};