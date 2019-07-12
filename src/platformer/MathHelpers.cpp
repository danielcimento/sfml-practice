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