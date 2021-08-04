#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Engine
{
	Shader::Shader()
	{
				
	}
	
	void Shader::Use()
	{
		glUseProgram(program);
	}

	void Shader::CompileShader(const char* vertex, const char* fragment)
	{
		int success;

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex, nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		
		if (!success)
		{
			CheckShaderError(vertexShader);
		}
		else
		{
			std::cout << "Vertex Shader Successfully Compiled!" << std::endl;
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragment, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			CheckShaderError(fragmentShader);
		}
		else
		{
			std::cout << "Fragment Shader Successfully Compiled!" << std::endl;
		}

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);	
		
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cout << "Could not link program: "<<infoLog << std::endl;
		}
	
	};
	
	void Shader::CheckShaderError(unsigned int shader)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader,512, nullptr, infoLog);
		
		std::cout << infoLog << std::endl; 
	};



	void Shader::SetFloat1(const char* name, float val)
	{
		glUniform1f(glGetUniformLocation(program, name), val);
	}
	void Shader::SetFloat2(const char* name, glm::vec2 val)
	{
	}
	void Shader::SetFloat3(const char* name, glm::vec3 val)
	{
		glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(val));
	}
	void Shader::SetFloat4(const char* name, glm::vec4 val)
	{
		glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(val));
	}
	void Shader::SetInt1(const char* name, int val)
	{
		glUniform1i(glGetUniformLocation(program, name), val);
	}
	void Shader::SetInt2(const char* name, glm::vec2 val)
	{
	}
	void Shader::SetInt3(const char* name, glm::vec3 val)
	{
	}
	void Shader::SetInt4(const char* name, glm::vec4 val)
	{
	}
	void Shader::SetMat4(const char* name, glm::mat4 val)
	{
		glUniformMatrix4fv(glGetUniformLocation(program,name),1,GL_FALSE,glm::value_ptr(val));
	}
}