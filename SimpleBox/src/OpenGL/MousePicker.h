#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
namespace Engine
{
	
	class MousePicker
	{
	private:
		glm::vec3 currentRay;
		int width = 1920;
		int height = 900;
		glm::mat4 projection;
		glm::mat4 view;

	
		
	
		glm::vec3 ToWorldsCoords(glm::vec4 eyeCoords);
		glm::vec3 calculateMouseRay(double x, double y);
		glm::vec2 GetNormalizeDeviceCoords(double x, double y);	
		glm::vec4 toEyeCoords(glm::vec4 clipCoords);
	
	public:
		void SetDims(int x, int y);
		glm::vec3 GetCurrentRay();
		glm::vec3 Update(glm::mat4 view, glm::mat4 projection, double x , double y);
		MousePicker(glm::mat4 projection, glm::mat4 view);

	};
}