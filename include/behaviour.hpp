#pragma once

#include <Arduino.h>

#include <map>

class Light;

class Behaviour
{
public:
	virtual String name();
	virtual void run();
	virtual void reset() {}

	static std::map<int, Behaviour*> *getBehavMap();
	static Behaviour *findBehav(int id);
	static int registerBehav(Behaviour *behav, int id=-1);

	void wait(unsigned long ms);

	void Ron() {this->Rset(true);}
	void Yon() {this->Yset(true);}
	void Gon() {this->Gset(true);}

	void Roff() {this->Rset(false);}
	void Yoff() {this->Yset(false);}
	void Goff() {this->Gset(false);}

	void Rset(bool val);
	void Yset(bool val);
	void Gset(bool val);

	void bindLight(Light *light);
protected:
	Light *light;
};