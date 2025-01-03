#pragma once

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>
#include <filesystem>
#include <boxer/boxer.h>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

#include <opencv2/opencv.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Platform.h"
#include "VehicleInfo.h"
#include "GridConfiguration.h"

typedef struct _Point {

    glm::vec2 position;

    template<class Archive>
    void serialize(Archive & archive) {
        archive(CEREAL_NVP(position.x), CEREAL_NVP(position.y));
    }

    float distance(glm::vec2 u, glm::vec2 v) {
        glm::vec2 diffVec = u - v;

        return sqrt(pow(diffVec.x, 2) + pow(diffVec.y, 2));
        /*return glm::abs(u-v);*/

    }

    cv::Point2f toCV() {
        return cv::Point2f(position.x, position.y);
    }

    bool grab(glm::vec2 testPosition, float viewRadius) {
        return (distance(testPosition, position) < viewRadius);
    }


} Point;


typedef struct _BezierPoint {
    glm::vec2 back = glm::vec2(0, 0);
    glm::vec2 center = glm::vec2(0,0);
    glm::vec2 front = glm::vec2(0,0);

    bool symmetric = true;
    bool frontDisable = false;
    bool backDisable = false;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(back.x), CEREAL_NVP(back.y), 
                CEREAL_NVP(center.x), CEREAL_NVP(center.y), 
                CEREAL_NVP(front.x), CEREAL_NVP(front.y), 
                CEREAL_NVP(symmetric), 
                CEREAL_NVP(frontDisable), CEREAL_NVP(backDisable));
    }

    float distance(glm::vec2 u, glm::vec2 v) {
        glm::vec2 diffVec = u - v;

        return sqrt(pow(diffVec.x, 2) + pow(diffVec.y, 2));
        /*return glm::abs(u-v);*/

    }

    bool grabCenter(glm::vec2 testPosition, float viewRadius) {
        return (distance(testPosition, center) < viewRadius);
    }

    bool grabFront(glm::vec2 testPosition, float viewRadius) {
        return (distance(testPosition, front) < viewRadius);
    }

    bool grabBack(glm::vec2 testPosition, float viewRadius) {
        return (distance(testPosition, back) < viewRadius);
    }

    void setAsymmetric() {
        symmetric = false;
    }

    void setFront(glm::vec2 newPosition) {
        front = newPosition; 

        if (symmetric) {
            back = center + (center - front);
        }
    }

    void setBack(glm::vec2 newPosition) {
        back = newPosition;

        if (symmetric) {
            front = center + (center - back);
        }
    }

    void setCenter(glm::vec2 newPosition) {
        glm::vec2 backWRTCenter = back - center;
        glm::vec2 frontWRTCenter = front - center;

        center = newPosition;
        back = center + backWRTCenter;
        front = center + frontWRTCenter;
    }

} BezierPoint;

typedef struct _BezierSpline {

    std::vector<BezierPoint> bezierPoints;

    void reset() {
        bezierPoints.clear();
    }

    int grabCenter(glm::vec2 testPosition, float viewRadius) {
        for (int i = 0; i < bezierPoints.size(); i++) {
            if(bezierPoints[i].grabCenter(testPosition, viewRadius)) {
                return i;
            }
        }
        return -1;
    }

    int grabFront(glm::vec2 testPosition, float viewRadius) {
        float scalar = 2.0;
        for (int i = 0; i < bezierPoints.size(); i++) {
            if(bezierPoints[i].grabFront(testPosition, viewRadius*scalar)) {
                return i;
            }
        }
        return -1;
    }

    int grabBack(glm::vec2 testPosition, float viewRadius) {
        float scalar = 1.5;
        for (int i = 0; i < bezierPoints.size(); i++) {
            if(bezierPoints[i].grabBack(testPosition, viewRadius*scalar)) {
                return i;
            }
        }
        return -1;
    }

    void eraseAt(int i) {
        bezierPoints.erase(bezierPoints.begin()+i);
    }

    std::vector<glm::vec2> 
    interpolateSegment(BezierPoint pt1, BezierPoint pt2) {
        glm::vec2 A = pt1.center;
        glm::vec2 B = pt1.front;
        glm::vec2 C = pt2.back;
        glm::vec2 D = pt2.center;

        glm::vec2 P;
        glm::vec2 last = A;

        std::vector<glm::vec2> segmentPoints;

        static double outputStepsize = 0.1;

        for (double t = 0; t <= 1.0; t += outputStepsize) {
            P.x = pow((1-t), 3)*A.x 
                + 3*t*pow((1-t), 2) * B.x 
                + 3* (1-t) * pow(t, 2) * C.x
                + pow(t, 3) * D.x;

            P.y = pow((1-t), 3)*A.y 
                + 3*t*pow((1-t), 2) * B.y 
                + 3* (1-t) * pow(t, 2) * C.y
                + pow(t, 3) * D.y;

            segmentPoints.push_back(P);
        }

        return segmentPoints;

    }

    std::vector<glm::vec2>
    interpolateSpline() {
        std::vector<glm::vec2> outputPoints;

        for (size_t i = 1; i < bezierPoints.size(); i++) {
            std::vector<glm::vec2> segmentPoints = 
                interpolateSegment(bezierPoints[i-1], bezierPoints[i]);

            outputPoints.insert(
                    outputPoints.end(), 
                    segmentPoints.begin(), 
                    segmentPoints.end());
        }

        return outputPoints;
    }

    template<class Archive>
    void serialize(Archive &archive) {
        bezierPoints.shrink_to_fit();
        archive(CEREAL_NVP(bezierPoints));
    }

} BezierSpline;


