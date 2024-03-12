#pragma once
#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();
	bool DebugMode = false;
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getKeys(){ return keys; }
	GLfloat getXChange() { return xChange; }
	GLfloat getYChange() { return yChange; }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }
	/// <summary>
	/// manually close the window
	/// </summary>
	void SetWindowShouldClose() { glfwSetWindowShouldClose(mainWindow, GL_TRUE); }

	/// <summary>
	/// set callback to the function when a key is pressed
	/// </summary>
	void SetWindowCallBack(){
		glfwSetKeyCallback(mainWindow, handleKeys);
		glfwSetCursorPosCallback(mainWindow, handleMouse);
	}

	~Window();

private:
	GLFWwindow* mainWindow;
	bool keys[1024]; //array of keys, 1024 is the max number of keys on a keyboard
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//mouse variables
	bool mouseFirstMoved;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	//callbacks
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

