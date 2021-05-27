#ifndef ZOOMINGDEMO_H
#define ZOOMINGDEMO_H

#include "BaseEngine.h"
#include "ExampleDragableObjects.h"

#include "ExampleFilterPointClasses.h" 

/* 
This is a relatively simple demo to illustrate how filters can be used for translation and zoom in/out - you do not NEED to do this!
*/
class ZoomingDemo : public BaseEngine
{
public:

	/**
	Constructor
	*/
	ZoomingDemo()
		// Set up filters - first translate points then scale
		: m_filterScaling(0,0, this), m_filterTranslation(0,0, &m_filterScaling)
	{}

	// Initialise things for my specific game/program
	int virtInitialise() override;

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create any moving objects
	int virtInitialiseObjects() override;

	/** Draw any strings */
	void virtDrawStringsUnderneath() override;
	void virtDrawStringsOnTop() override;

	// Handle pressing of a mouse button
	void virtMouseDown( int iButton, int iX, int iY ) override;

	// Handle pressing of a key
	virtual void virtKeyDown(int iKeyCode) override;

	// Handle mouse wheel
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) override;

protected:
	FilterPointsScaling m_filterScaling;
	FilterPointsTranslation m_filterTranslation;

};

#endif
