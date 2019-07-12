#include "Floor.hpp"

Floor::Floor(Camera *pCamera) {
	camera = pCamera;
	shape = sf::RectangleShape(sf::Vector2f(2000.0f, 16.0f));
	texture.loadFromFile("content/sheet.png", sf::IntRect(128, 0, 16, 16));
	texture.setRepeated(true);
	shape.setTexture(&texture);
	shape.setTextureRect(sf::IntRect(0, 0, 2000.0f, 16.0f));
	shape.scale(3, 3);
	shape.move(sf::Vector2f(-1920 / 2, 1080));
}

void Floor::draw(sf::RenderTarget &target) {
	shape.move(-camera->getCameraTransform());
	target.draw(shape);
	shape.move(camera->getCameraTransform());
}

sf::Vector2f Floor::getPosition() {
	return shape.getPosition();
}