 #include "header.h"

#include "MazeDemoObject.h"
#include "MazeDemoMain.h"
#include "UtilCollisionDetection.h"

MazeDemoObject::MazeDemoObject(MazeDemoMain* pEngine, int iMapX, int iMapY)
: DisplayableObject(pEngine)
, m_pMainEngine( pEngine )
, m_iMapX(iMapX)
, m_iMapY(iMapY)
, m_iDir(0)
{
	// The ball coordinate will be the centre of the ball
	// Because we start drawing half the size to the top-left.
	m_iStartDrawPosX = -25;
	m_iStartDrawPosY = -25;

	// Record the ball size as both height and width
	m_iDrawWidth = 50;
	m_iDrawHeight = 50;
	
	// Put item at specific coordinates
	m_iCurrentScreenX = iMapX*50+25+25;
	m_iCurrentScreenY = iMapY*50+25+40;

	// And make it visible
	setVisible(true);
}

MazeDemoObject::~MazeDemoObject(void)
{
}

void MazeDemoObject::virtDraw()
{
	// Do not draw if it should not be visible or program is paused
	if (!isVisible() )
		return;

//	int iSize = 25;
	int iTick = m_pMainEngine->getModifiedTime()/20; // 1 per 20ms
	int iFrame = iTick % 30;
	int iSize = 10 + iFrame;
	if ( iFrame > 15 )
		iSize = 10 + (30-iFrame);

	m_pMainEngine->drawForegroundOval(
			m_iCurrentScreenX - iSize,
			m_iCurrentScreenY - iSize,
			m_iCurrentScreenX + iSize-1,
			m_iCurrentScreenY + iSize-1,
			0x00ffff );
}

