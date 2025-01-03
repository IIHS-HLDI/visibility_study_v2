#include "VehicleProject.h"

std::shared_ptr<VehicleProject> loadProject(std::string inputFile) {
    std::ifstream vehicleProjectFile(inputFile);

    cereal::JSONInputArchive archive(vehicleProjectFile);

    std::shared_ptr<VehicleProject> project = std::make_shared<VehicleProject>();

    // Handle the case where the project name does not match the computed name
    // if (project->projectName != project->getProjectName()) {

    // }

    archive(*project);
    return project;

}

void saveProject(VehicleProject project, std::string saveFile) {

    std::ofstream vehicleProjectFile(saveFile);

    cereal::JSONOutputArchive archive(vehicleProjectFile);

    archive(project);
}


