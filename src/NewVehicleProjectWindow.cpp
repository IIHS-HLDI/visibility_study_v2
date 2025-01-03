#include "NewVehicleProjectWindow.h"
//#include "Combo.h"



NewVehicleProjectWindow::NewVehicleProjectWindow(std::shared_ptr<ApplicationState> appState):
    appState(appState)
{
    //reset();
}


// std::vector<std::string> enumerateImageFiles(std::string path) {


// }

void NewVehicleProjectWindow::reset() {
    project.reset();
    project = std::make_shared<VehicleProject>();
    //project = new VehicleProject;
}


void NewVehicleProjectWindow::layout() {

	// Save Folder path
	// N: drive shared research programs/ direct visibility maps by default.

	ImGui::SeparatorText("Vehicle Information");
	static uint16_t u16_one = 1;
	ImGui::InputScalar("Year", ImGuiDataType_U16, (uint16_t*) &(project->vehicleInfo.year), &u16_one, &u16_one, "%04u");
	

    ImGui::Combo(
        "Vehicle Make",
        (int*) &(project->vehicleInfo.make),
        vehicleMakeStrings,
        IM_ARRAYSIZE(vehicleMakeStrings));

	//ImGui::Text("Selection: %s, index = %d", vehicleMakeData.input, vehicleMakeData.index);
	//ImGui::Combo("Make", 
		//(int*) &vehicleInfo.make,
		//vehicleMakeStrings,
		//IM_ARRAYSIZE(vehicleMakeStrings));

	ImGui::InputText("Model", (project->vehicleInfo.model), MAX_VEHICLE_INFO_CHAR_BUF);
	ImGui::InputText("Details", (project->vehicleInfo.details), MAX_VEHICLE_INFO_CHAR_BUF);
	
	ImGui::Combo(
		"Vehicle Class",
		(int*) &(project->vehicleInfo._class),
		vehicleClassStrings,
		IM_ARRAYSIZE(vehicleClassStrings));

	ImGui::InputText("VIN", project->vehicleInfo.VIN, VIN_LENGTH, ImGuiInputTextFlags_CharsUppercase);


	ImGui::SeparatorText("Measurement Configurations");

	static ImVector<int> active_tabs;

	static int next_tab_id = 0;
	//if (next_tab_id == 0) // Initialize with some default tabs
		//for (int i = 0; i < 3; i++)
			//active_tabs.push_back(next_tab_id++);

	// TabItemButton() and Leading/Trailing flags are distinct features which we will demo together.
	// (It is possible to submit regular tabs with Leading/Trailing flags, or TabItemButton tabs without Leading/Trailing flags...
	// but they tend to make more sense together)
	static bool show_leading_button = false;
	static bool show_trailing_button = true;
	//ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
	//ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

	// Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
	static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
		ImGuiTabBarFlags_Reorderable | 
		ImGuiTabBarFlags_FittingPolicyResizeDown |
	ImGuiTabBarFlags_TabListPopupButton;


	if (ImGui::BeginTabBar("Measurement Configurations", tab_bar_flags))
	{

        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
			active_tabs.push_back(next_tab_id++); // Add new tab
            MeasurementConfig config;
            project->measurements.push_back(config);
        }

		// Submit our regular tabs
		for (int n = 0; n < active_tabs.size(); n++)
		{
			bool open = true;
			char name[256];

			snprintf(name, IM_ARRAYSIZE(name), "%d: %s %s %s", 
                    active_tabs[n],
                    occSizeStrings[project->measurements[active_tabs[n]].occupantSize], 
                    seatTrackPositionStrings[project->measurements[active_tabs[n]].seatTrackPosition],
                    seatHeightStrings[project->measurements[active_tabs[n]].seatHeight]);

			if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
			{
                ImGui::PushID(active_tabs[n]);
                MeasurementConfig* currentConfig = &(project->measurements[active_tabs[n]]);


                ImGui::SeparatorText("Representation");
                ImGui::Combo("Occupant Size", 
                        (int*) &(project->measurements[active_tabs[n]].occupantSize),
                        occSizeStrings,
                        IM_ARRAYSIZE(occSizeStrings));

                ImGui::Combo("Seat Track", 
                        (int*) &(project->measurements[active_tabs[n]].seatTrackPosition),
                        seatTrackPositionStrings,
                        IM_ARRAYSIZE(seatTrackPositionStrings));

                ImGui::Combo("Seat Height", 
                        (int*) &(project->measurements[active_tabs[n]].seatHeight),
                        seatHeightStrings,
                        IM_ARRAYSIZE(seatHeightStrings));

                ImGui::SeparatorText("Camera Position Offsets");
                ImGui::Text("Input camera position offsets as positive distance from vehicle front left corner.");

                // TODO: pictoral diagram input
                ImGui::InputDouble("X Offset (cm) (+)", &(currentConfig->measurementOffsets.rigPosition.x), 1.0, 1.0, "%.3f");
                ImGui::InputDouble("Y Offset (cm) (-)", &(currentConfig->measurementOffsets.rigPosition.y), 1.0, 1.0, "%.3f");
                ImGui::InputDouble("Z Offset (cm) (+)", &(currentConfig->measurementOffsets.rigPosition.z), 1.0, 1.0, "%.3f");
                // Clip input to positive values
                currentConfig->measurementOffsets.rigPosition.x = std::max(0.0, currentConfig->measurementOffsets.rigPosition.x);
                currentConfig->measurementOffsets.rigPosition.y = std::max(0.0, currentConfig->measurementOffsets.rigPosition.y);
                currentConfig->measurementOffsets.rigPosition.z = std::max(0.0, currentConfig->measurementOffsets.rigPosition.z);



                if (ImGui::Button("Choose Image Files")) {
                    nfdpathsetsize_t numPaths = 0;

                    NFD::UniquePathSet outPaths = chooseImageFiles();

                    NFD::PathSet::Count(outPaths, numPaths);

                    currentConfig->images.resize(numPaths);
                    imagePreviews.resize(numPaths);

                    for (nfdpathsetsize_t i = 0; i < numPaths; i++) {
                        NFD::UniquePathSetPath path;
                        NFD::PathSet::GetPath(outPaths, i, path);
                        currentConfig->images[i].imageFilepath = path.get();
                        /*currentConfig->images[i].width = */
                        cv::Mat currImage = cv::imread(currentConfig->images[i].imageFilepath);
                        auto imageSize = currImage.size();

                        currentConfig->images[i].width = imageSize.width;
                        currentConfig->images[i].height = imageSize.height;

                        // imagePreviews[i].load(currentConfig->images[i].imageFilepath, 0.07);
                    }
                    
                }

                // static bool runOnce = false;

                // if (!runOnce) {
                //     imagePreviews.resize(currentConfig->images.size());;
                // }

                ImGui::SameLine();
                ImGui::Checkbox("Copy files to project folder?", &copyFiles);
                if (ImGui::TreeNode("Image Files")) {

                    for (unsigned int i = 0; i < currentConfig->images.size(); i++)
                    {
                        if (ImGui::TreeNode((void*)(intptr_t)i, "Image %d: %s", i, currentConfig->images[i].imageFilepath.c_str())) {


//                             if (imagePreviews[i].isLoaded()) {
//                                 imagePreviews[i].layout();
//                             } else {
//                                 imagePreviews[i].load(currentConfig->images[i].imageFilepath, 0.04);
//                                 imagePreviews[i].layout();
//                             }


                            ImGui::SameLine(0,30);

                            ImGui::InputDouble("Yaw Angle", &(currentConfig->images[i].yawAngle), 0.01, 1.0, "%0.2f");
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }

				//ImGui::Text("This is the %s tab!", name);
				ImGui::EndTabItem();
                ImGui::PopID();
			}

			if (!open){
                project->measurements.erase(project->measurements.begin()+active_tabs[n]);
				active_tabs.erase(active_tabs.Data + n);
            }else{
				n++;
            }
		}

		ImGui::EndTabBar();
	}
	ImGui::Separator();
	//ImGui::TreePop();


    //static char imagesFolderBuf[BUFSIZ];
	//ImGui::InputText("Image File Directory", imagesFolderBuf, BUFSIZ);
	//ImGui::PushID(1);
	//ImGui::PopID();

	ImGui::SeparatorText("Project Location");

	//static char projectFolderBuf[BUFSIZ];
	ImGui::InputText("Project Folder", &(project->parentDirectory));

        // if(std::filesystem::exists(project->parentDirectory)) {

        // } else {

        // }
    // }
	ImGui::SameLine();
	if (ImGui::Button("...")) {
		chooseParentDirectory();
	}

    ImGui::Separator();

	if (ImGui::Button("Cancel")) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Create")) {
		//TODO: make new project logic
        this->initializeProject();
        this->appState->currentProject.swap(project);
        this->appState->projectLoaded = true;
        //this->appState->currentProject(project);

		ImGui::CloseCurrentPopup();
	}
}

