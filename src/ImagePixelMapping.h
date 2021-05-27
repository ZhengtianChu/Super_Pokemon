#pragma once

#include "SimpleImage.h"

/*
This supports a class which can map coordinates and pixel colours of an image when drawing it
It can be used for things like rotating or shifting images, or changing the colours of an image - e.g. for transparency.
Important note: It could be SLOW to use this on a large image! It applies the transformation to each pixel. If the tranformation is complex (e.g. rotation) this could take it a while (comparatively), slowing down the frame rate.
*/
class ImagePixelMapping
{
public:
	// Map the x and y coodinates to new ones.
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) = 0;

	// Most implementations will not change this, so just return true, but it allows colour modification if needed.
	virtual bool changePixelColour(int x, int y, int& colour, DrawingSurface* pTarget)
	{ 
		return true; 
	}
};


/*
This is a simple coordinate mapping function whose only purpose is to avoid drawing from outside of the image.
It just rejects any drawing outside of the area of the image
*/
class CroppedImagePixelMapping : public ImagePixelMapping
{
public:
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;
		return true;
	}
};


/* 
This is a coordiante mapping function for shifting x and y coordinates, e.g. to scroll around an image
*/
class ImagePixelMappingShiftXY : public ImagePixelMapping
{
public:
	ImagePixelMappingShiftXY(double x, double y)
		: xShift(x), yShift(y)
	{
	}
	
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		x += xShift;
		y += yShift;

		// Ensure it is within the size of the image
		while (x > image.getWidth())
			x -= image.getWidth();
		while (x < 0)
			x += image.getWidth();
		while (y > image.getHeight())
			y -= image.getHeight();
		while (y < 0)
			y += image.getHeight();

		return true;
	}

	// Extract the current drawing position
	double getXShift() { return xShift; }
	double getYShift() { return yShift; }

	// Set the current drawing position
	void setXShift( double x ) { xShift = x; }
	void setYShift( double y ) { yShift = y; }

protected: 
	double xShift;
	double yShift;
};


/*
This allows a rotation to be applied around the centre of the image.
You could easily create a variant which rotates around other points.
*/
class ImagePixelMappingRotate : public ImagePixelMapping
{
public:
	ImagePixelMappingRotate(double r)
		: rotation(r)
	{
	}

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		// Shift offset to the centre of the image, so we can rotate around centre
		x -= image.getWidth() / 2;
		y -= image.getHeight() / 2;

		// Rotate it
		double dAngle = atan(y / (x + 0.0001));
		if (x < 0)
			dAngle += M_PI;
		double hyp = ::sqrt(x*x + y * y);
		dAngle += (double)rotation;

		x = hyp * ::cos(dAngle);
		y = hyp * ::sin(dAngle);

		// Shift offset back to the corner
		x += image.getWidth() / 2;
		y += image.getHeight() / 2;

		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}

	double getRotation() { return rotation; }
	void setRotation(double r) { rotation = r; }

protected:
	double rotation;
};

/* 
Class to do a variety of different coordinate mapping and re-colouring that could be desired.

*/
class ImagePixelMappingRotateAndColour : public ImagePixelMappingRotate
{
public:
	/* Rotation amount and rotation centre are set in this constructor.
	Colours will be defaulted - so you need to manually set them if needed.
	*/
	ImagePixelMappingRotateAndColour(double r , int iRotationCentreX, int iRotationCentreY )
		: ImagePixelMappingRotate(r), m_iRotationCentreX(iRotationCentreX), m_iRotationCentreY(iRotationCentreY),
		m_iTransparencyColour(-1),
		m_iEverySecondPixelColour(-1), m_iEveryThirdPixelColour(-1), m_iEveryFourthPixelColour(-1),
		m_iMergeColour(-1), m_iMergePercentage(-1), m_iBrightnessPercentage(100),
		m_iSourceShiftX(0), m_iSourceShiftY(0)
	{
	}

