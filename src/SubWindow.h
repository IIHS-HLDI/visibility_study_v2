#pragma once

//#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glm/vec2.hpp>
#include <string>
#include <memory>


#include "ApplicationState.h"

class SubWindow
{
	
protected:	

    std::shared_ptr<ApplicationState> appState;

	std::string windowName;
	float width;
	float height;

	ImVec2 position;


public:
	SubWindow(const std::string windowName, std::shared_ptr<ApplicationState> appState);

	virtual bool init();

	virtual void layout();

	virtual void close();

	virtual bool isMouseOn(double mouseX, double mouseY);

	virtual glm::dvec2 mouseToWindowPosition(double mouseX, double mouseY);


};

