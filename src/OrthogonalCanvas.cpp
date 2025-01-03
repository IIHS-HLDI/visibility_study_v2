#include "OrthogonalCanvas.h"

void OrthogonalCanvas::update() {

    if (appState->projectLoaded &&
            appState->selectedMeasurementConfig >= 0) {

        auto& images = appState->currentProject->measurements[appState->selectedMeasurementConfig].images;
        // TODO
        
        if (appState->projectionMode == MARKERLESS && 
                appState->markerlessConfig.cameraProfile->loaded) {

            for (int i = 0; i < images.size(); i++) {
                images[i].orthogonal.points.clear();

                if (images[i].perspective.points.size() > 0) {
                    std::vector<cv::Point2f> cvPoints = images[i].perspective.getPoints();
                    std::vector<cv::Point2f> uvCoords = images[i].sceneToUVCoords(cvPoints);
                    std::vector<cv::Point2f> projectedStdPoints = 
                        appState
                        ->markerlessConfig
                        .cameraProfile
                        ->imageToWorldTransform(
                            uvCoords, 
                            images[i],
                            appState->currentProject
                                ->measurements[appState->selectedMeasurementConfig]
                                .measurementOffsets,
                            appState->markerlessConfig.intersectHeight,
                            appState->markerlessConfig.handleDistortion);
                    for (cv::Point2f point : projectedStdPoints) {
                        images[i].orthogonal.points.push_back({glm::vec2(point.x, point.y)});
                    }
                }




                /*for (Point point : images[i].perspective.points) {*/
                /**/
                /*}*/

                images[i].orthogonal.splines.clear();

                for (BezierSpline& spline : images[i].perspective.splines) {
                    /*spline.bezierPoints.clear();*/
                    BezierSpline orthoSpline;

                    for (BezierPoint& point : spline.bezierPoints) {

                        std::vector<cv::Point2f> bezierPoints = {
                            cv::Point2f(point.back.x, point.back.y),
                            cv::Point2f(point.center.x, point.center.y),
                            cv::Point2f(point.front.x, point.front.y),
                        };

                        std::vector<cv::Point2f> uvPoints = images[i].sceneToUVCoords(bezierPoints);
                        
                        std::vector<cv::Point2f> projectedPoints = 
                            appState
                            ->markerlessConfig
                            .cameraProfile
                            ->imageToWorldTransform(
                                uvPoints, 
                                images[i],
                                appState->currentProject
                                    ->measurements[appState->selectedMeasurementConfig]
                                    .measurementOffsets,
                                appState->markerlessConfig.intersectHeight,
                                appState->markerlessConfig.handleDistortion);

                        BezierPoint nextPoint;
                        nextPoint.back = glm::vec2(projectedPoints[0].x, projectedPoints[0].y),
                        nextPoint.center = glm::vec2(projectedPoints[1].x, projectedPoints[1].y),
                        nextPoint.front = glm::vec2(projectedPoints[2].x, projectedPoints[2].y),
                        nextPoint.symmetric = point.symmetric,
                        orthoSpline.bezierPoints.push_back(nextPoint);

                    }

                    images[i].orthogonal.splines.push_back((orthoSpline));
                }

                /*appState->markerlessConfig.cameraProfile.imageToWorldTransform(*/
                /**/
                /*        );*/
            }
        } else if (appState->projectionMode == GRID) {


            for (int i = 0; i < images.size(); i++) {
                float flipX = 1;
                float flipY = 1;
                if (images[i].gridConfig.flipX) {
                    flipX = -1;
                }
                if (images[i].gridConfig.flipY) {
                    flipY = -1;
                }
                images[i].gridConfig.updatePerspectiveMatrices();

                images[i].orthogonal.points.clear();

                if (images[i].perspective.points.size() > 0) {


                    std::vector<cv::Point2f> cvPoints = images[i].perspective.getPoints();

                    std::vector<cv::Point2f> projectedPoints(images[i].perspective.points.size());

                    cv::perspectiveTransform(cvPoints, projectedPoints, images[i].gridConfig.pM_inv);
                            
                    for (cv::Point2f point : projectedPoints) {
                        images[i].orthogonal.points.push_back({.position = images[i].gridConfig.gridToWorldTransform(point)});
                                /*{glm::vec2(point.x, point.y)});*/
                    }
                }

                images[i].orthogonal.splines.clear();

                for (BezierSpline& spline : images[i].perspective.splines) {
                    /*spline.bezierPoints.clear();*/
                    BezierSpline orthoSpline;

                    for (BezierPoint& point : spline.bezierPoints) {

                        std::vector<cv::Point2f> bezierPoints = {
                            cv::Point2f(point.back.x, point.back.y),
                            cv::Point2f(point.center.x, point.center.y),
                            cv::Point2f(point.front.x, point.front.y),
                        };

                        // std::vector<cv::Point2f> uvPoints = images[i].sceneToUVCoords(bezierPoints);

                        std::vector<cv::Point2f> projectedPoints(3);

                        cv::perspectiveTransform(bezierPoints, projectedPoints, images[i].gridConfig.pM_inv);


                        BezierPoint nextPoint;

                        nextPoint.back = images[i].gridConfig.gridToWorldTransform(projectedPoints[0]);

                            /*glm::vec2(*/
                            /*        (projectedPoints[0].x-appState->gridConfig.botLeft.x)*flipX + appState->gridConfig.xOffset, */
                            /*        (projectedPoints[0].y-appState->gridConfig.botLeft.y)*flipY + appState->gridConfig.yOffset),*/

                        nextPoint.center = images[i].gridConfig.gridToWorldTransform(projectedPoints[1]);
                            /*glm::vec2(*/
                            /*        (projectedPoints[1].x-appState->gridConfig.botLeft.x)*flipX + appState->gridConfig.xOffset, */
                            /*        (projectedPoints[1].y-appState->gridConfig.botLeft.x)*flipY),*/
                        nextPoint.front = images[i].gridConfig.gridToWorldTransform(projectedPoints[2]);
                        /*glm::vec2(projectedPoints[2].x*flipX, projectedPoints[2].y*flipY),*/

                        nextPoint.symmetric = point.symmetric,
                        orthoSpline.bezierPoints.push_back(nextPoint);

                    }

                    images[i].orthogonal.splines.push_back((orthoSpline));

                }
            }
        }
    }
}


