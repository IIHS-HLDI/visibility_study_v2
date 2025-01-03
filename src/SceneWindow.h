#pragma once
#include <boxer/boxer.h>
#include "SubWindow.h"
/*#include "View.h"*/
#include "Canvas.h"
/*#include "Scene.h"*/

class SceneWindow {
private:

	ImVec2 position;
    std::shared_ptr<ApplicationState> appState;
    float width;
    float height;

	GLuint fbo;
	GLuint tex;
	GLuint rbo;

    /*View view;*/
    Canvas canvas;

    std::string windowName;

    /*ImGuiIO& io;*/

    bool isHovered;
    bool isFocused;


    /*ImVec2 mousePositionAbsolute*/

	/*friend class Scene;*/

public:
	/*Scene scene;*/
    /*using SubWindow::SubWindow;*/
    SceneWindow(const std::string windowName, 
             std::shared_ptr<ApplicationState> appState);


	/*bool init();*/

	void layout();

	void close();

	void rescale();
};

