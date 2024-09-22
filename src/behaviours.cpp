#include "behaviour.hpp"
#include "behaviours.hpp"

int OriginalBehaviour::id = Behaviour::registerBehav(new OriginalBehaviour(),0);
int FeuBehaviour::id = Behaviour::registerBehav(new FeuBehaviour());
int FeuOrangeBehaviour::id = Behaviour::registerBehav(new FeuOrangeBehaviour());
int Clem1Behaviour::id = Behaviour::registerBehav(new Clem1Behaviour());
int Clem2Behaviour::id = Behaviour::registerBehav(new Clem2Behaviour());
int Clem3Behaviour::id = Behaviour::registerBehav(new Clem3Behaviour());
int Clem4Behaviour::id = Behaviour::registerBehav(new Clem4Behaviour());
int Clem5Behaviour::id = Behaviour::registerBehav(new Clem5Behaviour());
int Vincent1Behaviour::id = Behaviour::registerBehav(new Vincent1Behaviour());