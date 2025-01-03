#pragma once

#include <memory>

#include "ApplicationState.h"

class PaintTool {

private:
    std::shared_ptr<ApplicationState> appState;

public:
    PaintTool(std::shared_ptr<ApplicationState> appState);

    /*void addPoint();*/

    /*void addPointAtClick();*/


};
