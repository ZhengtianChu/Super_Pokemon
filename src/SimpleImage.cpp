#include "header.h"

#include "ImageManager.h"
#include "DrawingSurface.h"
#include "SimpleImage.h"

#include "BaseEngine.h"

#include "ImagePixelMapping.h"


// Singleton imagemanager if we want one rather than creating and maintaining one
ImageManager* ImageManager::g_pManager = nullptr;


// We use this object if we need a null object which does nothing.
// This is to avoid needing special cases in the function below.
static CroppedImagePixelMapping nullMapping;


// Set this to restrict drawing only to redraw window - which is faster when there are a lot of images, or for image drag
// but bad for student CW as they then need to understand that they need to set redraw bounds before image draw
// TODO Q: is it worth draw function having parameter to crop or not, and allow caller to decide?
// #define RESTRICT_TO_REDRAW_WINDOW 1





// Default constructor - image does not exist
SimpleImage::SimpleImage()
	: theData(nullptr)
	, m_pRescaleSurface(nullptr)
	, m_iTransparencyColour(-1)
{
}


// Standard constructor taking a pointer to the image which is being used
SimpleImage::SimpleImage(std::shared_ptr<RawImageData> theData)
	: theData(theData)
	, m_pRescaleSurface(nullptr)
	, m_iTransparencyColour(-1)
{
}


// Determine whether the image was actually loaded correctly and exists for use
bool SimpleImage::exists() const { return theData != nullptr; }





// NOTE: this is a static method! It needs no object to work on.
// Modify the size and position of image to draw to fit onto target
bool SimpleImage::adjustXYWidthHeight( DrawingSurface* pTarget,
	int& iXSource, int& iYSource,
	int& iXTargetVirtual, int& iYTargetVirtual,
	int& iSourceWidth, int& iSourceHeight) 
{
	//std::cout << "Image draw src x " << iXSource << " y " << iYSource
	//	<< " size " << iSourceWidth << "," << iSourceHeight << " dest " << iXTargetVirtual << "," << iYTargetVirtual << std::endl;
#if 1
	//
	// X coordinates:
	//

	int iXTargetReal = pTarget->convertVirtualToRealXPosition(iXTargetVirtual);
	int iYTargetReal = pTarget->convertVirtualToRealYPosition(iYTargetVirtual);

// Set this to restrict drawing only to redraw window - which is faster when there are a lot of images, or for image drag
// but bad for student CW as they then need to understand that they need to set redraw bounds before image draw
#if RESTRICT_TO_REDRAW_WINDOW
	int minTargetX = pTarget->getRealRedrawMinX(); 
	int maxTargetX = pTarget->getRealRedrawMaxX(); 
#else
	int minTargetX = 0;
	int maxTargetX = pTarget->getSurfaceWidth();
#endif

	int currentRHS = iXTargetReal +  pTarget->convertVirtualToRealXPosition( iSourceWidth ) - pTarget->convertVirtualToRealXPosition(0);
	if (currentRHS < minTargetX ) // Off the left of screen
		return false;
	if (iXTargetReal >= maxTargetX ) // Off the right of screen
		return false;
	//std::cout << "Image RHS x " << currentRHS << " with max " << maxTargetX << std::endl;

	//
	// Y coordinates:
	//

#if RESTRICT_TO_REDRAW_WINDOW
	int minTargetY = pTarget->getRealRedrawMinY();
	int maxTargetY = pTarget->getRealRedrawMaxY(); 
#else
	int minTargetY = 0;
	int maxTargetY = pTarget->getSurfaceHeight();
#endif

	int currentBottom = iYTargetReal + pTarget->convertVirtualToRealYPosition(iSourceHeight) - pTarget->convertVirtualToRealYPosition(0);
	//std::cout << "Image height " << iSourceHeight << " converted position " << pTarget->convertVirtualToRealYPosition(iSourceHeight)
	//		<< " - " << pTarget->convertVirtualToRealYPosition(0) 
	//		<< " = " << (pTarget->convertVirtualToRealYPosition(iSourceHeight) - pTarget->convertVirtualToRealYPosition(0)) << std::endl;
	if (currentBottom < minTargetY) // Off the top of screen
		return false;
	if (iYTargetReal >= maxTargetY) // Off the bottom of screen
		return false;

	//std::cout << "Image bottom x " << currentBottom << " with max " << maxTargetY << std::endl;

	//
	// Now check for trimming on X
	//

	if (iXTargetReal < minTargetX) // Need some trimming on left of image
	{
		int iRealTrim = minTargetX - iXTargetReal;
		int iVirtualTrim = pTarget->convertRealToVirtualXPosition(minTargetX) - pTarget->convertRealToVirtualXPosition(iXTargetReal);
		iXTargetVirtual += iVirtualTrim;
		iXSource += iVirtualTrim;
		iSourceWidth -= iVirtualTrim;
		//std::cout << "Trim left by real " << iRealTrim << " : virtual " << iVirtualTrim << std::endl;
	}

	if (currentRHS > maxTargetX) // Need some trimming on right of image
	{
		int iRealTrim = currentRHS - maxTargetX;
		int iVirtualTrim = pTarget->convertRealToVirtualXPosition(currentRHS) - pTarget->convertRealToVirtualXPosition(maxTargetX);
		iSourceWidth -= iVirtualTrim;
		//std::cout << "Trim right by real " << iRealTrim << " : virtual " << iVirtualTrim << std::endl;
	}

	//
	// And check for trimming on Y
	//

	if (iYTargetReal < minTargetY) // Need some trimming on top of image
	{
		int iRealTrim = minTargetY - iYTargetReal;
		int iVirtualTrim = pTarget->convertRealToVirtualYPosition(minTargetY)
					- pTarget->convertRealToVirtualYPosition(iYTargetReal);
		iYTargetVirtual += iVirtualTrim;
		iYSource += iVirtualTrim;
		iSourceHeight -= iVirtualTrim;
		//std::cout << "Trim top by real " << iRealTrim << " : virtual " << iVirtualTrim << std::endl;
	}

	if (currentBottom > maxTargetY) // Need some trimming on right of image
	{
		int iRealTrim = currentBottom - maxTargetY;
		int iVirtualTrim = pTarget->convertRealToVirtualYPosition(currentBottom)
					- pTarget->convertRealToVirtualYPosition(maxTargetY);
		iSourceHeight -= iVirtualTrim;
		//std::cout << "Trim bottom by real " << iRealTrim << " : virtual " << iVirtualTrim << std::endl;
	}
#endif
	//std::cout << "  Modified src x " << iXSource << " y " << iYSource << " size " << iSourceWidth << "," << iSourceHeight << " dest " << iXTargetVirtual << "," << iYTargetVirtual << std::endl;
	return true; // Possibly reduced size, but is visible
}




