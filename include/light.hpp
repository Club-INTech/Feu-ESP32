#pragma once

#include <behaviour.hpp>

class Light
{
public:
	Light(int red, int yellow, int green);
	~Light();

	void init();
	void run();

	void setBehaviour(Behaviour *behav);

	void setR(bool en);
	void setY(bool en);
	void setG(bool en);

	float getTimescale();
	void setTimescale(float timescale);

	bool isBehavEnabled();
	Behaviour *getBehav();
	void setEnableBehav(bool en);
	void reset();
private:
	int red, yellow, green;
	bool enabled;
	float timescale;

	Behaviour *behav;
};