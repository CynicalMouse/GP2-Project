#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Audio.h"
#include <iostream>

class BouncyBall
{
public:
	// CONSTRUCTOR & DESTRUCTOR //
	BouncyBall();
	~BouncyBall();

	void initialiseBall(Audio* audio, unsigned int hitSound, int seed);
	void drawBouncyBall(Camera activeCamera);

	void updatePosition();
	void onCollision(bool collideBall, glm::vec3 incomingDirection);
	void inverseX();
	void inverseZ();
	void applyForce(float force, SDL_Keycode key);

	// ATTRIBUTES //
	Audio* audioReference;
	unsigned int ballSound;
	bool muted;

	// GETTERS & SETTERS //
	float getRadius()
	{
		return radius;
	}

	glm::vec3 getPosition()
	{
		glm::vec3 vec3 = *ballTransform.getPos();
		return vec3;
	}

	glm::vec3 getVelocity()
	{
		return velocity;
	}

	void setVelocity(glm::vec3 newVelocity)
	{
		velocity = newVelocity;
	}

	void setPosition(glm::vec3 newPosition)
	{
		ballTransform.setPos(newPosition);
	}

private:
	void initialiseAssets();
	void initialisePhysics();
	void initialisePosition();

	void shaderLink();
	void physics();

	// ASSETS //

	Shader shaderBouncyBall;
	Mesh ball;
	Texture textureBouncyBall;

	// ATTRIBUTES //
	glm::vec3 velocity;

	glm::vec3 color;
	Transform ballTransform;

	float distanceToCamera;

	bool touchingGround;

	float diameter;
	float radius;

	float density;
	float mass;

	float gravity;
	float friction;
	
};