void SimpleImage::renderImage( DrawingSurface* pTarget, 
							int iXSource, int iYSource, 
							int iXTarget, int iYTarget, 
							int iWidth, int iHeight ) const
{
	if (!adjustXYWidthHeight(pTarget, iXSource, iYSource, iXTarget, iYTarget, iWidth, iHeight))
		return; // No need - off the screen

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			// Draw pixel unless it matches the transparency colour
			if ( ( m_iTransparencyColour == -1 ) || (theData->getRawPixelColour(iXS, iYS) != m_iTransparencyColour) )
				pTarget->setPixel(iXT, iYT, theData->getRawPixelColour(iXS, iYS));
			//((unsigned int *)pTarget->pixels)[iXT + iYT * iIntsPerScreenRow] = theData->getPixelColour( iXS, iYS );
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}



/*
Render image to screen at a specific position
Draw the image to the specified surface using a specified colour as the transparency colour.
Assumes that the bottom left pixel is the transparency colour if you do not provide one.
*/
void SimpleImage::renderImageWithMask(DrawingSurface* pTarget,
	int iXSource, int iYSource,
	int iXTarget, int iYTarget,
	int iWidth, int iHeight,
	int iMaskColor ) const
{
	if (!adjustXYWidthHeight(pTarget, iXSource, iYSource, iXTarget, iYTarget, iWidth, iHeight))
		return; // No need - off the screen

	if (iMaskColor == -1)
	{ // Assumes that the bottom right pixel is the transparency colour if you do not provide one.
		iMaskColor = theData->getRawPixelColour(theData->getWidth() - 1, theData->getHeight() - 1);
	}

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;

	for ( int iYOffset = 0 ; iYOffset < iHeight ; iYOffset++ )
	{
		iXS = iXSource; 
		iXT = iXTarget;
		for ( int iXOffset = 0 ; iXOffset < iWidth ; iXOffset++ )
		{
			int iPixel = theData->getRawPixelColour(iXS, iYS);
			if ( iPixel != iMaskColor)
				pTarget->setPixel(iXT, iYT, iPixel);
			iXS++;
			iXT++;
		}
		iYS++; 
		iYT++;
	}
}



