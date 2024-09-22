#include <behaviour.hpp>
#include <light.hpp>
#include <Arduino.h>

std::map<int, Behaviour*> *Behaviour::getBehavMap() {
	static std::map<int, Behaviour*> map;
	return &map;
}

int Behaviour::registerBehav(Behaviour *behav, int id) {
	static int cId = 0;
	if (!behav)
		return false;
	if (id < 0)
		id = cId;
	Behaviour::getBehavMap()->insert(std::pair<int,Behaviour*>(id,behav));
	cId = std::max(id+1, cId);
	return id;
}

Behaviour *Behaviour::findBehav(int id) {
	std::map<int, Behaviour*> *map = Behaviour::getBehavMap();
	std::map<int, Behaviour*>::iterator it = map->find(id);
	if (it == map->end())
		return nullptr;
	return it->second;
}

void Behaviour::wait(unsigned long ms) {
	if (!this->light || !this->light->isBehavEnabled())
		return;
	float t = ((float)ms)*this->light->getTimescale();
	unsigned long st = millis();
	while (millis()-st < t) {
		if (!this->light || !this->light->isBehavEnabled())
			return;
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}

void Behaviour::Rset(bool val) {
	if (!this->light || !this->light->isBehavEnabled())
		return;
	this->light->setR(val);
}

void Behaviour::Yset(bool val) {
	if (!this->light || !this->light->isBehavEnabled())
		return;
	this->light->setY(val);
}

void Behaviour::Gset(bool val) {
	if (!this->light || !this->light->isBehavEnabled())
		return;
	this->light->setG(val);
}

void Behaviour::bindLight(Light *light) {
	this->light = light;
}