#pragma once

#include <memory>
#include <thread>
#include <future>
#include <vector>
#include <string>
#include <filesystem>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <nfd.hpp>
#include <nfd.h>

#include "SubWindow.h"
#include "ApplicationState.h"
#include "VehicleProject.h"
/*#include "ImagePreview.h"*/
#include "Preview.h"

#include <opencv2/opencv.hpp>

// #include "PreviewLoader.h"

class ProjectExplorer : public SubWindow {
private:
    //PreviewLoader& previewLoader;

    std::shared_ptr<MeasurementConfig> currentConfig;
    std::vector<Preview> imagePreviews;
    std::vector<std::jthread> imagePreviewLoaders;

public:

    using SubWindow::SubWindow;

    /*std::future<std::vector<Preview>> loadImagePreviews(MeasurementConfig& measurementConfig);*/

    /*static Preview loadImagePreview(std::string imageFilepath);*/

    void nameChangePopup();

    void reloadImagePreviews();

    NFD::UniquePathSet chooseImageFiles();

    void loadCurrentProject();

    bool init() override;

    void layout() override;


};


