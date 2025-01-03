#include "Canvas.h"

Canvas::Canvas(std::shared_ptr<ApplicationState> appState): 
    appState(appState) {

    zoom = 0.1f;
    pan = ImVec2(0.0f,0.0f);
}

/**
* Transform point from world space to screen space ImVec2->ImVec2
*/
ImVec2 Canvas::transformPoint(ImVec2 input) {
    return ImVec2(
            position.x + input.x * zoom + size.x/2 + pan.x,
            position.y + input.y * -zoom + size.y/2 + pan.y);
}

/**
* Transform glm::vec2 point from world space to ImVec2 screen space
*/
ImVec2 Canvas::transformPoint(glm::vec2 input) {
    return ImVec2(
            position.x + input.x * zoom + size.x/2 + pan.x,
            position.y + input.y * -zoom + size.y/2 + pan.y);
}

/**
* Transform and convert ImVec point from world space to glm::vec2 in screen space
*/
glm::vec2 Canvas::transformPointImToG(ImVec2 input) {
    return glm::vec2(
            position.x + input.x * zoom + size.x/2 + pan.x,
            position.y + input.y * -zoom + size.y/2 + pan.y);
}

/**
* Transform ImVec2 in screen space to world space Imvec2
*/
ImVec2 Canvas::screenToCanvas(ImVec2 input) {

    return ImVec2(
            input.x/zoom + (-(pan.x + position.x + size.x/2))/zoom,
            (size.y/2 + pan.y + position.y)/zoom - input.y/zoom);


            /*position.x + input.x * zoom + size.x/2 + pan.x,*/
            /*position.y + input.y * -zoom + size.y/2 + pan.y);*/
}

/**
* Transform screen space glm::vec2 to world space ImVec2
*/
ImVec2 Canvas::screenToCanvas(glm::vec2 input) {

    return ImVec2(
            input.x/zoom + (-(pan.x + position.x + size.x/2))/zoom,
            (size.y/2 + pan.y + position.y)/zoom - input.y/zoom);
}


/**
* Transform screen space ImVec2 to world space glm::vec2
*/
glm::vec2 Canvas::screenToCanvasImToG(ImVec2 input) {

    return glm::vec2(
            input.x/zoom + (-(pan.x + position.x + size.x/2))/zoom,
            (size.y/2 + pan.y + position.y)/zoom - input.y/zoom);
}


void Canvas::handleBezierToolClick() {

}

// Draw a paint tool point
void Canvas::drawPoint(Point point) {

    int opacity = std::floor(255*appState->pointsOpacity);

    // half of enclosing square side length
    static int viewRadius = 10;

    ImDrawList* drawList = ImGui::GetWindowDrawList();


    // draw enclosing rect
    drawList->AddRect(
            transformPoint(ImVec2(point.position.x - viewRadius, 
                point.position.y - viewRadius)),
            transformPoint(ImVec2(point.position.x + viewRadius,
                point.position.y + viewRadius)),
            IM_COL32(255, 0, 0, opacity));
    
    // Draw crosshair
    drawList->AddLine(
            transformPoint(ImVec2(point.position.x - 3, point.position.y)), 
            transformPoint(ImVec2(point.position.x + 3, point.position.y)), 
            IM_COL32(255, 0, 0, opacity), 1.0f);

    drawList->AddLine(
            transformPoint(ImVec2(point.position.x, point.position.y + 3)), 
            transformPoint(ImVec2(point.position.x, point.position.y - 3)), 
            IM_COL32(255, 0, 0, opacity), 1.0f);

}

/**
* Loop through points
*/
void Canvas::drawPoints(std::vector<Point> points) {
    for (Point point : points) {
        drawPoint(point);
    }
}


