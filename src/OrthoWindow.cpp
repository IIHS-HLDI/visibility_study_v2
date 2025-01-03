#include "OrthoWindow.h"


OrthoWindow::OrthoWindow(const std::string windowName,
        std::shared_ptr<ApplicationState> appState) : 
    appState(appState),
    canvas(appState) {
    /*view(appState) {*/

    this->windowName = windowName;
    this->width = 0;
    this->height = 0;

}


/*bool OrthoWindow::init() {*/
/*	return true;*/
/*}*/


void OrthoWindow::layout() {
	
	ImGui::Begin(windowName.c_str());

    canvas.layout();

	ImGui::End();
}


