#pragma once

#include "OrthogonalCanvas.h"
#include <imgui.h>

class OrthoWindow {
	ImVec2 position;
    std::shared_ptr<ApplicationState> appState;
    float width;
    float height;

    OrthogonalCanvas canvas;

    std::string windowName;

public:
    OrthoWindow(const std::string windowName, 
             std::shared_ptr<ApplicationState> appState);

	/*bool init();*/

	void layout();

};
