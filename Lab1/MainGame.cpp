#include "MainGame.h"
#include <iostream>
#include <string>
#include <map>

unsigned int indices[] = { 0, 1, 2 };

Transform transform;

// CONSTRUCTOR & DESTRUCTOR //
#pragma region Constructor & Destructor
MainGame::MainGame()
{
	gameState = GameState::PLAY;
	Display* gameDisplay = new Display(); //new display

	activeShader = 1;
	shaderList[0] = NULL;
}

MainGame::~MainGame()
{
}

#pragma endregion

void MainGame::run()
{
	initialiseAudio();
	initialiseSystems(); 
	initialiseShaders();
	initialiseTextures();
	initialiseMeshes();
	

	// Play background music
	audio.playAudio(music, audio.NONE);

	gameLoop();
}

// INITIALISATION //
#pragma region Initialisation

void MainGame::initialiseSystems()
{
	gameDisplay.initialiseDisplay();

	cameras[0].initialiseCamera(glm::vec3(7.2, 22.0, 8.0), 70.0f, (float)gameDisplay.getWidth() / gameDisplay.getHeight(), 0.01f, 1000.0f);
	cameras[0].setForward(glm::vec3(-0.5, -0.85, 0.002));
	cameras[1].initialiseCamera(glm::vec3(0, 5, -20), 70.0f, (float)gameDisplay.getWidth() / gameDisplay.getHeight(), 0.01f, 1000.0f);

	cameraIndex = 0;

	activeCamera = cameras[cameraIndex];

	for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
	{
		bouncyBalls[i].initialiseBall(&audio, hit, i);
	}

	player.initialisePlayer();

	// Bind mouse to window
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void MainGame::initialiseShaders()
{
	shaderGeneric.initialiseShader("..\\res\\shaders\\generic");
	shaderPixel.initialiseShader("..\\res\\shaders\\pixel");
	shaderFog.initialiseShader("..\\res\\shaders\\fog");
	shaderBump.initialiseShader("..\\res\\shaders\\bump");
	shaderToon.initialiseShader("..\\res\\shaders\\toon");
	shaderRim.initialiseShader("..\\res\\shaders\\rimLight");
	shaderSkybox.initialiseShader("..\\res\\shaders\\skybox");
	shaderRipple.initialiseShader("..\\res\\shaders\\ripple");
	shaderLava.initialiseShader("..\\res\\shaders\\lava");
	shaderDissolve.initialiseShader("..\\res\\shaders\\dissolve");
	shaderDisplacement.initialiseShader("..\\res\\shaders\\displacement");
	shaderEyeball.initialiseShader("..\\res\\shaders\\eyeball");

	// List of shaders for the changing ball
	shaderList[0] = &shaderPixel;
	shaderList[1] = &shaderFog;
	shaderList[2] = &shaderBump;
	shaderList[3] = &shaderToon;
	shaderList[4] = &shaderRim;

	// Start on bump
	activeShader = 2;
}

void MainGame::initialiseTextures()
{
	textureWater.initialiseTextureWrapped("..\\res\\textures\\Water.jpg");
	textureWater2.initialiseTextureWrapped("..\\res\\textures\\Water2.jpg");
	textureLava.initialiseTextureWrapped("..\\res\\textures\\Lava.jpg");
	textureBricks.initialiseTextureWrapped("..\\res\\textures\\bricks.jpg");
	textureBricksNormals.initialiseNormals("..\\res\\textures\\bricksNormals.jpg");
	textureGrass.initialiseTextureWrapped("..\\res\\textures\\grass.png");
	textureEyeball.initialiseTextureWrapped("..\\res\\textures\\eyeball.jpg");
	textureBouncePad.initialiseTextureWrapped("..\\res\\textures\\bouncePad.jpg");
	textureTonk.initialiseTexture("..\\res\\textures\\tonk.png");

	std::string skyboxFilepaths[6] =
	{
		"..\\res\\textures\\skybox\\right.jpg",
		"..\\res\\textures\\skybox\\left.jpg",
		"..\\res\\textures\\skybox\\top.jpg",
		"..\\res\\textures\\skybox\\bottom.jpg",
		"..\\res\\textures\\skybox\\front.jpg",
		"..\\res\\textures\\skybox\\back.jpg"
	};

	skyboxTexture.initialiseCubeMap(skyboxFilepaths);


	texturePebbleDiffuse.initialiseTextureWrapped("..\\res\\textures\\riverPebblesDiffuse.jpg");
	texturePebbleDisplacement.initialiseDisplacement("..\\res\\textures\\riverPebblesDisplacement.jpg");
	texturePebbleNormals.initialiseNormals("..\\res\\textures\\riverPebblesNormals.jpg");
}

void MainGame::initialiseMeshes()
{
	ball.loadMeshFile("..\\res\\models\\Ball.obj");
	tonk.loadMeshFile("..\\res\\models\\tonk.obj");

	Vertex planeVertices[] = { Vertex(glm::vec3(-1.0, 1.0, 0), glm::vec2(0.0, 1.0)),
					Vertex(glm::vec3(1.0, 1.0, 0), glm::vec2(1.0, 1.0)),
					Vertex(glm::vec3(-1.0, -1.0, 0), glm::vec2(0.0, 0.0)),
					Vertex(glm::vec3(-1.0, -1.0, 0), glm::vec2(0.0, 0.0)),
					Vertex(glm::vec3(1.0, 1.0, 0), glm::vec2(1.0, 1.0)),
					Vertex(glm::vec3(1.0, -1.0, 0), glm::vec2(1.0, 0.0)) };

	groundPlane.loadVertexs(planeVertices, sizeof(planeVertices) / sizeof(planeVertices[0]));

	cube.loadMeshFile("..\\res\\models\\Cube2.obj");

	for (int i = 0; i < sizeof(enclosure) / sizeof(enclosure[0]); i++)
	{
		enclosure[i].loadMeshFile("..\\res\\models\\Cube2.obj");
	}
}

void MainGame::initialiseAudio()
{
	music = audio.loadSound("..\\res\\sounds\\Silver Blue Light.wav");
	audio.changeVolume(music, 0.15f);

	hit = audio.loadSound("..\\res\\sounds\\bs_hit_16.wav");
	audio.changeVolume(hit, 0.25f);
}

#pragma	endregion

// SHADER LINKS //
#pragma region Shader Links
void MainGame::pixelLink(float pixelSize, unsigned int inputTexture)
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderPixel.getID(), "inputTexture");

	//set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderPixel.setFloat("pixelSize", pixelSize);
}

