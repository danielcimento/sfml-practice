#include "Camera.hpp"
#include "MathHelpers.hpp"
#include <cstdlib>

Camera::Camera(sf::Vector2f initialPosition) {
	cameraCenter = initialPosition;
	trauma = 0.0;
}

void Camera::addTrauma(float traumaValue) {
	trauma = std::min(trauma + traumaValue, 1.0f);
}

void Camera::update(std::vector<CameraDrawable*> trackables, float timeDelta) {
	trauma = approach(0.0, trauma, timeDelta);
	std::cout << trauma << std::endl;
	// TODO: follow all trackables
	sf::Vector2f cameraTarget = trackables[0]->getPosition() - sf::Vector2f(1920 / 2, 1080 * 0.8);
	// TODO: Figure out why this doesn't track
	if(!trackables[0]->followVertically()) {
		cameraTarget = sf::Vector2f(cameraTarget.x, cameraCenter.y);
	}
	cameraCenter = asymptoticApproach(cameraTarget, cameraCenter, 0.3, timeDelta);

	float shake = trauma * trauma;
	currentAngle = 30.0 * shake * ((float)(rand()) / (float(RAND_MAX) + 1.0f));
	offsetX = 30.0 * shake * ((float)(rand()) / (float(RAND_MAX) + 1.0f));
	offsetY = 30.0 * shake * ((float)(rand()) / (float(RAND_MAX) + 1.0f));

}

sf::Vector2f Camera::getCameraTransform() {
	return sf::Vector2f(cameraCenter.x + offsetX, cameraCenter.y + offsetY);
}