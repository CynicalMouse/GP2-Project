#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;

class Display
{
public:
	// CONSTRUCTOR & DESTRUCTOR //
	Display();
	~Display();

	void initialiseDisplay();

	// DISPLAY MANAGEMENT //
	void clearDisplay(float r, float g, float b, float a);
	void changeBuffer();

	// GETTERS //
	float getWidth();
	float getHeight();
	SDL_Window* getWindow();

private:
	void displayError(std::string errorString);

	// ATTRIBUTES //
	SDL_GLContext glContext;
	SDL_Window* window;
	float screenWidth;
	float screenHeight;
};

