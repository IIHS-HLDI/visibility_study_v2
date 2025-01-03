#include "ProjectExplorer.h"

bool ProjectExplorer::init() {
    return true;
}

// void ProjectExplorer::removeImagePreview(unsigned int i) {
//     imagePreviews.
//     size_t oldSize = imagePreviews.size();
//     size_t newSize = currentConfig.images.size();

//     std::vector<Preview> oldPreviews = imagePreviews;

//     for (size_t i = 0; i < newSize; i++) {
//     }

//     imagePreviews.resize(newSize);
// }
//
void ProjectExplorer::reloadImagePreviews() {

    auto currentConfig = appState->getCurrentMeasurementConfig();
    //imagePreviews.clear();
    imagePreviews.resize(currentConfig.images.size());

    for (int i = 0; i < currentConfig.images.size(); i++) {
        if (imagePreviews[i].isLoaded()) {
            imagePreviews[i].load(currentConfig.images[i].imageFilepath, 0.04);
        }
    }
}



void ProjectExplorer::nameChangePopup() {
    if (ImGui::BeginPopupModal("Name Change!")) {
        ImGui::Text("You have modified a value that will change the project name.");
        if (ImGui::Button("Cancel")) {
        }

        if (ImGui::Button("Update Name and Modify Filesystem")) {
        }

        if (ImGui::Button("Just update name")) {


            ImGui::CloseCurrentPopup();
        }

        // ImGui::SameLine();
        // if (ImGui::Button("Quit without saving")) {
        //     appState->shouldClose = true;
        //     ImGui::CloseCurrentPopup();
        //     /*glfwTerminate();*/
        // }
        // ImGui::SameLine();
        // if (ImGui::Button("Save and quit")) {
        //     saveProject(*(appState->currentProject), appState->currentProject->projectFilePath);
        //     appState->shouldClose = true;
        //     ImGui::CloseCurrentPopup();
        // }
        ImGui::EndPopup();
    }
}

