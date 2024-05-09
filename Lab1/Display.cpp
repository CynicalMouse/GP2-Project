#include "Display.h"

// CONSTRUCTOR & DESTRUCTOR //
#pragma region Constructor & Destructor
Display::Display()
{
	screenWidth = 1280.0f;
	screenHeight = 720.0f; 
	glContext = NULL;
	window = nullptr;
}

Display::~Display()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

#pragma endregion

// INITIALISATION //
void Display::initialiseDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// Get the primary monitor
	SDL_DisplayMode mode;
	if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
	{
		displayError("Failed to get display mode");
		return;
	}

	// Set window attributes for borderless fullscreen
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create a borderless fullscreen window
	window = SDL_CreateWindow("OpenGL Project", 0, 0, mode.w, mode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

	if (window == nullptr)
	{
		displayError("Failed to create display window");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		displayError("Failed to create GL context");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		displayError("Failed to initialise GLEW");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Enable alpha blending so models can be transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}

// DISPLAY MANAGEMENT //
#pragma region Display Management
void Display::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Change buffers
void Display::changeBuffer()
{
	SDL_GL_SwapWindow(window);
}

#pragma endregion

// GETTERS //
#pragma region Getters
float Display::getWidth()
{
	return screenWidth;
}

float Display::getHeight()
{
	return screenHeight;
}

SDL_Window* Display::getWindow()
{
	return window;
}

#pragma endregion

// ERROR HANDLING //
void Display::displayError(std::string errorString)
{
	std::cerr << errorString << std::endl;
	std::cout << "press any  key to quit...";
	std::cin.ignore();
	SDL_Quit();
}