void OrthogonalCanvas::drawGrid() {
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    MeasurementImage currentImage = appState->getCurrentImage();

    float f_flipX = 1;
    float f_flipY = 1;
    if (currentImage.gridConfig.flipX) {
        f_flipX = -1;
    }
    if (currentImage.gridConfig.flipY) {
        f_flipY = -1;
    }

    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.xOffset, currentImage.gridConfig.yOffset)), 
            transformPoint(ImVec2(currentImage.gridConfig.xOffset, currentImage.gridConfig.yOffset+f_flipY*currentImage.gridConfig.height*100)), 
            IM_COL32(255, 0,0,255),
            1.0f);

    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.xOffset , currentImage.gridConfig.yOffset )), 
            transformPoint(ImVec2(currentImage.gridConfig.xOffset+ f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset )), 
            IM_COL32(255, 0,0,255),
            1.0f);

    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.xOffset, currentImage.gridConfig.yOffset+f_flipY*currentImage.gridConfig.height*100)), 
            transformPoint(ImVec2(currentImage.gridConfig.xOffset+ f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset+f_flipY*currentImage.gridConfig.height*100)), 
            IM_COL32(255, 0,0,255),
            1.0f);

    drawList->AddLine(
            transformPoint(ImVec2(currentImage.gridConfig.xOffset+f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset+f_flipY*currentImage.gridConfig.height*100)), 
            transformPoint(ImVec2(currentImage.gridConfig.xOffset+ f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset)), 
            IM_COL32(255, 0,0,255),
            1.0f);


    float xOffset = (currentImage.gridConfig.width*100)/(currentImage.gridConfig.divsX);

    for (int i = 1; i < currentImage.gridConfig.divsX; i++) {
        // std::vector<cv::Point2f> points(2);
        // std::vector<cv::Point2f> ortho(2);

        // points[0] = cv::Point2f(ortho_corners[0].x + xOffset*i, ortho_corners[0].y);
        // points[1] = cv::Point2f(ortho_corners[1].x + xOffset*i, ortho_corners[1].y);

        // cv::perspectiveTransform(points, ortho, currentImage.gridConfig.pM);

        if ((i) % currentImage.gridConfig.markerIntervalX == 0) {
            drawList->AddLine(
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*(xOffset*i), currentImage.gridConfig.yOffset)),
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*(xOffset*i), currentImage.gridConfig.yOffset + f_flipY*currentImage.gridConfig.height*100)),
                IM_COL32(255,255,0,255),
                1.0f);
        } else {
            drawList->AddLine(
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*(xOffset*i), currentImage.gridConfig.yOffset)),
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*(xOffset*i), currentImage.gridConfig.yOffset + f_flipY*currentImage.gridConfig.height*100)),
                // transformPoint(ImVec2(ortho[0].x, ortho[0].y)),
                // transformPoint(ImVec2(ortho[1].x, ortho[1].y)),
                IM_COL32(255, 0,0,255),
                1.0f);
        }

    }

    float yOffset = (currentImage.gridConfig.height*100)/(currentImage.gridConfig.divsY);

    for (int i = 1; i < currentImage.gridConfig.divsY; i++) {
        // std::vector<cv::Point2f> points(2);
        // std::vector<cv::Point2f> ortho(2);

        // points[0] = cv::Point2f(ortho_corners[0].x, ortho_corners[0].y + yOffset*i);
        // points[1] = cv::Point2f(ortho_corners[3].x, ortho_corners[3].y + yOffset*i);

        // cv::perspectiveTransform(points, ortho, currentImage.gridConfig.pM);

        if ((i) % currentImage.gridConfig.markerIntervalY == 0) {
            drawList->AddLine(
                transformPoint(ImVec2(currentImage.gridConfig.xOffset, currentImage.gridConfig.yOffset + f_flipY*(yOffset*i))),
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset + f_flipY*(yOffset*i))),
                IM_COL32(255, 255,0,255),
                1.0f);
        } else {
            drawList->AddLine(
                transformPoint(ImVec2(currentImage.gridConfig.xOffset, currentImage.gridConfig.yOffset + f_flipY*(yOffset*i))),
                transformPoint(ImVec2(currentImage.gridConfig.xOffset + f_flipX*currentImage.gridConfig.width*100, currentImage.gridConfig.yOffset + f_flipY*(yOffset*i))),
                IM_COL32(255, 0,0,255),
                1.0f);
        }
    }


}

