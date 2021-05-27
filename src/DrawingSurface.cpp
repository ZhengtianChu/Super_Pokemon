#include "header.h"


#if defined(_MSC_VER)
#include <SDL_syswm.h>
#endif

#include <math.h>

#include "BaseEngine.h"
#include "DisplayableObject.h"

#include "DrawingSurface.h"
#include "ExampleFilterPointClasses.h"

#include "FontManager.h"


#include <utility>





// Constant for PI - used in GetAngle below.
const double DrawingSurface::MY_PI = 3.14159265358979323846;


DrawingSurface::DrawingSurface(BaseEngine* m_pCreatorEngine)
	: m_pSDLSurface(nullptr), m_pCreatorEngine(m_pCreatorEngine), mySDLSurfaceLockedCount(0), m_pFilter(nullptr),
	checkBoundsForDrawings(false), m_iBoundsTop(0), m_iBoundsBottom(0), m_iBoundsLeft(0), m_iBoundsRight(0),
	m_bTempUnlocked(false)
{
	// Default to checking that points are on the screen unless anyone says otherwise (e.g. unless someone clears it)
	this->setDrawPointsFilter(m_pCreatorEngine);
}

/*
Determine the angle (in radians)  of point 2 from point 1.
Note that it also checks the quadrant, so you get a result from 0 to 2PI.
Implemented as a template so you can use with ints, doubles, etc
*/
double DrawingSurface::getAngle(double tX1, double tY1, double tX2, double tY2)
{
	double dAngle = MY_PI / 2; // Default when X1==X2
	if (tX1 != tX2)
		dAngle = atan((double)(tY2 - tY1) / (double)(tX2 - tX1));
	else if (tY2 < tY1)
		dAngle += MY_PI;

	if (tX2 < tX1)
		dAngle += MY_PI;

	return dAngle;
}

/*
Draw a string in the specified font to the specified surface (foreground or background).
*/
void DrawingSurface::drawFastString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont)
{
	if (pFont == NULL)
		pFont = m_pCreatorEngine->getDefaultFont();

	SDL_Color color = { (Uint8)((uiColour & 0xff0000) >> 16), (Uint8)((uiColour & 0xff00) >> 8), (Uint8)((uiColour & 0xff)), 0 };

	if ((pFont != NULL) && (pFont->getTTFFont() != NULL))
	{
		SDL_Surface *sText = TTF_RenderText_Solid(pFont->getTTFFont(), pText, color);
		SDL_Rect rcDest = { iX,iY,0,0 };

		mySDLTempUnlockSurface();
		SDL_BlitSurface(sText, NULL, m_pSDLSurface, &rcDest);
		mySDLTempRelockSurface();
		SDL_FreeSurface(sText);
	}
}

/*
Draw a scalable string in the specified font to the specified surface (foreground or background).
*/
void DrawingSurface::drawScalableString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont)
{
	if (pFont == NULL)
		pFont = m_pCreatorEngine->getDefaultFont();

	SDL_Color color = { (Uint8)((uiColour & 0xff0000) >> 16), (Uint8)((uiColour & 0xff00) >> 8), (Uint8)((uiColour & 0xff)), 0 };
	unsigned int testColor = 0;
	
	if ((pFont != NULL) && (pFont->getTTFFont() != NULL))
	{
		SDL_Surface *sText = TTF_RenderText_Solid(pFont->getTTFFont(), pText, color);
		SDL_Rect rcDest = { iX,iY,0,0 };

		for (int x = 0; x < sText->w; x++)
		{
			for (int y = 0; y < sText->h; y++)
			{
				// Get colour from the surface drawing to...
				switch (sText->format->BitsPerPixel)
				{
				case 8:
					testColor = *((Uint8*)sText->pixels + y * sText->pitch + x);
					break;
				case 16:
					testColor = *((Uint16*)sText->pixels + y * sText->pitch / 2 + x);
					break;
				case 32:
					testColor = *((Uint32*)sText->pixels + y * sText->pitch / 4 + x);
					break;
				default: // Should never happen
					testColor = 0;
					break;
				}				
				//testColor = ((unsigned int*)(sText->pixels))[y*sText->w + x];
				//printf("%08x ", testColor);
				if ( testColor )
					setPixel(x + iX, y + iY, uiColour);
			}
			//printf("\n");
		}

		SDL_FreeSurface(sText);
	}
}
/* Copy all of the background (e.g. tiles) to the foreground display. e.g. removing any object drawn on top. */
void DrawingSurface::copyEntireSurface( DrawingSurface* pFrom )
{
	memcpy(m_pSDLSurface->pixels, pFrom->m_pSDLSurface->pixels, sizeof(unsigned int) * getIntsPerWindowRow() * getSurfaceHeight());
//	::SDL_UpperBlitScaled
}


