#ifndef CAMERA_H
#define CAMERA_H

class CameraDrawable;

class Camera {
	private:
		sf::Vector2f cameraCenter;
		float trauma;
		float currentAngle;
		float offsetX;
		float offsetY;
	public:
		Camera(sf::Vector2f initialPosition);
		void addTrauma(float traumaValue);
		void update(std::vector<CameraDrawable*> trackableElements, float timeDelta);
		sf::Vector2f getCameraTransform();
};

class CameraDrawable {
	private:
		Camera *camera;
		float weight;
	public:
		virtual void draw(sf::RenderTarget &target) = 0;
		virtual sf::Vector2f getPosition() = 0;
		bool followVertically() {
			return true;
		}
};

#endif