// This was added as an example of how you can apply some semi-transparency if you wish
void SimpleImage::renderImageWithMaskAndTransparency(DrawingSurface* pTarget,
	int iXSource, int iYSource,
	int iXTarget, int iYTarget,
	int iWidth, int iHeight,
	int iMaskColour, int opacityPercentage) const
{
	if (!adjustXYWidthHeight(pTarget, iXSource, iYSource, iXTarget, iYTarget, iWidth, iHeight))
		return; // No need - off the screen

	int iXS, iYS = iYSource, iXT, iYT = iYTarget;

	for (int iYOffset = 0; iYOffset < iHeight; iYOffset++)
	{
		iXS = iXSource;
		iXT = iXTarget;
		for (int iXOffset = 0; iXOffset < iWidth; iXOffset++)
		{
			int iPixel = theData->getRawPixelColour(iXS, iYS);
			if (iPixel != iMaskColour)
			{
				int iBackground = pTarget->rawGetPixel(iXT, iYT);
				int iR = (0xff & (iPixel >> 16)) * opacityPercentage + (0xff & (iBackground >> 16)) * (100 - opacityPercentage);
				int iG = (0xff & (iPixel >> 8)) * opacityPercentage + (0xff & (iBackground >> 8)) * (100 - opacityPercentage);
				int iB = (0xff & iPixel) * opacityPercentage + (0xff & iBackground) * (100 - opacityPercentage);
				//std::cout << "R " << iR << " G " << iG << " B " << iB << std::endl;
				//std::cout << "R " << (iR / 100) << " G " << (iG / 100) << " B " << (iB / 100) << std::endl;
 				pTarget->setPixel(iXT, iYT, (((iR/100)&0xff)<<16) | (((iG/100) & 0xff) <<8) | ((iB/100)&0xff));
			}
			iXS++;
			iXT++;
		}
		iYS++;
		iYT++;
	}
}



