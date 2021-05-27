#include "header.h"

// This is a basic implementation, without fancy stuff

#include "BaseEngine.h"

#include "SimpleDemo.h"

#include "ImageManager.h"

#include "TileManager.h"

#include "DisplayableObject.h"

#include "ExampleObjects.h"




/*
Do any setup of back buffer prior to locking the screen buffer.
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed.
Note: if you need to set this up again later then you can call lockAndSetupBackground() - see below - which will safely call this for you.
*/
void SimpleDemo::virtSetupBackgroundBuffer()
{
	// Mostly black
	fillBackground( 0x000000 );

	// With some speckled colours on it
	for ( int iX = 0 ; iX < getWindowWidth() ; iX++ )
		for ( int iY = 0 ; iY < this->getWindowHeight() ; iY++ )
			switch( rand()%100 )
			{
			case 0: setBackgroundPixel( iX, iY, 0xFF0000 ); break;
			case 1: setBackgroundPixel( iX, iY, 0x00FF00 ); break;
			case 2: setBackgroundPixel( iX, iY, 0x0000FF ); break;
			case 3: setBackgroundPixel( iX, iY, 0xFFFF00 ); break;
			case 4: setBackgroundPixel( iX, iY, 0x00FFFF ); break;
			case 5: setBackgroundPixel( iX, iY, 0xFF00FF ); break;
			}
}




/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int SimpleDemo::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	// Create array with default size for one object
	createObjectArray(1); 

	// Store this object in first element of the array
	storeObjectInArray(0, new BouncingObject(this,200,100,100.0, 100.0));

	// Append a second object to the array as well
	appendObjectToArray(new BouncingObject(this, 100, 200, 300.0, 200.0, 3.5, 1.0 ));
	
	return 0;
}









/* Draw the string that moving objects should be drawn on top of */
void SimpleDemo::virtDrawStringsUnderneath()
{
	// Build the string to print
	char buf[128];
	sprintf(buf, "Changing random number %6d - underneath objects", rand() );
	drawForegroundString(50, 10, buf, 0x00ffff, NULL);
	drawForegroundString(450, 50, "Underneath the objects", 0xff0000, NULL);
}


/* Draw any string which should appear on top of moving objects - i.e. objects move behind these */
void SimpleDemo::virtDrawStringsOnTop()
{
	// Build the string to print
	char buf[128];
	sprintf(buf, "Changing time %6d - on top of objects", rand());
	drawForegroundString(150, 200, buf, 0xff00ff, NULL);
	drawForegroundString(450, 240, "On top of the objects", 0x00ff00, NULL);

	if (isPaused())
		drawForegroundString(350, 400, "*** PAUSED ***", 0xff0000, NULL);
}




// Override to handle a mouse press
// In this case re-setup the backgrouns, so next time it is drawn the new background is drawn
// This will eliminate anything drawn to the background in the meantime.
void SimpleDemo::virtMouseDown( int iButton, int iX, int iY )
{
	// Redraw the background
	lockAndSetupBackground();
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void SimpleDemo::virtKeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		if (isPaused())
			unpause();
		else
			pause();
		break;
	}
}
