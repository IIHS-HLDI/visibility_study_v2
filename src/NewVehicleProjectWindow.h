#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>
#include <nfd.hpp>
#include <nfd.h>
#include <opencv2/opencv.hpp>

#include <regex>
#include <filesystem>
#include <algorithm>

//#include <Windows.h>

#include "Platform.h"
#include "ApplicationState.h"
#include "VehicleProject.h"
#include "VehicleInfo.h"
#include "Preview.h"

// TODO: rename to ProjectGenerator
class NewVehicleProjectWindow
{
private:
    std::shared_ptr<ApplicationState> appState;
    std::shared_ptr<VehicleProject> project;
    //VehicleProject* project;
    std::string projectDirectory;
	//VehicleInfo vehicleInfo;
    std::vector<Preview> imagePreviews;

    bool copyFiles = false;
    
    // std::vector<std::string> enumerateImageFiles(std::string path);
public:

	const char* popupName = "New Project";
	NewVehicleProjectWindow(std::shared_ptr<ApplicationState> appState);
    void copyImageFiles();
    void reset();
	void layout();
	void chooseParentDirectory();
    NFD::UniquePathSet chooseImageFiles();
    void initializeProject();

};

