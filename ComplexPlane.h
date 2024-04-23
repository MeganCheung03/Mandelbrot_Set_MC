#pragma once
#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

class ComplexPlane : public Drawable
{
public:
	ComplexPlane(int pixelWidth, int pixelHeight);
	void draw(RenderTarget& target, RenderStates states) const;
	void updateRender();
	void zoomIn();
	void zoomOut();
	void setCenter(Vector2i mousePixel);
	void setMouseLocation(Vector2i mousePixel);
	void loadText(Text& text);

	enum class State { CALCULATING, DISPLAYING };

private:
	VertexArray m_vArray;
	State m_state;
	Vector2f m_mouseLocation;
	Vector2i m_pixel_size;
	float m_aspectRatio;
	Vector2f m_plane_center;
	Vector2f m_plane_size;
	int m_zoomCount;
	int m_pixelHeight;
	int m_pixelWidth;

	unsigned int countInterations(Vector2f coord);
	void interationsToRGB(size_t count, Uint8& g, Uint8& b);
	Vector2f mapPixelToCoords(Vector2i mousePixel);
};
#endif