// Copies image files to project directory.
void NewVehicleProjectWindow::copyImageFiles() {


    // Loop through measurement configs
    for (size_t i = 0; i < project->measurements.size(); i++) {

        std::ostringstream measurementDirectory;
        
        /*measurementDirectory << */
        /*    project->projectDirectory << */
        /*    PATH_SEP << */
        /*    occSizePathName[project->measurements[i].occupantSize];*/
        
        // Loop through images in measurement config
        for (size_t j = 0; j < project->measurements[i].images.size(); j++) {

            /*std::ostringstream outpath;*/
            /**/
            /*std::filesystem::copy_file(*/
            /*        std::filesystem::path(imageFilepath), */
            /*        std:::filesystem:path());*/
        }
    }
}

void NewVehicleProjectWindow::initializeProject() {

    //TODO: edge cases and exception handling if files already exist
    // static std::regex space_re(" +");

    // std::ostringstream projectNameStream;

    // projectNameStream << 
    //     project->vehicleInfo.year << "_" <<
    //     vehicleMakeStrings[project->vehicleInfo.make] << "_" <<
    //     project->vehicleInfo.model << "_" <<
    //     project->vehicleInfo.details;



    // project->projectName = std::regex_replace(projectNameStream.str(), space_re, "_");
    project->projectName = project->getProjectName();


    std::ostringstream savePath;

    savePath << project->parentDirectory << PATH_SEP << 
        project->projectName;

    project->projectDirectory = savePath.str();


    // if (std::filesystem::exists(project->projectDirectory)) {
    // } else {
    //     std::filesystem::create_directory(project->projectDirectory);
    // }

    savePath << PATH_SEP << project->projectName << ".ogreproj";

    project->projectFilePath = savePath.str();



    // if (std::filesystem::exists(project->projectDirectory)) {

    // } else {
    // }

    // TODO: make this procedural for 

    // TODO: independent maps + full map
    // TODO: add button for removing an image
    // TODO: add button for inserting image
    //
    // TODO: info.csv
    // column 1: VehicleName
    // column 2: VehicleClass suv,pickup,sedan
    // column 3: VehicleVIN,
    //
    //
    //

    if (std::filesystem::exists(project->projectDirectory)) 
    {

    } else {
        std::filesystem::create_directory(project->projectDirectory);

        for (size_t i = 0; i < project->measurements.size(); i++) 
        {
            std::ostringstream measurementFolder;

            measurementFolder << 
                project->projectDirectory << PATH_SEP << 
                occSizePathName[project->measurements[i].occupantSize];

            std::filesystem::create_directory(measurementFolder.str());

            measurementFolder << PATH_SEP << "Independent_Maps";

            std::filesystem::create_directory(measurementFolder.str());

        }

    }


    if (copyFiles) {
        // TODO: implement
        /*copyImageFiles();*/
    }

    // std::ofstream _50M_path;
    // _50M_path << savePath << "50M";

    // std::filesystem::create_directory(_50M_path.str());

    // std::ofstream _05F_path;
    // _05F_path << savePath << "05F";

    // std::filesystem::create_directory(_05F_path.str());


    saveProject(*project, project->projectFilePath);
}

