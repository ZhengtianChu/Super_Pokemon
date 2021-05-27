#include "header.h"

// This is a basic implementation, without fancy stuff

#include "BaseEngine.h"

#include "DraggingDemo.h"

#include "ImageManager.h"

#include "TileManager.h"

#include "DisplayableObject.h"

#include "ExampleDragableObjects.h"
#include "ExampleObjects.h"




/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void DraggingDemo::virtSetupBackgroundBuffer()
{
	fillBackground( 0x000000 );

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
int DraggingDemo::virtInitialiseObjects()
{
	// Destroy any existing objects
	destroyOldObjects(true);

	// Add three draggable shapes. Just using the base class objects for this demo but you can subclass them
	createObjectArray(5);
	storeObjectInArray(0, new DragableObject(this, 60, 80, false) );
	storeObjectInArray(1, new DragableObject(200, 200, this, 80, 60, false) );
	storeObjectInArray(2, new DragableObject(300, 300, this, 100, 100, false) );
	storeObjectInArray(3, new DragableImageObject(200, 400, this, "demo.png", false));
	storeObjectInArray(4, new ImageObject(400, 400, this, "demo.png", false));
	
	return 0;
}



/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void DraggingDemo::virtKeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;
	}
}
