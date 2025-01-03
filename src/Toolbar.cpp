#include "Toolbar.h"

bool Toolbar::init() {
    return true;
}

void Toolbar::layout() {
// [Option] Automatically update axis based on parent split (inside of doing it via right-click on the toolbar)
    // Pros:
    // - Less user intervention.
    // - Avoid for need for saving the toolbar direction, since it's automatic.
    // Cons: 
    // - This is currently leading to some glitches.
    // - Some docking setup won't return the axis the user would expect.
    const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

    // ImGuiAxis_X = horizontal toolbar
    // ImGuiAxis_Y = vertical toolbar
    ImGuiAxis toolbar_axis = ImGuiAxis_X;

    // 1. We request auto-sizing on one axis
    // Note however this will only affect the toolbar when NOT docked.
    ImVec2 requested_size = (toolbar_axis == ImGuiAxis_X) ? ImVec2(-1.0f, 0.0f) : ImVec2(0.0f, -1.0f);
    ImGui::SetNextWindowSize(requested_size);

    // 2. Specific docking options for toolbars.
    // Currently they add some constraint we ideally wouldn't want, but this is simplifying our first implementation
    ImGuiWindowClass window_class;
    window_class.DockingAllowUnclassed = true;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar; // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a working Undock widget for _NoTabBar to work
    //window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
    if (toolbar_axis == ImGuiAxis_X)
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
    else
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
    ImGui::SetNextWindowClass(&window_class);

    // 3. Begin into the window
    const float font_size = ImGui::GetFontSize();
    const ImVec2 icon_size(ImFloor(font_size * 1.7f), ImFloor(font_size * 1.7f));

    ImGui::Begin(windowName.c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    // 4. Overwrite node size
    ImGuiDockNode* node = ImGui::GetWindowDockNode();
    if (node != NULL)
    {
        // Overwrite size of the node
        ImGuiStyle& style = ImGui::GetStyle();
        const ImGuiAxis toolbar_axis_perp = (ImGuiAxis)(toolbar_axis ^ 1);
        const float TOOLBAR_SIZE_WHEN_DOCKED = style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
        node->WantLockSizeOnce = true;
        node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;

        if (TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
            if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None)
                toolbar_axis = (ImGuiAxis)(node->ParentNode->SplitAxis ^ 1);
    }

    
    // 5. Dummy populate tab bar
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
    //UndockWidget(icon_size, toolbar_axis);
    //
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Select Tool: "); ImGui::SameLine();
    if (appState->projectionMode == GRID) {
        if (ImGui::BeginCombo("##", toolStringGrid[appState->currentTool], ImGuiComboFlags_WidthFitPreview)) {
            for (int n = 0; n < IM_ARRAYSIZE(toolStringGrid); n++)
            {
                const bool is_selected = (appState->currentTool == n);
                if (ImGui::Selectable(toolStringGrid[n], is_selected))
                    appState->currentTool = (Tool) n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
    } else {
        if (ImGui::BeginCombo("##", toolString[appState->currentTool], ImGuiComboFlags_WidthFitPreview)) {
            for (int n = 0; n < IM_ARRAYSIZE(toolString); n++)
            {
                const bool is_selected = (appState->currentTool == n);
                if (ImGui::Selectable(toolString[n], is_selected))
                    appState->currentTool = (Tool) n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    //if(ImGui::Button("Bezier Tool")) {
        //appState->currentTool = 0;
    //}
    //ImGui::SameLine();
    //if(ImGui::Button("Paint Tool")) {
        //appState->currentTool = 1;
    //}


    // context menu
    if (appState->currentTool == BEZIER_TOOL) {
        // ImGui::RadioButton("Add");
        ImGui::SameLine(0, 30);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Bezier Tool Mode:");

        for (int i = 0; i < IM_ARRAYSIZE(bezierModeString);i++) {
            ImGui::SameLine(0, 30);
            ImGui::PushID(i);
            ImGui::RadioButton(bezierModeString[i], (int*) &(appState->bezierToolState.mode), i);
            ImGui::PopID();

        }

        ImGui::SameLine(0,30);

        if (ImGui::Button("(+) New Spline") && appState->isProjectAndImageLoaded()) {
            auto& splines = appState->
                currentProject->
                measurements[appState->selectedMeasurementConfig].
                images[appState->selectedImage].perspective.splines;


            splines.push_back({});

            // select last added spline
            appState->bezierToolState.selectedSpline = splines.size() - 1;
        }

        if (appState->isProjectAndImageLoaded()) {
            ImGui::SameLine();


            auto& splines = appState->
                currentProject->
                measurements[appState->selectedMeasurementConfig].
                images[appState->selectedImage].perspective.splines;

            static int currentItemIdx = 0;

            std::ostringstream comboDisplay;
            comboDisplay << "Spline " << appState->bezierToolState.selectedSpline + 1;

            if (ImGui::BeginCombo("##splineCombo", comboDisplay.str().c_str(), ImGuiComboFlags_WidthFitPreview)) {
                for (int n = 0;
                    n < splines.size();
                    n++) {

                    const bool isSelected =
                        (appState->bezierToolState.selectedSpline == n);
                    // Sprintf
                    //
                    std::ostringstream display;
                    display << "Spline " << n + 1;

                    /*ImGui::SetNextItemAllowOverlap();*/
                    if (ImGui::Selectable(display.str().c_str(), isSelected)) {
                        /*ImGui::SameLine();*/
                        appState->bezierToolState.selectedSpline = n;

                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                appState->queueCommand();
                splines[appState->bezierToolState.selectedSpline].reset();
                appState->reportUpdate();
            }

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::SetTooltip("This will clear all control points from the currently selected spline");
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                /*splines.pop_*/
                appState->queueCommand();
                if (splines.size() == 1) {
                    splines[appState->bezierToolState.selectedSpline].reset();
                }
                else {
                    splines.erase(splines.begin() + appState->bezierToolState.selectedSpline);

                    appState->bezierToolState.selectedSpline = (appState->bezierToolState.selectedSpline + 1) % splines.size();
                }
                appState->reportUpdate();
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::SetTooltip("This will delete the currently selected spline");
            }
        }


    }

    if (appState->currentTool == PAINT_TOOL) {
        ImGui::SameLine(0, 30);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Paint Tool Mode:");

        for (int i = 0; i < IM_ARRAYSIZE(paintModeString);i++) {
            ImGui::SameLine(0, 30);
            ImGui::RadioButton(paintModeString[i], (int*) &(appState->paintToolState.mode), i);

        }

        if (appState->isProjectAndImageLoaded()) {
            ImGui::SameLine(0, 30);
            ImGui::AlignTextToFramePadding();
            if(ImGui::Button("Clear Points")) {
                ImGui::OpenPopup("Clear Points?");
            }

            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(),
                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            if (ImGui::BeginPopupModal("Clear Points?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Are you sure you want to clear all points?\nThis operation cannot be undone.");
                ImGui::Separator();
                if (ImGui::Button("Yes", ImVec2(120, 0))) {
                    MeasurementImage& image = 
                        appState->getCurrentImageRef();

                    appState->queueCommand();
                    image.perspective.points.clear();
                    appState->reportUpdate();

                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }
    //for (int icon_n = 0; icon_n < 10; icon_n++)
    //{
        //char label[32];
        //ImFormatString(label, IM_ARRAYSIZE(label), "%02d", icon_n);
        //if (icon_n > 0 && toolbar_axis == ImGuiAxis_X)
            //ImGui::SameLine();
        //ImGui::Button(label, icon_size);
    //}
    ImGui::PopStyleVar(2);

    // 6. Context-menu to change axis
    if (node == NULL || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
    {
        if (ImGui::BeginPopupContextWindow())
        {
            //ImGui::TextUnformatted("");
            ImGui::Separator();
            if (ImGui::MenuItem("Horizontal", "", (toolbar_axis == ImGuiAxis_X)))
                toolbar_axis = ImGuiAxis_X;
            if (ImGui::MenuItem("Vertical", "", (toolbar_axis == ImGuiAxis_Y)))
                toolbar_axis = ImGuiAxis_Y;
            ImGui::EndPopup();
        }
    }

    ImGui::End();

    // Output user stored data
    //*p_toolbar_axis = toolbar_axis;
}

void Toolbar::close() {
}
