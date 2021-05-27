#include "header.h"

// This is a basic implementation, without fancy stuff

#include "BaseEngine.h"

#include "ImageMappingDemo.h"

#include "ImageManager.h"

#include "TileManager.h"

#include "DisplayableObject.h"

#include "ExampleDragableObjects.h"







/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void ImageMappingDemo::virtSetupBackgroundBuffer()
{
	fillBackground(0x404040);

	for (int iX = 0; iX < getWindowWidth(); iX++)
		for (int iY = 0; iY < this->getWindowHeight(); iY++)
			switch (rand() % 100)
			{
			case 0: setBackgroundPixel(iX, iY, 0xFF0000); break;
			case 1: setBackgroundPixel(iX, iY, 0xC00000); break;
			case 2: setBackgroundPixel(iX, iY, 0x800000); break;
			case 3: setBackgroundPixel(iX, iY, 0x400000); break;
			}
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int ImageMappingDemo::virtInitialiseObjects()
{
	// Destroy any existing objects
	destroyOldObjects(true);

	// Add three draggable shapes. Just using the base class objects for this demo but you can subclass them
	createObjectArray(5);

	// Understand what this one does first before looking at the others...
	MappableImageObject* ob = new MappableImageObject(this, 50, 50, this, "demo.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0,0));
	storeObjectInArray(0, ob );

	ob = new MappableImageObject(this, 250, 250, this, "demo.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0, 0));
	storeObjectInArray(1, ob);

	// Check out what happen in virtMainLoopDoBeforeUpdate() to see how the coordinate mapping objects are modified constantly
	ob = new MappableImageObject(&rotator, 450, 450, this, "demo.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0, 0));
	storeObjectInArray(2, ob);

	ob = new MappableImageObject(&shifter1, 250, 450, this, "demo.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0, 0));
	storeObjectInArray(3, ob);

	ob = new MappableImageObject(&shifter2, 450, 250, this, "demo.png", false);
	ob->setTransparencyColour(ob->getPixelColour(0, 0));
	storeObjectInArray(4, ob);
	return 0;
}



/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void ImageMappingDemo::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode(0);
		break;
	}
}

// This forces a screen redraw every pass through the loop.
void ImageMappingDemo::virtMainLoopDoBeforeUpdate()
{
	int iOffset = getModifiedTime() / 10;

	rotator.setRotation((double)iOffset / -100.0);
	shifter1.setXShift((double)iOffset);
	shifter2.setYShift((double)iOffset/2.0);

	this->redrawDisplay();

}

bool ImageMappingDemo::mapCoordinates(double& x, double& y, const SimpleImage& image)
{
	int iOffset = getModifiedTime() / 10;

	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= (image.getWidth() - 0.5)) return false;
	if (y >= (image.getHeight() - 0.5)) return false;

	// Demo 1 - shift x
//	x = ((int)(x + 0.5) + iOffset) % image.getWidth();

	// Demo 2 - shift y
//	y = ((int)(y + 0.5) + iOffset) % image.getHeight();

	// Demo 3 - simple rotation

	// Shift offset to the centre of the image, so we can rotate around centre
	x -= image.getWidth()/2;
	y -= image.getHeight()/2;

	// Rotate it
	//double dAngle = (double)iOffset / 100;
	double dAngle = atan( y / (x+0.0001) );
	if (x < 0) 
		dAngle += M_PI;
	double hyp = ::sqrt(x*x + y * y);
	dAngle += (double)iOffset / 100;

	x = hyp * ::cos(dAngle);
	y = hyp * ::sin(dAngle);

	// Shift offset back to the corner
	x += image.getWidth()/2;
	y += image.getHeight()/2;

	if (x < 0) return false;
	if (y < 0) return false;
	if (x >= (image.getWidth() - 0.5)) return false;
	if (y >= (image.getHeight() - 0.5)) return false;

	return true;
}
