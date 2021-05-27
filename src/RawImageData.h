#pragma once

#include "header.h"

#include <map>
#include <string>
#include <memory>

/**********************
STOP!!!!
You don't need to understand most of what is in this file.
Instead go and look at the ImageObject class to see how to use it.
If you need more advanced features you can then look at SimpleImage.
If you still need more information then you can look here.
************************/




/*
Class which deals with the data for a specified loaded image
You should not need to interact with this class - instead ask the ImageManager to make you a SimpleImage object, or create your own class (or sub-class of SimpleImage) to use this class.
*/
class RawImageData
{
protected:
	int m_iWidth; // Width of the loaded image
	int m_iHeight; // Height of the loaded image
	unsigned int* m_aiPixels; // The pixel data
	const std::string filename; // Filename which was loaded

	// This is a pointer to the image surface which was loaded
	// Note: normally this is freed, but you can tell it not to free these when loaded - which may make blitting faster.
	SDL_Surface* m_pImagesurface = nullptr;

	// Do not allow copy constructor
	RawImageData(const RawImageData& src) = delete;

	// Do not allow assignment operator - which would cause problems with the internal data pointers.
	RawImageData& operator= (const RawImageData& src) = delete;

public:
	RawImageData(std::string filename)
		: m_iWidth(0), m_iHeight(0), m_aiPixels(NULL), filename(filename), m_pImagesurface(nullptr)
	{}

	~RawImageData()
	{
		//if (m_aiPixels != NULL)
		//	delete[] m_aiPixels;
		freeSurface();
	}

	SDL_Surface* getSurface() 
	{
		return m_pImagesurface;
	}

	void freeSurface()
	{
		if (m_pImagesurface != nullptr)
		{
			SDL_FreeSurface(m_pImagesurface);
			m_pImagesurface = nullptr;
			m_aiPixels = nullptr;
		}
		else if (m_aiPixels != NULL)
		{
			delete[] m_aiPixels;
			m_aiPixels = nullptr;
		}
	}



	// Ask for the width of the image
	int getWidth() const { return m_iWidth; }

	// Ask for the height of the image
	int getHeight() const { return m_iHeight; }

	// Get the colour of a specified pixel, by x and y position within the image
	int getRawPixelColour(int iX, int iY) const { return 0xffffff & (m_aiPixels[(iY%m_iHeight) * m_iWidth + (iX%m_iWidth)]); }

	// Function to work out the X coordinate to retrieve a point from given a rotation and cropping
	static int getXForRotationAndCrop(int iImageWidth, int iX, int iY, int iRotation, int iCropLeft, int iCropTop)
	{
		switch ((iRotation + 4) % 4)
		{
		default: return iX + iCropLeft;
		case 1: return iY + iCropTop;
		case 2: return iImageWidth - 1 - iX - iCropLeft;
		case 3: return iImageWidth - 1 - iY - iCropTop;
		}
	}

	// Function to work out the Y coordinate to retrieve a point from given a rotation and cropping
	static int getYForRotationAndCrop(int iImageHeight, int iX, int iY, int iRotation, int iCropLeft, int iCropTop)
	{
		switch ((iRotation + 4) % 4)
		{
		default: return iY + iCropTop;
		case 1: return iImageHeight - 1 - iCropLeft - iX;
		case 2: return iImageHeight - 1 - iY - iCropTop;
		case 3: return iX + iCropLeft;
		}
	}

	// Load the image with the specified filename
	bool loadImage(std::string strFileName, int num90degRotations = 0, int cropTop = 0, int cropBottom = 0, int cropLeft = 0, int cropRight = 0, bool cacheSurface = false)
	{
		const char* szFileName = strFileName.c_str();

		SDL_Surface* loadedSurface = IMG_Load(szFileName);
		if (loadedSurface != nullptr)
		{
			 // Make a 32 bit version
			SDL_Surface* tempSurface = SDL_CreateRGBSurface(
					SDL_SWSURFACE, loadedSurface->w, loadedSurface->h, 32,
					0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			//SDL_Rect dest = { cropLeft, cropTop, m_imagesurface->w - cropRight , m_imagesurface->h - cropBottom };
			SDL_BlitSurface(loadedSurface, NULL, tempSurface, NULL);
			SDL_FreeSurface(loadedSurface);

			if ((num90degRotations % 2) == 1)
			{
				m_iWidth = tempSurface->h - cropTop - cropBottom;
				m_iHeight = tempSurface->w - cropLeft - cropRight;
			}
			else
			{
				m_iWidth = tempSurface->w - cropLeft - cropRight;
				m_iHeight = tempSurface->h - cropTop - cropBottom;
			}

			// Free any current surface
			freeSurface();

			// Create the new surface for use
			m_pImagesurface = SDL_CreateRGBSurface( SDL_SWSURFACE, m_iWidth, m_iHeight, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			if (m_pImagesurface == nullptr)
			{
				printf("Error creating surface in RawImage::loadImage() !");
			}
			else
			{
				m_aiPixels = (unsigned int*)m_pImagesurface->pixels;

				int iIntsPerScreenRow = tempSurface->pitch / sizeof(unsigned int);

				for (int iY = 0; iY < m_iHeight; iY++)
				{
					for (int iX = 0; iX < m_iWidth; iX++)
					{
						// Calculate which pixel to pull from the image
						m_aiPixels[iY * m_iWidth + iX]
							= 0xff000000 | (((unsigned int *)tempSurface->pixels)[getXForRotationAndCrop(tempSurface->w, iX, iY, num90degRotations, cropLeft, cropTop)
								+ getYForRotationAndCrop(tempSurface->h, iX, iY, num90degRotations, cropLeft, cropTop) * iIntsPerScreenRow]);
						//std::cout << (m_aiPixels[iY * m_iWidth + iX] >> 24) << " "; // output alpha, just to check it - seems to be ff for everything
					}
				}

				return true;
			}
		}
		return false;
	}

	// Set full alpha for other pixels and zero alpha for these pixels
	void setTransparentColour(unsigned int colour)
	{
		colour = colour & 0xffffff; // Remove any alpha first
		for (int iY = 0; iY < m_iHeight; iY++)
			for (int iX = 0; iX < m_iWidth; iX++)
				if ( (m_aiPixels[iY * m_iWidth + iX] & 0xffffff) == colour)
					m_aiPixels[iY * m_iWidth + iX] = m_aiPixels[iY * m_iWidth + iX] & 0xffffff;
				else
					m_aiPixels[iY * m_iWidth + iX] = m_aiPixels[iY * m_iWidth + iX] | 0xff000000;
	}

	// Set all pixels to opaque - ff alpha
	void setOpaque()
	{
		unsigned int * p = m_aiPixels;
		for (int iPixel = 0; iPixel < m_iHeight*m_iWidth; iPixel++, p++)
			(*p)=(*p) | 0xff000000;
	}

	// Set all pixels to a specific alpha
	void setAlpha( unsigned int alpha )
	{
		unsigned int * p = m_aiPixels;
		alpha = alpha << 24;
		for (int iPixel = 0; iPixel < m_iHeight*m_iWidth; iPixel++, p++)
			(*p) = ((*p) & 0xffffff) | alpha;
	}

	// Has an image been loaded correctly
	bool isLoaded() const { return m_aiPixels != NULL; }

	// Get the filename of the image file which was loaded - or resized!
	const std::string& getImageURL() { return filename; }

};