typedef struct _Annotation {

    std::vector<BezierSpline> splines;
    std::vector<Point> points;

    std::vector<cv::Point2f> getPoints() {
        std::vector<cv::Point2f> cvPoints(points.size());

        for (size_t i = 0; i < points.size(); i++) {
            cvPoints[i] = cv::Point2f(points[i].position.x,
                    points[i].position.y);
        }

        return cvPoints;
    }

    int grabPoint(glm::vec2 testPosition, float viewRadius) {
        for (size_t i = 0; i < points.size(); i++) {
            if (points[i].grab(testPosition, viewRadius)) {
                return i;
            }
        }

        return -1;
    }
    /*_Annotation() : splines(), points() {}*/

    _Annotation() : splines() {
        splines.push_back({});
    };

    template<class Archive>
    void serialize(Archive &archive) {
        splines.shrink_to_fit();
        points.shrink_to_fit();
        archive(CEREAL_NVP(splines), CEREAL_NVP(points));
    }

} Annotation;



typedef struct _MeasurementImage {

    std::string imageFilepath = "";

    float height;
    float width;

    double yawAngle = 0.0;

    Annotation perspective;
    Annotation orthogonal;

    GridConfiguration gridConfig;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(imageFilepath), 
                CEREAL_NVP(perspective),
                CEREAL_NVP(height),
                CEREAL_NVP(width),
                CEREAL_NVP(yawAngle),
                CEREAL_NVP(gridConfig));
    }

    std::vector<cv::Point2f> sceneToUVCoords(std::vector<cv::Point2f> scenePoints) {
        std::vector<cv::Point2f> uvPoints(scenePoints.size());

        for (unsigned int i = 0; i < scenePoints.size(); i++) {
            uvPoints[i].x = scenePoints[i].x + (width/2.0f);
            uvPoints[i].y = -1*(scenePoints[i].y - (height/2.0f));
        }

        return uvPoints;
    }


} MeasurementImage;

static const char* occSizeStrings[4] = {
    "None",
    "5th Female",
    "50th Male",
    "95th Male",
};

static const char* occSizePathName[4] = {
    "NONE",
    "05F",
    "50M",
    "95M",
};

enum OccupantSize {
    OS_NONE,
    FEMALE_5TH,
    MALE_50TH,
    MALE_95TH,
};

static const char* seatTrackPositionStrings[4] = {
    "None",
    "Forward",
    "Middle",
    "Rearward",
};

enum SeatTrackPosition {
    SEAT_TRACK_NONE,
    SEAT_TRACK_FORWARD,
    SEAT_TRACK_MIDDLE,
    SEAT_TRACK_REARWARD,
};

static const char* seatHeightStrings[4] = {
    "None",
    "Down",
    "Middle",
    "Up",
};

enum SeatHeight {
    SEAT_HEIGHT_NONE,
    SEAT_HEIGHT_LOWEST,
    SEAT_HEIGHT_MIDDLE,
    SEAT_HEIGHT_HIGHEST,
};

typedef struct _MeasurementOffsets {

    // Offsets measured from front left corner of the vehicle
    glm::dvec3 rigPosition = glm::dvec3(0,0,0);

    double rigRadius = 0.0;
    double lensOffset = 0.0;
    double lensHeightOffset = 0.0;

    template<class Archive>
    void serialize(Archive &archive){
        archive(CEREAL_NVP(rigPosition.x),
                CEREAL_NVP(rigPosition.y),
                CEREAL_NVP(rigPosition.z), 
                CEREAL_NVP(rigRadius), 
                CEREAL_NVP(lensOffset), 
                CEREAL_NVP(lensHeightOffset));
    }

} MeasurementOffsets;

typedef struct _MeasurementConfig {
    OccupantSize occupantSize = OccupantSize::OS_NONE;
    SeatTrackPosition seatTrackPosition = SeatTrackPosition::SEAT_TRACK_NONE;
    SeatHeight seatHeight = SeatHeight::SEAT_HEIGHT_NONE;

    MeasurementOffsets measurementOffsets;

    std::vector<MeasurementImage> images;

    template<class Archive>
    void serialize(Archive &archive) {
        images.shrink_to_fit();
        archive(CEREAL_NVP(occupantSize), 
                CEREAL_NVP(seatTrackPosition), 
                CEREAL_NVP(seatHeight), 
                CEREAL_NVP(measurementOffsets),
                CEREAL_NVP(images));
    }

    void addImage(MeasurementImage image) {
        this->images.push_back(image);
    }

    void deleteImage(size_t i) {
        this->images.erase(this->images.begin() + i);
    }



} MeasurementConfig;

