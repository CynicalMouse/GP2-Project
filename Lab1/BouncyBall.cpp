#include "BouncyBall.h"

BouncyBall::BouncyBall()
{
	audioReference = NULL;
	ballSound = NULL;
	density = 0;
	diameter = 0;
	distanceToCamera = 0.0f;
	friction = 0.0f;
	gravity = 0.0f;
	mass = 0.0f;
	radius = 0.0f;
	touchingGround = false;
}

BouncyBall::~BouncyBall()
{

}

// INITIALISATION //
#pragma region  Drawing & Initialisation

void BouncyBall::initialiseBall(Audio* audio, unsigned int hitSound, int seed)
{
	// seed random number generator
	srand(seed);

	initialiseAssets();
	initialisePhysics();
	initialisePosition();

	audioReference = audio;
	ballSound = hitSound;
	muted = false;
}

void BouncyBall::initialiseAssets()
{
	shaderBouncyBall.initialiseShader("..\\res\\shaders\\bouncyBall");
	textureBouncyBall.initialiseTextureWrapped("..\\res\\textures\\bouncyBall.jpg");
	ball.loadMeshFile("..\\res\\models\\Ball.obj");

	// create random color for ball
	float r = (float)(rand() % 5); // red
	float g = (float)(rand() % 5); // green
	float b = (float)(rand() % 5); // blue
	color = glm::vec3(r, g, b);
}

void BouncyBall::initialisePhysics()
{
	// Random Directions
	float x = -2.5f + (static_cast<float>(std::rand()) / RAND_MAX) * 2.5f;
	float y = -0.1f - (static_cast<float>(std::rand()) / RAND_MAX) * 0.25f;
	float z = -2.5f + (static_cast<float>(std::rand()) / RAND_MAX) * 2.5f;
	velocity = glm::vec3(x, y, z);

	// Just down for testing
	//velocity = glm::vec3(0.0, -0.1, 0.0);

	// Calculate random diameter, between 0.2 and 0.5
	diameter = 0.2f + (static_cast<float>(std::rand()) / RAND_MAX) * 0.3f;
	radius = diameter / 2;

	// Calculate random density, between 90 and 110
	density = 90.0f + (static_cast<float>(std::rand()) / RAND_MAX) * 20.0f;

	// Calculate mass of ball
	// Volume * density
	mass = ((4.0f / 3.0f) * (3.14159f) * (pow(radius, 3.0f))) * density;

	gravity = 0.0025f;
	friction = 0.0000005f;

	touchingGround = false;
}

void BouncyBall::initialisePosition()
{
	// Initial location
	ballTransform.setPos(glm::vec3(3.0, 6.0, 14.5));
	ballTransform.setRot(glm::vec3(0.0, 0.0, 0.0));
	ballTransform.setScale(glm::vec3(diameter, diameter, diameter));
}

#pragma endregion

// DRAWING //
#pragma region Drawing
void BouncyBall::shaderLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderBouncyBall.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBouncyBall.getID());
	glUniform1i(texture1Location, 0);


	// set color here
	shaderBouncyBall.setVec3("addedColor", color);
}

void BouncyBall::drawBouncyBall(Camera activeCamera)
{
	shaderBouncyBall.bindShader();
	shaderLink();
	shaderBouncyBall.updateShader(ballTransform, activeCamera);

	ball.draw();

	// Calculate distance between camera and ball for distance audio
	float dx = activeCamera.getPosition().x - ballTransform.getPos()->x;
	float dy = activeCamera.getPosition().y - ballTransform.getPos()->y;
	float dz = activeCamera.getPosition().z - ballTransform.getPos()->z;

	// Clamp to affect max and min volume
	// Stops it from getting extremely loud when right next to ball. No need for a distance above 50.f as its already quiet
	distanceToCamera = std::max(3.5f, std::min(std::sqrt(dx * dx + dy * dy + dz * dz), 50.0f));
}

#pragma endregion

// PHYSICS //

void BouncyBall::updatePosition()
{	
	// On collision checks
	onCollision(false, glm::vec3(0, 0, 0));

	// Physics
	physics();

	// Update Position
	ballTransform.setPos(glm::vec3(ballTransform.getPos()->x + velocity.x,
								   ballTransform.getPos()->y + velocity.y,
								   ballTransform.getPos()->z + velocity.z));

}

void BouncyBall::physics()
{
	float frictionX = friction;
	float frictionZ = friction;

	float difference = abs(velocity.x - velocity.z);

	// Change friction applied to largest velocity so both hit 0.0f at same time
	// Stops ball from curving at end of movement
	if (abs(velocity.x) > abs(velocity.z))
	{
		frictionX = abs(frictionX + (difference / 25));
	}
	else if (abs(velocity.z) > abs(velocity.x))
	{
		frictionZ = abs(frictionZ + (difference / 25));
	}

	velocity.x = (velocity.x > 0.0) ? std::max(velocity.x - frictionX, 0.0f) : std::min(velocity.x + frictionX, 0.0f);
	velocity.z = (velocity.z > 0.0) ? std::max(velocity.z - frictionZ, 0.0f) : std::min(velocity.z + frictionZ, 0.0f);


	// gravity
	if (!touchingGround)
	{
		velocity.y = velocity.y - (gravity * mass);
	}
	
	// Stop ball from bouncing indefinetly when on ground (ground y = 0.0f)
	if (velocity.y < -0.001 && ballTransform.getPos()->y < diameter + 0.001f)
	{
		touchingGround = true;
		velocity.y = 0.0f;
	}
}

void BouncyBall::onCollision(bool collideBall, glm::vec3 incomingVelocity)
{
	// Ground
	// No need for ground collider, its always at 0.0
	if (ballTransform.getPos()->y <= (0.0 + diameter) && velocity.y < 0.0)
	{
		velocity.y *= 0.8;
		velocity.y *= -1;

		if (muted == false)
		{
			audioReference->playAudioDistance(ballSound, audioReference->NONE, distanceToCamera);
		}
	}

	if (collideBall)
	{
		// Swap velocities
		velocity = incomingVelocity;
		
		// Playing a sound here would get annoying but it would be done like this
		//audioReference->playAudioDistance(ballSound, audioReference->NONE, distanceToCamera);
	}
}

void BouncyBall::inverseX()
{
	velocity.x *= -1;

	// Playing a sound here would get annoying but it would be done like this
	//audioReference->playAudioDistance(ballSound, audioReference->NONE, distanceToCamera);
}

void BouncyBall::inverseZ()
{
	velocity.z *= -1;

	// Playing a sound here would get annoying but it would be done like this
	//audioReference->playAudioDistance(ballSound, audioReference->NONE, distanceToCamera);
}

void BouncyBall::applyForce(float force, SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_UP:
			velocity.z += force;
			break;
		case SDLK_DOWN:
			velocity.z -= force;
			break;
		case SDLK_RIGHT:
			velocity.x -= force;
			break;
		case SDLK_LEFT:
			velocity.x += force;
			break;
		case SDLK_t:
			velocity.y += force;
			touchingGround = false;
			break;
	}
	
}