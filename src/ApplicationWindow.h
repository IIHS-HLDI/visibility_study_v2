#pragma once

#include <optional>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nfd.h>

#include "ApplicationState.h"
#include "NewVehicleProjectWindow.h"
#include "OutputWriter.h"
#include "FileSelectors.h"


/**
 * 
 */
class ApplicationWindow {
private:

    /** 
     * Enumerated type for main menu (top bar) options
     *
     * This is needed in the ApplicationWindow because popups cannot be 
     * called directly from the main menu layout code
     */
    enum MainMenuOption : int {
        MMO_NEW_PROJECT,
        MMO_OPEN_PROJECT,
        MMO_QUIT,
        MMO_UNDO,
        MMO_REDO,
        MMO_VIEW_HELP,
        MMO_WRITE_FILES,
    };
    
    // pointer to application state
    std::shared_ptr<ApplicationState> appState;

    // label at the top bar
	std::string windowLabel;

    // new project dialog
	NewVehicleProjectWindow newVehicleProjectWindow;

    // ImGui things
	ImGuiWindowFlags windowFlags;
	ImGuiDockNodeFlags dockspaceFlags;
	ImGuiViewport* mainViewport;

public:
    // Pointer to glfw window for event handling
	GLFWwindow* window;

	int width;
	int height;


    void writeOutputFiles();

	ApplicationWindow(std::shared_ptr<ApplicationState> appState);

	uint32_t getWidth();

	uint32_t getHeight();

    void resetDockspace();

	
	void init();

	void layout();

};

