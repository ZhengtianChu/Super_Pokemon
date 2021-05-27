#pragma once
#include "BaseEngine.h"
class FlashingDemo :
	public BaseEngine
{
public:
	FlashingDemo();
	~FlashingDemo();

	// Draw a simple starfield, as the bouncing ball
	virtual void virtSetupBackgroundBuffer() override;

	// This forces a screen redraw every pass through the loop.
	virtual void virtMainLoopDoBeforeUpdate() override;

	// Overridden to create three new SDL surfaces for the flashing
	virtual void virtCreateSurfaces() override;

private:
	DrawingSurface m_oTheExtraSurface1;
	DrawingSurface m_oTheExtraSurface2;
	DrawingSurface m_oTheExtraSurface3;
	int m_iCurrentSurfaceNumber;
};

