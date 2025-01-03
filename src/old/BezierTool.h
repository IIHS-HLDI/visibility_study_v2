#pragma once

#include <memory>

#include "ApplicationState.h"

class BezierTool {
private:
    std::shared_ptr<ApplicationState> appState;
public:
    BezierTool(std::shared_ptr<ApplicationState> appState);

};
