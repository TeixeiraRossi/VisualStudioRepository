#pragma once
class Camera2D
{
public:
	float zoom = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;

	void processScroll(float yoffset);
	void processDrag(float dx, float dy);
};