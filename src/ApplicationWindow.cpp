#include "ApplicationWindow.h"


/**
* Initializer for ApplicationWindow. 
* 
*/
ApplicationWindow::ApplicationWindow(std::shared_ptr<ApplicationState> appState) :
    newVehicleProjectWindow(appState),
    appState(appState)
{

    // set default width and height
	width = 2000;
	height = 1200;
	
	mainViewport = nullptr;
	dockspaceFlags = ImGuiDockNodeFlags_None;
	windowFlags = ImGuiWindowFlags_None;
}

/**
 * Getter for application window height
 *
 * returns the height of the application window
 */
uint32_t ApplicationWindow::getHeight() {
	return height;
}

/**
 * Getter for application window width
 *
 * returns the width of the application window
 */
uint32_t ApplicationWindow::getWidth() {
	return width;
}

void ApplicationWindow::writeOutputFiles() {
    OutputWriter::writeOutputFiles(*(appState->currentProject), appState);


}



/**
* Initializes the ApplicationWindow.
*
* return false if initialization fails
*/
void ApplicationWindow::init() {

	windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	mainViewport = ImGui::GetMainViewport();
	windowFlags |= ImGuiWindowFlags_MenuBar
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavFocus;
	
	//dockspaceFlags = ImGuiDockNodeFlags_NoCloseButton;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) {
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 1.2f;
    io.IniFilename = NULL;

}

void ApplicationWindow::resetDockspace() {

    ImGuiViewport* mainViewport = ImGui::GetMainViewport();

    ImGuiID dockspaceID = ImGui::GetID("MainDockspace");

    ImGui::DockBuilderRemoveNode(dockspaceID);
    ImGui::DockBuilderAddNode(dockspaceID, 
            ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceID, mainViewport->Size);

    auto Sidebar = ImGui::DockBuilderSplitNode(dockspaceID,
            ImGuiDir_Left, 0.25f,
            nullptr, &dockspaceID);


    auto perspAndToolbar = ImGui::DockBuilderSplitNode(dockspaceID,
            ImGuiDir_Up, 0.25f,
            nullptr, &dockspaceID);

    auto ortho = ImGui::DockBuilderSplitNode(dockspaceID,
            ImGuiDir_Right, 0.25f,
            nullptr, &dockspaceID);

	ImGui::DockBuilderDockWindow("Perspective View", dockspaceID);
	ImGui::DockBuilderDockWindow("Toolbar", perspAndToolbar);

	ImGui::DockBuilderDockWindow("Project Explorer", Sidebar);
	ImGui::DockBuilderDockWindow("Measurement Configuration", Sidebar);
	ImGui::DockBuilderDockWindow("Orthogonal View", ortho);


	ImGui::DockBuilderFinish(dockspaceID);
}