void MainGame::fogLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderBump.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLava.getID());
	glUniform1i(texture1Location, 0);
}

void MainGame::bumpLink(unsigned int diffuse, unsigned int normal)
{
	// Locate texture uniforms
	GLuint texture1Location = glGetUniformLocation(shaderBump.getID(), "inputTexture");
	GLuint texture2Location = glGetUniformLocation(shaderBump.getID(), "normalT");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glUniform1i(texture1Location, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glUniform1i(texture2Location, 1);

	// Set attributes
	shaderBump.setVec3("lightDirection", lightDirection);
}

void MainGame::toonLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderBump.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureWater2.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderToon.setVec3("lightDirection", lightDirection);
}

void MainGame::rimLink()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderRim.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureWater2.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderRim.setMat4("model", transform.getModel());
	shaderRim.setFloat("intensity", 3.0f);
	shaderRim.setVec3("cameraPosition", activeCamera.getPosition());
}

void MainGame::skyboxLink()
{
	GLuint cubemapLocation = glGetUniformLocation(shaderSkybox.getID(), "skybox");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.getID());
	glUniform1i(cubemapLocation, 0);

	// Set attributes
	shaderSkybox.setMat4("view", activeCamera.GetViewMatrix());
	shaderSkybox.setMat4("projection", activeCamera.GetProjectionMatrix());
}