/* 
Copy some of the background onto the foreground, e.g. removing an object which was drawn on top.
Note that x, y, width and height are trimmed to fit inside THIS surface (that is being copied FROM) and that these are REAL not VIRTUAL positions. (Ignore this if you aren't using filters.)
*/
void DrawingSurface::copyRectangleFrom(DrawingSurface* pFrom, int iRealX, int iRealY, int iWidth, int iHeight, int iSourceOffsetX, int iSourceOffsetY)
{
	if (iRealX + iWidth < 0)
		return; // Nothing to do
	if (iRealY + iHeight < 0)
		return; // Nothing to do
	if (iRealX >= pFrom->getSurfaceWidth() )
		return; // Nothing to do
	if (iRealY >= pFrom->getSurfaceHeight() )
		return; // Nothing to do

	// Ensure position is within the bounds
	if (iRealX < 0) { iWidth += iRealX;  iRealX = 0; /*Note that iRealX was negative*/ }
	if (iRealY < 0) { iHeight += iRealY; iRealY = 0; /*Note that iRealY was negative*/ }
	// In case source offsets are -ve...
	if (iRealX + iSourceOffsetX < 0) { iWidth += (iRealX + iSourceOffsetX);  iRealX = -iSourceOffsetX; /*Note that offset was negative*/ }
	if (iRealY + iSourceOffsetY < 0) { iHeight += (iRealY + iSourceOffsetY); iRealY = -iSourceOffsetY; /*Note that offset was negative*/ }

	// Ensure width is within bounds
	if ((iRealX + iWidth) >= pFrom->getSurfaceWidth())
		iWidth = pFrom->getSurfaceWidth() - iRealX;
	// Ensure height is within bounds
	if ((iRealY + iHeight) >= pFrom->getSurfaceHeight())
		iHeight = pFrom->getSurfaceHeight() - iRealY;
	// In case source offsets were +ve...
	if (iRealX + iSourceOffsetX + iWidth >= pFrom->getSurfaceWidth() )    iWidth  = pFrom->getSurfaceWidth()  - iRealX - iSourceOffsetX;
	if (iRealY + iSourceOffsetY + iHeight >= pFrom->getSurfaceHeight() )  iHeight = pFrom->getSurfaceHeight() - iRealY - iSourceOffsetY;

	int iStartDest = iRealY * getIntsPerWindowRow() + iRealX;
	int iStartSrc = (iRealY + iSourceOffsetY) * getIntsPerWindowRow() + iRealX + iSourceOffsetX;
	int iIncrement = getIntsPerWindowRow() - iWidth;

	//std::cout << "Copy to " << iRealX << "," << iRealY << " from " << (iRealX + iSourceOffsetX) << "," << (iRealY + iSourceOffsetY) << " size " << iWidth << "," << iHeight << std::endl;

// Use drawing code or use blit?
#if AVOID_BLIT
	unsigned int * puiSource = ((unsigned int *)pFrom->m_pSDLSurface->pixels) + iStartSrc;
	unsigned int * puiDest = ((unsigned int *)m_pSDLSurface->pixels) + iStartDest;
	for (int i = 0; i < iHeight; i++)
	{
		// Copy a line
		for (int j = 0; j < iWidth; j++)
			*puiDest++ = *puiSource++;
		// Align on the next line
		puiSource += iIncrement;
		puiDest += iIncrement;
	}
#else
	SDL_Rect rectDest = { iRealX, iRealY, iWidth, iHeight };
	SDL_Rect rectSrc = { iRealX + iSourceOffsetX, iRealY + iSourceOffsetY, iWidth, iHeight };
	mySDLTempUnlockSurface();
	CHECK_BLIT_SURFACE(this);
	::SDL_BlitSurface(pFrom->m_pSDLSurface, &rectSrc, m_pSDLSurface, &rectDest);
	mySDLTempRelockSurface();
#endif
}