/*
This render uses two blit operations, which may be quicker than the pixel copies, but is more restrictive.
First it will resize by blitting to an appropriately sized surface. Then it will copy from that surface, cropping to fit the visible screen area.
I had some problems making this work on some systems but it may be useful as long as it works for you.
iXDrawLocation, iYDrawLocation : location to draw to
iTargetWidth, iTargetHeight : size of target rectangle
iLeftInImage, iTopInImage : position in image
iWidthInImage, iHeightInImage : size of source rectangle
*/
void SimpleImage::renderImageBlit(BaseEngine* pEngine, DrawingSurface* pTarget,
	int iXDrawLocation, int iYDrawLocation, int iTargetWidth, int iTargetHeight,
	int iLeftInImage, int iTopInImage, int iWidthInImage, int iHeightInImage)
{
#if AVOID_BLIT
	renderImageWithMask( pTarget,
		iXDrawLocation, iYDrawLocation,
		iLeftInImage, iTopInImage,
		iWidthInImage, iHeightInImage);
#else

	int iRealX0 = pEngine->convertVirtualPixelToClickedXPosition(iXDrawLocation);
	int iRealY0 = pEngine->convertVirtualPixelToClickedYPosition(iYDrawLocation);
	int iRealX1 = pEngine->convertVirtualPixelToClickedXPosition(iXDrawLocation + iTargetWidth);
	int iRealY1 = pEngine->convertVirtualPixelToClickedYPosition(iYDrawLocation + iTargetHeight);

	if (iRealX1 < 0)
	{	// Off the edge of screen
		// std::cout << "left" << std::endl; 
		return;
	}
	if (iRealY1 < 0)
	{	// Off the top of screen
		//std::cout << "top" << std::endl; 
		return;
	}
	if (iRealX0 >= pTarget->getSurfaceWidth())
	{	// Off the edge of screen
		// std::cout << "right" << std::endl; 
		return;
	}
	if (iRealY0 >= pTarget->getSurfaceHeight())
	{	// Off the bottom of screen
		// std::cout << "bottom" << std::endl; 
		return;
	}

	::SDL_Rect rectSrc = { iLeftInImage, iTopInImage, iWidthInImage, iHeightInImage };
	::SDL_Rect rectDest = { iRealX0, iRealY0, iRealX1 - iRealX0, iRealY1 - iRealY0 };

	SDL_Surface* src = getTheData()->getSurface();
	SDL_Surface* dest = *pTarget;

	if ((iWidthInImage != (iRealX1 - iRealX0)) || (iHeightInImage != (iRealY1 - iRealY0)))
	{ // Need a rescale first
		int newWidth = iRealX1 - iRealX0;
		int newHeight = iRealY1 - iRealY0;
		if ((m_pRescaleSurface == nullptr)
			|| (newWidth > m_pRescaleSurface->w)
			|| (newHeight > m_pRescaleSurface->h))
		{ // Need a bigger surface as this does not exist or is not big enough
			if (m_pRescaleSurface != nullptr)
				SDL_FreeSurface(m_pRescaleSurface);
			m_pRescaleSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
		}
		else
		{
			// Clear current content, including the alpha, before the copy
			memset(m_pRescaleSurface->pixels, 0, m_pRescaleSurface->h * m_pRescaleSurface->w * 4/*32 bits*/);
		}

		::SDL_Rect rectTemp = { 0, 0, newWidth, newHeight };

		// Now copy to the resized surface before copying from that to the destination
		::SDL_BlitScaled(src, &rectSrc, m_pRescaleSurface, &rectTemp);

		// Now set it up so that the source is the temp one
		src = m_pRescaleSurface;
		rectSrc = rectTemp;
	}

	int minRedrawX = pTarget->getRealRedrawMinX();
	int maxRedrawX = pTarget->getRealRedrawMaxX();
	int minRedrawY = pTarget->getRealRedrawMinY();
	int maxRedrawY = pTarget->getRealRedrawMaxY();
	//std::cout << "Redraw rect: " << minRedrawX << "," << minRedrawY << " to " << maxRedrawX << "," << maxRedrawY << std::endl;
	::SDL_Rect rectClip = { minRedrawX, minRedrawY, maxRedrawX - minRedrawX, maxRedrawY - minRedrawY };

	// In case this was the screen surface which we will have locked, unlock it now 
	pTarget->mySDLTempUnlockSurface();
	::SDL_SetClipRect(dest, &rectClip);
	CHECK_BLIT_SURFACE(pTarget);
	::SDL_BlitSurface(src, &rectSrc, dest, &rectDest);
	::SDL_SetClipRect(dest, nullptr);
	pTarget->mySDLTempRelockSurface();
#endif
}



