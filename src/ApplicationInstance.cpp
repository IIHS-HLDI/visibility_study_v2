#include "ApplicationInstance.h"

ApplicationInstance::ApplicationInstance():
    appState {std::make_shared<ApplicationState>()},
	perspectiveWindow("Perspective View", appState),
    orthogonalWindow("Orthogonal View", appState),
    projectExplorer("Project Explorer", appState),
    measurementConfig("Measurement Configuration", appState),
    toolbar("Toolbar", appState),
    outputWriter(appState),

	appWindow(this->appState)
{


    // Setup native file dialog guard, which automatically frees memory
    NFD::Guard nfdGuard;

	// Set error callback function
	glfwSetErrorCallback(glfwErrorCallback);

	// Initialize glfw, error and exit if initialization fails.
	if (!glfwInit()) {
        boxer::show("Fatal error during glfw initialization", "ERROR!");
        exit(1);
	}

	// add GLFW context window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create glfw window 
	window = glfwCreateWindow(appWindow.getWidth(), appWindow.getHeight(), "Visibility Tool", NULL, NULL);

	// check window initialization
	if (window == NULL) {
        boxer::show("Fatal error during glfw window initialization", "ERROR!");
        exit(1);
	}

	// Embed reference to application object
	glfwSetWindowUserPointer(window, this);

	appWindow.window = window;

    glfwSetKeyCallback(window, keypressCallback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// load opengl using glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        boxer::show("Failed to load OpenGL", "ERROR!");
        exit(1);
	}

	glfwGetFramebufferSize(window, &(appWindow.width), &(appWindow.height));
	glViewport(0, 0, appWindow.width, appWindow.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, appWindow.width, appWindow.height, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.31f, 0.68f, 1.00f);
    // Adjust  styling
    // Change these values for styling
    // ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    // style.ScrollbarRounding = 0;

    // style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    // style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    // style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    // style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    // style.Colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    // style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    // style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    // style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    // style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    // style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    // style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    // style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    // style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    // style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    // style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    // style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    // style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    // style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
    // style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    // style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    // style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    // style.Colors[ImGuiCol_Button]                = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    // style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    // style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    // style.Colors[ImGuiCol_Header]                = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    // style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    // style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    // style.Colors[ImGuiCol_Column]                = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    // style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    // style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    // style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    // style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    // style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    // style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
    // style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
    // style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    // style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    // style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    // style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    // style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);


    // Initialize ImGui for opengl and glfw
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init();

// Initialize app window dockspace
appWindow.init();
}

//void ApplicationInstance::writeOutput() {
    /*if (appState->projectLoaded) {*/
    /*    outputWriter.writeOutputFiles();*/
    /*}*/
//}

// TODO: This for now, todo make an ErrorHandler
void ApplicationInstance::glfwErrorCallback(
    int error,
    const char* description)
{
    boxer::show(description, "ERROR!");
    exit(1);
}


bool isKey(int key, int scancode, const char* check) {
    const char* keyName = glfwGetKeyName(key, scancode);
    // std::string keyNameString(keyName);
    //
    // std::transform(keyNameString.begin(), keyNameString.end(), keyNameString.begin(), ::tolower);
    // std::transform(check.begin(), check.end(), check.begin(), ::tolower);


    return keyName != nullptr && strcmp(keyName, check) == 0;
}


