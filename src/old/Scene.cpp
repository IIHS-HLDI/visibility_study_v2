#include "Scene.h"


//#include <windows.h>
#include <iostream>
#include <fstream>
#include "ApplicationState.h"



Scene::Scene() : 
    members(), 
    camera(), 
    image() {

	// if (imgStatus != Image::GOOD) {
	// 	MessageBox(NULL,
	// 		"Fatal error during glfw window initialization",
	// 		"ERROR!", MB_OK);
	// }
}

void Scene::setApplicationPointer(std::shared_ptr<ApplicationState> appState) {
    this->appState.swap(appState);
}


void Scene::init() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);


}


void Scene::setViewportWidth(double width) {
	camera.setViewportWidth(width);
}

void Scene::setViewportHeight(double height) {
	camera.setViewportHeight(height);
}
void Scene::update() {
    if (appState->projectLoaded) {
        image.load(appState->currentProject->measurements[appState->selectedMeasurementConfig].images[appState->selectedImage].imageFilepath);
        appState->markReload = false;
    }
}

void Scene::render() {

    if (appState->markReload) {
        update();
    }


	image.render();

	camera.applyTransform();

	for (DisplayGrid& displayGrid : members) {
		displayGrid.render();
	}
}

void Scene::addMember(DisplayGrid drawable) {
	members.push_back(drawable);
}

void Scene::close() {
	image.close();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

}