	// No rotation stored, so everything gets initialised to default values - remember to set the values you need!
	ImagePixelMappingRotateAndColour()
		: ImagePixelMappingRotate(0), m_iRotationCentreX(0), m_iRotationCentreY(0),
		m_iTransparencyColour(-1),
		m_iEverySecondPixelColour(-1), m_iEveryThirdPixelColour(-1), m_iEveryFourthPixelColour(-1),
		m_iMergeColour(-1), m_iMergePercentage(-1), m_iBrightnessPercentage(100),
		m_iSourceShiftX(0), m_iSourceShiftY(0)
		{
		}

	// Virtual destructor because image can delete subclass objects through a pointer
	// No disadvantage since we already have a virtual function anyway (see below)
	virtual ~ImagePixelMappingRotateAndColour()
	{
	}

	// This works out a new position in the image given the various shifts which have been applied.
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		// First apply any shift into the image
		x += m_iSourceShiftX;
		y += m_iSourceShiftY;

		// Now check that coordinates are actually within the image
		// To be honest I'm thinking this may be unnecessary, since we validate after the rotation anyway
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		if (rotation > 0)
		{	// Don't bother doing these calculations if we have no rotation, they would just slow it down...

			// Shift offset to the rotation centre, so we can rotate around centre
			x -= m_iRotationCentreX;
			y -= m_iRotationCentreY;

			// Rotate it
			double dAngle = atan(y / (x + 0.0001));
			if (x < 0)
				dAngle += M_PI;
			double hyp = ::sqrt(x*x + y * y);
			dAngle += (double)rotation;

			x = hyp * ::cos(dAngle);
			y = hyp * ::sin(dAngle);

			// Shift offset back to the corner
			x += m_iRotationCentreX;
			y += m_iRotationCentreY;

			// Verify that new coordinates are still within the image
			if (x < 0) return false;
			if (y < 0) return false;
			if (x >= (image.getWidth() - 0.5)) return false;
			if (y >= (image.getHeight() - 0.5)) return false;
		}

