#include "OpenGL/Window.h"

int main()
{
	Engine::Window*  window = new Engine::Window("OpenGL",1920,900
	
	
	
	);

	window->Init();
	window->GameLoop();


	return 0;
}