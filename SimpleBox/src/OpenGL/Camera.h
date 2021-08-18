#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
namespace Engine
{
	enum Directions
	{
		FORWARD,
		BACK,
		LEFT,
		RIGHT
	};

	class Camera {
	public:
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		float cameraSpeed = 0.01;

		float MouseSensitivity = 0.3;

		Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f));

		void ProccessKey(Directions dir);

		void ProccessMouseMvmnt(float xOffset, float yOffset);

		void updateCameraVectors();
	public:
		glm::vec3 cameraPos;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight;
		glm::mat4 view;
		glm::mat4 projection;







	};
};