void NewVehicleProjectWindow::chooseParentDirectory() {

	NFD::UniquePath outPath;

	nfdresult_t result = NFD::PickFolder(outPath);
	if (result == NFD_OKAY) {
        project->parentDirectory = outPath.get();
	}
	else if (result == NFD_CANCEL) {

	}
	else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
	}

	return;
}

NFD::UniquePathSet NewVehicleProjectWindow::chooseImageFiles(){

    // auto-freeing memory
    NFD::UniquePathSet outPaths;

    // prepare filters for the dialog
    nfdfilteritem_t filterItem[1] = {{"Image Files", "jpg,jpeg,png"}};

    // show the dialog
    nfdresult_t result = NFD::OpenDialogMultiple(outPaths, filterItem, 1);
    if (result == NFD_OKAY) {
        std::cout << "Success!" << std::endl;

        nfdpathsetsize_t numPaths;
        NFD::PathSet::Count(outPaths, numPaths);

        for (nfdpathsetsize_t i = 0; i < numPaths; ++i) {

            NFD::UniquePathSetPath path;
            NFD::PathSet::GetPath(outPaths, i, path);
            std::cout << "Path " << i << ": " << path.get() << std::endl;
        }
    } else if (result == NFD_CANCEL) {
        std::cout << "User pressed cancel." << std::endl;
    } else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
    }

    return outPaths;
}
