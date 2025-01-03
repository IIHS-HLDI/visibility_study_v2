#include "Camera2D.h"

Camera2D::Camera2D() :
	size(100, 100),
	position(0, 300)
{
	scale = 0.00001;

}

void Camera2D::applyTransform() {
    // glViewport(0, 0, size.x, size.y);
	// glMatrixMode(GL_PROJECTION);
	// glLoadIdentity();

	// glOrtho(
		// -size.x / 2,
		// size.x / 2,
		// size.y / 2,
		// -size.y / 2,
		// -1,
		// 1);

	// glMatrixMode(GL_MODELVIEW);

	// glLoadIdentity();

	// glScalef((GLfloat) scale, (GLfloat) -scale, 1.0);
	// glTranslatef((GLfloat) - position.x, (GLfloat)position.y, 0);
}

//void Camera2D::update() {
//
//}

void Camera2D::setViewportWidth(double width) {
	size.x = width;
}

void Camera2D::setViewportHeight(double height) {
	size.y = height;
}

void Camera2D::setX(double xPos) {
	position.x = xPos;
}

void Camera2D::setY(double yPos) {
	position.y = yPos;
}

void Camera2D::setScale(double _scale) {
	scale = _scale;
}