/**
* Draw bezier point and handles
*/
void Canvas::drawBezierPoint(BezierPoint bezierPoint) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    int opacity = std::floor(255*appState->splineOpacity);

    // Set Base view radius
    static float baseViewRadius = 6.0;

    // Scale view radius based on zoom level
    float viewRadius = baseViewRadius / zoom;

    float frontScale = 2;
    float backScale = 1.5;
   


    // Draw back control point
    drawList->AddRect(
            transformPoint(ImVec2(bezierPoint.back.x - viewRadius*backScale, 
                bezierPoint.back.y - viewRadius*backScale)),
            transformPoint(ImVec2(bezierPoint.back.x + viewRadius*backScale,
                bezierPoint.back.y + viewRadius*backScale)),
            IM_COL32(0, 255, 0, opacity), 0,0,2.0);

    // Draw front control point
    drawList->AddRect(
            transformPoint(ImVec2(bezierPoint.front.x - viewRadius*frontScale, 
                bezierPoint.front.y - viewRadius*frontScale)),
            transformPoint(ImVec2(bezierPoint.front.x + viewRadius*frontScale,
                bezierPoint.front.y + viewRadius*frontScale)),
            IM_COL32(0, 0, 255, opacity),0,0, 2.0);

    // Draw center square
    drawList->AddRect(
        transformPoint(ImVec2(bezierPoint.center.x - viewRadius,
            bezierPoint.center.y - viewRadius)),
        transformPoint(ImVec2(bezierPoint.center.x + viewRadius,
            bezierPoint.center.y + viewRadius)),
        IM_COL32(255, 0, 0, opacity), 0, 0, 2.0);
    
    // draw back handle
    drawList->AddLine(
            transformPoint(bezierPoint.center),
            transformPoint(bezierPoint.back),
            IM_COL32(0, 255, 0, opacity));

    // draw front handle
    drawList->AddLine(
            transformPoint(bezierPoint.center),
            transformPoint(bezierPoint.front),
            IM_COL32(0, 255, 0, opacity));
}


/**
* Draw spline 
*/
void Canvas::drawSpline(BezierSpline bezierSpline, bool drawHandles) {

    int opacity = std::floor(255*appState->splineOpacity);

    static int numSegs = 100;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    if (!bezierSpline.bezierPoints.empty()) {

        for (int i = 1; i < bezierSpline.bezierPoints.size(); i++) {

            drawList->AddBezierCubic(
                    transformPoint(bezierSpline.bezierPoints[i-1].center),
                    transformPoint(bezierSpline.bezierPoints[i-1].front),
                    transformPoint(bezierSpline.bezierPoints[i].back),
                    transformPoint(bezierSpline.bezierPoints[i].center),
                    IM_COL32(255, 0, 0, opacity), 1.5f);

            if (drawHandles) {
                drawBezierPoint(bezierSpline.bezierPoints[i-1]);
            }
        }

        if (drawHandles) {
            drawBezierPoint(bezierSpline.bezierPoints.back());
        }
    }
}

// TODO: make separate method for no handles

/**
* Loop through splines and draw them
*/
void Canvas::drawSplines(std::vector<BezierSpline> splines, bool drawHandles) {
    for (int i = 0; i < splines.size(); i++){
        if (appState->bezierToolState.selectedSpline == i && drawHandles) {
            drawSpline(splines[i], true);
        } else if (appState->bezierToolState.selectedSpline != i && drawHandles) {
            drawSpline(splines[i], false);
        } else {
            drawSpline(splines[i], false);
        }
    }
}


/**
* Check if mouse is within the bounds of the canvas
*/
bool Canvas::mouseOn(ImVec2 mousePos) {
    return mousePos.x > position.x 
        && mousePos.x < position.x + size.x 
        && mousePos.y > position.y 
        && mousePos.y < position.y + size.y;
}

/**
* Handle zoom in/out event with scroll wheel
*/
void Canvas::handleZoom() {
    static ImGuiIO& io = ImGui::GetIO();
    static ImDrawList* drawList = ImGui::GetWindowDrawList();

    static float oldZoom = 0.1;

    static ImVec2 oldMousePosCanvas;

    if (abs(io.MouseWheel) > 0 &&
            (zoom + io.MouseWheel/10) < 10 && 
            0 < (zoom + io.MouseWheel/10) && mouseOn(io.MousePos)) {

        oldMousePosCanvas = screenToCanvas(io.MousePos);

        float zoomFactor;
        if (ImGui::IsKeyDown(ImGuiKey_RightShift) 
            || ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
            zoomFactor = 1.0f + io.MouseWheel/zoomSlowDivisor;
        } else {
            zoomFactor = 1.0f + io.MouseWheel/zoomFastDivisor;
        }

        oldZoom = zoom;
        zoom *= zoomFactor;


        ImVec2 offset = ImVec2(oldMousePosCanvas.x - screenToCanvas(io.MousePos).x, oldMousePosCanvas.y - screenToCanvas(io.MousePos).y );
        pan.x -= offset.x*zoom;
        pan.y += offset.y*zoom;
    }
}