void ApplicationInstance::keypressCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {

    // Get application instance pointer
    ApplicationInstance* appInstance =
        (ApplicationInstance*)glfwGetWindowUserPointer(window);


    //if (key == GLFW_KEY_S
        //&& action == GLFW_RELEASE
        //&& mods == (GLFW_MOD_CONTROL & GLFW_MOD_SHIFT)) {

        //if (auto parentDirectory = chooseParentDirectory()) {
            //VehicleProject projectToSaveAs = appInstance->appState->currentProject->saveAs(*parentDirectory);

            //saveProject(projectToSaveAs, projectToSaveAs.projectFilePath);
        //}
    //}


    // if ImGui widget is focused, then passthrough keypresses
    if (!ImGui::GetIO().WantTextInput) {

        if (isKey(key, scancode, "o") &&
            action == GLFW_RELEASE &&
            mods == GLFW_MOD_CONTROL) {
            appInstance->appState->openProject();
        }
        else if (isKey(key, scancode, "z") &&
            action == GLFW_RELEASE &&
            mods == GLFW_MOD_CONTROL) {
            appInstance->appState->undo();
        }
        else if (isKey(key, scancode, "y") &&
            action == GLFW_RELEASE &&
            mods == GLFW_MOD_CONTROL) {
            appInstance->appState->redo();
		}
        else if (isKey(key, scancode, "s") &&
            action == GLFW_RELEASE &&
            mods == GLFW_MOD_CONTROL) {


            appInstance->appState->saveCurrentProject();
            //saveProject(*(appInstance->appState->currentProject),
                //appInstance->appState->currentProject->projectFilePath);
            //appInstance->appState->lastSave = std::chrono::system_clock::now();

        } else if (isKey(key,scancode, "W") &&
                action == GLFW_RELEASE &&
                mods == GLFW_MOD_CONTROL) {
                if (appInstance->appState->projectLoaded) {
                    // OutputWriter::writeOutputFiles(*(appState->currentProject), appState);
                    if (auto outputFilepath = chooseOutputCSV()) {
                        OutputWriter::writeOutputFileAs(
                                *(appInstance->appState->currentProject), 
                                appInstance->appState, 
                                *outputFilepath);
                    }
                }
            // OutputWriter::writeOutputFiles(*(appInstance->appState->currentProject), appInstance->appState);
        
        } else if (isKey(key,scancode, "w") &&
                action == GLFW_RELEASE &&
                mods == GLFW_MOD_CONTROL){

            appInstance->appWindow.writeOutputFiles();

            //ImGui::OpenPopup("Write Successful");

            //appInstance->appWindow.writeOutputFiles();


        }

        if (appInstance->appState->currentTool == BEZIER_TOOL) {
            if (isKey(key,scancode,"a") &&
                    action == GLFW_PRESS) {
                appInstance->appState->bezierToolState.mode = BEZIER_ADD;
            } else if (isKey(key,scancode,"e") &&
                    action == GLFW_PRESS) {
                appInstance->appState->bezierToolState.mode = BEZIER_EDIT;
            } else if (isKey(key,scancode,"d") &&
                    action == GLFW_PRESS) {
                appInstance->appState->bezierToolState.mode = BEZIER_DELETE;
            }
        } else if (appInstance->appState->currentTool == PAINT_TOOL) {
            if (isKey(key,scancode,"a") &&
                    action == GLFW_PRESS) {
                appInstance->appState->paintToolState.mode = PAINT_ADD;
            } else if (isKey(key,scancode,"d") &&
                    action == GLFW_PRESS) {
                appInstance->appState->paintToolState.mode = PAINT_DELETE;
            } 
        }
        // if (key == GLFW_KEY_O && 
        //         action == GLFW_RELEASE && 
        //         mods == GLFW_MOD_CONTROL) {

        //     appInstance->openProject();

        // } else if (key == GLFW_KEY_Z &&
        //         action == GLFW_RELEASE &&
        //         mods == GLFW_MOD_CONTROL) {
        //     appInstance->undo();
        // }

        // if (appInstance->appState->currentTool == BEZIER_TOOL) {
        //     if (key == GLFW_KEY_A && 
        //             action == GLFW_PRESS) {
        //         appInstance->appState->bezierToolState.mode = BEZIER_ADD;
        //     } else if (key == GLFW_KEY_E &&
        //             action == GLFW_PRESS) {
        //         appInstance->appState->bezierToolState.mode = BEZIER_EDIT;
        //     } else if (key == GLFW_KEY_D &&
        //             action == GLFW_PRESS) {
        //         appInstance->appState->bezierToolState.mode = BEZIER_DELETE;
        //     }
        // }

    }

}




/**
 * Main execution loop of the program
 *
 *
 */
void ApplicationInstance::mainLoop() {


	while (!glfwWindowShouldClose(window) && !appState->shouldClose) {

        // Poll glfw for key, resize, and mouse events
		glfwPollEvents();

        // Create new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


        // Layout and render each subwindow
		appWindow.layout();
        toolbar.layout();
        projectExplorer.layout();
        measurementConfig.layout();
		perspectiveWindow.layout();
		orthogonalWindow.layout();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
    // if (appState->unsavedChanges) {
    // }

}





ApplicationInstance::~ApplicationInstance() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}


