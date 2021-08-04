#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace Engine {

	std::map<std::string, Shader> ResourceManager::shaders;
	std::map<std::string, unsigned int> ResourceManager::textures;
	Shader& ResourceManager::LoadShader(std::string name,const char* vertexSrc, const char* fragmentSrc)
	{
		shaders[name] = LoadShaderFromFile(vertexSrc, fragmentSrc);
		
		return shaders[name];

	}

	Shader& ResourceManager::GetShader(std::string name)
	{
		return shaders[name];

	}

	unsigned int ResourceManager::LoadTexture(std::string name, std::string location, MapType type)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		int width, height, nChannels;
		//stbi__vertically_flip_on_load(true);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(location.c_str(), &width, &height, &nChannels, 0);
		
		GLenum format;
		GLenum format2;

		switch (nChannels)
		{
		case 1:
			format = GL_RED;
			format2 = GL_RED;
			break;
		case 2:
			format = GL_RG;
			format2 = GL_RG;
			break;
		case 3:
			if (type|GL_SRGB == MapType::SRGB)
			{
				format = GL_SRGB;
			}
			else if (type|GL_RGB == MapType::RGB)
			{
				format = GL_RGB;
			}
			format2 = GL_RGB;
			break;
		case 4:
			if (type|GL_SRGB == MapType::SRGB)
			{
				format = GL_SRGB_ALPHA;
			}
			else if(type|GL_RGB == MapType::RGB)
			{
				format = GL_RGBA;
			}
			format2 = GL_RGBA;
			break;
			
		}

		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format2, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		textures[name] = texture;
		return texture;
	}

	unsigned int ResourceManager::GetTexture(std::string name)
	{
		return textures[name];
	}

	Shader ResourceManager::LoadShaderFromFile(const char* vertexSrc, const char* fragmentSrc)
	{
		Shader shader;
		
		std::string vertex  = ReadFile(vertexSrc);
		std::string fragment = ReadFile(fragmentSrc);

		

		shader.CompileShader(vertex.c_str(), fragment.c_str());

		return shader;
		
	}
	std::string Engine::ResourceManager::ReadFile(const char* location)
	{
		std::string loc;
		std::string line;

		std::fstream file;
		file.open(location);

		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				loc += line;
				loc+= '\n';
			}
		}
		else
		{
			std::cout << "Could not open file at location: " << location << std::endl;
		}
		file.close();
		return loc;
	}
}