void OrthogonalCanvas::layout() {

    static ImGuiIO& io = ImGui::GetIO();
    static ImDrawList* drawList = ImGui::GetWindowDrawList();

    Canvas::drawBase();

    Canvas::handlePan();
    Canvas::handleZoom();



    // Draw origin
    if (appState->orthoShowOrigin) {
        drawList->AddLine(
            transformPoint(ImVec2(0, -25)),
            transformPoint(ImVec2(0, 100)),
            IM_COL32(0, 255, 255, 255), 2.0f);

        drawList->AddLine(
            transformPoint(ImVec2(-25, 0)),
            transformPoint(ImVec2(100, 0)),
            IM_COL32(0, 255, 255, 255), 2.0f);
    }

    // draw camera position if in markerless
    if (appState->projectionMode == MARKERLESS && appState->isProjectAndImageLoaded() && appState->orthoShowCamera) {
		auto measurementConfig = appState->getCurrentMeasurementConfig(); 
		auto image = appState->getCurrentImage(); 

		drawList->AddRect(transformPoint(ImVec2(measurementConfig.measurementOffsets.rigPosition.x - 10, 
						-measurementConfig.measurementOffsets.rigPosition.y - 10)),
				transformPoint(ImVec2(measurementConfig.measurementOffsets.rigPosition.x + 10, 
						-measurementConfig.measurementOffsets.rigPosition.y + 10)),
				IM_COL32(0, 255, 0, 255));

		drawList->AddLine(
				transformPoint(ImVec2(
						measurementConfig.measurementOffsets.rigPosition.x,
						-measurementConfig.measurementOffsets.rigPosition.y)),

		transformPoint(ImVec2(measurementConfig.measurementOffsets.rigPosition.x + 100*sind(image.yawAngle),
				-measurementConfig.measurementOffsets.rigPosition.y + 100*cosd(image.yawAngle))),
		IM_COL32(0, 255, 0, 255), 2.0f);
	
    } 

    // Draw orthogonal grid if in grid projection mode, a project is loaded and option to show grid is true
    if (appState->projectionMode == GRID 
            && appState->isProjectAndImageLoaded() 
			&& appState->orthoShowGrid) {

        drawGrid();
    }
    

    // Draw splines and points if a measurement configuration is loaded
    if (appState->isMeasurementConfigLoaded()) {
        auto& images = appState->getCurrentMeasurementConfigImages();

        for (MeasurementImage image : images) {

            if (appState->markReload) {
                update();
                appState->markReload = false;
            }
            drawSplines(image.orthogonal.splines, false);
            drawPoints(image.orthogonal.points);
        }

    }

}
