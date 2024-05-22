#include"../include/options.h"

unsigned int Options::width = STANDART_WIDTH;
unsigned int Options::height = STANDART_HEIGHT;

void Options::initialize()
{
    if (Options::fullscreenOn)
    {
        Options::width = sf::VideoMode::getDesktopMode().width;
        Options::height = sf::VideoMode::getDesktopMode().height;
    }
}