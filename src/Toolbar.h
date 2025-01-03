#pragma once


#include <imgui.h>
#include <imgui_internal.h>

#include "SubWindow.h"
#include "ApplicationState.h"

class Toolbar : public SubWindow {
private:
public:

    using SubWindow::SubWindow;

    bool init() override;

    void layout() override;

    void close() override;
};
