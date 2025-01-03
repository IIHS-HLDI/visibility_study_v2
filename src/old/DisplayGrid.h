#pragma once
#include "Drawable.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include <vector>

class DisplayGrid :
    public Drawable
{
private:
    std::vector<glm::vec2> vertices;
    std::vector<glm::uvec2> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    glm::dvec2 position;
    double x_spacing;
    double y_spacing;
    double height;
    double width;
public:
    DisplayGrid(double x_pos, double y_pos, double x_spacing, double y_spacing,
        double width, double height);

    void update();

    void render() override;

    ~DisplayGrid();
};

