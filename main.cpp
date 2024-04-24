//Tafari, Megan
#include "ComplexPlane.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int main()
{
	int pixelWidth = VideoMode::getDesktopMode().width / 2;
	int pixelHeight = VideoMode::getDesktopMode().height / 2;

	VideoMode vm(pixelWidth, pixelHeight);

	RenderWindow window(vm, "Mandelbrot Set", Style::Default);

	ComplexPlane complexPlane(pixelWidth, pixelHeight);

	while (window.isOpen())
	{
		//handles user input
		Font font;
		
		if (!font.loadFromFile("KOMIKAP_.ttf"))
		{
			cout << "Error.";
		}//end of user input
		
		Text text;
		text.setFont(font);
		text.setString("Click, click, click for cool stuff");
		text.setCharacterSize(12);
		text.setFillColor(Color::Yellow);
		text.setStyle(Text::Bold);
		window.draw(text);

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				//handles event to close window
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					complexPlane.zoomOut();
					complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					complexPlane.zoomIn();
					complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
			} //end of mouse clicks
			if (event.type == sf::Event::MouseMoved)
			{
				complexPlane.setMouseLocation(Vector2i(event.mouseButton.x, event.mouseButton.y));
			} //end of mouse move
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				window.close();
			} //check if window will close from keyboard click

			//update scene segment
			complexPlane.updateRender();
			complexPlane.loadText(text);

			//draw scene segment
			window.clear();
			window.draw(complexPlane);
			window.draw(text);
			window.display();
		}
	} //end of while loop
}
