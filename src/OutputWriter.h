#pragma once

#include <memory>

#include <string>

#include <filesystem>

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


#include "Platform.h"
#include "ApplicationState.h"

class OutputWriter {
private:
    std::shared_ptr<ApplicationState> appState;

public:

    OutputWriter(std::shared_ptr<ApplicationState> appState);


    static void writeOutputFiles(VehicleProject project, std::shared_ptr<ApplicationState> appState);
    static void writeOutputFileAs(VehicleProject project, std::shared_ptr<ApplicationState> appState, std::string filepath);
};
