#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::initialisePlayer()
{
	initialiseAssets();
	initialisePhysics();
	initialisePosition();
}

void Player::initialiseAssets()
{
	shaderPlayer.initialiseShader("..\\res\\shaders\\player");
	texturePlayer.initialiseTextureWrapped("..\\res\\textures\\gravel.jpg");
	player.loadMeshFile("..\\res\\models\\monkey3.obj");
}

void Player::initialisePhysics()
{
	velocity = glm::vec3(0.0, -0.1, 0.0);

	size = 0.5f;

	mass = 10.0f;

	gravity = 0.0025f;
	friction = 0.0025f;

	touchingGround = false;
}

void Player::initialisePosition()
{
	// Initial location
	playerTransform.setPos(glm::vec3(0.0, 6.0, 0.0));
	playerTransform.setRot(glm::vec3(0.0, 0.0, 0.0));
	playerTransform.setScale(glm::vec3(size, size, size));
}

void Player::shaderLink()
{
	// Locate texture uniform
	GLuint texture1Location = glGetUniformLocation(shaderPlayer.getID(), "inputTexture");

	// Set textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePlayer.getID());
	glUniform1i(texture1Location, 0);
}

void Player::drawPlayer(Camera activeCamera)
{
	shaderPlayer.bindShader();
	shaderLink();
	shaderPlayer.updateShader(playerTransform, activeCamera);

	player.draw();
}

void Player::movePlayerPosition(SDL_Keycode key)
{
	switch (key)
	{
	case SDLK_w:
		velocity.z += 0.005f;
		break;

	case SDLK_s:
		velocity.z -= 0.005f;
		break;

	case SDLK_a:
		velocity.x += 0.005f;
		break;

	case SDLK_d:
		velocity.x -= 0.005f;
		break;
	}
}

void Player::updatePosition()
{
	// On collision checks
	onCollision();

	// Physics
	physics();

	// Update Position
	playerTransform.setPos(glm::vec3(playerTransform.getPos()->x + velocity.x,
									 playerTransform.getPos()->y + velocity.y,
									 playerTransform.getPos()->z + velocity.z));
}

void Player::onCollision()
{
	// handle collision
}

void Player::physics()
{
	velocity.x = (velocity.x > 0.0) ? std::max(velocity.x - friction, 0.0f) : std::min(velocity.x + friction, 0.0f);
	velocity.z = (velocity.z > 0.0) ? std::max(velocity.z - friction, 0.0f) : std::min(velocity.z + friction, 0.0f);

	// Gravity
	if (!touchingGround)
	{
		velocity.y = velocity.y - (gravity * mass);
	}

	// Stop player going under ground
	if (playerTransform.getPos()->y < 0.0)
	{
		playerTransform.setPos(glm::vec3(playerTransform.getPos()->x, 0.0f, playerTransform.getPos()->z));
	}

	// Stop player from bouncing indefinetly when on ground (ground y = 0.0f)
	if (velocity.y < -0.001 && playerTransform.getPos()->y < size + 0.001f)
	{
		touchingGround = true;
		velocity.y = 0.0f;
	}
}

void Player::applyForceUp(float force)
{
	velocity.y += force;
	touchingGround = false;
}