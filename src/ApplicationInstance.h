#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>
#include <cstring>
#include <algorithm>

#include <nfd.hpp>
#include <boxer/boxer.h>

#include "ApplicationWindow.h"
#include "SceneWindow.h"
#include "ProjectExplorer.h"
#include "Toolbar.h"
#include "OrthoWindow.h"
#include "MeasurementConfigurator.h"
#include "OutputWriter.h"

#include "ApplicationState.h"
#include "VehicleProject.h"

#include "FileSelectors.h"

/**
 * Container for the application runtime itself
 */
class ApplicationInstance
{
private:

    std::shared_ptr<ApplicationState> appState;

	GLFWwindow* window;

    // Subwindows
	ApplicationWindow appWindow;
    ProjectExplorer projectExplorer;
    MeasurementConfigurator measurementConfig;
    Toolbar toolbar;
	SceneWindow perspectiveWindow;
	OrthoWindow orthogonalWindow;
    
    // Subsystem
    OutputWriter outputWriter;

public:

    /**
     * Application instance constructor
     */
	ApplicationInstance();

    /*void resetDockspace();*/

    /**
     * Callback function for glfw errors. This is called whenever glfw
     * experiences an error
     */
    static void glfwErrorCallback(int error, const char* description);


    /**
     * Callback function for keypresses.
     *
     * ImGui library handles keypresses for widgets and .
     * This is used for global shortcuts, e.g. ctrl+s for save. 
     */
	static void keypressCallback(
            GLFWwindow* window, 
            int key,
            int scancode,
            int action,
            int mods);



    /**
    * Application main loop
    * 
    */
	void mainLoop();

    /** 
     * application instance destructor
     */ 
    ~ApplicationInstance();

};