void ProjectExplorer::layout() {

    ImGui::Begin(windowName.c_str());
    if (appState->projectLoaded) {
        //TODO: increase fontsise and bold here
        ImGui::Text("Project: %s", appState->currentProject->projectName.c_str());
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Vehicle Information")) {
            static uint16_t u16_one = 1;

            static bool nameChange = false;
            

            if (ImGui::InputScalar("Year", ImGuiDataType_U16, (uint16_t*) &(appState->currentProject->vehicleInfo.year), &u16_one, &u16_one, "%04u")){
                nameChange = true;
            }

            /*if (ImGui::ComboAutoSelect("Make", vehicleMakeData)) {*/
            /*    appState->currentProject->vehicleInfo.make = (VehicleMake) vehicleMakeData.index;*/
            /*}*/
            if (ImGui::Combo(
                "Vehicle Make",
                (int*) &(appState->currentProject->vehicleInfo.make),
                vehicleMakeStrings,
                IM_ARRAYSIZE(vehicleMakeStrings))) {

                nameChange = true;
                // ImGui::OpenPopup("Name Change!");
            }

            if (ImGui::InputText("Model", 
                    (appState->currentProject->vehicleInfo.model),
                    MAX_VEHICLE_INFO_CHAR_BUF)) {
                nameChange = true;
                // ImGui::OpenPopup("Name Change!");
            }

            if (ImGui::InputText("Details", 
                    (appState->currentProject->vehicleInfo.details), 
                    MAX_VEHICLE_INFO_CHAR_BUF)) {
                // ImGui::OpenPopup("Name Change!");
                nameChange = true;
            }

            if (nameChange) {
                ImGui::Text("Update project name and move files?");
                if (ImGui::Button("Yes")) {
                    appState->currentProject->updateName();
                    saveProject(*(appState->currentProject), appState->currentProject->projectFilePath);
                    std::shared_ptr<VehicleProject> project = loadProject(appState->currentProject->projectFilePath);

                    appState->currentProject.swap(project);
                    appState->projectLoaded = true;

                    nameChange = false;
                } 
                ImGui::SameLine();
                if (ImGui::Button("No")) {
                    nameChange = false;
                }
            }

            // nameChangePopup();


            ImGui::Combo(
                "Vehicle Class",
                (int*) &(appState->currentProject->vehicleInfo._class),
                vehicleClassStrings,
                IM_ARRAYSIZE(vehicleClassStrings));

            ImGui::InputText("VIN", appState->currentProject->vehicleInfo.VIN, VIN_LENGTH,
                    ImGuiInputTextFlags_CharsUppercase);


        }

        if(ImGui::CollapsingHeader("Measurement Configurations")){
            static ImVector<int> active_tabs;

            static int next_tab_id = 0;
            if (next_tab_id == 0) {
                for (int i = 0; i < appState->currentProject->measurements.size(); i++) {
                    active_tabs.push_back(next_tab_id++);
                }
            }

            /*static bool show_leading_button = false;*/
            /*static bool show_trailing_button = true;*/
            //ImGui::Checkbox("Show Leading TabItemButton()", &show_leading_button);
            //ImGui::Checkbox("Show Trailing TabItemButton()", &show_trailing_button);

            // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                ImGuiTabBarFlags_Reorderable | 
                ImGuiTabBarFlags_FittingPolicyResizeDown |
            ImGuiTabBarFlags_TabListPopupButton;
        if (ImGui::BeginTabBar("Measurement Configurations", tab_bar_flags)) {

            if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
                active_tabs.push_back(next_tab_id++); // Add new tab
                MeasurementConfig config;
                appState->currentProject->measurements.push_back(config);
            }

            for (int n = 0; n < active_tabs.size(); n++)
            {
                bool open = true;
                char name[256];

                snprintf(name, IM_ARRAYSIZE(name), "%d: %s %s %s", 
                        active_tabs[n],
                        occSizeStrings[appState->currentProject->measurements[active_tabs[n]].occupantSize], 
                        seatTrackPositionStrings[appState->currentProject->measurements[active_tabs[n]].seatTrackPosition],
                        seatHeightStrings[appState->currentProject->measurements[active_tabs[n]].seatHeight]);

                if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
                {
                    ImGui::PushID(active_tabs[n]);
                    MeasurementConfig& currentConfig = (appState->currentProject->measurements[active_tabs[n]]);


                    ImGui::SeparatorText("Representation");
                    ImGui::PushItemWidth(150);
                    ImGui::Combo("Occupant Size", 
                            (int*) &(appState->currentProject->measurements[active_tabs[n]].occupantSize),
                            occSizeStrings,
                            IM_ARRAYSIZE(occSizeStrings));
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(150);
                    ImGui::Combo("Seat Track", 
                            (int*) &(appState->currentProject->measurements[active_tabs[n]].seatTrackPosition),
                            seatTrackPositionStrings,
                            IM_ARRAYSIZE(seatTrackPositionStrings));
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(150);
                    ImGui::Combo("Seat Height", 
                            (int*) &(appState->currentProject->measurements[active_tabs[n]].seatHeight),
                            seatHeightStrings,
                            IM_ARRAYSIZE(seatHeightStrings));
                    ImGui::PopItemWidth();

                    ImGui::SeparatorText("Camera Position Offsets");


                    /*ImGui::Dummy(ImVec2(0.0f, 20.0f));*/
                    ImGui::TextWrapped("Input camera position offsets as positive distance from vehicle front left corner.");

                    // TODO: pictoral diagram input
                    ImGui::PushItemWidth(150);
                    if (ImGui::InputDouble("X Offset (cm) (+)", &(currentConfig.measurementOffsets.rigPosition.x), 1.0, 1.0, "%.3f")) {
                        appState->reportChanges();
                    }
                    ImGui::PopItemWidth();
                    ImGui::PushItemWidth(150);
                    if (ImGui::InputDouble("Y Offset (cm) (-)", &(currentConfig.measurementOffsets.rigPosition.y), 1.0, 1.0, "%.3f")) {
                        appState->reportChanges();
                    }
                    ImGui::PopItemWidth();
                    ImGui::PushItemWidth(150);
                    if (ImGui::InputDouble("Z Offset (cm) (+)", &(currentConfig.measurementOffsets.rigPosition.z), 1.0, 1.0, "%.3f")) {
                        appState->reportChanges();
                    }
                    ImGui::PopItemWidth();
                    // Clip input to positive values
                    currentConfig.measurementOffsets.rigPosition.x = std::max(0.0, currentConfig.measurementOffsets.rigPosition.x);
                    currentConfig.measurementOffsets.rigPosition.y = std::max(0.0, currentConfig.measurementOffsets.rigPosition.y);
                    currentConfig.measurementOffsets.rigPosition.z = std::max(0.0, currentConfig.measurementOffsets.rigPosition.z);

                    ImGui::SeparatorText("Additional Offsets");

                    ImGui::PushItemWidth(150);
                    if (ImGui::InputDouble("Twist Radius (cm)",
                        &(currentConfig
                            .measurementOffsets
                            .rigRadius),
                        1.0, 1.0, "%.3f")) {
                        appState->reportChanges();
                    }
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(150);
                    if (ImGui::InputDouble("Lens Offset (cm)",
                        &(currentConfig
                            .measurementOffsets
                            .lensOffset),
                        1.0, 1.0, "%.3f")) {
                        appState->reportChanges();
                    }
                    ImGui::PopItemWidth();

                    
                    static bool runOnce = false;

                    if (!runOnce) {
                        imagePreviews.resize(currentConfig.images.size());;
                        // imagePreviewLoaders.resize(currentConfig.images.size());;
                        // reloadImagePreviews();
                        runOnce = true;
                    }
                    //


                    ImGui::SeparatorText("Images");

                    if (ImGui::Button("Add images")) {

                        nfdpathsetsize_t numPaths = 0;

                        NFD::UniquePathSet outPaths = chooseImageFiles();

                        NFD::PathSet::Count(outPaths, numPaths);

                        //imagePreviews.resize(numPaths);

                        for (unsigned int i = 0; i < numPaths; i++) {
                            
                            NFD::UniquePathSetPath path;
                            NFD::PathSet::GetPath(outPaths, i, path);

                            // TODO try catch on this
                            cv::Mat currImage = cv::imread(path.get());
                            auto imageSize = currImage.size();

                            currentConfig.addImage({
                                .imageFilepath = path.get(),
                                .height = (float)imageSize.height,
                                .width = (float)imageSize.width,
                                .yawAngle = 0.0
                            });

                            //imagePreviews.resize();


                        }
                        reloadImagePreviews();
                        // currentConfig.images.resize(numPaths);

                        appState->reportChanges();
                        
                    }
                    ImGui::Dummy(ImVec2(0.0f, 20.0f));

                    // TODO: fix adding images from here
                    if (ImGui::TreeNode("Image Files")) {

                        for (unsigned int i = 0; i < currentConfig.images.size(); i++)
                        {
                            // char *buf; 
                            // strcpy(buf, std::filesystem::path(currentConfig->images[i].imageFilepath).filename().c_str());

                            if (ImGui::TreeNode((void*)(intptr_t)i, 
                                        "Image %d: %s", i, 
                                        std::filesystem::path(currentConfig.images[i].imageFilepath)
                                        .filename()
                                        .string()
                                        .c_str())){

                                MeasurementImage& currentImage = currentConfig.images[i];

                                // std::thread loadThread(&Preview::load, &imagePreviews[i], 
                                //             currentConfig.images[i].imageFilepath,
                                //             0.04);


                                if (imagePreviews[i].isLoaded()) {
                                    imagePreviews[i].layout();
                                } else {
                                    imagePreviews[i].load(currentConfig.images[i].imageFilepath, 0.04);
                                }


                                /*if (previewFuture[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {*/
                                    /*previewFuture[i].wait();*/
                                    /*previewFuture[i].get().layout();*/
                                /*}*/
                                /*if (previewFuture[i].valid()) {*/
                                /*}*/
                                ImGui::SameLine(0,30);
                                ImGui::BeginGroup();

                                ImGui::PushItemWidth(300);
                                if (ImGui::InputText("Image Filepath", &(currentConfig.images[i].imageFilepath))) {
                                    appState->reportChanges();
                                }
                                ImGui::PopItemWidth();


                                if (appState->projectionMode == MARKERLESS) {
                                    ImGui::PushItemWidth(100);
                                    if (ImGui::InputDouble("Yaw Angle", &(currentConfig.images[i].yawAngle), 15.0, 1.0, "%0.1f")) {
                                        appState->reportChanges();
                                    }
                                    ImGui::PopItemWidth();
                                } else if (appState->projectionMode == GRID) {
                                    ImGui::PushItemWidth(100);
                                    if (ImGui::InputFloat("Grid width (meters)", &(currentImage.gridConfig.width), 0, 100)) {
                                        appState->reportChanges();
                                    }
                                    if (ImGui::InputFloat("Grid height (meters)", &(currentImage.gridConfig.height), 0, 100)) {
                                        appState->reportChanges();
                                    }

                                    if (ImGui::InputInt("Longitudinal divisions", &(currentImage.gridConfig.divsX), 1, 100)) {
                                        appState->reportChanges();
                                    }
                                    if (ImGui::InputInt("Lateral divisions", &(currentImage.gridConfig.divsY), 1, 100)) {
                                        appState->reportChanges();
                                    }

                                    if (ImGui::InputInt("Longitudinal delineation interval", &(currentImage.gridConfig.markerIntervalX), 1, 100)) {
                                        appState->reportChanges();
                                    }
                                    if (ImGui::InputInt("Lateral delineation interval", &(currentImage.gridConfig.markerIntervalY), 1, 100)) {
                                        appState->reportChanges();
                                    }
                                    ImGui::PopItemWidth();

                                    if (ImGui::Checkbox("Flip X?", &(currentImage.gridConfig.flipX))) {
                                        appState->reportChanges();
                                    }
                                    ImGui::SameLine(0, 20);
                                    if (ImGui::Checkbox("Flip Y?", &(currentImage.gridConfig.flipY))) {
                                        appState->reportChanges();
                                    }


                                    ImGui::PushItemWidth(100);
                                    if (ImGui::InputFloat("Origin x-offset (cm)", &(currentImage.gridConfig.xOffset), 0.0f, 0.0f, "%.3f")) {
                                        appState->reportChanges();
                                    }
                                    if (ImGui::InputFloat("Origin y-offset (cm)", &(currentImage.gridConfig.yOffset), 0.0f, 0.0f, "%.3f")) {
                                        appState->reportChanges();
                                    }
                                    ImGui::PopItemWidth();

                                    // ImGui::SliderFloat("Control Sensitivity", &(app_config->corner_control_sensitivity), 0.0, 10.0f);
                                    ImGui::Button("1", ImVec2(50, 50));
                                    ImVec2 mouse_delta = ImGui::GetMouseDragDelta(0, 0.0f);
                                    ImGuiIO& io = ImGui::GetIO();

                                    if (ImGui::IsItemActive()) {
                                        ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);

                                        currentImage.gridConfig.nudgePosition(1, mouse_delta.x/100, -mouse_delta.y/100);

                                        appState->reportChanges();

                                    }

                                    ImGui::SameLine();
                                    ImGui::Button("2", ImVec2(50, 50));
                                    if (ImGui::IsItemActive()) {
                                        ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
                                        currentImage.gridConfig.nudgePosition(2, mouse_delta.x/100, -mouse_delta.y/100);

                                        appState->reportChanges();
                                    }

                                    ImGui::Button("0", ImVec2(50, 50));
                                    if (ImGui::IsItemActive()) {
                                        ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
                                        currentImage.gridConfig.nudgePosition(0, mouse_delta.x/100, -mouse_delta.y/100);

                                        appState->reportChanges();
                                    }

                                    ImGui::SameLine();
                                    ImGui::Button("3", ImVec2(50, 50));
                                    if (ImGui::IsItemActive()) {
                                        ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
                                        currentImage.gridConfig.nudgePosition(3, mouse_delta.x/100, -mouse_delta.y/100);
                                        
                                        appState->reportChanges();
                                    }
                                }

                                ImGui::Dummy(ImVec2(0.0f, 20.0f));
                                if (ImGui::Button("Delete")) {
                                    ImGui::OpenPopup("Delete Image?");
                                }

                                if (ImGui::BeginPopupModal("Delete Image?")) {
                                    ImGui::TextWrapped("Deleting this image will delete all annotations associated with it." 
                                            " You will not be able to recover the data."
                                            " Are you sure you want to continue?");

                                    if (ImGui::Button("Cancel")) {
                                        ImGui::CloseCurrentPopup();
                                    }
                                    ImGui::SameLine();

                                    if (ImGui::Button("Delete Image")) {

                                        currentConfig.deleteImage(i);
                                        reloadImagePreviews();
                                        // imagePreviews.erase(this->imagePreviews.begin() + i);
                                        // removeImagePreview(i);
                                        //resetImagePreviews();
                                        // TODO: reset image previews
                                        ImGui::CloseCurrentPopup();
                                    }

                                    ImGui::EndPopup();
                                }

                                if(ImGui::Button("Push to View")) {
                                    appState->selectedMeasurementConfig = active_tabs[n];
                                    appState->selectedImage = i;
                                    appState->markReload = true;
                                    appState->bezierToolState.selectedSpline = 0;

                                    //TODO: event dispatch
                                    //
                                    //EventDispatcher -> dispatch imageLoaded
                                }
                                ImGui::EndGroup();
                                ImGui::TreePop();
                            }

                            // free(buf);
                        }
                        ImGui::TreePop();
                    }


                    //ImGui::Text("This is the %s tab!", name);
                    ImGui::EndTabItem();
                    ImGui::PopID();
                }

                if (!open){
                    appState->currentProject->measurements.erase(appState->currentProject->measurements.begin()+active_tabs[n]);
                    active_tabs.erase(active_tabs.Data + n);
                }else{
                    n++;
                }
            }

            ImGui::EndTabBar();
        }
        }
        ImGui::Separator();
    } else {
        ImGui::Text("No project loaded");
    }
    ImGui::End();
}

NFD::UniquePathSet ProjectExplorer::chooseImageFiles(){

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
        /*std::cout << "User pressed cancel." << std::endl;*/
    } else {
        /*std::cout << "Error: " << NFD::GetError() << std::endl;*/
    }

    return outPaths;
}


/*void ProjectExplorer::close() {*/
/*    for (ImagePreview preview : imagePreviews) {*/
/*        preview.close();*/
/*    }*/
/*}*/