void MainGame::rippleLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderBump.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureWater2.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderRipple.setFloat("time", counter / 15.0f);
	shaderRipple.setFloat("amplitude", 2.5f);
	shaderRipple.setFloat("frequency", 1.0f);
}

void MainGame::lavaLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderBump.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLava.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderLava.setFloat("time", counter);
}

void MainGame::dissolveLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderDissolve.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureWater.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderDissolve.setFloat("time", dissolveCounter);
}

void MainGame::displacementLink(glm::mat4 model)
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderDisplacement.getID(), "diffuseMap");
	GLuint texture2Location = glGetUniformLocation(shaderDisplacement.getID(), "normalMap");
	GLuint texture3Location = glGetUniformLocation(shaderDisplacement.getID(), "depthMap");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePebbleDiffuse.getID());
	glUniform1i(texture1Location, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePebbleNormals.getID());
	glUniform1i(texture2Location, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texturePebbleDisplacement.getID());
	glUniform1i(texture3Location, 2);

	// Set attributes
	shaderDisplacement.setVec3("viewPos", activeCamera.getPosition());
	shaderDisplacement.setMat4("projection", activeCamera.GetProjectionMatrix());
	shaderDisplacement.setMat4("view", activeCamera.GetViewMatrix());
	shaderDisplacement.setMat4("model", model);
	shaderDisplacement.setVec3("lightPos", lightDirection);
	shaderDisplacement.setFloat("heightScale", 0.1f);
}

void MainGame::eyeballLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderDissolve.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureEyeball.getID());
	glUniform1i(texture1Location, 0);

	// Set attributes
	shaderEyeball.setVec3("eyePos", glm::vec3(-1.0, 4.0, -30.0));
	shaderEyeball.setVec3("cameraPos", activeCamera.getPosition());
}

void MainGame::genericLink(unsigned int inputTexture)
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderGeneric.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, inputTexture);
	glUniform1i(texture1Location, 0);
}

#pragma endregion

// DRAW OBJECTS //
#pragma region Draw Objects

// Render quad for displacement map
// BASED ON THE TUTORIAL FOUND HERE: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping
unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad()
{
	if (quadVAO == 0)
	{
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);


		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};
		// configure plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

float MainGame::toRadians(float degrees)
{
	// Transform.setRot takes radians as its rotation value
	// Converts degrees to radians to its easier to write in an angle for objects
	// // E.g can right 89 degrees instead of 1.56
	// Anymore digits of pi are unnecessary, this is roughly what NASA use for space flight
	float radians = degrees / (180 / 3.141592653589793);
	return radians;
}

void MainGame::drawObjects()
{
	drawSkybox();
	drawGround();
	drawWater();
	drawLava();
	drawBouncePad();
	drawCube();
	drawEnclosure();
	drawEyeball();
	drawBall();
	drawBallDissolve();
	drawTonk();

	// loop through bouncy balls and draw them
	for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
	{
		bouncyBalls[i].drawBouncyBall(activeCamera);
	}

	player.drawPlayer(activeCamera);
}

void MainGame::drawBall()
{
	transform.setPos(glm::vec3(-3.0, 4.0, 1.0));
	transform.setRot(glm::vec3(0.0, counter, 0.0));
	transform.setScale(glm::vec3(0.5, 0.5, 0.5));

	shaderList[activeShader]->bindShader();

	switch (activeShader)
	{
	case 0:
		pixelLink(150.0f, textureGrass.getID());
		break;

	case 1:
		fogLink();
		break;

	case 2:
		bumpLink(textureBricks.getID(), textureBricksNormals.getID());
		break;

	case 3:
		toonLink();
		break;

	case 4:
		rimLink();
		break;
	}

	shaderList[activeShader]->updateShader(transform, activeCamera);
	ball.draw();
}