/*
Draw a vertical sided region.
If two points are the same then it is a triangle.
To do an arbitrary triangle, just draw two next to each other, one for left and one for right.
Basically to ensure that the triangle is filled (no pixels are missed) it is better to draw lines down each column than to try to draw at arbitrary angles.
This means that we have a shape where the starting and ending points are horizontally fixed (same x coordinate), and we are drawing a load of vertical lines from points on the top to points on the bottom of the region.
*/
void DrawingSurface::drawVerticalSidedRegion(
	double fX1, double fX2,// X positions
	double fY1a, double fY2a, // Start y positions for x1 and x2
	double fY1b, double fY2b, // End y positions for x1 and x2
	unsigned int uiColour)
{
	if ( checkBoundsForDrawings )
	{
		if (fX1 < m_iBoundsLeft && fX2 < m_iBoundsLeft)
			return; // No need to draw cos this is off the left of the display
		if (fX1 > m_iBoundsRight && fX2 > m_iBoundsRight)
			return; // No need to draw cos this is off the right of the display
		if (fY1a < m_iBoundsTop && fY1b < m_iBoundsTop && fY2a < m_iBoundsTop && fY2b < m_iBoundsTop)
			return; // No need to draw cos this is off the top of the display
		if (fY1a > m_iBoundsBottom && fY1b > m_iBoundsBottom && fY2a > m_iBoundsBottom && fY2b > m_iBoundsBottom)
			return; // No need to draw cos this is off the bottom of the display
	}

	// Ensure X1<  X2, otherwise steps will go wrong!
	// Switch the points if x and y are wrong way round
	if (fX2<  fX1) { std::swap(fX1, fX2); std::swap(fY1a, fY2a); std::swap(fY1b, fY2b); }

	int iXStart = (int)(fX1 + 0.5);
	int iXEnd = (int)(fX2 + 0.5);

	// If integer x positions are the same then avoid floating point inaccuracy problems by a special case
	if (iXStart == iXEnd)
	{
		int iYStart = (int)(fY1a + 0.5);
		int iYEnd = (int)(fY2a + 0.5);
		for (int iY = iYStart; iY <= iYEnd; iY++)
			setPixel(iXStart, iY, uiColour);
	}
	else
	{
		// Draw left hand side
		int iYStart = (int)(fY1a + 0.5);
		int iYEnd = (int)(fY1b + 0.5);
		if (iYStart > iYEnd) std::swap(iYStart, iYEnd);
		//printf( "Firstline %d to %d (%f to %f)\n", iYStart, iYEnd, fY1a, fY1b );
		for (int iY = iYStart; iY <= iYEnd; iY++)
			setPixel(iXStart, iY, uiColour);

		// Draw the middle
		for (int iX = iXStart + 1; iX< iXEnd; iX++)
		{
			double fYStart = fY1a + ((((double)iX) - fX1)*(fY2a - fY1a)) / (fX2 - fX1);
			double fYEnd = fY1b + ((((double)iX) - fX1)*(fY2b - fY1b)) / (fX2 - fX1);
			if (fYEnd<  fYStart) std::swap(fYStart, fYEnd);
			int iYStart2 = (int)(fYStart + 0.5);
			int iYEnd2 = (int)(fYEnd + 0.5);
			//printf( "Line from %d to %d (%f to %f)\n", iYStart, iYEnd, fYStart, fYEnd );
			for (int iY = iYStart2; iY <= iYEnd2; iY++)
				setPixel(iX, iY, uiColour);
		}

		// Draw right hand side
		iYStart = (int)(fY2a + 0.5);
		iYEnd = (int)(fY2b + 0.5);
		if (iYStart>  iYEnd) std::swap(iYStart, iYEnd);
		//printf( "Last line %d to %d (%f to %f)\n", iYStart, iYEnd, fY2a, fY2b );
		for (int iY = iYStart; iY <= iYEnd; iY++)
			setPixel(iXEnd, iY, uiColour);
	}
}


/*
Draw a triangle, as two vertical sided regions.
Try this on paper to see how it works.
Basically to ensure that the triangle is filled (no pixels are missed) it is better to draw lines down each column than to try to draw at arbitrary angles.
*/
void DrawingSurface::drawTriangle(
	double fX1, double fY1,
	double fX2, double fY2,
	double fX3, double fY3,
	unsigned int uiColour)
{
	if (checkBoundsForDrawings)
	{
		if (fX1 < m_iBoundsLeft && fX2 < m_iBoundsLeft && fX3 < m_iBoundsLeft)
			return; // No need to draw cos this is off the left of the display
		if (fX1 >= m_iBoundsRight && fX2 >= m_iBoundsRight && fX3 >= m_iBoundsRight)
			return; // No need to draw cos this is off the right of the display
		if (fY1 < m_iBoundsTop && fY2 < m_iBoundsTop && fY3 < m_iBoundsTop)
			return; // No need to draw cos this is off the top of the display
		if (fY1 >= m_iBoundsBottom && fY2 >= m_iBoundsBottom && fY3 >= m_iBoundsBottom )
			return; // No need to draw cos this is off the bottom of the display
	}

	// Ensure order is 1 2 3 from left to right
	if (fX1 > fX2) { std::swap(fX1, fX2); std::swap(fY1, fY2); } // Bigger of 1 and 2 is in position 2
	if (fX2 > fX3) { std::swap(fX2, fX3); std::swap(fY2, fY3); } // Bigger of new 2 and 3 is in position 3
	if (fX1 > fX2) { std::swap(fX1, fX2); std::swap(fY1, fY2); } // Bigger of 1 and new 2 is in position 2

	if (fX1 == fX2)
		drawVerticalSidedRegion(fX1, fX3, fY1, fY3, fY2, fY3, uiColour);
	else if (fX2 == fX3)
		drawVerticalSidedRegion(fX1, fX3, fY1, fY2, fY1, fY3, uiColour);
	else
	{
		// Split into two triangles. Find position on line 1-3 to split at
		double dSplitPointY = (double)fY1 +
			(((double)((fX2 - fX1)*(fY3 - fY1)))
				/ (double)(fX3 - fX1));
		drawVerticalSidedRegion(fX1, fX2, fY1, fY2, fY1, dSplitPointY, uiColour);
		drawVerticalSidedRegion(fX2, fX3, fY2, fY3, dSplitPointY, fY3, uiColour);
	}
}





