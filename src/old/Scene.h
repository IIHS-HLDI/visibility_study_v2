#pragma once

#include <vector>
#include <memory>

/*#include "Camera2D.h"*/
/*#include "Drawable.h"*/
/*#include "DisplayGrid.h"*/

#include "ApplicationState.h"

// TODO: removethis in general
#include "Image.h"
class Scene
{
private:
    std::shared_ptr<ApplicationState> appState;
	std::vector<DisplayGrid> members;
	Image image;
	uint8_t imgStatus;
	friend class Camera2D;


	GLuint VAO;
	GLuint VBO;

public:
	Camera2D camera;

	Scene();
    void setApplicationPointer(std::shared_ptr<ApplicationState> appState);
	void init();
	void update();
	void render();
	void addMember(DisplayGrid displayGrid);
	void setViewportWidth(double width);
	void setViewportHeight(double height);
	void close();
protected:
};