void MainGame::drawBallDissolve()
{
	transform.setPos(glm::vec3(-3.0, 3.0, -3.0));
	transform.setRot(glm::vec3(0.0, 0.0, 0.0));
	transform.setScale(glm::vec3(0.5, 0.5, 0.5));

	shaderDissolve.bindShader();
	dissolveLink();
	shaderDissolve.updateShader(transform, activeCamera);

	ball.draw();
}

void MainGame::drawGround()
{
	transform.setPos(glm::vec3(0.0, 0.0, 0.0));
	transform.setRot(glm::vec3(toRadians(90), 0.0, 0.0));
	transform.setScale(glm::vec3(4.5, 4.5, 4.5));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
	model = glm::rotate(model, toRadians(90), glm::vec3(toRadians(-90), 0.0, 0.0));
	model = glm::scale(model, glm::vec3(4.5, 4.5, 4.5));

	shaderDisplacement.bindShader();
	displacementLink(model);
	shaderDisplacement.updateShader(transform, activeCamera);

	//shaderBump.bindShader();
	//bumpLink();
	//shaderBump.updateShader(transform, activeCamera);

	//groundPlane.drawVertexes();
	renderQuad();
}

void MainGame::drawWater()
{
	transform.setPos(glm::vec3(-9.0, 0.0, 0.0));
	transform.setRot(glm::vec3(toRadians(90), 0.0, 0.0));
	transform.setScale(glm::vec3(4.5, 4.5, 4.5));

	shaderRipple.bindShader();
	rippleLink();
	shaderRipple.updateShader(transform, activeCamera);

	groundPlane.drawVertexes();
}

void MainGame::drawLava()
{
	transform.setPos(glm::vec3(9.0, 0.0, 0.0));
	transform.setRot(glm::vec3(toRadians(90), 0.0, 0.0));
	transform.setScale(glm::vec3(4.5, 4.5, 4.5));

	shaderLava.bindShader();
	lavaLink();
	shaderLava.updateShader(transform, activeCamera);

	groundPlane.drawVertexes();
}

void MainGame::drawCube()
{
	transform.setPos(glm::vec3(2.0, 5.0, 2.0));
	transform.setRot(glm::vec3(counter, counter * 1.2, counter * 1.5));
	transform.setScale(glm::vec3(0.5, 0.5, 0.5));

	shaderPixel.bindShader();
	pixelLink(150.0f, textureGrass.getID());
	shaderPixel.updateShader(transform, activeCamera);
	
	cube.draw();
}

void MainGame::drawSkybox()
{
	transform.setPos(glm::vec3(0.0, 0.0, 0.0));
	transform.setRot(glm::vec3(0.0, 0.0, 0.0));
	transform.setScale(glm::vec3(1.0, 1.0, 1.0));

	// Disable depth masking so that other objects are disblayed over the skybox
	glDepthMask(GL_FALSE);

	// Camera is inside the skybox, disable face culling so it can be seen
	glDisable(GL_CULL_FACE);

	shaderSkybox.bindShader();
	skyboxLink();
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.getID());
	shaderSkybox.updateShader(transform, activeCamera);

	cube.draw();

	// Reenable depth mask and face culling
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
}

