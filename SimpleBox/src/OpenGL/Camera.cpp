#include "Camera.h"

namespace Engine {
	Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraDir) :
		cameraPos(cameraPos), cameraFront(cameraDir)
	{
		view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
		projection = glm::perspective(45.0f,  1.0f, 0.1f, 100.0f);
		cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	}

	void Camera::ProccessKey(Directions dir)
	{
		if (dir == Directions::FORWARD)
		{
			cameraPos += glm::normalize(glm::cross(cameraUp,cameraRight))*cameraSpeed;
		}
		if (dir == Directions::BACK)
		{
			cameraPos -= glm::normalize(glm::cross(cameraUp, cameraRight))*cameraSpeed;
		}
		if (dir == Directions::RIGHT)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
		}
		if (dir == Directions::LEFT)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
		}
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void Camera::ProccessMouseMvmnt(float xOffset, float yOffset)
	{
		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;

		Yaw += xOffset;
		Pitch +=yOffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
	
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		

		// update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	void Camera::updateCameraVectors()
	{
		
			// calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			cameraFront = glm::normalize(front);
			// also re-calculate the Right and Up vector
			cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f,1.0f,0.0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

			//std::cout << "Camera Front: " << cameraFront.x<<","<< cameraFront.y <<","  << cameraFront.z << std::endl;

	}

	
	

}