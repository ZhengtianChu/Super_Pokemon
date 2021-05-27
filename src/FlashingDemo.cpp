#include "header.h"
#include "FlashingDemo.h"



FlashingDemo::FlashingDemo()
	: m_iCurrentSurfaceNumber(0)
	, m_oTheExtraSurface1(this)
	, m_oTheExtraSurface2(this)
	, m_oTheExtraSurface3(this)
{
	m_iDefaultUpdatePeriod = 200; // Change default update rate to 500ms
}


FlashingDemo::~FlashingDemo()
{
}

// This forces a screen redraw every pass through the loop.
void FlashingDemo::virtMainLoopDoBeforeUpdate()
{
	// We change which surface is being used for the background each time through the loop.
	m_iCurrentSurfaceNumber = ( m_iCurrentSurfaceNumber + 1) % 4;
	switch (m_iCurrentSurfaceNumber%4)
	{
	case 0: m_pBackgroundSurface = &m_oTheBackgroundSurface; break;
	case 1: m_pBackgroundSurface = &m_oTheExtraSurface1; break;
	case 2: m_pBackgroundSurface = &m_oTheExtraSurface2; break;
	case 3: m_pBackgroundSurface = &m_oTheExtraSurface3; break;
	}
	this->redrawDisplay(); // Force redraw to copy background again
}

void FlashingDemo::virtSetupBackgroundBuffer()
{
	fillBackground(0);
	m_oTheExtraSurface1.fillSurface(0x400000);
	m_oTheExtraSurface2.fillSurface(0x004000);
	m_oTheExtraSurface3.fillSurface(0x000040);

	// Note that the main background surface will have been locked before this call, but the new surfaces we created were not, so ...
	m_oTheExtraSurface1.mySDLLockSurface();
	m_oTheExtraSurface2.mySDLLockSurface();
	m_oTheExtraSurface3.mySDLLockSurface();

	for (int iX = 0; iX < getWindowWidth(); iX++)
		for (int iY = 0; iY < this->getWindowHeight(); iY++)
		{
			switch (rand() % 100)
			{
			case 0: setBackgroundPixel(iX, iY, 0xFF0000); break;
			case 1: setBackgroundPixel(iX, iY, 0x00FF00); break;
			case 2: setBackgroundPixel(iX, iY, 0x0000FF); break;
			case 3: setBackgroundPixel(iX, iY, 0xFFFF00); break;
			case 4: setBackgroundPixel(iX, iY, 0x00FFFF); break;
			case 5: setBackgroundPixel(iX, iY, 0xFF00FF); break;
			}
			switch (rand() % 100)
			{
			case 0: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0xFF0000); break;
			case 1: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0x00FF00); break;
			case 2: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0x0000FF); break;
			case 3: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0xFFFF00); break;
			case 4: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0x00FFFF); break;
			case 5: m_oTheExtraSurface1.rawSetPixel(iX, iY, 0xFF00FF); break;
			}
			switch (rand() % 100)
			{
			case 0: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0xFF0000); break;
			case 1: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0x00FF00); break;
			case 2: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0x0000FF); break;
			case 3: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0xFFFF00); break;
			case 4: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0x00FFFF); break;
			case 5: m_oTheExtraSurface2.rawSetPixel(iX, iY, 0xFF00FF); break;
			}
			switch (rand() % 100)
			{
			case 0: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0xFF0000); break;
			case 1: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0x00FF00); break;
			case 2: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0x0000FF); break;
			case 3: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0xFFFF00); break;
			case 4: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0x00FFFF); break;
			case 5: m_oTheExtraSurface3.rawSetPixel(iX, iY, 0xFF00FF); break;
			}
		}

	// Unlock the surfaces again now we have finished with them
	m_oTheExtraSurface1.mySDLUnlockSurface();
	m_oTheExtraSurface2.mySDLUnlockSurface();
	m_oTheExtraSurface3.mySDLUnlockSurface();
}

/* You could override this, e.g. to create different sized surfaces, but I assume you probably won't usually do so.
Hint: don't bother to override this unless you want to do something unusual
*/
void FlashingDemo::virtCreateSurfaces()
{
	BaseEngine::virtCreateSurfaces();
	m_oTheExtraSurface1.createSurface(m_iWindowWidth, m_iWindowHeight);
	m_oTheExtraSurface2.createSurface(m_iWindowWidth, m_iWindowHeight);
	m_oTheExtraSurface3.createSurface(m_iWindowWidth, m_iWindowHeight);
}
