// Lerping function
float approach(float goal, float current, float delta) {
	float difference = goal - current;

	if(difference > delta) {
		return current + delta;
	} else if(difference < -delta) {
		return current - delta;
	} else {
		return goal;
	}
}

// TODO: asymptotic smoothing
float asymptoticApproach(float target, float current, float ratio, float delta) {
	return current + (target - current) * ratio * delta;
}

sf::Vector2f asymptoticApproach(sf::Vector2f target, sf::Vector2f current, float ratio, float delta) {
	return sf::Vector2f(asymptoticApproach(target.x, current.x, ratio, delta), asymptoticApproach(target.y, current.y, ratio, delta));
}