#include "header.h"

#include "BaseEngine.h"

#include "MazeDemoObject.h"

#include "MazeDemoMain.h"

#include "ImageManager.h"

#include "TileManager.h"


MazeDemoMain::MazeDemoMain(void)
	: m_state(stateInit) // Initial state is initialisation
{
}

MazeDemoMain::~MazeDemoMain(void)
{
}

void MazeDemoMain::virtSetupBackgroundBuffer()
{
	// Switches on state to implement some state-based behaviour
	switch( m_state )
	{
	case stateInit: // Reload the level data if in initial state, then set up tile manager accordingly
		fillBackground( 0xffff00 );
		{
			const char* data[] = {
				"bbbbbbbbbbbbbbbbbbbbbbb",
				"baeaeadadaeaeaahcdefaab",
				"babbbcbcbcbbbebbbbbebbb",
				"badadgdadhdadhbaaaeeaab",
				"bgbcbbbcbbbcbebebebebab",
				"babadadadadababaaffaaab",
				"bfbcbbbcbbbcbebbbbbbabb",
				"bahadadhdadadabacdefghb",
				"bfbbbcbibcbbbebebgbbbeb",
				"badadadadadadaeeeeeeeeb",
				"bbbbbbbbbbbbbbbbbbbbbbb" };

			// Specify how many tiles wide and high
			m_oTiles.setMapSize( 23, 11 ); 
			// Set up the tiles
			for ( int y = 0 ; y < 11 ; y++ )
				for (int x = 0; x < 23; x++)
					m_oTiles.setMapValue( x, y, data[y][x]-'a' );

			for ( int y = 0 ; y < 11 ; y++ )
			{
				for ( int x = 0 ; x < 23 ; x++ )
					std::cout << m_oTiles.getMapValue(x,y);
				std::cout << std::endl;
			}

			// Specify the screen x,y of top left corner
			m_oTiles.setTopLeftPositionOnScreen( 25, 40 );
		}
		return;

	case stateMain:
		fillBackground( 0 );
		// Tell it to draw tiles from x1,y1 to x2,y2 in tile array, to the background of this screen
		m_oTiles.drawAllTiles( this, this->getBackgroundSurface() );
		break;

	case statePaused:
		fillBackground( 0 );
		m_oTiles.drawAllTiles(this, this->getBackgroundSurface() );
		break;
	} // End switch
}



// Here we create 5 moving balls which will change the tile states as they go over them
int MazeDemoMain::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();

	// Destroy any existing objects
	destroyOldObjects(true);

	// Creates an array one element larger than the number of objects that you want.
	createObjectArray(5);

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	storeObjectInArray( 0, new MazeDemoObject(this, 1, 1) );
	storeObjectInArray( 1, new MazeDemoObject(this, 9, 9) );
	storeObjectInArray( 2, new MazeDemoObject(this, 13, 9) );
	storeObjectInArray( 3, new MazeDemoObject(this, 9, 5) );
	storeObjectInArray( 4, new MazeDemoObject(this, 13, 5) );

	// NOTE: We also need to destroy the objects at some point, but the method at the top of this function will destroy all objects pointed at by the 
	// array elements so we can ignore that here.

	// Make everything invisible to start with
	setAllObjectsVisible(false);

	return 0;
}







/* Draw text labels */
void MazeDemoMain::virtDrawStringsUnderneath()
{
	switch( m_state )
	{
	case stateInit:
		drawForegroundString( 100, 300, "Initialised and waiting for SPACE", 0x0, NULL );
		break;
	case stateMain:
		drawForegroundString( 250, 10, "Running", 0xffffff, NULL );
		break;
	case statePaused:
		drawForegroundString( 200, 300, "Paused. Press SPACE to continue", 0xffffff, NULL );
		break;
	}
}




// Override to add a node at specified point
void MazeDemoMain::virtMouseDown( int iButton, int iX, int iY )
{
}




/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void MazeDemoMain::virtKeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		setExitWithCode( 0 );
		break;

	case SDLK_SPACE: // SPACE Pauses/unpauses or starts the program if in initial state
		switch( m_state )
		{
		case stateInit:
			// Go to state main
			m_state = stateMain;
			// Force redraw of background
			lockAndSetupBackground();
			// Ensure objects become visible now - we hid them initially
			setAllObjectsVisible(true);
			// Redraw the whole screen now
			redrawDisplay();
			break;

		case stateMain:
			this->pause();
			// Go to state main
			m_state = statePaused;
			// Force screen redraw
			lockAndSetupBackground();
			redrawDisplay();
			break;

		case statePaused:
			// Go to state main
			m_state = stateMain;
			this->unpause();
			// Force redraw of background
			lockAndSetupBackground();
			// Redraw the whole screen now
			redrawDisplay();
			break;
		} // End switch on current state
		break; // End of case SPACE
	}
}



