#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ResourceManager.h"
#include "imgui.h"
#include "Camera.h"
#include "imgui_impl_glfw.h"
#include "MousePicker.h"
#include "imgui_impl_opengl3.h"
namespace Engine
{
	 struct WindowProperties
	{
		static std::string Name;
		static float Width;
		static float Height;
		static GLFWwindow* window;
		
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