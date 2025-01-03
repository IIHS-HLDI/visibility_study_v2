#include "SubWindow.h"

SubWindow::SubWindow(const std::string windowName, 
        std::shared_ptr<ApplicationState> appState) : appState(appState) {

	this->windowName = windowName;
	this->width = 0;
	this->height = 0;
}

bool SubWindow::init() {
	//glGenFramebuffers(1, &fbo);
	//glGenTextures(1, &tex);
	//glGenRenderbuffers(1, &rbo);
	return 0;
}

void SubWindow::layout() {
	ImGui::Begin(windowName.c_str());
	ImGui::End();
}

void SubWindow::close() {
	//glDeleteFramebuffers(1, &fbo);
	//glDeleteTextures(1, &tex);
	//glDeleteRenderbuffers(1, &rbo);
}

bool SubWindow::isMouseOn(double mouseX, double mouseY) {
	return (position.x <= mouseX
		&& mouseX < position.x + width
		&& position.y <= mouseY
		&& mouseY < position.y + height);
}

glm::dvec2 SubWindow::mouseToWindowPosition(double mouseX, double mouseY) {
	return glm::dvec2(0.0f, 0.0f);
}
