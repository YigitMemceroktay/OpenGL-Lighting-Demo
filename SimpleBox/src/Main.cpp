#include "OpenGL/Window.h"

int main()
{
	Engine::Window*  window = new Engine::Window("OpenGL",1050,600);

	window->Init();
	window->GameLoop();


	return 0;
}