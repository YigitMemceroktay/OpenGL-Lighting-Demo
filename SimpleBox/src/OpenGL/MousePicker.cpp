#include "MousePicker.h"

namespace Engine {
	MousePicker::MousePicker(glm::mat4 projection, glm::mat4 view):projection(projection), view(view)
	{

	}
	glm::vec3 MousePicker::ToWorldsCoords(glm::vec4 eyeCoords)
	{
		glm::mat4 invertView = glm::inverse(view);
		glm::vec4 rayWorld = invertView * eyeCoords;
		return glm::normalize(glm::vec3(rayWorld));
	};
	glm::vec3 MousePicker::calculateMouseRay(double x, double y)
	{
		glm::vec2 normalizeCoords = GetNormalizeDeviceCoords(x, y);
		//std::cout << "Normalize Coords: " << normalizeCoords.x << std::endl;
		glm::vec4 clipCoords = glm::vec4(normalizeCoords.x, normalizeCoords.y, -1, -1);
		//std::cout << "Clip Coords: " << clipCoords.x << std::endl;
		glm::vec4 eyeCoords = toEyeCoords(clipCoords);
		
		glm::vec3 worldRay = ToWorldsCoords(eyeCoords);
		return worldRay;
	}
	glm::vec2 MousePicker::GetNormalizeDeviceCoords(double x, double y)
	{
	
		float X = 2 * (x - 250) / (width - 250) - 1.f;
			float Y = (2 * y / height) - 1.f;
			return glm::vec2(X, -Y);
		
	
	}
	void MousePicker::SetDims(int x, int y)
	{
		this->width = x;
		this->height = y;
	}
	glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords)
	{
		glm::mat4 inverseProjection = glm::inverse(projection);
		glm::vec4 eyeCoords = inverseProjection * clipCoords;
		return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f	, 0.f);
	}

	glm::vec3 MousePicker::GetCurrentRay()
	{
		return currentRay;
	}
	glm::vec3 MousePicker::Update(glm::mat4 view,glm::mat4 projection, double x, double y) 
	{
		this->view = view;
		this->projection = projection;
		currentRay = calculateMouseRay(x, y);	
		return currentRay;
	}
}