#include "OutputWriter.h"

OutputWriter::OutputWriter(std::shared_ptr<ApplicationState> appState) 
    : appState(appState) {}


void OutputWriter::writeOutputFileAs(VehicleProject project, std::shared_ptr<ApplicationState> appState, std::string filepath) {
    std::string measurementType;
    if (appState->projectionMode == GRID) {
        measurementType = "grid";
    } else if (appState->projectionMode == MARKERLESS) {
        measurementType = "markerless";
    }


    // Check if a project and image is loaded
    if (appState->isProjectAndImageLoaded()) {
        auto measurementConfig = appState->getCurrentMeasurementConfig();
        auto image = appState->getCurrentImage();

        // open independent map file stream
        std::ofstream independentMapFile;
        independentMapFile.open(filepath, std::ios::out | std::ios::trunc);

        // Write table header
        independentMapFile << 
            "neck,seat_track,seat_height,series_type,spline_no,point_no,x_meters,y_meters" 
            << std::endl;
        
        // Loop through and write points
        for (size_t i = 0; i < image.orthogonal.points.size(); i++) {

            // check for invalid points
            if (!std::isnan(image.orthogonal.points[i].position.x) && !std::isnan(image.orthogonal.points[i].position.y)) {

                // write data
                independentMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                    << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                    << seatHeightStrings[measurementConfig.seatHeight] << ","
                    << "Point" << ","
                    << -1 << ","
                    << i+1 << ","
                    << image.orthogonal.points[i].position.x/100 << ","
                    << image.orthogonal.points[i].position.y/100 << std::endl;
                    // internally points are stored as cm, so must divide by 100 to get output in meters
            }

        }
        // Loop through and write splines
        for (size_t i = 0; i < image.orthogonal.splines.size(); i++) {
            // interpolate splines
            std::vector<glm::vec2> interpolatedCurve = image.orthogonal.splines[i].interpolateSpline();

            // loop through interpolated points
            for (size_t j = 0; j < interpolatedCurve.size(); j++) {
                // check for invalid points
                if (!std::isnan(interpolatedCurve[j].x) && !std::isnan(interpolatedCurve[j].y)) {

                    // write data
                    independentMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                        << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                        << seatHeightStrings[measurementConfig.seatHeight] << ","
                        << "Spline" << ","
                        << i+1 << ","
                        << j+1 << ","
                        << interpolatedCurve[j].x/100 << ","
                        << interpolatedCurve[j].y/100 << std::endl;
                    // internally points are stored as cm, so must divide by 100 to get output in meters
                }

            }

        }

        // close file
        independentMapFile.close();

    }
}

void OutputWriter::writeOutputFiles(VehicleProject project, std::shared_ptr<ApplicationState> appState) {

    std::string measurementType;
    if (appState->projectionMode == GRID) {
        measurementType = "grid";
    } else if (appState->projectionMode == MARKERLESS) {
        measurementType = "markerless";
    }

    std::ostringstream infoFilePath;
    infoFilePath << project.projectDirectory
        << PATH_SEP
        << "info.csv";

    std::ofstream infoFile;
    infoFile.open(infoFilePath.str(), std::ios::out | std::ios::trunc);

    infoFile << "VehicleName,VehicleClass,VehicleVIN"
        << std::endl;

    infoFile << project.projectName << ","
        << vehicleClassStringsLowercase[project.vehicleInfo._class] << ","
        << project.vehicleInfo.VIN;

    infoFile.close();


    for (MeasurementConfig measurementConfig : 
            project.measurements) {

        std::ostringstream measurementFolder;
        measurementFolder <<
            project.projectDirectory << PATH_SEP <<
            occSizePathName[measurementConfig.occupantSize];

        std::ostringstream combinedMapFilePath;
        combinedMapFilePath 
            << measurementFolder.str() 
            << PATH_SEP 
            << "Map_all_" 
            << appState->getMeasurementTypeString() << ".csv";

        std::ofstream combinedMapFile;
        combinedMapFile.open(combinedMapFilePath.str(), std::ios::out | std::ios::trunc);

        combinedMapFile <<
            "neck,seat_track,seat_height,image_no,image,yaw_angle_deg,series_type,spline_no,point_no,x_meters,y_meters" << 
            std::endl;


        // Write independent maps
        int imageNo = 1;
        for (MeasurementImage image : measurementConfig.images) {
            // Skip output if image is empty
            if (image.perspective.splines.empty() && image.perspective.points.empty()) {
                
                continue;
            }

            std::ostringstream independentMapFilepath;
            independentMapFilepath 
                << measurementFolder.str() 
                << PATH_SEP << "Independent_Maps" 
                << PATH_SEP 
                << "Map_"
                << image.yawAngle 
                << "_deg_"
                << "int_"
                << appState->markerlessConfig.intersectHeight
                << "_"
                << appState->getMeasurementTypeString()
                << "_"
                <<  imageNo
                << ".csv";

            std::ofstream independentMapFile;
            independentMapFile.open(independentMapFilepath.str(), std::ios::out | std::ios::trunc);

            independentMapFile << 
                "neck,seat_track,seat_height,series_type,spline_no,point_no,x_meters,y_meters" 
                << std::endl;
            
            // Write points
            for (size_t i = 0; i < image.orthogonal.points.size(); i++) {

                if (!std::isnan(image.orthogonal.points[i].position.x) && !std::isnan(image.orthogonal.points[i].position.y)) {

                    combinedMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                        << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                        << seatHeightStrings[measurementConfig.seatHeight] << ","
                        << imageNo << ","
                        << std::filesystem::path(image.imageFilepath).filename() << ","
                        << image.yawAngle << ","
                        << "Point" << ","
                        << -1 << ","
                        << i+1 << ","
                        << image.orthogonal.points[i].position.x/100 << ","
                        << image.orthogonal.points[i].position.y/100 << std::endl;

                    independentMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                        << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                        << seatHeightStrings[measurementConfig.seatHeight] << ","
                        << "Point" << ","
                        << -1 << ","
                        << i+1 << ","
                        << image.orthogonal.points[i].position.x/100 << ","
                        << image.orthogonal.points[i].position.y/100 << std::endl;
                }

            }
            // Write splines
            for (size_t i = 0; i < image.orthogonal.splines.size(); i++) {

                std::vector<glm::vec2> interpolatedCurve = image.orthogonal.splines[i].interpolateSpline();
                for (size_t j = 0; j < interpolatedCurve.size(); j++) {
                    if (!std::isnan(interpolatedCurve[j].x) && !std::isnan(interpolatedCurve[j].y)) {

                        combinedMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                            << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                            << seatHeightStrings[measurementConfig.seatHeight] << ","
                            << imageNo << ","
                            << std::filesystem::path(image.imageFilepath).filename() << ","
                            << image.yawAngle << ","
                            << "Spline" << ","
                            << i+1 << ","
                            << j+1 << ","
                            << interpolatedCurve[j].x/100 << ","
                            << interpolatedCurve[j].y/100 << std::endl;

                        independentMapFile << occSizePathName[measurementConfig.occupantSize] << ","
                            << seatTrackPositionStrings[measurementConfig.seatTrackPosition] << ","
                            << seatHeightStrings[measurementConfig.seatHeight] << ","
                            << "Spline" << ","
                            << i+1 << ","
                            << j+1 << ","
                            << interpolatedCurve[j].x/100 << ","
                            << interpolatedCurve[j].y/100 << std::endl;
                    }

                }

            }

            independentMapFile.close();

            imageNo++;
        }

        combinedMapFile.close();
    }
}
