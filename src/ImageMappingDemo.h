#ifndef ROTATION_H
#define ROTATION_H

#include "BaseEngine.h"
#include "ImagePixelMapping.h"

/*
This is a demo showing how to use the coordinate mapping objects in image objects
*/
class ImageMappingDemo : public BaseEngine, public ImagePixelMapping
{
public:

	/**
	Constructor
	*/
	ImageMappingDemo()
		: rotator(0.0), shifter1(0, 0), shifter2(0, 0)
	{}

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create any moving objects
	int virtInitialiseObjects() override;

	// Handle pressing of a key
	virtual void virtKeyDown(int iKeyCode) override;

	// This forces a screen redraw every pass through the loop.
	virtual void virtMainLoopDoBeforeUpdate() override;

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override;

protected:
	ImagePixelMappingRotate rotator;
	ImagePixelMappingShiftXY shifter1;
	ImagePixelMappingShiftXY shifter2;

};

#endif
