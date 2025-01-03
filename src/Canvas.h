#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <imgui.h>

#include "ApplicationState.h"
#include "Image.h"
#include "VehicleProject.h"
#include "GridConfiguration.h"

class Canvas {

private:


    Image image;

    ImVec2 position;
    ImVec2 size;
    ImVec2 pan;
    float zoom;

    bool isHovered;
    bool isActive;

    ImVec2 origin;
    ImVec2 mousePositionInCanvas;

    void handlePaintToolMouseInput();
    void handleBezierToolMouseInput();
    void handleGridToolMouseInput();


protected:

    ImVec2 transformPoint(ImVec2 input);
    ImVec2 transformPoint(glm::vec2 input);
    ImVec2 screenToCanvas(ImVec2 input);
    ImVec2 screenToCanvas(glm::vec2 input);

    glm::vec2 transformPointImToG(ImVec2 input);
    glm::vec2 screenToCanvasImToG(ImVec2 input);
    std::shared_ptr<ApplicationState> appState;

    void handlePan();

    void handleZoom();

    void drawImage();

    void drawBase();

    void drawGrid();


public:
    const float zoomFastDivisor = 50.0f;
    const float zoomSlowDivisor = 500.0f;

    bool mouseOn(ImVec2 mousePos);
    Canvas(std::shared_ptr<ApplicationState> appState);
    virtual void layout();


    void handleBezierToolClick();

    virtual void drawSplines(std::vector<BezierSpline> splines, bool drawHandles);


    void drawSpline(BezierSpline bezierSpline, bool drawHandles);
    void drawBezierPoint(BezierPoint bezierPoint);

    void drawPoint(Point point);
    void drawPoints(std::vector<Point> points);
};
