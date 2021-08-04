#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ResourceManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
namespace Engine
{
	struct WindowProperties
	{
		std::string Name;
		int Width;
		int Height;
		GLFWwindow* window;
		
	};

	struct Timer
	{
		float currentTime;
		float lastTime = 0.0f;
		float deltaTime;
	};

	class Window
	{
	public:
		Window(std::string name, int Width, int Height);

	public:
		void Init();
	public:
		void GameLoop();
	private:
		void ProccessKey();
		WindowProperties props;




	};
}