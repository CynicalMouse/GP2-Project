#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Vertex.h"
#include "BouncyBall.h"
#include "Audio.h"
#include "Player.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	// CONSTRUCTOR & DESTRUCTOR //
	MainGame();
	~MainGame();

	void run();

private:
	
	// INITALIZATION //
	void initialiseSystems();
	void initialiseShaders();
	void initialiseTextures();
	void initialiseMeshes();
	void initialiseAudio();

	// GAME LOOP //
	void gameLoop();
	void processInput();
	void updateChaseCamera();
	void checkCollision();
	void ballCollision();
	bool checkBouncePad();
	void drawGame();

	// SHADER LINKS //
	void pixelLink(float pixelSize, unsigned int inputTexture);
	void fogLink();
	void bumpLink(unsigned int diffuse, unsigned int normal);
	void toonLink();
	void rimLink();
	void skyboxLink();
	void rippleLink();
	void lavaLink();
	void dissolveLink();
	void displacementLink(glm::mat4 model);
	void eyeballLink();
	void genericLink(unsigned int inputTexture);

	// DRAW OBJECTS //
	// Converts degrees to radians
	float toRadians(float degrees);

	void drawObjects();

	void drawBall();
	void drawBallDissolve();
	void drawGround();
	void drawWater();
	void drawLava();
	void drawCube();
	void drawSkybox();
	void drawEnclosure();
	void drawEyeball();
	void drawBouncePad();
	void drawTonk();

	// ASSETS //

	Display gameDisplay;
	GameState gameState;

	// Shaders
	Shader shaderGeneric;
	Shader shaderPixel;
	Shader shaderFog;
	Shader shaderBump;
	Shader shaderToon;
	Shader shaderRim;
	Shader shaderSkybox;
	Shader shaderRipple;
	Shader shaderLava;
	Shader shaderDissolve;
	Shader shaderDisplacement;
	Shader shaderEyeball;

	Shader* shaderList[5];
	int activeShader;

	// Mesh
	Mesh monkey;
	Mesh ball;
	Mesh groundPlane;
	Mesh cube;
	Mesh tonk;

	// Textures
	Texture textureWater2;
	Texture textureWater;
	Texture textureLava;
	Texture textureBricks;
	Texture textureBricksNormals;
	Texture textureGrass;
	Texture textureEyeball;
	Texture textureBouncePad;
	Texture textureTonk;

	Texture texturePebbleDiffuse;
	Texture texturePebbleDisplacement;
	Texture texturePebbleNormals;

	Texture skyboxTexture;

	// Cameras
	Camera activeCamera;
	Camera cameras[2];
	unsigned int cameraIndex;
	int mouseX, mouseY;
	int prevMouseX = 0;
	int prevMouseY = 0;

	// Audio
	Audio audio;
	unsigned int music;
	unsigned int hit;
	bool muted;


	// BOUNCY BALLS //
	// Change size of array to change number of balls
	// Can get VERY loud and annoying
	BouncyBall bouncyBalls[32];
	
	// Bouncy Ball Enclosure Walls
	Mesh enclosure[4];


	// PLAYER //
	Player player;


	// BOUNCE PAD //
	// Change to make bounce pad larger and move position
	glm::vec3 bouncePadPosition = glm::vec3(18.0f, 0.0f, 0.0f);
	glm::vec3 bouncePadScale = glm::vec3(4.5f, 4.5f, 4.5f);

	// Center on both axis
	float bouncePadCenterX = 18.0f;
	float bouncePadCenterZ = 0.0f;

	// Width and length
	float bouncePadWL = 4.5f;

	// Bounds of bounce pad
	float boundMaxX = bouncePadCenterX - bouncePadWL;
	float boundMinX = bouncePadCenterX + bouncePadWL;

	float boundMaxZ = bouncePadCenterZ - bouncePadWL;
	float boundMinZ = bouncePadCenterZ + bouncePadWL;

	// MISC ATTRIBUTES //

	float counter = 0;
	float dissolveCounter = 0;
	float dissolveAmount = 0.025f;

	glm::vec3 lightDirection = glm::vec3(0.5f, 0.8f, 0.5f);
};