/**
* Handle pan events with mouse right click
*/
void Canvas::handlePan() {
    static ImGuiIO& io = ImGui::GetIO();
    static ImDrawList* drawList = ImGui::GetWindowDrawList();
    const bool isHovered = ImGui::IsItemHovered(); // Hovered
    const bool isActive = ImGui::IsItemActive();   // Held   
                                                  //
    if (isHovered) {
        appState->canvasPosition = screenToCanvas(io.MousePos);
    }
                                                  
    if (isActive && ImGui::IsMouseDragging(ImGuiMouseButton_Right,-1)) {
        pan.x += io.MouseDelta.x;
        pan.y += io.MouseDelta.y;
    }
}

/**
* Draw currently selected image on canvas
*/
void Canvas::drawImage() {
    static ImGuiIO& io = ImGui::GetIO();
    static ImDrawList* drawList = ImGui::GetWindowDrawList();
    if (appState->projectLoaded && appState->selectedImage>=0) {
        if (appState->markReload) {
            ImageLoadStatus status = image.load(appState->currentProject->measurements[(appState->selectedMeasurementConfig)].images[(appState->selectedImage)].imageFilepath);

            if (status != ImageLoadStatus::GOOD) {
                std::ostringstream errorMessage;
                errorMessage << "The image filepath: \"" <<
                    appState->currentProject->measurements[appState->selectedMeasurementConfig].images[(appState->selectedImage)].imageFilepath <<
                    "\" does not exist.";
                boxer::show(errorMessage.str().c_str(), "ERROR!");
            }

            appState->markReload = false;
        }
        drawList->AddImage((ImTextureID) image.getTexture(), transformPoint(ImVec2(-image.getWidth()/2,-image.getHeight()/2)), transformPoint(ImVec2(image.getWidth()/2, image.getHeight()/2)));
    }
}

