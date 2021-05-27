#include "header.h"


#include "BaseEngine.h"

#include "DisplayableObject.h"


DisplayableObject::DisplayableObject(BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 ) 
	:
	// Current position of object on the screen
	m_iCurrentScreenX(0),
	m_iCurrentScreenY(0),
	// Offset within the drawing area to draw at:
	m_iStartDrawPosX( useTopLeftFor00 ? 0 : -iWidth/2 ),
	m_iStartDrawPosY( useTopLeftFor00 ? 0 : -iHeight / 2 ),
	// Size of the thing inside the tile:
	m_iDrawWidth(iWidth),
	m_iDrawHeight(iHeight),
	// Store pointer to the game object, for later use
	m_pEngine(pEngine),
	m_iColour(0xff00ff),
	m_bVisible(true),
	m_bDeleteWhenContainerDeleted(true)
{
	fixPosition(); // Fit it onto the screen!
}

DisplayableObject::DisplayableObject(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00)
	:
	// Current position of object on the screen
	m_iCurrentScreenX(xStart),
	m_iCurrentScreenY(yStart),
	// Offset within the drawing area to draw at:
	m_iStartDrawPosX(useTopLeftFor00 ? 0 : -iWidth / 2),
	m_iStartDrawPosY(useTopLeftFor00 ? 0 : -iHeight / 2),
	// Size of the thing inside the tile:
	m_iDrawWidth(iWidth),
	m_iDrawHeight(iHeight),
	// Store pointer to the game object, for later use
	m_pEngine(pEngine),
	m_iColour(0xff00ff),
	m_bVisible(true),
	m_bDeleteWhenContainerDeleted(true)
{
	fixPosition(); // Fit it onto the screen!
}


// Destructor
DisplayableObject::~DisplayableObject()
{
}

// Draw the object - override to implement the actual drawing of the correct object
// This just draws a magenta square at the current location
void DisplayableObject::virtDraw()
{
	if (isVisible())
	{
		// Draw the object - using the default colour
		for (int iX = m_iCurrentScreenX + m_iStartDrawPosX; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth); iX++)
			for (int iY = m_iCurrentScreenY + m_iStartDrawPosY; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight); iY++)
				m_pEngine->setForegroundPixel(iX, iY, m_iColour);
	}
}






// Handle the update action, moving the object and/or handling any game logic
void DisplayableObject::virtDoUpdate( int iCurrentTime )
{
	// Override can handle player input or do some AI actions to redirect object and set the current screen position
	// Set m_iCurrentScreenX, m_iCurrentScreenY
}


// Fix current position to be on the screen if it wasn't initially
void DisplayableObject::fixPosition()
{
	if ((m_iCurrentScreenX + m_iStartDrawPosX) < 0)
		m_iCurrentScreenX = -m_iStartDrawPosX;
	else if ((m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth) >= m_pEngine->getWindowWidth())
		m_iCurrentScreenX = m_pEngine->getWindowWidth() - m_iStartDrawPosX - m_iDrawWidth;

	if ((m_iCurrentScreenY + m_iStartDrawPosY) < 0)
		m_iCurrentScreenY = -m_iStartDrawPosY;
	else if ((m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight) >= m_pEngine->getWindowHeight())
		m_iCurrentScreenY = m_pEngine->getWindowHeight() - m_iStartDrawPosY - m_iDrawHeight;
}

/** Redraw the whole screen - background and objects */
void DisplayableObject::redrawDisplay() 
{ 
	m_pEngine->redrawDisplay(); 
}

/*
Call this to specify that only a section of the screen should be redrawn - default to updating the part the object would be drawn to.
Note: need to call it before and after updating the positions, to redraw old and new position
*/
void DisplayableObject::redrawRectangle()
{
	/* Call this to specify that only a section of the screen should be redrawn */
	m_pEngine->redrawRectangle(m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
}

/* 
Return true if object is at least partially within the redraw area
*/
bool DisplayableObject::virtIsObjectInRedrawArea()
{
	/* Call this to specify that only a section of the screen should be redrawn */
	if (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth < m_pEngine->getRedrawRectVirtualLeft())
		return false; // Too far to left
	if (m_iCurrentScreenX + m_iStartDrawPosX >= m_pEngine->getRedrawRectVirtualRight())
		return false; // Too far to right
	if (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight < m_pEngine->getRedrawRectVirtualTop())
		return false; // Too far above
	if (m_iCurrentScreenY + m_iStartDrawPosY >= m_pEngine->getRedrawRectVirtualBottom())
		return false; // Too far below
	return true;
}
