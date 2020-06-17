#include <iostream>
#include "Game.h"
#include "Box2D/Box2D.h"







int main()
{
	//begin
	srand(time(nullptr));
	Game gameInst(1024,768);
	gameInst.run();
	
	

	return 0;
}


