// dependencies
#include <irrlicht.h>
#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <cmath>
#include "Receiver.h"
using namespace irr;
using namespace std;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
	//hide console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	// handles events
	MyEventReceiver Receiver;

	// variables
	const float G	 = 6.674e-11;
	const int mass 	 = 5.972e24;
	int state		 = 1;
	float x 		 = 300;
	float y			 = 200;
	float xvel		 = 0;
	float yvel 		 = 0;
	float xaccel 	 = 0;
	float yaccel 	 = 0;
	float r 		 = sqrtf(powf(300 - x, 2) + powf(300 - y, 2)) / 10;
	float theta 	 = atan2f(x-300, y-300);
	float grav 		 = -G * (mass) / powf(r, 2);

	// create device
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(600, 600), 16, false, false, false, &Receiver);

	if (device == 0) {
		return 1; // could not create device
	}

	// set window title
	device->setWindowCaption(L"Spaceorbit");

	// create video driver
	IVideoDriver* driver = device->getVideoDriver();

	// load textures
	ITexture* stars  = driver->getTexture("Media/Stars.png");
	driver->makeColorKeyTexture(stars, position2d<s32>(0, 0));

	// antialiasing
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().AntiAliasing = EAAM_FULL_BASIC;

	// program loop
	while (device->run() && driver)
	{
			if (Receiver.IsKeyDown(KEY_ESCAPE))
			{
				return 0;
			}

			driver->beginScene(true, true, SColor(255, 0, 0, 0));

			// draw sprites
			driver->draw2DImage(stars, position2d<s32>(0, 0),rect<s32>(0, 0, 600, 600), 0,SColor(255, 255, 255, 255), true); // stars
			driver->draw2DPolygon(vector2d<s32>(300,300), 25, SColor(255,255,255,255), 50); // parent
			driver->draw2DPolygon(vector2d<s32>(x,y), 5, SColor(255,255,255,255), 50);// satellite

			switch (state)
			{
			case (1):
				x = device->getCursorControl()->getPosition().X;
				y = device->getCursorControl()->getPosition().Y;
				if (Receiver.MouseState.LeftButtonDown)
				{
					state = 2;
				}
				break;
			case (2):
				driver->draw2DLine(position2d<s32>(x,y), position2d<s32>(device->getCursorControl()->getPosition()), SColor(255,255,255,255));
				if (!(Receiver.MouseState.LeftButtonDown))
				{
					xvel = (device->getCursorControl()->getPosition().X - x)/250;
					yvel = (device->getCursorControl()->getPosition().Y - y)/250;
					state = 3;
				}
				break;
			case (3):
				// physics
				theta  = atan2f(x-300, y-300);
				r 	   = sqrtf(powf(300 - x, 2) + powf(300 - y, 2)) / 10;
				grav   = -G * (mass) / powf(r, 2);
				xaccel = (sinf(theta) * grav);
				yaccel = (cosf(theta) * grav);
				xvel  += xaccel;
				yvel  += yaccel;
				x 	  += xvel;
				y 	  += yvel;
				if ((r * 10 < 30) || Receiver.IsKeyDown(KEY_KEY_R))
				{
					state = 1;
				}
				break;
			}
			driver->endScene();
	}
	device->drop();

	return 0;
}
