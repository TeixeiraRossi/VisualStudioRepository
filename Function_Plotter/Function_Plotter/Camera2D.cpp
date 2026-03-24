#include "Camera2D.h"

void Camera2D::processScroll(float yoffset) {
	if (yoffset > 0)
		zoom *= 0.9f;
	else
		zoom *= 1.1f;
}

void Camera2D::processDrag(float dx, float dy) {
	offsetX -= dx * 0.002f * zoom;
	offsetY += dy * 0.002f * zoom;
}