void MainGame::drawEnclosure()
{
	for (int i = 0; i < sizeof(enclosure) / sizeof(enclosure[0]); i++)
	{
		switch (i)
		{
			case 0:
				transform.setPos(glm::vec3(0.0, 6.0, 8.0));
				transform.setRot(glm::vec3(0.0, 0.0, 0.0));
				transform.setScale(glm::vec3(6.0, 6.0, 0.5));
				break;

			case 1:
				transform.setPos(glm::vec3(0.0, 6.0, 21.0));
				transform.setRot(glm::vec3(0.0, 0.0, 0.0));
				transform.setScale(glm::vec3(6.0, 6.0, 0.5));
				break;

			case 2:
				transform.setPos(glm::vec3(-6.0, 6.0, 14.5));
				transform.setRot(glm::vec3(0.0, toRadians(90), 0.0));
				transform.setScale(glm::vec3(6.0, 6.0, 0.5));
				break;

			case 3:
				transform.setPos(glm::vec3(6.0, 6.0, 14.5));
				transform.setRot(glm::vec3(0.0, toRadians(90), 0.0));
				transform.setScale(glm::vec3(6.0, 6.0, 0.5));
				break;
		}

		shaderBump.bindShader();
		bumpLink(textureBricks.getID(), textureBricksNormals.getID());
		shaderBump.updateShader(transform, activeCamera);

		enclosure[i].draw();
	}
	

}

void MainGame::drawEyeball()
{
	transform.setPos(glm::vec3(-1.0, 4.0, -30.0));
	transform.setRot(glm::vec3(0.0, 0.0, 0.0));
	transform.setScale(glm::vec3(5.0, 5.0, 5.0));

	shaderEyeball.bindShader();
	eyeballLink();
	shaderEyeball.updateShader(transform, activeCamera);

	ball.draw();
}

void MainGame::drawBouncePad()
{
	transform.setPos(bouncePadPosition);
	transform.setRot(glm::vec3(toRadians(90), 0.0, 0.0));
	transform.setScale(bouncePadScale);

	shaderGeneric.bindShader();
	genericLink(textureBouncePad.getID());
	shaderGeneric.updateShader(transform, activeCamera);

	groundPlane.drawVertexes();
}

void MainGame::drawTonk()
{
	transform.setPos(glm::vec3(8.0, 0.0, 3.0));
	transform.setRot(glm::vec3(0.0, toRadians(90), 0.0));
	transform.setScale(glm::vec3(1.0, 1.0, 1.0));

	shaderGeneric.bindShader();
	genericLink(textureTonk.getID());
	shaderGeneric.updateShader(transform, activeCamera);

	tonk.draw();
}

#pragma endregion

// COLLISION //
#pragma region Collision

void MainGame::checkCollision()
{
	ballCollision();

	if (checkBouncePad())
	{
		player.applyForceUp(1.0f);
	}
}

void MainGame::ballCollision()
{
	// Collision between balls
	for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
	{
		for (int j = 0; j < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); j++)
		{
			// Stop self collision
			if (i == j)
			{
				continue;
			}

			// Calculate distance between balls
			float dx = bouncyBalls[i].getPosition().x - bouncyBalls[j].getPosition().x;
			float dy = bouncyBalls[i].getPosition().y - bouncyBalls[j].getPosition().y;
			float dz = bouncyBalls[i].getPosition().z - bouncyBalls[j].getPosition().z;

			float distance = std::sqrt((dx * dx) + (dy * dy) + (dz * dz));

			// If balls touch
			if (distance <= bouncyBalls[i].getRadius() + bouncyBalls[j].getRadius())
			{
				// Store ball velocities so that they dont get overwriten before reasignment
				glm::vec3 ballVeclocityI = bouncyBalls[i].getVelocity();
				glm::vec3 ballVeclocityJ = bouncyBalls[j].getVelocity();

				// Swap velocities
				bouncyBalls[i].onCollision(true, ballVeclocityJ);
				bouncyBalls[j].onCollision(true, ballVeclocityI);
			}
		}

		// Collision with walls
		// If balls touch a wall
		if (bouncyBalls[i].getPosition().x <= -5.5 + bouncyBalls[i].getRadius() || bouncyBalls[i].getPosition().x >= 5.5 - bouncyBalls[i].getRadius())
		{
			bouncyBalls[i].inverseX();
		}

		if (bouncyBalls[i].getPosition().z <= 8.5 + bouncyBalls[i].getRadius() || bouncyBalls[i].getPosition().z >= 20.5 - bouncyBalls[i].getRadius())
		{
			bouncyBalls[i].inverseZ();
		}

		// Check if a ball gets out of the enclosure
		if (bouncyBalls[i].getPosition().z < 8.0 + bouncyBalls[i].getRadius() ||
			bouncyBalls[i].getPosition().z > 21.0 - bouncyBalls[i].getRadius() ||
			bouncyBalls[i].getPosition().x < -6.0 + bouncyBalls[i].getRadius() ||
			bouncyBalls[i].getPosition().x > 6.0 - bouncyBalls[i].getRadius())
		{
			bouncyBalls[i].setPosition(glm::vec3(3.0, 6.0, 14.5));
		}
	}
}

