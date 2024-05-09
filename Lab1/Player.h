#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Audio.h"
#include <iostream>

class Player
{
public:
	// CONSTRUCTOR & DESTRUCTOR //
	Player();
	~Player();

	// CREATION & DRAWING //
	void initialisePlayer();
	void updatePosition();
	void drawPlayer(Camera activeCamera);

	// MOVEMENT //
	void movePlayerPosition(SDL_Keycode key);
	void applyForceUp(float force);

	// GETTERS & SETTERS
	glm::vec3 getPosition()
	{
		return *playerTransform.getPos();
	}

	float getSize()
	{
		return size;
	}

	void setRotation(glm::vec3 newRotation)
	{
		playerTransform.setRot(newRotation);
	}

	// ATTRIBUTES //
	glm::vec3 velocity;

private:
	// INITALIZATION //
	void initialiseAssets();
	void initialisePhysics();
	void initialisePosition();

	void shaderLink();
	void physics();
	void onCollision();

	// ASSETS //
	Shader shaderPlayer;
	Mesh player;
	Texture texturePlayer;

	// ATTRIBUTES //
	Transform playerTransform;

	bool touchingGround;

	float size;
	float mass;

	float gravity;
	float friction;
};

