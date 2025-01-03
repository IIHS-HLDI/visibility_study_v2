#pragma once

#include <optional>

#include <imgui.h>
#include <imgui_internal.h>
#include <nfd.hpp>
#include <nfd.h>

#include "SubWindow.h"
#include "ApplicationState.h"

#include "BuiltinProfiles.h"

class MeasurementConfigurator : public SubWindow {
private:
public:

    using SubWindow::SubWindow;

    bool init() override;

    void layout() override;

    std::optional<std::string> selectCameraProfile();

    void close() override;
};
