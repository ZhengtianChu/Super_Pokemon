#pragma once

#include "header.h"

/**********************
STOP!!!!
You don't need to understand most of what is in this file.
Instead go and look at the ImageObject class to see how to use objects of this class.
If you need more advanced features you can come back and look at SimpleImage.
************************/

#include <memory>

#include "RawImageData.h"

class DrawingSurface;
class BaseEngine;
class ImagePixelMapping;



/*
SimpleImage version
SimpleImage requires that you specify the details of the drawing when you call the render functions.
ImageRenderer maintains this information within the object.
*/
class SimpleImage
{
public:
	// Default constructor - image does not exist
	SimpleImage();

	// Standard constructor taking a pointer to the image which is being used
	SimpleImage(std::shared_ptr<RawImageData> theData);

	~SimpleImage()
	{
		if (m_pRescaleSurface != nullptr)
		{
			SDL_FreeSurface(m_pRescaleSurface);
			m_pRescaleSurface = nullptr;
		}
	}


	// Determine whether the image was actually loaded correctly and exists for use
	bool exists() const;

	// Ask for the width of the image
	int getWidth() const { return theData->getWidth(); }

	// Ask for the height of the image
	int getHeight() const { return theData->getHeight(); }

	// Get the colour of a specified pixel, by x and y position
	int getPixelColour(int x, int y) const { return theData->getRawPixelColour(x, y); }

	// Draw the image to the specified surface - just copying pixel colours with no extra work
	void renderImage(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight) const;

	// Draw the image to the specified surface using a specified colour as the transparency colour.
	void renderImageWithMask(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight,
		int iMaskColour = -1) const;



	// Draw the image to the specified surface using a specified colour as the transparency colour and a level of transparency.
	void renderImageWithMaskAndTransparency(DrawingSurface* pTarget,
		int iXSource, int iYSource,
		int iXTarget, int iYTarget,
		int iWidth, int iHeight,
		int iMaskColour, int opacityPercentage ) const;


	/*
	This render uses a blit operation if possible.
	I had some problems making this work on some systems but it may be useful to you - as long as it works - because it may be faster than other methods of drawing one pixel at a time.
	iXDrawLocation, iYDrawLocation : location to draw to
	iTargetWidth, iTargetHeight : size of target rectangle
	iLeftInImage, iTopInImage : position in image
	iWidthInImage, iHeightInImage : size of source rectangle
	Transparency colour (i.e. alpha values) must have been set before calling this method if you need it. See setTransparencyColour()
	*/
	void renderImageBlit(BaseEngine* pEngine, DrawingSurface* pTarget,
		int iXDrawLocation, int iYDrawLocation, int iTargetWidth, int iTargetHeight,
		int iLeftInImage, int iTopInImage, int iWidthInImage, int iHeightInImage);


	// Modify the size and position of image to draw to fit onto target
	static bool adjustXYWidthHeight(DrawingSurface* pTarget,
		int& iXSource, int& iYSource,
		int& iXTarget, int& iYTarget,
		int& iWidth, int& iHeight);


	// Set a specific colour to be the transparancy colour - the transparancy colour is used by renderImageWithMaskAndTransparency() and renderImageBlit()
	void setTransparencyColour(int colour)
	{
		m_iTransparencyColour = colour;
		// And update the alpha values for transparency in case we blit the surface later...
		if (m_iTransparencyColour == -1)
			getTheData()->setOpaque();
		else
			getTheData()->setTransparentColour(m_iTransparencyColour);
	}

	// Should not need this. Provide a pointer to new raw image data to be stored in here.
	void setRawImageData(std::shared_ptr<RawImageData> data)
	{
		theData = data;
	}

	// Should not need this. Retrieve a pointer to the raw image data stored in here.
	std::shared_ptr<RawImageData> getRawImageData()
	{
		return theData;
	}

	// Get the raw data shared pointer - do not use unless you know what you are doing!
	const std::shared_ptr<RawImageData>& getTheData() const { return theData; }

	// Get URL for current image
	const std::string getImageURL() { if (theData == nullptr) return ""; else return theData->getImageURL(); }

	/*
	Render the image by allowing a custom mapping function to determine which pixel to get from the image.
	Needs to know: the base engine and drawing surface
	The x and y location and the width and height on the surface to draw to
	A ImagePixelMapping function to work out how to find the position in the image.
	*/
	void renderImageApplyingMapping(BaseEngine* pEngine, DrawingSurface* pTarget,
		int iXDrawLocation, int iYDrawLocation, int iWidth, int iHeight,
		ImagePixelMapping* mapping) const;


protected:
	// Transparency colour : -1 means none
	int m_iTransparencyColour;

	// Shared pointer to the actual image data
	std::shared_ptr<RawImageData> theData;

	// Surface used only for rescaling when using a blit. Cached to avoid continuous recreation.
	SDL_Surface* m_pRescaleSurface;
};



