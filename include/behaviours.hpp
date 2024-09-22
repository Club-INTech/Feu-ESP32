#pragma once

#include <behaviour.hpp>

class OriginalBehaviour : public Behaviour {
	String name() {
		return "Original (presque)";
	}

	void run() {
		for (int i=0;i<10;i++) {
			Ron();
			Gon();
			wait(50);
			Roff();
			Goff();
			wait(50);
		}
		for (int i=0;i<10;i++) {
			Yon();
			wait(50);
			Yoff();
			wait(50);
		}
		for (int i=0;i<5;i++) {
			Ron();
			wait(50);
			Roff();
			wait(50);
		}
		for (int i=0;i<5;i++) {
			Gon();
			wait(50);
			Goff();
			wait(50);
		}

	}
public:
	static int id;
};

class FeuBehaviour : public Behaviour {
	String name() {
		return "Feu tricolore";
	}

	void run() {
		Ron();
		wait(30000);
		Roff();
		Gon();
		wait(30000);
		Goff();
		Yon();
		wait(3000);
		Yoff();
	}
public:
	static int id;
};

class FeuOrangeBehaviour : public Behaviour {
	String name() {
		return "Feu tricolore en travaux";
	}

	void run() {
		Yon();
		wait(1000);
		Yoff();
		wait(1000);
	}
public:
	static int id;
};

class Clem1Behaviour : public Behaviour {
	String name() {
		return "Clem 1 - Bounce";
	}

	void run() {
		unsigned long t = 100;
		Ron();
		wait(t);
		Yon();
		wait(t);
		Roff();
		wait(t);
		Gon();
		wait(t);
		Yoff();
		wait(t);
		Yon();
		wait(t);
		Goff();
		wait(t);
		Ron();
		wait(t);
		Yoff();
	}
public:
	static int id;
};

class Clem2Behaviour : public Behaviour {
	String name() {
		return "Clem 2 - Random";
	}

	void run() {
		long r = random(11);
		if (r < 5)
			Ron();
		else
			Roff();

		wait(100);

		r = random(11);
		if (r < 5)
			Yon();
		else
			Yoff();

		wait(100);

		r = random(11);
		if (r < 5)
			Gon();
		else 
			Goff();

		wait(100);
	}
public:
	static int id;
};

class Clem3Behaviour : public Behaviour {
	String name() {
		return "Clem 3 - Random 2";
	}

	void run() {
		long t = random(30,400);

		Ron();
		wait(t);
		Roff();
		wait(t);
		Yon();
		wait(t);
		Yoff();
		wait(t);
		Gon();
		wait(t);
		Goff();
	}
public:
	static int id;
};


class Clem4Behaviour : public Behaviour {
	String name() {
		return "Clem 4 - Random 3";
	}

	void run() {
		Ron();
		wait(random(20,400));
		Roff();
		wait(random(20,400));
		Yon();
		wait(random(20,400));
		Yoff();
		wait(random(20,400));
		Gon();
		wait(random(20,400));
		Goff();
	}
public:
	static int id;
};

class Clem5Behaviour : public Behaviour {
	String name() {
		return "Clem 5 - Bug";
	}

	void run() {
		Ron();
		wait(1000);
		Roff();
		wait(400);
		Yon();
		wait(random(20,400));
		Yoff();
		wait(80);
		Ron();
		wait(200);
		Roff();
		wait(400);
		Gon();
		wait(random(20,400));
		Goff();
	}
public:
	static int id;
};

class Vincent1Behaviour : public Behaviour {
	String name() {
		return "Vincent 1 - Caca v2";
	}

	void run() {
		static bool up = false;
		if (random(4) == 0)
			up ^= true;

		if (up) {
			Ron();
			wait(100);
			Roff();
			Yon();
			wait(100);
			Yoff();
			Gon();
			wait(100);
			Goff();
		} else {
			Gon();
			wait(100);
			Goff();
			Yon();
			wait(100);
			Yoff();
			Ron();
			wait(100);
			Roff();
		}
	}
public:
	static int id;
};