// Find a better name for this
void Canvas::drawBase() {
    static ImGuiIO& io = ImGui::GetIO();
    static ImDrawList* drawList = ImGui::GetWindowDrawList();

    size = ImGui::GetContentRegionAvail();

    position = ImGui::GetCursorScreenPos();

    if (size.x < 50.0f) size.x = 50.0f;
    if (size.y < 50.0f) size.y = 50.0f;

    // draw canvas background and border
    ImVec2 endPosition = ImVec2(position.x + size.x, position.y + size.y);

    drawList->AddRectFilled(position, endPosition, IM_COL32(50,50,50,255));

    drawList->AddRect(position, endPosition, IM_COL32(255,255,255,255));

    drawList->PushClipRect(position, endPosition);


    ImGui::InvisibleButton("canvas", size,
            ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

}

/**
* Draw the perspective grid 
*/
void Canvas::drawGrid() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    int gridOpacity = std::floor(255*appState->gridOpacity);

    // get the current measurement image
    MeasurementImage currentImage = appState->getCurrentImage();

    // update matrices
    currentImage.gridConfig.updatePerspectiveMatrices();

    // Draw top left corner
    drawList->AddRect(
            transformPoint(ImVec2(currentImage.gridConfig.topLeft.x-50, currentImage.gridConfig.topLeft.y-50)), 
            transformPoint(ImVec2(currentImage.gridConfig.topLeft.x+50, currentImage.gridConfig.topLeft.y+50)), 
            IM_COL32(255, 0,0,gridOpacity));

    // Draw top right corner
    drawList->AddRect(
            transformPoint(ImVec2(currentImage.gridConfig.topRight.x - 50, currentImage.gridConfig.topRight.y-50)), 
            transformPoint(ImVec2(currentImage.gridConfig.topRight.x+50, currentImage.gridConfig.topRight.y+50)), 
            IM_COL32(255, 0,0,gridOpacity));

    // draw bottom left corner
    drawList->AddRect(
            transformPoint(ImVec2(currentImage.gridConfig.botLeft.x-50, currentImage.gridConfig.botLeft.y-50)), 
            transformPoint(ImVec2(currentImage.gridConfig.botLeft.x+50, currentImage.gridConfig.botLeft.y+50)), 
            IM_COL32(255, 0,0,gridOpacity));

    // draw bottom right corner
    drawList->AddRect(
            transformPoint(ImVec2(currentImage.gridConfig.botRight.x - 50, currentImage.gridConfig.botRight.y-50)), 
            transformPoint(ImVec2(currentImage.gridConfig.botRight.x+50, currentImage.gridConfig.botRight.y+50)), 
            IM_COL32(255, 0,0,gridOpacity));

    // draw right side
    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.botRight.x, currentImage.gridConfig.botRight.y)), 
            transformPoint(ImVec2(currentImage.gridConfig.topRight.x, currentImage.gridConfig.topRight.y)), 
            IM_COL32(255, 0,0,gridOpacity),
            1.0f);
    
    // draw left side
    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.botLeft.x, currentImage.gridConfig.botLeft.y)), 
            transformPoint(ImVec2(currentImage.gridConfig.topLeft.x, currentImage.gridConfig.topLeft.y)), 
            IM_COL32(255, 0,0,gridOpacity),
            1.0f);

    // draw bottom side
    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.botLeft.x, currentImage.gridConfig.botLeft.y)), 
            transformPoint(ImVec2(currentImage.gridConfig.botRight.x, currentImage.gridConfig.botRight.y)), 
            IM_COL32(255, 0,0,gridOpacity),
            1.0f);

    // draw top side
    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.topLeft.x, currentImage.gridConfig.topLeft.y)), 
            transformPoint(ImVec2(currentImage.gridConfig.topRight.x, currentImage.gridConfig.topRight.y)), 
            IM_COL32(255, 0,0,gridOpacity),
            1.0f);



	std::vector<cv::Point2f> src_shape(4);
	std::vector<cv::Point2f> dst_shape(4);
	
    // Define the shape of quadrilateral in perspective view
	src_shape[0] = cv::Point2f(currentImage.gridConfig.botLeft.x, currentImage.gridConfig.botLeft.y);
	src_shape[1] = cv::Point2f(currentImage.gridConfig.topLeft.x, currentImage.gridConfig.topLeft.y);
	src_shape[2] = cv::Point2f(currentImage.gridConfig.topRight.x, currentImage.gridConfig.topRight.y);
	src_shape[3] = cv::Point2f(currentImage.gridConfig.botRight.x, currentImage.gridConfig.botRight.y);
	
    // Define the shape of quadrilateral in orthogonal view (based off of grid dimensions)
	dst_shape[0] = cv::Point2f(currentImage.gridConfig.botLeft.x, currentImage.gridConfig.botLeft.y);
	dst_shape[1] = cv::Point2f(currentImage.gridConfig.botLeft.x, currentImage.gridConfig.botLeft.y + currentImage.gridConfig.height);
	dst_shape[2] = cv::Point2f(currentImage.gridConfig.botLeft.x + currentImage.gridConfig.width, currentImage.gridConfig.botLeft.y + currentImage.gridConfig.height);
	dst_shape[3] = cv::Point2f(currentImage.gridConfig.botLeft.x + currentImage.gridConfig.width, currentImage.gridConfig.botLeft.y);

    // 
	std::vector<cv::Point2f> ortho_corners(4);
	cv::perspectiveTransform(src_shape, ortho_corners, currentImage.gridConfig.pM_inv);

    // Calculate the xOffset based off of the grid width and number of divisions along the x axis
    float xOffset = (currentImage.gridConfig.width*100)/(currentImage.gridConfig.divsX);

    for (int i = 1; i < currentImage.gridConfig.divsX; i++) {
        std::vector<cv::Point2f> points(2);
        std::vector<cv::Point2f> ortho(2);

        points[0] = cv::Point2f(ortho_corners[0].x + xOffset*i, ortho_corners[0].y);
        points[1] = cv::Point2f(ortho_corners[1].x + xOffset*i, ortho_corners[1].y);

        cv::perspectiveTransform(points, ortho, currentImage.gridConfig.pM);

        // Delineate lines based on marker interval
        if ((i) % currentImage.gridConfig.markerIntervalX == 0) {
            drawList->AddLine(
                transformPoint(ImVec2(ortho[0].x, ortho[0].y)),
                transformPoint(ImVec2(ortho[1].x, ortho[1].y)),
                IM_COL32(255,255,0,gridOpacity),
                1.0f);
        } else {
            drawList->AddLine(
                transformPoint(ImVec2(ortho[0].x, ortho[0].y)),
                transformPoint(ImVec2(ortho[1].x, ortho[1].y)),
                IM_COL32(255, 0,0,gridOpacity),
                1.0f);
        }

    }

    // Calculate the yOffset based off of the grid height and number of divisions along the y axis
    float yOffset = (currentImage.gridConfig.height*100)/(currentImage.gridConfig.divsY);

    for (int i = 1; i < currentImage.gridConfig.divsY; i++) {
        std::vector<cv::Point2f> points(2);
        std::vector<cv::Point2f> ortho(2);

        points[0] = cv::Point2f(ortho_corners[0].x, ortho_corners[0].y + yOffset*i);
        points[1] = cv::Point2f(ortho_corners[3].x, ortho_corners[3].y + yOffset*i);

        cv::perspectiveTransform(points, ortho, currentImage.gridConfig.pM);

        if ((i) % currentImage.gridConfig.markerIntervalY == 0) {
            drawList->AddLine(
                transformPoint(ImVec2(ortho[0].x, ortho[0].y)),
                transformPoint(ImVec2(ortho[1].x, ortho[1].y)),
                IM_COL32(255, 255,0,gridOpacity),
                1.0f);
        } else {
            drawList->AddLine(
                transformPoint(ImVec2(ortho[0].x, ortho[0].y)),
                transformPoint(ImVec2(ortho[1].x, ortho[1].y)),
                IM_COL32(255, 0,0,gridOpacity),
                1.0f);
        }
    }



}

