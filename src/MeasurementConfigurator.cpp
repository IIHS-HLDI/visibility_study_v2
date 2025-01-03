#include "MeasurementConfigurator.h"

bool MeasurementConfigurator::init() {
    return true;
}

void MeasurementConfigurator::layout() {
    ImGui::Begin(windowName.c_str());

    ImGui::PushItemWidth(150);
    ImGui::Combo(
            "Projection Mode",
            (int*) &(appState->projectionMode),
            projectionModeStrings,
            IM_ARRAYSIZE(projectionModeStrings));
    ImGui::PopItemWidth();

    if (appState->projectionMode == MARKERLESS) {


        // if (appState->markerlessConfig)


        // if (appState->markerlessConfig.cameraProfile.loaded) {
        //     ImGui::Text("Camera Profile: %s", 
        //             appState->markerlessConfig.cameraProfile.lensDescriptor.c_str());

        //     /*ImGui::SeparatorText("")*/
        ImGui::PushItemWidth(100);
        if(ImGui::InputFloat("Focal Length", &(appState->markerlessConfig.cameraProfile->focalLengthMillimeter), 0, 10)) {
            appState->markReload = true;
        }
        ImGui::PopItemWidth();

        ImGui::Checkbox("Distortion", &(appState->markerlessConfig.handleDistortion));
        // } else {
        //     ImGui::Text("No Camera Profile Loaded");
        // }

        // static int selectedDefaultProfile = 0;

        ImGui::Text("Camera Profile");
        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::BeginCombo("##CameraProfileSelect", Builtin::CameraProfiles[appState->markerlessConfig.selectedDefaultProfile].lensDescriptor.c_str(),ImGuiComboFlags_WidthFitPreview)) {

            for (int n = 0; n < IM_ARRAYSIZE(Builtin::CameraProfiles); n++) {
                const bool isSelected = (appState->markerlessConfig.selectedDefaultProfile == n);

                if (ImGui::Selectable(Builtin::CameraProfiles[n].lensDescriptor.c_str(), isSelected)) {
                    appState->markerlessConfig.selectedDefaultProfile = n;
                    std::shared_ptr<CameraProfile> builtinProfile = std::make_shared<CameraProfile>(Builtin::CameraProfiles[n]);
                    appState->markerlessConfig.cameraProfile.swap(builtinProfile);
                }
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine(0, 30);

        if (ImGui::Button("Load custom profile")) {
            if (auto profilePath = selectCameraProfile()) {
                appState->markerlessConfig.cameraProfile.reset();
                appState->markerlessConfig.cameraProfile = std::make_shared<CameraProfile>();


                appState->markerlessConfig.cameraProfile->loadProfile(*profilePath);
            }
        }


        ImGui::PushItemWidth(100);
        if (ImGui::InputFloat("Intersect Height (cm) (0 is ground)", &(appState->markerlessConfig.intersectHeight), 0.0f, 10.0f, "%.2f")) {
            appState->markReload = true;
        }
        ImGui::PopItemWidth();
        /*ImGui::SliderFloat();*/
    } else if (appState->projectionMode == GRID) {
        /*
        ImGui::PushItemWidth(100);
        ImGui::InputFloat("Grid width (meters)", &(appState->gridConfig.width), 0, 100);
        ImGui::InputFloat("Grid height (meters)", &(appState->gridConfig.height), 0, 100);

        ImGui::InputInt("Longitudinal divisions", &(appState->gridConfig.divsX), 1, 100);
        ImGui::InputInt("Lateral divisions", &(appState->gridConfig.divsY), 1, 100);

        ImGui::InputInt("Longitudinal delineation interval", &(appState->gridConfig.markerIntervalX), 1, 100);
        ImGui::InputInt("Lateral delineation interval", &(appState->gridConfig.markerIntervalY), 1, 100);
        ImGui::PopItemWidth();

        ImGui::Checkbox("Flip X?", &(appState->gridConfig.flipX));
        ImGui::SameLine(0, 20);
        ImGui::Checkbox("Flip Y?", &(appState->gridConfig.flipY));


        ImGui::PushItemWidth(100);
        ImGui::InputFloat("Origin x-offset (cm)", &(appState->gridConfig.xOffset), 0.0f, 0.0f, "%.3f");
        ImGui::InputFloat("Origin y-offset (cm)", &(appState->gridConfig.yOffset), 0.0f, 0.0f, "%.3f");
        ImGui::PopItemWidth();

        // ImGui::SliderFloat("Control Sensitivity", &(app_config->corner_control_sensitivity), 0.0, 10.0f);
        ImGui::Button("1", ImVec2(50, 50));
        ImVec2 mouse_delta = ImGui::GetMouseDragDelta(0, 0.0f);
        ImGuiIO& io = ImGui::GetIO();

        if (ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);

            appState->gridConfig.nudgePosition(0, mouse_delta.x/100, -mouse_delta.y/100);

            // grid_config->grid->nudge_corner(1,
            //     mouse_delta.x * app_config->corner_control_sensitivity / app_config->corner_control_scaler,
            //     -mouse_delta.y * app_config->corner_control_sensitivity / app_config->corner_control_scaler);
        }

        ImGui::SameLine();
        ImGui::Button("2", ImVec2(50, 50));
        if (ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
            appState->gridConfig.nudgePosition(1, mouse_delta.x/100, -mouse_delta.y/100);
            // grid_config->grid->nudge_corner(2,
            //     mouse_delta.x * app_config->corner_control_sensitivity / app_config->corner_control_scaler,
            //     -mouse_delta.y * app_config->corner_control_sensitivity / app_config->corner_control_scaler);
        }

        ImGui::Button("0", ImVec2(50, 50));
        if (ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
            appState->gridConfig.nudgePosition(3, mouse_delta.x/100, -mouse_delta.y/100);
            // grid_config->grid->nudge_corner(0,
            //     mouse_delta.x * app_config->corner_control_sensitivity / app_config->corner_control_scaler,
            //     -mouse_delta.y * app_config->corner_control_sensitivity / app_config->corner_control_scaler);
        }

        ImGui::SameLine();
        ImGui::Button("3", ImVec2(50, 50));
        if (ImGui::IsItemActive()) {
            ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
            appState->gridConfig.nudgePosition(3, mouse_delta.x/100, -mouse_delta.y/100);
            // grid_config->grid->nudge_corner(3,
            //     mouse_delta.x * app_config->corner_control_sensitivity / app_config->corner_control_scaler,
            //     -mouse_delta.y * app_config->corner_control_sensitivity / app_config->corner_control_scaler);
        }
        */

    }

    ImGui::End();
}

std::optional<std::string> 
MeasurementConfigurator::selectCameraProfile() {

    std::string pathStr;
    NFD::UniquePath outPath;

    // prepare filters for the dialog
    nfdfilteritem_t filterItem[1] = {{"OGRE Camera File", "ocp"}};

    // show the dialog
    nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);
    if (result == NFD_OKAY) {
        pathStr = outPath.get();
        //std::cout << "Success!" << std::endl << outPath.get() << std::endl;
    } else if (result == NFD_CANCEL) {
        return std::nullopt;
        //std::cout << "User pressed cancel." << std::endl;
    } else {
        std::cout << "Error: " << NFD::GetError() << std::endl;
        // TODO: replace with error window
        return std::nullopt;
    }

    // NFD::Guard will automatically quit NFD.
    return std::make_optional(pathStr);
    
}


void MeasurementConfigurator::close() {
}