/*
Render the image by allowing a custom mapping function to determine which pixel to get from the image.
Needs to know: the base engine and drawing surface
The x and y location and the width and height on the surface to draw to
A ImagePixelMapping function to work out how to find the position in the image.
*/
void SimpleImage::renderImageApplyingMapping(BaseEngine* pEngine, DrawingSurface* pTarget,
	int iXDrawLocation, int iYDrawLocation, int iWidth, int iHeight,
	ImagePixelMapping* mapping) const
{
	if (mapping == nullptr)
		mapping = &nullMapping;

	int xStart = iXDrawLocation;
	int yStart = iYDrawLocation;
	int xMinOffset = 0;
	int xMaxOffset = iWidth;
	int yMinOffset = 0;
	int yMaxOffset = iHeight;

	int iLHS = pEngine->convertClickedToVirtualPixelXPosition(0);
	int iTop = pEngine->convertClickedToVirtualPixelYPosition(0);
	int iRHS = pEngine->convertClickedToVirtualPixelXPosition(pEngine->getWindowWidth());
	int iBottom = pEngine->convertClickedToVirtualPixelYPosition(pEngine->getWindowHeight());

	if (!pEngine->getRedrawAllScreen())
	{
		if ((xStart + iWidth < pEngine->getRedrawRectVirtualLeft())
			|| (xStart > pEngine->getRedrawRectVirtualRight())
			|| (yStart + iHeight < pEngine->getRedrawRectVirtualTop())
			|| (yStart > pEngine->getRedrawRectVirtualBottom()))
		{
			//std::cout << "Ignore piece outside redraw region" << std::endl;
			return;
		}
	}
	else
	{
		if ((xStart > iRHS)
			|| (xStart + iWidth < iLHS)
			|| (yStart > iBottom)
			|| (yStart + iHeight < iTop))
		{
			//std::cout << "Ignore piece off screen" << std::endl;
			return;
		}
	}

	if (xStart + xMinOffset < iLHS)
		xMinOffset = iLHS - xStart;
	if (yStart + yMinOffset + iWidth < iTop)
		xMinOffset = iLHS - xStart;

	if (xStart + xMaxOffset > iRHS)
		xMaxOffset = iRHS - xStart;
	if (yStart + yMaxOffset > iBottom)
		yMaxOffset = iBottom - yStart;

	// Note: this method can also handle partial redraw of screen, by reducing the size of the draw area:
	if (!pEngine->getRedrawAllScreen())
	{
		/*
		if ((xStart + xMaxOffset < pEngine->getRedrawRectVirtualLeft())
		|| (xStart + xMinOffset > pEngine->getRedrawRectVirtualRight())
		|| (yStart + yMaxOffset < pEngine->getRedrawRectVirtualTop())
		|| (yStart + yMinOffset > pEngine->getRedrawRectVirtualBottom()))
		{
		std::cout << "Ignore piece off display" << std::endl;
		return;
		}
		*/

		//std::cout << "Rect x  " << pEngine->getRedrawRectVirtualLeft()
		//	<< " to " << pEngine->getRedrawRectVirtualRight()
		//	<< " y " << pEngine->getRedrawRectVirtualTop()
		//	<< " to " << pEngine->getRedrawRectVirtualBottom() << std::endl;

		// Not redrawing entire screen...
		int iRedrawPos = pEngine->getRedrawRectVirtualLeft();
		if (iRedrawPos > xStart + xMinOffset)
			xMinOffset = iRedrawPos - xStart;
		iRedrawPos = pEngine->getRedrawRectVirtualTop();
		if (iRedrawPos > yStart + yMinOffset)
			yMinOffset = iRedrawPos - yStart;
		iRedrawPos = pEngine->getRedrawRectVirtualRight();
		if (iRedrawPos < xStart + xMaxOffset)
			xMaxOffset = iRedrawPos - xStart;
		iRedrawPos = pEngine->getRedrawRectVirtualBottom();
		if (iRedrawPos < yStart + yMaxOffset)
			yMaxOffset = iRedrawPos - yStart;
	}

	// Check for not visible
	if (xMaxOffset <= xMinOffset)
	{
		//std::cout << "Image not in x visible area" << std::endl;
		return;
	}
	if (yMaxOffset <= yMinOffset)
		return;

	double dx, dy;
	int iPixel;
	//std::cout << "Draw from " << xMinOffset << " to " << xMaxOffset << std::endl;
	for (int iYOffset = yMinOffset; iYOffset < yMaxOffset; ++iYOffset)
	{
		for (int iXOffset = xMinOffset; iXOffset < xMaxOffset; ++iXOffset)
		{
			dx = iXOffset;
			dy = iYOffset;
			if (mapping->mapCoordinates(dx, dy, *this))
			{ // It mapcoordinates returns false, do not draw the pixel
				int iColour = 0xffffff & theData->getRawPixelColour((int)(dx + 0.5), (int)(dy + 0.5));
				if (mapping->changePixelColour(iXDrawLocation + iXOffset, iYDrawLocation + iYOffset, iColour, pTarget))
				{ // If changePixelColour returns false it means we should not draw the pixel
					pTarget->setPixel(iXDrawLocation + iXOffset, iYDrawLocation + iYOffset, iColour );
				}
			}
		}
	}
}


