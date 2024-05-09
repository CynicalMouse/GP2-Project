#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <SDL/SDL.h>
#include <map>

struct Camera
{
public:
	// CONSTRUCTOR //
	Camera()
	{
		fov = 45.0f;
		speed = 0.05f;
		mouseSpeed = 0.2f;
		pitch = 0.0f;
		yaw = 0.0f;
		aspect = 0.0f;
		nearClip = 0.01f;
		farClip = 1000.0f;
	}

	void initialiseCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	// MOVEMENT //
#pragma region Movement

	void moveCameraPosition(SDL_Keycode key)
	{
		// OLD VARIABLES FOR ARROW KEY CAMERA LOOKING
		//float angle = mouseSpeed; // rotation angle
		//glm::mat4 rotation;
		//right = glm::normalize(glm::cross(forward, up)); // calculate the right vector

		switch (key)
		{
			case SDLK_w:
				pos += forward * speed;
				break;

			case SDLK_s:
				pos -= forward * speed;
				break;

			case SDLK_a:
				pos -= glm::normalize(glm::cross(forward, up)) * speed;
				break;

			case SDLK_d:
				pos += glm::normalize(glm::cross(forward, up)) * speed;
				break;

			case SDLK_SPACE:
				pos += up * speed;
				break;

			case SDLK_LSHIFT:
				pos -= up * speed;
				break;

			/* OLD ARROW KEY CAMERA MOVEMENT
			case SDLK_UP:
				if (pitch + angle < glm::radians(89.0f)) // limit pitch to 89 degrees so camera cant go upside down
				{
					rotation = glm::rotate(angle, right);
					forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
					pitch += angle; // update the pitch
				}
				break;

			case SDLK_DOWN:
				if (pitch - angle > glm::radians(-89.0f)) // limit pitch to -89 degrees so camera cant go upside down
				{
					rotation = glm::rotate(-angle, right);
					forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
					pitch -= angle; // update the pitch
				}
				break;

			case SDLK_LEFT:
				rotation = glm::rotate(angle, up);
				forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
				break;

			case SDLK_RIGHT:
				rotation = glm::rotate(-angle, up);
				forward = glm::vec3(rotation * glm::vec4(forward, 0.0f));
				break;
			*/
		}
	}

	float toRadians(float degrees)
	{
		// Anymore digits of pi are unnecessary, this is roughly what NASA use for space flight
		float radians = degrees / (180 / 3.141592653589793);
		return radians;
	}

	// Free cam mouse movement
	void handleFreeMouseMovement(int deltaX, int deltaY)
	{
		// Adjust the camera's orientation based on mouse movement
		yaw += deltaX * mouseSpeed;
		pitch += deltaY * mouseSpeed;

		// Limit pitch to prevent camera flipping
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		// Calculate new forward vector
		// GOT HELP FROM CHATGPT
		glm::vec3 newForward;
		newForward.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
		newForward.y = sin(toRadians(-pitch));
		newForward.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
		forward = glm::normalize(newForward);
	}

	// Chase cam mouse movement
	//void handleChaseMouseMovement(int deltaX, int deltaY)
	//{
	//	// Adjust the camera's orientation based on mouse movement
	//	//int rotation = deltaX * mouseSpeed;
	//	yaw = deltaX * mouseSpeed;
	//	pitch += deltaY * mouseSpeed;

	//	// Limit pitch to prevent camera flipping
	//	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	//	// Calculate new forward vector
	//	glm::vec3 newForward;
	//	newForward.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
	//	newForward.y = sin(toRadians(-pitch));
	//	newForward.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
	//	forward = glm::normalize(newForward);

	//	//return glm::vec3(0.0f, 0.0f, rotation);
	//}

#pragma endregion

	// SETTERS //
#pragma region Setters

	void setPosition(float x, float y, float z)
	{
		pos = glm::vec3(x, y, z);
	}

	void setPosition(glm::vec3 newPos)
	{
		pos = newPos;
	}

	void setForward(glm::vec3 newPos)
	{
		forward = newPos;
	}

#pragma endregion

	// GETTERS //
#pragma region Getters

	glm::vec3 getPosition() 
	{ 
		return pos; 
	}

	glm::vec3 getForward()
	{
		return forward;
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	glm::mat4 GetProjectionMatrix() const
	{
		return projection;
	}

#pragma endregion

private:
	// ATTRIBUTES //
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 projection;

	float pitch;
	float yaw;
	float aspect;
	float nearClip;
	float farClip;

	// Initial Field of View
	float fov;

	float speed;
	float mouseSpeed;

};