typedef struct _VehicleProject {
    std::string parentDirectory = "";
    std::string projectDirectory = "";
    std::string projectFilePath = "";
    std::string projectName = "";

    std::vector<MeasurementConfig> measurements;
    
    VehicleInfo vehicleInfo;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(parentDirectory),
                CEREAL_NVP(projectDirectory), 
                CEREAL_NVP(projectFilePath),
                CEREAL_NVP(projectName),
                CEREAL_NVP(measurements), CEREAL_NVP(vehicleInfo));
    }

    std::string getProjectName() {

        std::ostringstream projectNameStream;

        // Build project name from vehicle year, make, model, and details
        projectNameStream <<
            this->vehicleInfo.year << "_" <<
            vehicleMakeStrings[this->vehicleInfo.make] << "_" <<
            this->vehicleInfo.model << "_" <<
            this->vehicleInfo.details;

        // Replace spaces with underscores
        static std::regex space_re(" +");
        std::string result = std::regex_replace(projectNameStream.str(), space_re, "_");

        return result;
    }

    void updateName() {
        std::string oldName = this->projectName;

        this->projectName = this->getProjectName();

        // Move Files
        std::ostringstream newProjectDir;
        newProjectDir << parentDirectory
            << PATH_SEP 
            << this->projectName;

        std::filesystem::rename(this->projectDirectory, newProjectDir.str());
        this->projectDirectory = newProjectDir.str();


        std::ostringstream oldProjectFilePath;
        oldProjectFilePath << this->projectDirectory
            << PATH_SEP
            << oldName << ".ogreproj";

        std::ostringstream newProjectFilePath;
        newProjectFilePath << this->projectDirectory
            << PATH_SEP
            << this->projectName << ".ogreproj";


        std::filesystem::rename(oldProjectFilePath.str(), newProjectFilePath.str());

        this->projectFilePath = newProjectFilePath.str();




    }

    _VehicleProject saveAs(std::string parentDirectory) {
        _VehicleProject projectToSaveAs = *this;

        projectToSaveAs.parentDirectory = parentDirectory;

        std::filesystem::create_directory(projectToSaveAs.projectDirectory);

        std::ostringstream savePath;

        savePath << projectToSaveAs.parentDirectory << PATH_SEP <<
            projectToSaveAs.projectName;

        projectToSaveAs.projectDirectory = savePath.str();


        // if (std::filesystem::exists(project->projectDirectory)) {
        // } else {
        //     std::filesystem::create_directory(project->projectDirectory);
        // }

        savePath << PATH_SEP << projectToSaveAs.projectName << ".ogreproj";

        projectToSaveAs.projectFilePath = savePath.str();



        // if (std::filesystem::exists(project->projectDirectory)) {

        // } else {
        // }

        // TODO: make this procedural for 

        // TODO: independent maps + full map
        // TODO: add button for removing an image
        // TODO: add button for inserting image
        //
        // TODO: info.csv
        // column 1: VehicleName
        // column 2: VehicleClass suv,pickup,sedan
        // column 3: VehicleVIN,
        //
        //
        //

        if (std::filesystem::exists(projectToSaveAs.projectDirectory))
        {

        }
        else {
            std::filesystem::create_directory(projectToSaveAs.projectDirectory);

            for (size_t i = 0; i < projectToSaveAs.measurements.size(); i++)
            {
                std::ostringstream measurementFolder;

                measurementFolder <<
                    projectToSaveAs.projectDirectory << PATH_SEP <<
                    occSizePathName[projectToSaveAs.measurements[i].occupantSize];

                std::filesystem::create_directory(measurementFolder.str());

                measurementFolder << PATH_SEP << "Independent_Maps";

                std::filesystem::create_directory(measurementFolder.str());

            }

        }


        //if (copyFiles) {
            // TODO: implement
            /*copyImageFiles();*/
        //}

        // std::ofstream _50M_path;
        // _50M_path << savePath << "50M";

        // std::filesystem::create_directory(_50M_path.str());

        // std::ofstream _05F_path;
        // _05F_path << savePath << "05F";

        // std::filesystem::create_directory(_05F_path.str());


        //saveProject(projectToSaveAs, projectToSaveAs.projectFilePath);

        return projectToSaveAs;


    }

    void initialize() {

    }

    // void initialize(std::string savePath) {
    //     project->projectName = getProjectName();

    //     if (!std::filesystem::exists(project->projectDirectory))
    //     {
    //         std::filesystem::create_directory(project->projectDirectory);

    //         for (MeasurementConfig& measurement : measurement) {
    //             measurement.initialize();
    //         }

    //         for (size_t i = 0; i < project->measurements.size(); i++) {
    //             std::ostringstream
    //         }
    //     }


    //     saveProject(this, project->projectFilePath);
    // }

    // call only wher creating it not when loading/unloading

} VehicleProject;

std::shared_ptr<VehicleProject> loadProject(std::string inputFile);

void saveProject(VehicleProject project, std::string saveFile);
