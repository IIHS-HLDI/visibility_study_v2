#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "ApplicationState.h"

class EventManager
{
private:
    std::shared_ptr<ApplicationState> appState;
public:

    EventManager(std::shared_ptr<ApplicationState> appState);


	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void cursorPositionCallback(GLFWwindow* window, double mouseX, double mouseY);

	static void scrollCallback(GLFWwindow* window, double deltaX, double deltaY);

	static void keypressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void broadcast() {

	}

};

