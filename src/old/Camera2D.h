#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Camera2D
{
public:
	glm::dvec2 size;
	glm::dvec2 position;
	double scale;
	//glm::dvec2 scale;

	const double ppu = 1.0;

	//glm::dvec2 center;
	//glm::dvec2 position;
	//glm::dvec2 direction;

	//glm::dvec3 up;
	//glm::dvec3 camera_right;
	//glm::dvec3 camera_up;

	Camera2D();
	void applyTransform();
	//void update();

	void setViewportWidth(double width);

	void setViewportHeight(double height);


	void setX(double xPos);
	void setY(double yPos);

	void setScale(double scale);

	glm::dvec2 mouseToSceneCoords(double mouseX, double mouseY);
};

