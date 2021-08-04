#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
namespace Engine
{
	class Shader
	{
public:

		Shader();
		
		void Use();

		void CheckShaderError(unsigned int shader);

		void CompileShader(const char* vertex_src, const char* fragment_src);

		void SetFloat1(const char* name, float val);
		void SetFloat2(const char* name, glm::vec2 val);
		void SetFloat3(const char* name, glm::vec3 val);
		void SetFloat4(const char* name, glm::vec4 val);

		void SetInt1(const char* name, int val);
		void SetInt2(const char* name, glm::vec2 val);
		void SetInt3(const char* name, glm::vec3 val);
		void SetInt4(const char* name, glm::vec4 val);

		void SetMat4(const char* name, glm::mat4 val);
	private:
		unsigned int program;


	};

}