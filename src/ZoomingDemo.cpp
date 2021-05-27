#include "header.h"

// This is a basic implementation, without fancy stuff

#include "ImageManager.h"

#include "DisplayableObject.h"
#include "BaseEngine.h"

#include "ZoomingDemo.h"

#include "ExampleDragableObjects.h"
#include "SimpleFlashingDragableImageObject.h"





/*
Initialise things for my specific game/program
*/
int ZoomingDemo::virtInitialise()
{
	// These only need to be called once, so I didn't want to put them in functions which are called multiple times, even though I now need to override this method too
	getBackgroundSurface()->setDrawPointsFilter(&m_filterTranslation);
	getForegroundSurface()->setDrawPointsFilter(&m_filterTranslation);

	// Call base class version
	return BaseEngine::virtInitialise();
}


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void ZoomingDemo::virtSetupBackgroundBuffer()
{
	fillBackground( 0x000000 );
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int ZoomingDemo::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	// We don't need objects for this example, but I have added one anyway so you can test zoomed dragging
	createObjectArray(3);
	storeObjectInArray(0, new DragableObject( 100, 300, this, 60, 80, false) );
	storeObjectInArray(1, new DragableImageObject( 200, 200, this, "demo2a.png", false));
	storeObjectInArray(2, new SimpleFlashingDragableImageObject(400, 200, this, "demo2a.png", "demo2b.png", false));
	
	return 0;
}









/* Draw the string that moving objects should be drawn on top of */
void ZoomingDemo::virtDrawStringsUnderneath()
{
	// Build the string to print
	char buf[128];
	sprintf(buf, "Changing random %6d", rand() );
	drawForegroundString(50, 10, buf, 0x00ffff, NULL);
	drawForegroundString(450, 10, "Underneath the objects", 0xff0000, NULL);

	if ( isPaused() )
		drawForegroundString(150, 300, "*** PAUSED ***", 0xff0000, NULL);
}

/* Draw any string which should appear on top of moving objects - i.e. objects move behind these */
void ZoomingDemo::virtDrawStringsOnTop()
{
	// Build the string to print
	char buf[128];
	sprintf(buf, "Time %6d", rand());
	drawForegroundString(150, 40, buf, 0xff00ff, NULL);
	drawForegroundString(450, 40, "On top of the objects", 0x00ff00, NULL);
}




// Override to handle a mouse press
void ZoomingDemo::virtMouseDown( int iButton, int iX, int iY )
{
	switch (iButton)
	{
	case 1: // Left
		//m_filterScaling.compress();
		// Nothing because we are going to click to drag objects anyway
		break;
	case 2: // Middle
		m_filterScaling.compress();
		break;
	case 3: // Right
		m_filterScaling.stretch();
		break;
	}
	// Redraw the background
	redrawDisplay(); // Force total redraw
}


// Mouse wheel zooms in or out depending on whether the y scroll was positive or negative
void ZoomingDemo::virtMouseWheel(int x, int y, int which, int timestamp)
{
	// We grab the position of the centre of the screen before the zoom
	int iOldCentreX = convertClickedToVirtualPixelXPosition(this->getWindowWidth() / 2);
	int iOldCentreY = convertClickedToVirtualPixelYPosition(this->getWindowHeight() / 2);

	if (y < 0)
		m_filterScaling.compress();
	else if (y > 0)
		m_filterScaling.stretch();

	// Now we grab the position after the zoom
	int iNewCentreX = convertClickedToVirtualPixelXPosition(this->getWindowWidth() / 2);
	int iNewCentreY = convertClickedToVirtualPixelYPosition(this->getWindowHeight() / 2);
	
	// Apply a translation to offset so it appears to have zoomed on the centre by moving the old centre back to the centre of the screen
	//m_filterTranslation.changeOffset(iNewCentreX - iOldCentreX, iNewCentreY - iOldCentreY);
	// Uncomment the above line to zoom in on centre rather than top left

	// Redraw the background
	redrawDisplay(); // Force total redraw
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void ZoomingDemo::virtKeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;
	case SDLK_LEFT:
		m_filterTranslation.changeOffset(10, 0);
		redrawDisplay();
		break;
	case SDLK_RIGHT:
		m_filterTranslation.changeOffset(-10, 0);
		redrawDisplay();
		break;
	case SDLK_UP:
		m_filterTranslation.changeOffset(0, 10);
		redrawDisplay();
		break;
	case SDLK_DOWN:
		m_filterTranslation.changeOffset(0, -10);
		redrawDisplay();
		break;
	case SDLK_SPACE: // Space moves the top left back to the zero coordinates - to be on initial location
		m_filterTranslation.setOffset(0, 0);
		redrawDisplay();
		break;
	}
}
