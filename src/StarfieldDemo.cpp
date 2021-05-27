#include "header.h"
#include "StarfieldDemo.h"



StarfieldDemo::StarfieldDemo()
	: m_iOffset(0)
{
	m_iDefaultUpdatePeriod = 50; // Change default update rate to 50ms
}


StarfieldDemo::~StarfieldDemo()
{
}

void StarfieldDemo::virtSetupBackgroundBuffer()
{
	fillBackground(0);

	for (int iX = 0; iX < getWindowWidth(); iX++)
		for (int iY = 0; iY < this->getWindowHeight(); iY++)
			switch (rand() % 100)
			{
			case 0: setBackgroundPixel(iX, iY, 0xFF0000); break;
			case 1: setBackgroundPixel(iX, iY, 0x00FF00); break;
			case 2: setBackgroundPixel(iX, iY, 0x0000FF); break;
			case 3: setBackgroundPixel(iX, iY, 0xFFFF00); break;
			case 4: setBackgroundPixel(iX, iY, 0x00FFFF); break;
			case 5: setBackgroundPixel(iX, iY, 0xFF00FF); break;
			}
	//this->getBackgroundSurface()->setOpaque();
}



// This forces a screen redraw every pass through the loop.
void StarfieldDemo::virtMainLoopDoBeforeUpdate()
{
	m_iOffset = (m_iOffset+1) % getWindowHeight();
	virtSetupBackgroundBuffer(); // You could redraw background if you wanted each step, but it's faster to just shift it...
	this->redrawDisplay();
}

// Normally this just copies the background buffer. We will do the same thing but with an offset.
void StarfieldDemo::copyAllBackgroundBuffer()
{
	// Note: this relies upon the bounds checking and fixing which is within copyRectangleFrom in BaseEngine
	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, 0, 0, getWindowWidth(), getWindowHeight(), 0, m_iOffset);
	m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, 0, getWindowHeight() - m_iOffset, getWindowWidth(), getWindowHeight(), 0, m_iOffset - getWindowHeight());
}