void Canvas::handlePaintToolMouseInput() {
    static ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && // only process mouse input if the left mouse button is down
        mouseOn(io.MousePos)  // mouse is on the panel
        && ImGui::IsWindowFocused() &&  // window is focused
        appState->isProjectAndImageLoaded()) // and a project, measurement config, and image is loaded
    {

        Annotation& perspective = appState->getCurrentPerspectiveAnnotation();

        if (appState->paintToolState.mode == PAINT_ADD) {
            appState->queueCommand();
            perspective.points.push_back({ screenToCanvasImToG(io.MousePos) });
            appState->reportUpdate();
        }
        else if (appState->paintToolState.mode == PAINT_DELETE) {
            int idx;
            if (idx = perspective.grabPoint(screenToCanvasImToG(io.MousePos), 6.0 / zoom); idx >= 0) {
                appState->queueCommand();
                perspective.points.erase(perspective.points.begin() + idx);
                appState->reportUpdate();

            }
        }
    }
}

void Canvas::handleBezierToolMouseInput() {
    static ImGuiIO& io = ImGui::GetIO();

    static BezierPoint* activeBezierPoint = nullptr;

    static int pointGrabbed = -1;
    static int pointGrabIndex = 0;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
        mouseOn(io.MousePos) &&
        ImGui::IsWindowFocused() &&
        appState->isProjectAndImageLoaded()) {

        Annotation& perspective = appState->getCurrentPerspectiveAnnotation();

        // Add an empty spline if there are none
        // (There must be at least one spline selected at all times)
        if (perspective.splines.empty()) {
            perspective.splines.push_back({});
        }

        // Get reference to the currently selected spline
        BezierSpline& currentSpline = appState->getCurrentBezierSpline();

        if (appState->bezierToolState.mode == BEZIER_ADD) {
            if (activeBezierPoint == nullptr) { // if there is no point active

                appState->queueCommand();

                currentSpline.bezierPoints.push_back({ // push a new point to currently active spline
                    .back = screenToCanvasImToG(io.MousePos),
                    .center = screenToCanvasImToG(io.MousePos),
                    .front = screenToCanvasImToG(io.MousePos),
                    .symmetric = true,
                    .frontDisable = false,
                    .backDisable = false});

                activeBezierPoint = &currentSpline.bezierPoints.back();

            }
            else {
                // If user presses control, active point should become asymmetric
                if (io.KeyCtrl) {
                    activeBezierPoint->setAsymmetric();
                }
                activeBezierPoint->setFront(screenToCanvasImToG(io.MousePos));
            }
        }
        else if (appState->bezierToolState.mode == BEZIER_EDIT) {
            if (pointGrabbed >= 0 && pointGrabIndex == 0) { // 0 means center point is pointGrabbed
                currentSpline.bezierPoints[pointGrabbed].setCenter(screenToCanvasImToG(io.MousePos));
            }
            else if (pointGrabbed >= 0 && pointGrabIndex == -1) {// -1 means back point is pointGrabbed
                currentSpline.bezierPoints[pointGrabbed].setBack(screenToCanvasImToG(io.MousePos));
            }
            else if (pointGrabbed >= 0 && pointGrabIndex == 1) {// 1 means front point is pointGrabbed
                currentSpline.bezierPoints[pointGrabbed].setFront(screenToCanvasImToG(io.MousePos));
            }
            else {

                if (int center = currentSpline.grabCenter(screenToCanvasImToG(io.MousePos), 6.0 / zoom); center >= 0) {
                    appState->queueCommand();
                    pointGrabbed = center;
                    pointGrabIndex = 0;
                }
                else if (int back = currentSpline.grabBack(screenToCanvasImToG(io.MousePos), 6.0 / zoom); back >= 0) {
                    appState->queueCommand();
                    pointGrabbed = back;
                    pointGrabIndex = -1;

                }
                else if (int front = currentSpline.grabFront(screenToCanvasImToG(io.MousePos), 6.0 / zoom); front >= 0) {
                    appState->queueCommand();
                    pointGrabbed = front;
                    pointGrabIndex = 1;
                }
            }

            if (io.KeyCtrl && pointGrabbed >= 0) {
                currentSpline.bezierPoints[pointGrabbed].setAsymmetric();
            }

        }
        else if (appState->bezierToolState.mode == BEZIER_DELETE) {

            if (int center = currentSpline.grabCenter(screenToCanvasImToG(io.MousePos), 6.0 / zoom); center >= 0) {
                appState->queueCommand();
                currentSpline.eraseAt(center);
                appState->reportUpdate();
            }

        }
    }
    else {
        appState->reportUpdate();
        activeBezierPoint = nullptr;

        pointGrabbed = -1;
        pointGrabIndex = 0;
    }
}