bool MainGame::checkBouncePad()
{
	// Slightly different way of doing collision

	// Touching ground
	if (player.getPosition().y <= player.getSize() + 0.0f)
	{
		// Within X of pad
		if (player.getPosition().x <= boundMinX && player.getPosition().x >= boundMaxX)
		{
			// Within Z of pad
			if (player.getPosition().z <= boundMinZ && player.getPosition().z >= boundMaxZ)
			{
				// Touching pad
				return true;
			}
		}
	}
	return false;
}

#pragma endregion

// GAME LOOP //
#pragma region Game Loop
void MainGame::gameLoop()
{
	while (gameState != GameState::EXIT)
	{	
		processInput();
		updateChaseCamera();
		
		checkCollision();

		for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
		{
			bouncyBalls[i].updatePosition();
		}

		player.updatePosition();
		drawGame();

		// Limit game to 60FPS to avoid physics from going very fast
		// Remove for uncapped framerate
		SDL_Delay(1000 / 60);
		
	}
}

void MainGame::processInput()
{
	// Movement (WASD, Arrow Keys Camera)
#pragma region Movement

	// Get current state of keyboard (which keys are being pressed)
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents();

	// Map for Scancode to Keycode conversions
    std::map<SDL_Scancode, SDL_Keycode> keyMap = {
        {SDL_SCANCODE_W, SDLK_w},
        {SDL_SCANCODE_S, SDLK_s},
        {SDL_SCANCODE_A, SDLK_a},
        {SDL_SCANCODE_D, SDLK_d},
        {SDL_SCANCODE_SPACE, SDLK_SPACE},
        {SDL_SCANCODE_LSHIFT, SDLK_LSHIFT},
		/* OLD ARROW KEY CAMERA MOVEMENT
        {SDL_SCANCODE_UP, SDLK_UP},
        {SDL_SCANCODE_DOWN, SDLK_DOWN},
        {SDL_SCANCODE_LEFT, SDLK_LEFT},
        {SDL_SCANCODE_RIGHT, SDLK_RIGHT}
		*/
    };

	// Iterate over each key pair in map
	// Convert Scancode to Keycode
    for (const auto& key : keyMap)
    {
		// Only allow movement of 1st camera
        if (state[key.first] && cameraIndex == 0)
        {
            activeCamera.moveCameraPosition(key.second);
        }

		// Only control of object with second camera
		if (state[key.first] && cameraIndex == 1)
		{
			player.movePlayerPosition(key.second);
		}
    }

#pragma endregion

	// Other Controls
#pragma region Other Controls

    SDL_Event evnt;
    while (SDL_PollEvent(&evnt))
    {
		// Camera Movement via Mouse
		if (evnt.type == SDL_MOUSEMOTION && cameraIndex == 0)
		{
			activeCamera.handleFreeMouseMovement(evnt.motion.xrel, evnt.motion.yrel);
		}

		// Using events instead of scancodes for things that arent movement as scancodes can occur multiple times per frame
		// I.e. pressing r would change the ball shader multiple times. This way it does it just once.
		if (evnt.type == SDL_KEYDOWN)
		{
			// Play a sound
			if (evnt.key.keysym.sym == SDLK_e)
			{
				audio.playAudio(hit, audio.REVERB);
			}

			// Change shader on ball
			if (evnt.key.keysym.sym == SDLK_r)
			{
				activeShader += 1;

				if (activeShader > 4)
				{
					activeShader = 0;
				}
			}

			// Mute or unmute ball audio
			if (evnt.key.keysym.sym == SDLK_m)
			{
				switch (muted)
				{
					case true:
						std::cout << "Ball Audio Unmuted" << std::endl;
						break;

					case false:
						std::cout << "Ball Audio Muted" << std::endl;
						break;
				}

				muted = !muted;

				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].muted = muted;
				}
			}

			// Jump
			if (evnt.key.keysym.sym == SDLK_SPACE && cameraIndex == 1)
			{
				player.applyForceUp(0.5f);
			}

			// Apply force to balls
			if (evnt.key.keysym.sym == SDLK_t)
			{
				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].applyForce(0.25f, SDLK_t);
				}
			}

			if (evnt.key.keysym.sym == SDLK_UP)
			{
				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].applyForce(0.25f, SDLK_UP);
				}
			}

			if (evnt.key.keysym.sym == SDLK_DOWN)
			{
				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].applyForce(0.25f, SDLK_DOWN);
				}
			}

			if (evnt.key.keysym.sym == SDLK_LEFT)
			{
				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].applyForce(0.25f, SDLK_LEFT);
				}
			}

			if (evnt.key.keysym.sym == SDLK_RIGHT)
			{
				for (int i = 0; i < sizeof(bouncyBalls) / sizeof(bouncyBalls[0]); i++)
				{
					bouncyBalls[i].applyForce(0.25f, SDLK_RIGHT);
				}
			}

			// Change Camera
			// Note, when moving after changing camera it defualts to looking at mouseX zero, so left. Can be a bit jarring.
			if (evnt.key.keysym.sym == SDLK_c)
			{
				cameraIndex += 1;

				if (cameraIndex >= sizeof(cameras) / sizeof(*cameras))
				{
					cameraIndex = 0;
				}

				// Update camera's stored position
				cameras[cameraIndex].setPosition(activeCamera.getPosition());
				cameras[cameraIndex].setForward(activeCamera.getForward());

				activeCamera = cameras[cameraIndex];

				// Print to console which camera is being used
				switch (cameraIndex)
				{
					case 0:
						std::cout << "Free Cam Active!" << std::endl;
						break;

					case 1:
						std::cout << "Chase Cam Active!" << std::endl;
						break;
				}
			}

			// Exit
			if (evnt.key.keysym.sym == SDLK_ESCAPE)
			{
				gameState = GameState::EXIT;
			}
		}

        if (evnt.type == SDL_QUIT)
        {
            gameState = GameState::EXIT;
        }
    }
}

#pragma endregion

void MainGame::updateChaseCamera()
{
	if (cameraIndex == 1)
	{
		glm::vec3 cameraOffset(0.0f, 2.5f, -7.5f);
		glm::vec3 cameraPosition = player.getPosition() + cameraOffset;
		glm::vec3 lookAtDirection = glm::normalize(player.getPosition() - activeCamera.getPosition());

		activeCamera.setPosition(cameraPosition);
		activeCamera.setForward(lookAtDirection);
	}
}

void MainGame::drawGame()
{
	gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	drawObjects();

	// OLD - Rotates player model
	//player.setRotation(glm::vec3(0.0, counter, 0.0));

	// Increment counters
	counter = counter + 0.01f;
	dissolveCounter = dissolveCounter + dissolveAmount;

	if (dissolveCounter >= 1.0f || dissolveCounter <= 0.0f)
	{
		dissolveAmount *= -1;
	}

	gameDisplay.changeBuffer();
}

#pragma endregion