		return true;
	}

	// Here we want to change some pixel colours so we implement that facility here
	// Note that this function is rarely used and may potentially have some bugs I missed? Please let Jason know if so!
	virtual bool changePixelColour(int x, int y, int& iNewColour, DrawingSurface* pTarget) override
	{ 
		int iBackgroundColour = pTarget->getPixel(x, y);

		// If there is a transparency colour, and pixel to draw is that colour...
		if ( (m_iTransparencyColour != -1) && (iNewColour == m_iTransparencyColour) )
			return false; // Don't draw any pixel which should be transparency colour

		// Colour various pixels - only apply one of these?
		if ((m_iEverySecondPixelColour != -1) && ((x + y) % 2 == 0))
			iNewColour = m_iEverySecondPixelColour;
		if ((m_iEveryThirdPixelColour != -1) && ((x + y) % 3 == 0))
			iNewColour = m_iEveryThirdPixelColour;
		if ((m_iEveryFourthPixelColour != -1) && ((x + 2*y) % 4 == 0))
			iNewColour = m_iEveryFourthPixelColour;

		// Can merge colours
		// m_iMergeColour and m_iMergePercentage
		if ((m_iBrightnessPercentage < 100) || (m_iMergeColour != -1))
		{
			if (m_iBrightnessPercentage < 0)
				m_iBrightnessPercentage = 0;
			if (m_iBrightnessPercentage > 100)
				m_iBrightnessPercentage = 100;

			// Alter brightness of RGB separately by the specified amount
			double dR = ((0xFF & (iNewColour >> 16)) * m_iBrightnessPercentage) / 100.0;
			double dG = ((0xFF & (iNewColour >> 8)) * m_iBrightnessPercentage) / 100.0;
			double dB = ((0xFF & iNewColour) * m_iBrightnessPercentage) / 100.0;

			if (m_iMergeColour != -1)
			{
				int colour = m_iMergeColour;
				if (colour == -2) colour = iBackgroundColour;
				if (m_iMergePercentage < 0) m_iMergePercentage = 0;
				if (m_iMergePercentage > 100) m_iMergePercentage = 100;

				dR = (dR*m_iMergePercentage + (double)(colour >> 16 & 0xff)*(100 - m_iMergePercentage)) / 100.0;
				dG = (dG*m_iMergePercentage + (double)(colour >> 8 & 0xff)*(100 - m_iMergePercentage)) / 100.0;
				dB = (dB*m_iMergePercentage + (double)(colour & 0xff)*(100 - m_iMergePercentage)) / 100.0;
			}

			// Catch saturation - should never happen, but just in case of rounding issues...
			if (dR > 255) dR = 255;
			if (dG > 255) dG = 255;
			if (dB > 255) dB = 255;

			iNewColour = ((((int)(dR + 0.5)) & 0xFF) << 16)
				+ ((((int)(dG + 0.5)) & 0xFF) << 8)
				+ ((((int)(dB + 0.5)) & 0xFF));
		}

		return true; // Do draw the pixel
	}

	// Set centre of any rotation - if you apply a rotation!
	void setRotationCentre(int x, int y) 
	{
		m_iRotationCentreX = x; 
		m_iRotationCentreY = y;
	}

	// Set which pixel colour is transparent, and hence not drawn
	void setTransparencyColour(int iColour)
	{
		m_iTransparencyColour = iColour;
	}

	// Set how bright the image should be. 100% means full brightness, 0% means black.
	void setBrightnessPercentage(int iPercentage)
	{
		m_iBrightnessPercentage = iPercentage;
	}

	// Set how much of the foregeround to draw instead of background. 100% means foreground. 0% means keep the background entirely.
	// You can merge with a specific colour if you wish, rather than the background, by specifying a colour value as the second parameter
	void setMergePercentage(int iPercentage, int iMergeColour = -2 )
	{
		m_iMergePercentage = iPercentage;
		m_iMergeColour = iMergeColour; // -2 means use background
	}

	// Set second, third and fourth colours 
	void setNthPixelColours(int iSecond, int iThird = -1, int iFourth = -1)
	{
		m_iEverySecondPixelColour = iSecond;
		m_iEveryThirdPixelColour = iThird;
		m_iEveryFourthPixelColour = iFourth;
	}

	// Set the position of the top left corner of draw position in the iamge.
	// So these positions are added on to the x and y passed in when working out which pixel to draw
	// Think of this as doing the same as the ImagePixelMappingShiftXY objects
	void setTopLeftPositionInImage(int iX, int iY)
	{
		m_iSourceShiftX = iX;
		m_iSourceShiftY = iY;
	}

protected:
	// You can set the centre point for any rotation - by default it is the middle of the image
	int m_iRotationCentreX;
	int m_iRotationCentreY;

	// Variables used for drawing
	int m_iTransparencyColour;

	// If you set iBrightnessPercentage to other than 100 you can make the image darker or brighter (reduces or increases each of the RGB values). Note that each will be saturated at 255 though so going too bright can appear to change the colour.
	int m_iBrightnessPercentage; // Percentage brightness to use

	// If you set the m_iEverySecondPixelColour to other than - 1 then every alternate pixel will be coloured to the m_iEverySecondPixelColour colour.
	int m_iEverySecondPixelColour; 
	
	// Pixel colour for each third pixel - set to -1 to not use
	// If you set the m_iEveryThirdPixelColour to other than - 1 then every third pixel will be coloured to the m_iEveryThirdPixelColour colour.
	int m_iEveryThirdPixelColour; // Pixel colour for each third pixel - set to -1 to not use
							 
	// If you set the m_iEveryFourthPixelColour to other than - 1 then every fourth pixel will be coloured to the m_iEveryFourthPixelColour colour.
	int m_iEveryFourthPixelColour; // Pixel colour for each fourth pixel - set to -1 to not use
	
	// If you set iMergeColour to other than - 1 then any remaining pixel colours will be averaged with this one - e.g.to make it more grey ?
	int m_iMergeColour; // Merge colour - set to -1 to not use, otherwise this will average with current pixels

	// Merge with background. This is the percentage of the colour to keep - the rest will be the background colour.
	// Note also the brightness thing, so you can 'merge with black' rather than background by using that.
	int m_iMergePercentage;

	// You can shift the source point in the image if you want to draw a different part of the image
	int m_iSourceShiftX;
	int m_iSourceShiftY;
};
