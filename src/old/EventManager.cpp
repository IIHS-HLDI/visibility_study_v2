#include "EventManager.h"

EventManager::EventManager(std::shared_ptr<ApplicationState> appState) :
    appState(appState) {
}

void EventManager::cursorPositionCallback(
	GLFWwindow* window,
	double mouseX,
	double mouseY
)
{
	// ApplicationInstance* appInstance = (ApplicationInstance*)glfwGetWindowUserPointer(window);
    

}

void EventManager::mouseButtonCallback(
	GLFWwindow* window,
	int button,
	int action,
	int mods
)
{

}

void EventManager::scrollCallback(
	GLFWwindow* window,
	double deltaX,
	double deltaY
)
{
	//ApplicationInstance* app = (ApplicationInstance*) glfwGetWindowUserPointer(window);
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//app->scroll(deltaY);
}

void EventManager::keypressCallback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods
)
{

}