/*
Draw a rectangle on the specified surface
This is probably the easiest function to do, hence is a special case.
*/
void DrawingSurface::drawRectangle(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
{
	if (checkBoundsForDrawings)
	{
		if (iX1 < m_iBoundsLeft && iX2 < m_iBoundsLeft)
			return; // No need to draw cos this is off the left of the display
		if (iX1 >= m_iBoundsRight && iX2 >= m_iBoundsRight)
			return; // No need to draw cos this is off the right of the display
		if (iY1 < m_iBoundsTop && iY2 < m_iBoundsTop)
			return; // No need to draw cos this is off the top of the display
		if (iY1 >= m_iBoundsBottom && iY2 >= m_iBoundsBottom)
			return; // No need to draw cos this is off the bottom of the display
	}

	if (iX2 < iX1) { int t = iX1; iX1 = iX2; iX2 = t; }
	if (iY2 < iY1) { int t = iY1; iY1 = iY2; iY2 = t; }

	for (int iX = iX1; iX <= iX2; iX++)
		for (int iY = iY1; iY <= iY2; iY++)
			setPixel(iX, iY, uiColour);
}

/*
Draw an oval on the specified surface.
This is drawn by checking each pixel inside a bounding rectangle to see whether it should be filled or not.
There are probably faster ways to do this!
*/
void DrawingSurface::drawOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
{
	if (checkBoundsForDrawings)
	{
		if (iX1 < m_iBoundsLeft && iX2 < m_iBoundsLeft )
			return; // No need to draw cos this is off the left of the display
		if (iX1 >= m_iBoundsRight && iX2 >= m_iBoundsRight )
			return; // No need to draw cos this is off the right of the display
		if (iY1 < m_iBoundsTop && iY2 < m_iBoundsTop )
			return; // No need to draw cos this is off the top of the display
		if (iY1 >= m_iBoundsBottom && iY2 >= m_iBoundsBottom )
			return; // No need to draw cos this is off the bottom of the display
	}

	if (iX2 < iX1) { int t = iX1; iX1 = iX2; iX2 = t; }
	if (iY2 < iY1) { int t = iY1; iY1 = iY2; iY2 = t; }

	double fCentreX = ((double)(iX2 + iX1)) / 2.0;
	double fCentreY = ((double)(iY2 + iY1)) / 2.0;
	double fXFactor = (double)((iX2 - iX1) * (iX2 - iX1)) / 4.0;
	double fYFactor = (double)((iY2 - iY1) * (iY2 - iY1)) / 4.0;
	double fDist;

	for (int iX = iX1; iX <= iX2; iX++)
		for (int iY = iY1; iY <= iY2; iY++)
		{
			fDist = ((double)iX - fCentreX) * ((double)iX - fCentreX) / fXFactor
				+ ((double)iY - fCentreY) * ((double)iY - fCentreY) / fYFactor;
			if (fDist <= 1.0)
				setPixel(iX, iY, uiColour);
		}
}

/*
Draw an oval on the specified surface.
This is a REALLY slow version of the above function, which just draws a perimeter pixel.
Probably there is a much better way to do this.
*/
void DrawingSurface::drawHollowOval(int iX1, int iY1, int iX2, int iY2, int iX3, int iY3, int iX4, int iY4, unsigned int uiColour)
{
	if (checkBoundsForDrawings)
	{
		if (iX1 < m_iBoundsLeft && iX2 < m_iBoundsLeft && iX3 < m_iBoundsLeft && iX4 < m_iBoundsLeft)
			return; // No need to draw cos this is off the left of the display
		if (iX1 >= m_iBoundsRight && iX2 >= m_iBoundsRight && iX3 >= m_iBoundsRight && iX4 >= m_iBoundsRight)
			return; // No need to draw cos this is off the right of the display
		if (iY1 < m_iBoundsTop && iY2 < m_iBoundsTop && iY3 < m_iBoundsTop && iY4 < m_iBoundsTop)
			return; // No need to draw cos this is off the top of the display
		if (iY1 >= m_iBoundsBottom && iY2 >= m_iBoundsBottom && iY3 >= m_iBoundsBottom && iY4 >= m_iBoundsBottom)
			return; // No need to draw cos this is off the bottom of the display
	}

	if (iX2 < iX1) std::swap(iX1, iX2);
	if (iY2 < iY1) std::swap(iY1, iY2);
	if (iX4 < iX3) std::swap(iX3, iX4);
	if (iY4 < iY3) std::swap(iY3, iY4);

	double fCentreX1 = ((double)(iX2 + iX1)) / 2.0;
	double fCentreY1 = ((double)(iY2 + iY1)) / 2.0;
	double fXFactor1 = (double)((iX2 - iX1) * (iX2 - iX1)) / 4.0;
	double fYFactor1 = (double)((iY2 - iY1) * (iY2 - iY1)) / 4.0;
	double fCentreX2 = ((double)(iX4 + iX3)) / 2.0;
	double fCentreY2 = ((double)(iY4 + iY3)) / 2.0;
	double fXFactor2 = (double)((iX4 - iX3) * (iX4 - iX3)) / 4.0;
	double fYFactor2 = (double)((iY4 - iY3) * (iY4 - iY3)) / 4.0;
	double fDist1, fDist2;

	for (int iX = iX1; iX <= iX2; iX++)
		for (int iY = iY1; iY <= iY2; iY++)
		{
			fDist1 = ((double)iX - fCentreX1) * ((double)iX - fCentreX1) / fXFactor1
				+ ((double)iY - fCentreY1) * ((double)iY - fCentreY1) / fYFactor1;
			fDist2 = ((double)iX - fCentreX2) * ((double)iX - fCentreX2) / fXFactor2
				+ ((double)iY - fCentreY2) * ((double)iY - fCentreY2) / fYFactor2;
			if ((fDist1 <= 1.0) && (fDist2 >= 1.0))
				setPixel(iX, iY, uiColour);
		}
}


/*
Draw a line on the specified surface.
For each horizontal pixel position, work out which vertical position at which to colour in the pixel.
*/
void DrawingSurface::drawLine(double fX1, double fY1, double fX2, double fY2,
	unsigned int uiColour)
{
	if (checkBoundsForDrawings)
	{
		if (fX1 < m_iBoundsLeft && fX2 < m_iBoundsLeft )
			return; // No need to draw cos this is off the left of the display
		if (fX1 >= m_iBoundsRight && fX2 >= m_iBoundsRight )
			return; // No need to draw cos this is off the right of the display
		if (fY1 < m_iBoundsTop && fY2 < m_iBoundsTop )
			return; // No need to draw cos this is off the top of the display
		if (fY1 >= m_iBoundsBottom && fY2 >= m_iBoundsBottom )
			return; // No need to draw cos this is off the bottom of the display
	}

	int iX1 = (int)(fX1 + 0.5);
	int iX2 = (int)(fX2 + 0.5);
	int iY1 = (int)(fY1 + 0.5);
	int iY2 = (int)(fY2 + 0.5);

	int iSteps = (iX2 - iX1);
	if (iSteps < 0) iSteps = -iSteps;
	if (iY2 > iY1) iSteps += (iY2 - iY1); else iSteps += (iY1 - iY2);
	iSteps += 2;

	double fXStep = ((double)(fX2 - fX1)) / iSteps;
	double fYStep = ((double)(fY2 - fY1)) / iSteps;

	for (int i = 0; i <= iSteps; i++)
	{
		setPixel((int)(0.5 + fX1 + fXStep * i), (int)(0.5 + fY1 + fYStep * i), uiColour);
	}
}



/*
Draw a thick line on the specified surface.
This is like the DrawLine() function, but has a width to the line.
*/
void DrawingSurface::drawThickLine(double fX1, double fY1, double fX2, double fY2,
	unsigned int uiColour, int iThickness)
{
	if (checkBoundsForDrawings)
	{
		if (fX1 < m_iBoundsLeft && fX2 < m_iBoundsLeft )
			return; // No need to draw cos this is off the left of the display
		if (fX1 >= m_iBoundsRight && fX2 >= m_iBoundsRight )
			return; // No need to draw cos this is off the right of the display
		if (fY1 < m_iBoundsTop && fY2 < m_iBoundsTop )
			return; // No need to draw cos this is off the top of the display
		if (fY1 >= m_iBoundsBottom && fY2 >= m_iBoundsBottom )
			return; // No need to draw cos this is off the bottom of the display
	}

	if (iThickness < 2)
	{ // Go to the quicker draw function
		drawLine(fX1, fY1, fX2, fY2, uiColour);
		return;
	}

	double fAngle1 = getAngle(fX1, fY1, fX2, fY2);
	double fAngle1a = fAngle1 - ((5 * M_PI) / 4.0);
	double fAngle1b = fAngle1 + ((5 * M_PI) / 4.0);
	double fRectX1 = fX1 + iThickness * cos(fAngle1a) * 0.5;
	double fRectY1 = fY1 + iThickness * sin(fAngle1a) * 0.5;
	double fRectX2 = fX1 + iThickness * cos(fAngle1b) * 0.5;
	double fRectY2 = fY1 + iThickness * sin(fAngle1b) * 0.5;

	double fAngle2 = fAngle1 + M_PI;
	double fAngle2a = fAngle2 - ((5 * M_PI) / 4.0);
	double fAngle2b = fAngle2 + ((5 * M_PI) / 4.0);
	double fRectX3 = fX2 + iThickness * cos(fAngle2a) * 0.5;
	double fRectY3 = fY2 + iThickness * sin(fAngle2a) * 0.5;
	double fRectX4 = fX2 + iThickness * cos(fAngle2b) * 0.5;
	double fRectY4 = fY2 + iThickness * sin(fAngle2b) * 0.5;

	drawTriangle(fRectX1, fRectY1, fRectX2, fRectY2, fRectX3, fRectY3, uiColour);
	drawTriangle(fRectX3, fRectY3, fRectX4, fRectY4, fRectX1, fRectY1, uiColour);
}



/*
Draw a filled polygon on the specified surface.
The trick here is to not fill in any bits that shouldn't be filled, but to not miss anything.
This was a pain to write to be honest, and there is a chance it may have an error I have not found so far.
*/
void DrawingSurface::drawPolygon(
	int iPoints, double* pXArray, double* pYArray,
	unsigned int uiColour)
{
	if (iPoints == 1)
	{
		setPixel( (int)(pXArray[0]+0.5), (int)(pYArray[0] + 0.5), uiColour);
		return;
	}

	if (iPoints == 2)
	{
		drawLine(pXArray[0], pYArray[0], pXArray[1], pYArray[1], uiColour);
		return;
	}

	/*	if ( iPoints == 3 )
	{
	printf( "Draw triangle for points 0, 1, 2 of %d available\n", iPoints );
	DrawTriangle( pXArray[0], pYArray[0], pXArray[1], pYArray[1], pXArray[2], pYArray[2],
	uiColour, pTarget );
	return;
	}
	*/

	// Otherwise attempt to eliminate a point by filling the polygon, then call this again
	double fXCentre, fYCentre; //fX1, fX2, fX3, fY1, fY2, fY3;
	int i2, i3;
	double fAngle1, fAngle2, fAngle3;

	for (int i1 = 0; i1 < iPoints; i1++)
	{
		i2 = i1 + 1; if (i2 >= iPoints) i2 -= iPoints;
		i3 = i1 + 2; if (i3 >= iPoints) i3 -= iPoints;
		fXCentre = (pXArray[i1] + pXArray[i2] + pXArray[i3]) / 3.0;
		fYCentre = (pYArray[i1] + pYArray[i2] + pYArray[i3]) / 3.0;
		fAngle1 = getAngle(fXCentre, fYCentre, pXArray[i1], pYArray[i1]);
		fAngle2 = getAngle(fXCentre, fYCentre, pXArray[i2], pYArray[i2]);
		fAngle3 = getAngle(fXCentre, fYCentre, pXArray[i3], pYArray[i3]);
		// Now work out the relative angle positions and make sure all are positive
		fAngle2 -= fAngle1; if (fAngle2 < 0) fAngle2 += 2 * M_PI;
		fAngle3 -= fAngle1; if (fAngle3 < 0) fAngle3 += 2 * M_PI;
		if (fAngle2 < fAngle3)
		{ // Then points are in clockwise order so central one can be eliminated as long as we don't
		  // fill an area that we shouldn't
			bool bPointIsWithinTriangle = false;
			if (iPoints > 3)
			{ // Need to check that there isn't a point within the area - for convex shapes
				double fLineAngle12 = getAngle(pXArray[i1], pYArray[i1], pXArray[i2], pYArray[i2]);
				if (fLineAngle12 < 0)
					fLineAngle12 += M_PI * 2.0;
				double fLineAngle23 = getAngle(pXArray[i2], pYArray[i2], pXArray[i3], pYArray[i3]);
				if (fLineAngle23 < 0)
					fLineAngle23 += M_PI * 2.0;
				double fLineAngle31 = getAngle(pXArray[i3], pYArray[i3], pXArray[i1], pYArray[i1]);
				if (fLineAngle31 < 0)
					fLineAngle31 += M_PI * 2.0;

				for (int i = i3 + 1; i != i1; i++)
				{
					if (i >= iPoints)
					{
						i = 0;
						if (i1 == 0)
							break; // From the for loop - finished
					}

					// Otherwise we need to work out whether point i is to right of line  i3 to i1
					double fPointAngle1 = getAngle(pXArray[i1], pYArray[i1], pXArray[i], pYArray[i]);
					if (fPointAngle1 < 0)
						fPointAngle1 += M_PI * 2.0;
					fPointAngle1 -= fLineAngle12;
					if (fPointAngle1 < 0)
						fPointAngle1 += M_PI * 2.0;

					double fPointAngle2 = getAngle(pXArray[i2], pYArray[i2], pXArray[i], pYArray[i]);
					if (fPointAngle2 < 0)
						fPointAngle2 += M_PI * 2.0;
					fPointAngle2 -= fLineAngle23;
					if (fPointAngle2 < 0)
						fPointAngle2 += M_PI * 2.0;

					double fPointAngle3 = getAngle(pXArray[i3], pYArray[i3], pXArray[i], pYArray[i]);
					if (fPointAngle3 < 0)
						fPointAngle3 += M_PI * 2.0;
					fPointAngle3 -= fLineAngle31;
					if (fPointAngle3 < 0)
						fPointAngle3 += M_PI * 2.0;

					if ((fPointAngle1 < M_PI) && (fPointAngle2 < M_PI) && (fPointAngle3 < M_PI))
						bPointIsWithinTriangle = true;
				}
			}

			if (!bPointIsWithinTriangle)
			{// If not then try the next position
				//printf("Draw for points %d, %d, %d of %d available\n", i1, i2, i3, iPoints);
				drawTriangle(pXArray[i1], pYArray[i1], pXArray[i2], pYArray[i2],
					pXArray[i3], pYArray[i3], /*GetColour(iPoints)*/uiColour);
				// Remove the point i2 and then recurse			
				for (int i = i2; i < (iPoints - 1); i++)
				{
					//printf("\tCopy point %d to %d\n", i + 1, i);
					pXArray[i] = pXArray[i + 1];
					pYArray[i] = pYArray[i + 1];
				}
				if (iPoints > 3)
					drawPolygon(iPoints - 1, pXArray, pYArray, uiColour);
				return; // Done
			}
		}
	}
}

/* Added in 2014 since it was used in the GroundMovement playback program so may be useful elsewhere too, but students can ignore this */
void DrawingSurface::drawShortenedArrow(int iX1, int iY1, int iX2, int iY2,
	int iShortenedStart, int iShortenedEnd,
	unsigned int uiColour, int iThickness,
	int iHeadSize)
{
	if (checkBoundsForDrawings)
	{
		if (iX1 < m_iBoundsLeft && iX2 < m_iBoundsLeft)
			return; // No need to draw cos this is off the left of the display
		if (iX1 >= m_iBoundsRight && iX2 >= m_iBoundsRight)
			return; // No need to draw cos this is off the right of the display
		if (iY1 < m_iBoundsTop && iY2 < m_iBoundsTop)
			return; // No need to draw cos this is off the top of the display
		if (iY1 >= m_iBoundsBottom && iY2 >= m_iBoundsBottom)
			return; // No need to draw cos this is off the bottom of the display
	}

	double dAngle1 = getAngle(iX1, iY1, iX2, iY2);
	double dAngle2 = dAngle1 + M_PI;

	double dX1 = iX1 + iShortenedStart * cos(dAngle1);
	double dY1 = iY1 + iShortenedStart * sin(dAngle1);
	double dX2 = iX2 + iShortenedEnd * cos(dAngle2);
	double dY2 = iY2 + iShortenedEnd * sin(dAngle2);

	// First draw the line
	if (iThickness < 2)
	{ // Go to the quicker draw function
		drawLine(dX1, dY1, dX2, dY2, uiColour);
	}
	else
	{
		double dX1l = iX1 + iShortenedStart * cos(dAngle1);
		double dY1l = iY1 + iShortenedStart * sin(dAngle1);
		double dX2l = iX2 + (iShortenedEnd + iThickness * 1.5) * cos(dAngle2);
		double dY2l = iY2 + (iShortenedEnd + iThickness * 1.5) * sin(dAngle2);

		drawThickLine(dX1l, dY1l, dX2l, dY2l, uiColour, iThickness);
	}

	// Now draw the arrow head.
	// Need three points - one is end of line and others are at 60 degrees from it.

	drawTriangle(
		dX2, dY2,
		dX2 + iHeadSize * cos(dAngle2 + M_PI / 6.0), dY2 + iHeadSize * sin(dAngle2 + M_PI / 6.0),
		dX2 + iHeadSize * cos(dAngle2 - M_PI / 6.0), dY2 + iHeadSize * sin(dAngle2 - M_PI / 6.0),
		uiColour);
}

/* Added in 2014 since it was used in the GroundMovement playback program so may be useful elsewhere too, but students can ignore this */
void DrawingSurface::drawShortenedLine(int iX1, int iY1, int iX2, int iY2, int iShortenedStart, int iShortenedEnd, unsigned int uiColour, int iThickness)
{
	if (checkBoundsForDrawings)
	{
		if (iX1 < m_iBoundsLeft && iX2 < m_iBoundsLeft )
			return; // No need to draw cos this is off the left of the display
		if (iX1 >= m_iBoundsRight && iX2 >= m_iBoundsRight )
			return; // No need to draw cos this is off the right of the display
		if (iY1 < m_iBoundsTop && iY2 < m_iBoundsTop )
			return; // No need to draw cos this is off the top of the display
		if (iY1 >= m_iBoundsBottom && iY2 >= m_iBoundsBottom )
			return; // No need to draw cos this is off the bottom of the display
	}

	double dAngle1 = getAngle(iX1, iY1, iX2, iY2);
	double dAngle2 = dAngle1 + M_PI;

	// First draw the line
	if (iThickness < 2)
	{ // Go to the quicker draw function
		double dX1 = iX1 + iShortenedStart * cos(dAngle1);
		double dY1 = iY1 + iShortenedStart * sin(dAngle1);
		double dX2 = iX2 + iShortenedEnd * cos(dAngle2);
		double dY2 = iY2 + iShortenedEnd * sin(dAngle2);

		drawLine(dX1, dY1, dX2, dY2, uiColour);
	}
	else
	{
		double dX1l = iX1 + iShortenedStart * cos(dAngle1);
		double dY1l = iY1 + iShortenedStart * sin(dAngle1);
		double dX2l = iX2 + iShortenedEnd * cos(dAngle2);
		double dY2l = iY2 + iShortenedEnd * sin(dAngle2);

		//if (iX1 == iX2)
		//	printf("Draw shortened line %d,%d to %d,%d shortened by %d,%d is %f,%f %f,%f\n", iX1, iY1, iX2, iY2, iShortenedStart, iShortenedEnd, dX1l, dY1l, dX2l, dY2l);

		drawThickLine(dX1l, dY1l, dX2l, dY2l, uiColour, iThickness);
	}
}

// Get the minimum x coordinate which would be within the redraw region
int DrawingSurface::getVirtualRedrawMinX()
{
	int iMin = convertRealToVirtualXPosition(0);
	if (this->m_pCreatorEngine->getRedrawAllScreen())
		return iMin;

 	int iRedrawMin = this->m_pCreatorEngine->getRedrawRectVirtualLeft();
	if (iRedrawMin > iMin)
		iMin = iRedrawMin;
	return iMin;
}

int DrawingSurface::getVirtualRedrawMinY()
{
	int iMin = convertRealToVirtualYPosition(0);
	if (this->m_pCreatorEngine->getRedrawAllScreen())
		return iMin;

	int iRedrawMin = this->m_pCreatorEngine->getRedrawRectVirtualTop();
	if (iRedrawMin > iMin)
		iMin = iRedrawMin;
	return iMin;
}

int DrawingSurface::getVirtualRedrawMaxX()
{
	int iMax = convertRealToVirtualXPosition(getSurfaceWidth());
	if (this->m_pCreatorEngine->getRedrawAllScreen())
		return iMax;

	int iRedrawMax = this->m_pCreatorEngine->getRedrawRectVirtualRight();
	if (iMax > iRedrawMax )
		iMax = iRedrawMax;
	return iMax;
}

int DrawingSurface::getVirtualRedrawMaxY()
{
	int iMax = convertRealToVirtualYPosition(getSurfaceHeight());
	if (this->m_pCreatorEngine->getRedrawAllScreen())
		return iMax;

	int iRedrawMax = this->m_pCreatorEngine->getRedrawRectVirtualBottom();
	if (iMax > iRedrawMax )
		iMax = iRedrawMax;
	return iMax;
}

// Get the minimum x coordinate which would be within the redraw region
int DrawingSurface::getRealRedrawMinX()
{
	if (m_pCreatorEngine->getRedrawAllScreen())
		return 0;
	int iMin = 0;
	int iRedrawMin = this->m_pCreatorEngine->getRedrawRectRealLeft();
	if (iRedrawMin > iMin)
		iMin = iRedrawMin;
	return iMin;
}

int DrawingSurface::getRealRedrawMinY()
{
	if (m_pCreatorEngine->getRedrawAllScreen())
		return 0;
	int iMin = 0;
	int iRedrawMin = this->m_pCreatorEngine->getRedrawRectRealTop();
	if (iRedrawMin > iMin)
		iMin = iRedrawMin;
	return iMin;
}

int DrawingSurface::getRealRedrawMaxX()
{
	if (m_pCreatorEngine->getRedrawAllScreen())
		return getSurfaceWidth();
	int iMax = getSurfaceWidth();
	int iRedrawMax = this->m_pCreatorEngine->getRedrawRectRealRight();
	if (iMax > iRedrawMax )
		iMax = iRedrawMax;
	return iMax;
}

int DrawingSurface::getRealRedrawMaxY()
{
	if (m_pCreatorEngine->getRedrawAllScreen())
		return getSurfaceHeight();
	int iMax = getSurfaceHeight();
	int iRedrawMax = this->m_pCreatorEngine->getRedrawRectRealBottom();
	if (iMax > iRedrawMax )
		iMax = iRedrawMax;
	return iMax;
}


/*
Draw an oval on the specified surface - without any scaling or checking etc - BE CAREFUL WITH THIS!.
*/
void DrawingSurface::rawDrawOval(int iX1, int iY1, int iX2, int iY2, unsigned int uiColour)
{
	if (iX2 < iX1) { int t = iX1; iX1 = iX2; iX2 = t; }
	if (iY2 < iY1) { int t = iY1; iY1 = iY2; iY2 = t; }

	double fCentreX = ((double)(iX2 + iX1)) / 2.0;
	double fCentreY = ((double)(iY2 + iY1)) / 2.0;
	double fXFactor = (double)((iX2 - iX1) * (iX2 - iX1)) / 4.0;
	double fYFactor = (double)((iY2 - iY1) * (iY2 - iY1)) / 4.0;
	double fDist;

	for (int iX = iX1; iX <= iX2; iX++)
		for (int iY = iY1; iY <= iY2; iY++)
		{
			fDist = ((double)iX - fCentreX) * ((double)iX - fCentreX) / fXFactor
				+ ((double)iY - fCentreY) * ((double)iY - fCentreY) / fYFactor;
			if (fDist <= 1.0)
				rawSetPixel(iX, iY, uiColour);
		}
}
