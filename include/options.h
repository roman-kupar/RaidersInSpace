#ifndef OPTIONS_H
#define OPTIONS_H

#include<SFML/Window.hpp>

const unsigned int STANDART_WIDTH = 816, STANDART_HEIGHT = 480;

class Options
{
public:
	const static bool fullscreenOn = false;

	static unsigned int width, height;

	static void initialize();
};

#endif OPTIONS_H
