#include <light.hpp>
#include <Arduino.h>

Light::Light(int red, int yellow, int green) {
	this->red = red;
	this->yellow = yellow;
	this->green = green;

	this->behav = nullptr;
	this->enabled = false;
	this->timescale = 1.0f;
}

Light::~Light() {
	pinMode(this->red, INPUT);
	pinMode(this->yellow, INPUT);
	pinMode(this->green, INPUT);
}

void Light::init() {
	pinMode(this->red, OUTPUT);
	pinMode(this->yellow, OUTPUT);
	pinMode(this->green, OUTPUT);

	this->reset();
}

void Light::run() {
	if (!this->enabled || !this->behav) {
		vTaskDelay(1 / portTICK_PERIOD_MS);
		return;
	}
	this->behav->run();
}

void Light::setBehaviour(Behaviour *behav) {
	this->reset();
	if (this->behav != nullptr)
		this->behav->bindLight(nullptr);
	this->behav = behav;
	if (this->behav != nullptr)
		this->behav->bindLight(this);
}

void Light::setR(bool en) {
	digitalWrite(this->red, en ? HIGH : LOW);
}

void Light::setY(bool en) {
	digitalWrite(this->yellow, en ? HIGH : LOW);
}

void Light::setG(bool en) {
	digitalWrite(this->green, en ? HIGH : LOW);
}

float Light::getTimescale() {
	return this->timescale;
}

void Light::setTimescale(float timescale) {
	this->timescale = timescale;
}

bool Light::isBehavEnabled() {
	return this->enabled;
}

Behaviour *Light::getBehav() {
	return this->behav;
}

void Light::setEnableBehav(bool en) {
	this->enabled = en;
	if (!this->enabled)
		this->reset();
}

void Light::reset() {
	setR(false);
	setY(false);
	setG(false);
}