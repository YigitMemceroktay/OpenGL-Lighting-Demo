#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Shader.h"
#include <fstream>
#define BITWISE(x) 1 << x
enum  MapType
{
	AMBIENT_MAP = BITWISE(0),
	DIFFUSE_MAP = BITWISE(1),
	SPECULAR_MAP = BITWISE(2),
	NORMAL_MAP = BITWISE(3),
	HEIGHT_MAP = BITWISE(4),
	SRGB = AMBIENT_MAP | DIFFUSE_MAP,
	RGB = SPECULAR_MAP | NORMAL_MAP|HEIGHT_MAP

};


namespace Engine
{
	


	class ResourceManager
	{
	public:
		static std::map<std::string, Shader> shaders;
		static std::map<std::string,unsigned int> textures;
	public:
		static Shader& LoadShader(std::string name, const char* vertexSrc, const char* fragmentSrc);
		static Shader LoadShaderFromFile(const char* vertexSrc, const char* fragmentSrc);
		static std::string ReadFile(const char* location);
		static Shader& GetShader(std::string name);
		static unsigned int LoadTexture(std::string name, std::string location);
		static unsigned int GetTexture(std::string name);
	};

}