void Canvas::handleGridToolMouseInput() {
    static ImGuiIO& io = ImGui::GetIO();

    // static variable that stores which grid corner is grabbed
    // 
    // if gridCornerGrabbed is negative then no grid corner is grabbed.
    // 
    // gridCornerGrabbed == 0: bottom left corner is selected
    // gridCornerGrabbed == 1: top left corner is selected
    // gridCornerGrabbed == 2: top right corner is selected
    // gridCornerGrabbed == 3: bottom right corner is selected
    static int gridCornerGrabbed = -1;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
        mouseOn(io.MousePos) &&
        ImGui::IsWindowFocused() &&
        appState->isProjectAndImageLoaded()) {


        // get reference to the current grid configuration
        GridConfiguration& currentGridConfig = appState->getCurrentGridConfig();

        if (gridCornerGrabbed >= 0) { // if a corner is already selected, update its position
            currentGridConfig.setPosition(gridCornerGrabbed, screenToCanvasImToG(io.MousePos));
        }
        else { // if a corner is not already selected, check if the mouse is close enough to grab a corner
            appState->queueCommand(); // queue move grid command
            gridCornerGrabbed = currentGridConfig.grab(screenToCanvasImToG(io.MousePos));
        }

    }
    else {
        appState->reportUpdate(); // report move grid command update
        gridCornerGrabbed = -1;
    }

}

/**
* UI Layout method for canvas
*/
void Canvas::layout() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw background and borders
    drawBase();
    
    // handle pan and zoom inputs
    handlePan();
    handleZoom();

    // Draw the currently loaded image
    drawImage();

    // Draw grid if the projection mode is set to grid and an image is loaded
    if (appState->projectionMode == ProjectionMode::GRID 
            && appState->isProjectAndImageLoaded()) {

        drawGrid();
    }

    // Handle mouse input
	if (appState->currentTool == PAINT_TOOL) {
		this->handlePaintToolMouseInput();
	}
	else if (appState->currentTool == BEZIER_TOOL) {
		this->handleBezierToolMouseInput();
	}
	else if (appState->currentTool == GRID_TOOL) {
		this->handleGridToolMouseInput();
	}
    
    if (appState->isProjectAndImageLoaded()) {
        MeasurementImage currentImage = appState->getCurrentImage();

        drawSplines(currentImage.perspective.splines, true);
        drawPoints(currentImage.perspective.points);
    }

    drawList->PopClipRect();

}
