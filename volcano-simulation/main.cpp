#include "Game.h"
#include <time.h> 


int main()
{
	Game game("volcan-simulation",
		1920, 1080,
		4, 4,
		false);
	//MAIN LOOP
	float TARGET_FPS = 60;

	double lasttime = glfwGetTime();
	while (!game.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		game.update();
		game.render();
		
		while (glfwGetTime() < lasttime + 1.0 / TARGET_FPS) {

		}
		lasttime += 1.0 / TARGET_FPS;
	}

	return 0;
}