void ApplicationWindow::layout() {

	mainViewport = ImGui::GetMainViewport();


	ImGui::SetNextWindowPos(mainViewport->WorkPos);
	ImGui::SetNextWindowSize(mainViewport->WorkSize);
	ImGui::SetNextWindowViewport(mainViewport->ID);
	
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

	static bool mainWindowOpen = true;
	ImGui::Begin("Main Window", &mainWindowOpen, windowFlags);
	//ImGui::SetWindowFontScale(1.6f);
    static bool runOnce = true;
    if (runOnce) {
        resetDockspace();
        runOnce = false;
    }

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}

	//	ImGui::Text("Hello World");
	int mainMenuOption = -1;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Project", "CTRL+N")) {
                // Check if user wants to save changes
                newVehicleProjectWindow.reset();
                
				mainMenuOption = MMO_NEW_PROJECT;

			}
			if (ImGui::MenuItem("Open Project", "CTRL+O")) {
                mainMenuOption = MMO_OPEN_PROJECT;
			}
			if (ImGui::MenuItem("Save Project", "CTRL+S")) {

                saveProject(*(appState->currentProject), 
                        appState->currentProject->projectFilePath);
                appState->lastSave = std::chrono::system_clock::now();

			}
            if (ImGui::MenuItem("Save Project In New Location", "CTRL+SHIFT+S")) {
                if (auto parentDirectory = chooseParentDirectory()) {
                    VehicleProject projectToSaveAs = appState->currentProject->saveAs(*parentDirectory);

                    saveProject(projectToSaveAs, projectToSaveAs.projectFilePath);
                }
            }
			if (ImGui::MenuItem("Write Files", "CTRL+W")) {
                if (appState->projectLoaded) {
                    //OutputWriter::writeOutputFiles(*(appState->currentProject), appState);
                    mainMenuOption = MMO_WRITE_FILES;
                    //writeOutputFiles();
                }

                /*saveProject(*(appState->currentProject), appState->currentProject->projectFilePath);*/
			}
			if (ImGui::MenuItem("Write Output As", "CTRL+SHIFT+W")) {
                if (appState->projectLoaded) {
                    // OutputWriter::writeOutputFiles(*(appState->currentProject), appState);
                    if (auto outputFilepath = chooseOutputCSV()) {
                        OutputWriter::writeOutputFileAs(*(appState->currentProject), appState, *outputFilepath);
                    }
                }
			}

            if (ImGui::MenuItem("Quit", "CTRL+Q")) {
                mainMenuOption = MMO_QUIT;
            }
			/*if (ImGui::MenuItem("Import image")) {*/
			/**/
			/*}*/
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {
                mainMenuOption = MMO_UNDO;
			}

			if (ImGui::MenuItem("Redo", "CTRL+Y")) {

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {


            if (ImGui::BeginMenu("Perspective View")) {

                if (ImGui::MenuItem("Reset View")) {
                }

                ImGui::SliderFloat("Spline Opacity", 
                        &(appState->splineOpacity), 
                        0.0f, 1.0f);

                ImGui::SliderFloat("Points Opacity", 
                        &(appState->pointsOpacity), 
                        0.0f, 1.0f);

                ImGui::SliderFloat("Grid Opacity", 
                        &(appState->gridOpacity), 
                        0.0f, 1.0f);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Orthogonal View")) {

                if (ImGui::MenuItem("Reset View")) {
                }

                ImGui::Checkbox("Display Origin", 
                        &(appState->orthoShowOrigin));

                ImGui::Checkbox("Display Camera", 
                        &(appState->orthoShowCamera));

                ImGui::Checkbox("Display Grid",
                        &(appState->orthoShowGrid));

                ImGui::EndMenu();
            }

			ImGui::EndMenu();
		}
		// if (ImGui::BeginMenu("Tools")) {
		// 	ImGui::EndMenu();
		// }
        /*
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("View Help")) {
                mainMenuOption = MMO_VIEW_HELP;
			}
			if (ImGui::MenuItem("Keyboard Shortcuts")) {

			}
			ImGui::EndMenu();
		}
        */
		ImGui::EndMenuBar();
	}
	
	if (mainMenuOption == MMO_NEW_PROJECT) {
        newVehicleProjectWindow.reset();
		ImGui::OpenPopup("New Project");
    }

    if (mainMenuOption == MMO_OPEN_PROJECT) {
        // std::string projectFile;
        if (auto projectFile = chooseProjectFile()) {
            std::shared_ptr<VehicleProject> project = loadProject(*projectFile);

            appState->currentProject.swap(project);
            appState->projectLoaded = true;
        }
    }
    if (mainMenuOption == MMO_QUIT) {
        if (appState->unsavedChanges) {
			ImGui::OpenPopup("Unsaved Changes");
        }
        else {
            appState->shouldClose = true;
        }
    }
    if (mainMenuOption == MMO_UNDO) {
        appState->undo();
    }
    if (mainMenuOption == MMO_REDO) {
        appState->redo();
    }
    if (mainMenuOption == MMO_VIEW_HELP) {
        ImGui::OpenPopup("Help Menu");
    }
    if (mainMenuOption == MMO_WRITE_FILES) {

        writeOutputFiles();
        ImGui::OpenPopup("Write Successful");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();

	ImGui::SetNextWindowPos(center,
		ImGuiCond_Appearing,
		ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Write Successful")) {
		ImGui::Text("The output has been written successfully");

		if (ImGui::Button("OK")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();

	}
	
    ImGui::SetNextWindowPos(center, 
            ImGuiCond_Appearing, 
            ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Unsaved Changes")) {
        ImGui::Text("The active project has unsaved changes");

        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Quit without saving")) {
            appState->shouldClose = true;
            ImGui::CloseCurrentPopup();
            /*glfwTerminate();*/
        }
        ImGui::SameLine();
        if (ImGui::Button("Save and quit")) {
            appState->saveCurrentProject();
            //saveProject(*(appState->currentProject), appState->currentProject->projectFilePath);
            appState->shouldClose = true;
            ImGui::CloseCurrentPopup();
        }
		ImGui::EndPopup();
    }

    ImGui::SetNextWindowPos(center, 
            ImGuiCond_Appearing, 
            ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Help Menu")) {
        static int selected = -1;
        ImGui::BeginGroup();
        // for (int n = 0; n < IM_ARRAYSIZE(helpMenuTopics); n++)
        // {
        //     // sprintf(buf, "Help Topic", n);
        //     if (ImGui::Selectable(helpMenuTopics[n].topic.c_str(), selected == n))
        //         selected = n;
        // }
        ImGui::EndGroup();
        ImGui::SameLine(0, 30);

        //std::function<void()> layoutFunc = helpMenuTopics[selected].layoutFunc;
        //layoutFunc();
        //helpMenuTopics[selected].layoutFunc();
        // ImGui::Text(helpMenuTopics[selected].content.c_str());

        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
		ImGui::EndPopup();
    }

	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(800, 500));
	if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_None)) {

		newVehicleProjectWindow.layout();

		ImGui::EndPopup();
	}


	if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, 
                ImGuiDir_Down, 
                ImGui::GetFrameHeight(), 
                ImGuiWindowFlags_NoScrollbar | 
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Ready");
			ImGui::Text("|");
            if (!appState->projectLoaded) {
                ImGui::Text("No project loaded");
            } else {
                if (appState->unsavedChanges) {
                    ImGui::Text("%s (unsaved)",
                        appState->currentProject->projectFilePath.c_str());
                }
                else {
                    ImGui::Text("%s",
                        appState->currentProject->projectFilePath.c_str());
                }
            }
            if (appState->lastSave.time_since_epoch() != decltype(appState->lastSave)::duration::zero()) {
				ImGui::Text("|");

				std::time_t lastSaveTimeT =
					std::chrono::system_clock::to_time_t(appState->lastSave);

                char time_buf[50];
                size_t size = strftime(time_buf, 50, "%m/%d/%Y @ %I:%M:%S", std::localtime(&lastSaveTimeT));
				std::ostringstream lastSaveString;
				//lastSaveString << std::ctime(&lastSaveTimeT);
				ImGui::Text("Last save: %s", time_buf);
				//ImGui::Text(lastSaveString.str().c_str());

            }

            if (io.WantCaptureMouse) {
                ImGui::Text("|");
                ImGui::Text("%0.2f, %0.2f", io.MousePos.x, 
                        io.MousePos.y);
                ImGui::Text("|");
                ImGui::Text("%0.2f, %0.2f", appState->canvasPosition.x, 
                        appState->canvasPosition.y);
            }
			ImGui::EndMenuBar();
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();
}

