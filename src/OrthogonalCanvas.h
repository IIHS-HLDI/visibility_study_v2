#pragma once

#include "Canvas.h"

#include <memory>
#include <vector>

#include <imgui.h>
#include "GridConfiguration.h"


class OrthogonalCanvas : Canvas {

private:
protected:
    void drawGrid();
public:
    using Canvas::Canvas;
    void layout() override;
    void update();
};