void MazeDemoObject::virtDoUpdate( int iCurrentTime )
{
	// Do not update if it should not be visible or program is paused
	if (!isVisible() || getEngine()->isPaused() )
		return;

/********** STUFF FOR COLLISION DETECTION ********/

	// Iterate through the objects
	// We are looking for one which is too close to us
	DisplayableObject* pObject;
	for ( int iObjectId = 0 ; 
		 (pObject = m_pMainEngine->getDisplayableObject( iObjectId )
				) != NULL ;
		iObjectId++ )
	{
		if ( pObject == this ) // This is us, skip it
			continue;
		if (pObject == nullptr) // Object does not exist, skip it
			continue;		// If you need to cast to the sub-class type, you must use dynamic_cast, see lecture 19

		// Estimate the size - by re-calculating it
		int iTick = iCurrentTime/20; // 1 per 20ms
		int iFrame = iTick % 30;
		int iSize = 10 + iFrame;
		if ( iFrame > 15 )
			iSize = 10 + (30-iFrame);
		int iSizeOther = iSize; // Assume both the same size

		// We are just using base class parts
		//int iXDiff = pObject->getXCentre() - m_iCurrentScreenX;
		//int iYDiff = pObject->getYCentre() - m_iCurrentScreenY;
		// Pythagorus' theorum:
		// OLD CODE: ((iXDiff*iXDiff)+(iYDiff*iYDiff)) < ((iSizeOther+iSize)*(iSizeOther+iSize)) )

		// This defaults to checking circles, but change the #if below to do rectangle checks instead if you need to do that
#if 0
		// This code illustrates how you could do rectangular regions rather than the circles themselves
		if (CollisionDetection::checkRectangles(pObject->getXCentre() - iSizeOther, pObject->getXCentre() + iSizeOther,
												pObject->getYCentre() - iSizeOther, pObject->getYCentre() + iSizeOther, 
												m_iCurrentScreenX - iSize, m_iCurrentScreenX + iSize,
			 									m_iCurrentScreenY - iSize, m_iCurrentScreenY + iSize ) )
#else
		// This code checks for collision detection between circles
		if ( CollisionDetection::checkCircles(pObject->getXCentre(), pObject->getYCentre(), m_iCurrentScreenX, m_iCurrentScreenY, iSizeOther + iSize ) )
#endif
		{ // This code is executed if a collision detection was found
			// Move us to 1,1 and set direction right
			m_iMapX = 1+rand()%20;
			m_iMapY = 1+(rand()%2)*8;
			m_iDir = 1; // Face right
			m_oMover.setup( 
				m_iMapX *50 + 25 + 25, //m_iCurrentScreenX,
				m_iMapY *50 + 25 + 40, //m_iCurrentScreenY,
				m_iMapX *50 + 25 + 25,
				m_iMapY *50 + 25 + 40,
				iCurrentTime,
				iCurrentTime+400+rand()%200 );
			// Ask the mover where the object should be
			m_oMover.calculate( iCurrentTime );
			m_iCurrentScreenX = m_oMover.getX();
			m_iCurrentScreenY = m_oMover.getY();
			// Ensure that the object gets redrawn on the display, if something changed
			redrawDisplay();
			return;
		}
	}
/********** END OF COLLISION DETECTION CODE ********/

/***** This code shows how to use the MovementPosition object to do interpolated movement *****/

/* First set up the movement (start and end position and time), then ask it for the position based on current time. Check for it finishing, and if so then set up a new movement. */

	// If movement has finished
	if ( m_oMover.hasMovementFinished(iCurrentTime) )
	{
		MazeDemoTileManager& tm = m_pMainEngine->GetTileManager();
 
		// Handle any tile that we just moved onto
		switch ( tm.getMapValue( m_iMapX, m_iMapY ) )
		{
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			// Increase map value by 1
			tm.setAndRedrawMapValueAt( m_iMapX, m_iMapY, tm.getMapValue( m_iMapX, m_iMapY ) + 1, getEngine(), getEngine()->getBackgroundSurface());
			//std::cout << "Increased value by 1" << std::endl;
			break;
		case 8:
			// Set map value to 0
			tm.setAndRedrawMapValueAt( m_iMapX, m_iMapY, 0, getEngine(), getEngine()->getBackgroundSurface());
			break;
		}

		// Set off a new movement
		switch( rand() % 10 )
		{
		case 0: // Increase dir by 1
			m_iDir = ( m_iDir + 1 )%4;
			break;
		case 1: // Reduce dir by 1
			m_iDir = ( m_iDir + 3 )%4;
			break;
		}

		// Allow some control over the object by the player
		if ( m_pMainEngine->isKeyPressed( SDLK_UP ) )
			m_iDir = 0;
		if ( m_pMainEngine->isKeyPressed( SDLK_RIGHT ) )
			m_iDir = 1;
		if ( m_pMainEngine->isKeyPressed( SDLK_DOWN ) )
			m_iDir = 2;
		if ( m_pMainEngine->isKeyPressed( SDLK_LEFT ) )
			m_iDir = 3;


		switch ( tm.getMapValue( 
				m_iMapX + getXDiffForDirection(m_iDir),
				m_iMapY + getYDiffForDirection(m_iDir) ) )
		{
		case 0: // Passageway
		case 2: // Pellet
		case 3: // Pellet
		case 4: // Pellet
		case 5: // Pellet
		case 6: // Pellet
		case 7: // Pellet
		case 8: // Pellet
			// Allow move - set up new movement now
			m_iMapX += getXDiffForDirection(m_iDir);
			m_iMapY += getYDiffForDirection(m_iDir);

			m_oMover.setup( 
				m_iCurrentScreenX,
				m_iCurrentScreenY,
				m_iMapX *50 + 25 + 25,
				m_iMapY *50 + 25 + 40,
				iCurrentTime,
				iCurrentTime+400+rand()%200 );
			break;	
		case 1: // Wall
			m_iDir = rand()%4; // Rotate randomly
			break;
		}
	}

/*** Here is the code to use the MovementPosition object to calculate the current position for the object ***/

	// If making a move then do the move now, working out the new position and set the x and y to that position
	if ( !m_oMover.hasMovementFinished(iCurrentTime) )
	{
		// Ask the mover where the object should be
		m_oMover.calculate( iCurrentTime );
		m_iCurrentScreenX = m_oMover.getX();
		m_iCurrentScreenY = m_oMover.getY();
	}

	// Ensure that the object gets redrawn on the display, since something changed
	redrawDisplay();
}




