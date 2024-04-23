#include "ComplexPlane.h"
#include <cmath>
#include <iostream>
#include <complex>
#include <sstream>

using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
	m_pixelWidth = pixelWidth;
	m_pixelHeight = pixelHeight;
	m_aspectRatio = pixelHeight / pixelWidth; //careful of integer divide
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_plane_center = { 0.0f, 0.0f };
	m_zoomCount = 0;
	m_State = State::CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(m_pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const 
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	if (m_State == CALCULATING)
	{
		for (int j = 0; j < pixelWidth; ++j) //x coordinate
		{
			for (int i = 0; i < pixelHeight; ++i) //y coordinate
			{
				m_vArray[j + i * pixelWidth].position = { (float)j,(float)i };
				mapPixelToCoords(m_vArray[j + i * pixelWidth].position);
				countIterations(m_vArray[j + i * pixelWidth].position);
				Uint8 r, g, b;
				iterationsToRGB(m_vArray[j + i * pixelWidth].position);
				m_vArray[j + i * pixelWidth].color = { r,g,b };
				m_State = DISPLAYING;
			}
		}
	}
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float xCoord = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float yCoord = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xCoord, yCoord };
	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float xCoord = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float yCoord = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { xCoord, yCoord };
	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	Vector2f coords = mapPixelToCoords(mousePixel);
	m_plane_center = coords;
	m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream in;
	in << "Mandelbrot Set \n Center: (" << setCenter(/*smth*/) << ") \n Cursor: (" 
		<< setMouseLoactions(/*smth*/) << ") \n Left-click to Zoom in \n Right-click to Zoom out";
	text.setString(in.str());
}

unsigned int ComplexPlane::countInterations(Vector2f coord) //try understanding
{
	float real = coord.x;
	float imag = coord.y;
	float realTemp = real;
	float imagTemp = imag;
	size_t count = 0;

	while (count < MAX_ITER)
	{
		float realSquared = real * real;
		float imagSquared = imag * imag;
		if (realSquared + imagSquared > 4.0)
		{
			break; 
		}
		imag = 2.0 * real * imag + imagTemp;
		real = realSquared - imagSquared + realTemp;
		count++;
	}
	return count;
}

void ComplexPlane::interationsToRGB(size_t count, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)
	{
		//black
		r = 0;
		g = 0;
		b = 0; 
		return;
	}
	else if (count < 16)
	{
		//yellow
		r = 255;
		g = 255;
		b = 0;
	}
	else if (count < 32)
	{
		//purple
		r = 111;
		g = 0;
		b = 255;
	}
	else if (count < 48)
	{
		// Green
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count < 56)
	{
		//blue
		r = 0;
		g = 0;
		b = 255;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) //aim to understand
{
	float coordX = static_cast<float>(mousePixel.x) / static_cast<float>(m_pixelWidth);
	float coordY = static_cast<float>(mousePixel.y) / static_cast<float>(m_pixelHeight);
	float complexX = ((coordX*m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0f);
	float complexY = ((1.0f - coordY) * m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0f);
	return { complexX , complexY }
}