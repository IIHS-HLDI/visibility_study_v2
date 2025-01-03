#pragma once

#include <memory>
#include <optional>
#include <chrono>

#include <opencv2/opencv.hpp>


#include "CameraProfile.h"

#include "VehicleProject.h"
#include "BuiltinProfiles.h"
#include "FileSelectors.h"


enum ProjectionMode : int {
    MARKERLESS,
    GRID
};

static const char* projectionModeStrings[2] = {
	"Markerless",
	"Grid"
};

enum Tool : int {
    BEZIER_TOOL,
    PAINT_TOOL,
    GRID_TOOL,
};

static const char * bezierModeString[3] = {
    "Add",
    "Edit",
    "Delete"
};

enum BezierMode : int {
    BEZIER_ADD,
    BEZIER_EDIT,
    BEZIER_DELETE
};

static const char * paintModeString[2] = {
    "Add",
    "Delete"
};

enum PaintMode : int {
    PAINT_ADD,
    PAINT_DELETE
};

static const char * toolString[2] = {
    "Bezier Tool",
    "Paint Tool",
};

static const char * toolStringGrid[3] = {
    "Bezier Tool",
    "Paint Tool",
    "Grid Mode",
};

typedef struct _UIState {
    int placeholder;
} UIState;

typedef struct _BezierToolState {
    BezierMode mode = BEZIER_ADD;
    int selectedSpline = 0;
} BezierToolState;

typedef struct _PaintToolState {
    PaintMode mode = PAINT_ADD;
} PaintToolState;






typedef struct _MarkerlessConfiguration {

    int selectedDefaultProfile = 0;

    std::shared_ptr<CameraProfile> cameraProfile;

    float intersectHeight;

    bool handleDistortion = false;

    _MarkerlessConfiguration() {
        cameraProfile = std::make_shared<CameraProfile>(Builtin::CameraProfiles[selectedDefaultProfile]);
    };

} MarkerlessConfiguration;


typedef struct _ApplicationState {
    bool projectLoaded = false;

    std::shared_ptr<VehicleProject> currentProject = 
        std::make_shared<VehicleProject>();

    std::vector<VehicleProject> historyBuffer;
    VehicleProject copy;
    std::vector<VehicleProject> redoBuffer;

    Tool currentTool = BEZIER_TOOL;
    int selectedMeasurementConfig = -1;
    int selectedImage = -1;
    bool markReload = false;
    bool shouldClose = false;
    bool unsavedChanges = false;

    ImVec2 canvasPosition;

    BezierToolState bezierToolState;
    PaintToolState paintToolState;
    bool commandQueued = false;

    ProjectionMode projectionMode;
    GridConfiguration gridConfig;
    MarkerlessConfiguration markerlessConfig;

    bool orthoShowOrigin = true;
    bool orthoShowCamera = true;
    bool orthoShowGrid = true;


    float splineOpacity = 1.0f;

    float pointsOpacity = 1.0f;

    float gridOpacity = 1.0f;

    std::chrono::system_clock::time_point lastSave; // Store the last saved time

    /**
    * Push a command to the undo/redo system
    * 
    * Run this before an undoable change is made to the Project
    */
    void queueCommand() {
        redoBuffer.clear();

        if (!commandQueued) {
            commandQueued = true;
            copy = *currentProject;
        }
    }

    /**
     * Report change to the undo/redo system
     *
     * Run this after an undoable change is made to the project
     */
    void reportUpdate() {
        if (commandQueued) {
            historyBuffer.push_back(copy);
			unsavedChanges = true;
        }
        commandQueued = false;
        markReload = true;
    }

    /**
     * Check if the a project, measurement config, and image is currently loaded
     */
    bool isProjectAndImageLoaded() {
        return this->projectLoaded && 
            this->selectedMeasurementConfig >= 0 && 
            this->selectedImage >= 0;
    }

    /**
     * Check if a measurement configuration is loaded
     */
    bool isMeasurementConfigLoaded() {
        return this->projectLoaded &&
            this->selectedMeasurementConfig >= 0;
    }


    MeasurementImage getCurrentImage() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig]
            .images[selectedImage];
    }

    /**
     * Get the current MeasurementImage
     */
    MeasurementImage& getCurrentImageRef() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig]
            .images[selectedImage];
    }

    /**
     * Get the current measurement config
     */
    MeasurementConfig getCurrentMeasurementConfig() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig];
    }

    /**
     * Get the current perspective annotation
     */
    Annotation& getCurrentPerspectiveAnnotation() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig]
            .images[selectedImage]
            .perspective;
    }

    /**
     * Get the currently active bezier spline
     */
    BezierSpline& getCurrentBezierSpline() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig]
            .images[selectedImage]
            .perspective.splines[this->bezierToolState.selectedSpline];
    }


    /**
     * Get current grid config
     */
    GridConfiguration& getCurrentGridConfig() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig]
            .images[selectedImage]
            .gridConfig;
    }

    std::vector<MeasurementImage>& getCurrentMeasurementConfigImages() {
        return this
            ->currentProject
            ->measurements[selectedMeasurementConfig].images;
    }

    std::string getMeasurementTypeString() {
        if (projectionMode == GRID) {
            return "grid";
        }
        else if (projectionMode == MARKERLESS) {
            return "markerless";
        }
        return "";
    }

	/**
	 * Reverts the last action performed by a user
	 *
	 */
    void undo() {
        if (!this->historyBuffer.empty()) {
            std::shared_ptr<VehicleProject> prevState = std::make_shared<VehicleProject>();

            // get the last element in the history buffer
            *prevState = this->historyBuffer.back();

            // load current project into redo buffer
            redoBuffer.push_back(*(this->currentProject));

            // update current project with previous state
            this->currentProject.swap(prevState);

            this->historyBuffer.pop_back();

        }
    }

	/**
	 * Reverts the last undo performed by a user
	 *
	 */
    void redo() {
        if (!this->redoBuffer.empty()) {
            std::shared_ptr<VehicleProject> redoState = std::make_shared<VehicleProject>();


            // get last element in the redo buffer
            *redoState = this->redoBuffer.back();

            // load current state into history buffer
            historyBuffer.push_back(*(this->currentProject));

            // updatecurrent project with redo state
            this->currentProject.swap(redoState);

            this->redoBuffer.pop_back();
        }
    }
    
    void openProject() {
        if (auto projectFilepath = chooseProjectFile()) {
			std::shared_ptr<VehicleProject> project = loadProject(*projectFilepath);

			this->currentProject.swap(project);
			this->projectLoaded = true;
			this->historyBuffer.clear();

			this->selectedMeasurementConfig = -1;
			this->selectedImage = -1;
			this->commandQueued = false;
			this->markReload = false;
			this->unsavedChanges = false;
			this->shouldClose = false;

			this->bezierToolState.selectedSpline = 0;
        }
    }

    void reportChanges() {
        this->unsavedChanges = true;
    }

    void saveCurrentProject() {
        saveProject(*(this->currentProject),
            this->currentProject->projectFilePath);

        this->lastSave = std::chrono::system_clock::now();
        this->unsavedChanges = false;

    }

} ApplicationState;


