#include "header.h"

#include "BouncingBall.h"
#include "BouncingBallMain.h"



/** Constructor */
BouncingBall::BouncingBall( BouncingBallMain* pEngine, int iID, 
							int iDrawType, int iSize, int iColour,
							const char* szLabel, 
							int iXLabelOffset, int iYLabelOffset)
: DisplayableObject(iSize, iSize, pEngine, iSize, iSize, false)
, m_pEngine( pEngine )
, m_iID(iID)
, m_iDrawType(iDrawType)
, m_iSize(iSize)
, m_iColour(iColour)
, m_szLabel(szLabel)
, m_iXLabelOffset(iXLabelOffset)
, m_iYLabelOffset(iYLabelOffset)
{
	// The ball coordinate will be the centre of the ball
	// Because we start drawing half the size to the top-left.

	// And make it visible
	setVisible(true);
}

/** Destructor */
BouncingBall::~BouncingBall(void)
{
}

/**
Draw the player object.
Could be a simple shape but this makes a fake ball shape and labels it.
*/
void BouncingBall::virtDraw()
{
	// Test code - destroy objects, including itself if 'd' is pressed
	if (getEngine()->isKeyPressed('d'))
	{
		// Re-create all of the objects - this forces a change in the array.
		// Hopefully the code allows for this and avoids using objects after destruction but this is a test for it
		getEngine()->virtInitialiseObjects();
		return; // Need to do this because just destroyed itself!
	}

	// Do not draw if it should not be visible
	if ( !isVisible() )
		return;

	unsigned int uiColourMult = 0x010001;
	unsigned int uiColourText = 0xffffff;

	// Choose one of 8 colours:
	switch( m_iColour % 8 )
	{ 
	case 1: uiColourMult = 0x010000; uiColourText = 0xffffff; break;
	case 2: uiColourMult = 0x000100; uiColourText = 0xffffff; break;
	case 3: uiColourMult = 0x000001; uiColourText = 0xffffff; break;
	case 4: uiColourMult = 0x010001; uiColourText = 0; break;
	case 5: uiColourMult = 0x010100; uiColourText = 0; break;
	case 6: uiColourMult = 0x000101; uiColourText = 0; break;
	case 7: uiColourMult = 0x010101; uiColourText = 0; break;
	default: uiColourMult = 0x000000; break;
	}
	uiColourText = 0xff * (0x010101 - uiColourMult); // Text is 'opposite' colour to the ball

	// Concentric circles for pseudo-sphere
	int iRadiusSquared = (m_iDrawWidth/2) * (m_iDrawWidth/2);
	int iCentreX = m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth/2;
	int iCentreY = m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight/2;
	for ( int iX = m_iCurrentScreenX + m_iStartDrawPosX ; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth) ; iX++ )
		for ( int iY = m_iCurrentScreenY + m_iStartDrawPosY ; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight) ; iY++ )
			if ( ( (iX-iCentreX)*(iX-iCentreX) + (iY-iCentreY)*(iY-iCentreY) ) <= iRadiusSquared )
			{
				// 0xB0 is the range of values, 0xff is the brightest value.
				unsigned int uiColour = (0xB0 * ((iX-iCentreX)*(iX-iCentreX) + (iY-iCentreY)*(iY-iCentreY))) / iRadiusSquared;
				uiColour = 0xff - uiColour;
				getEngine()->setForegroundPixel( iX, iY, uiColourMult * uiColour );
			}

	// If there is a label then draw the text
	if ( (m_szLabel!=NULL) && (strlen(m_szLabel)>0) )
	{
		//GetEngine()->DrawString( iCentreX+m_iXLabelOffset+1, iCentreY+m_iYLabelOffset+1, m_szLabel, 0xffffff );
		getEngine()->drawForegroundString( iCentreX+m_iXLabelOffset, iCentreY+m_iYLabelOffset, m_szLabel, uiColourText );
	}
}

/**
Handle the update action, moving the object and/or handling any game logic
*/
void BouncingBall::virtDoUpdate( int iCurrentTime )
{
	// Ensure that the object gets redrawn on the display, if something changed
	redrawDisplay();
}




/** Constructor */
BouncingBall1::BouncingBall1(BouncingBallMain* pEngine, int iID,
	int iDrawType, int iSize, int iColour,
	const char* szLabel,
	int iXLabelOffset, int iYLabelOffset,
	TileManager* pTileManager)
	: BouncingBall(pEngine, iID, iDrawType, iSize, iColour, szLabel, iXLabelOffset, iYLabelOffset)
	, m_pTileManager(pTileManager), m_iLastUpdatedTime(0)
{
}

// Allows a caller to specify where the object will move from and to and when
void BouncingBall1::setMovement( int iStartTime, int iEndTime, int iCurrentTime,
							   int iStartX, int iStartY, int iEndX, int iEndY )
{
	m_oMovement.setup( iStartX, iStartY, iEndX, iEndY, iStartTime, iEndTime );
	m_oMovement.calculate( iCurrentTime );
	m_iCurrentScreenX = m_oMovement.getX();
	m_iCurrentScreenY = m_oMovement.getY();
}

/**
Handle the update action, moving the object and/or handling any game logic
*/
void BouncingBall1::virtDoUpdate( int iCurrentTime )
{
	// Test code - destroy objects, including itself if 'u' is pressed
	if (getEngine()->isKeyPressed('u'))
	{
		// Re-create all of the objects - this forces a change in the array.
		// Hopefully the code allows for this and avoids using objects after destruction but this is a test for it
		getEngine()->virtInitialiseObjects();
		return; // Need to do this because just destroyed itself!
	}


	// Work out current position
	m_oMovement.calculate(iCurrentTime);
	m_iCurrentScreenX = m_oMovement.getX();
	m_iCurrentScreenY = m_oMovement.getY();

	// If movement has finished then request instructions
	if ( m_oMovement.hasMovementFinished( iCurrentTime ) )
	{
		m_oMovement.reverse();
		m_oMovement.calculate(iCurrentTime);
		m_iCurrentScreenX = m_oMovement.getX();
		m_iCurrentScreenY = m_oMovement.getY();
	}

	if ( m_pTileManager->isValidTilePosition( m_iCurrentScreenX, m_iCurrentScreenY ) )
	{
		//if (iCurrentTime > m_iLastUpdatedTime + 60)
		{ // Max undates once per 60ms - prevents a lot of updates at once, helping to reduce load
			m_iLastUpdatedTime = iCurrentTime;
			int iTileX = m_pTileManager->getMapXForScreenX(m_iCurrentScreenX);
			int iTileY = m_pTileManager->getMapYForScreenY(m_iCurrentScreenY);
			int iCurrentTile = m_pTileManager->getMapValue(iTileX, iTileY);
			m_pTileManager->setAndRedrawMapValueAt(iTileX, iTileY, iCurrentTile + 1, getEngine(), getEngine()->getBackgroundSurface());
		}
	}

	// Ensure that the object gets redrawn on the display, if something changed
	redrawDisplay();
}


/** Constructor */
BouncingBall2::BouncingBall2( BouncingBallMain* pEngine, int iID, 
							int iDrawType, int iSize, int iColour,
							const char* szLabel,
							int iXLabelOffset, int iYLabelOffset)
: BouncingBall( pEngine, iID, iDrawType, iSize, iColour, szLabel, iXLabelOffset, iYLabelOffset )
, m_dX( iSize )
, m_dY( iSize )
, m_dSX(0)
, m_dSY(0)
{
}

void BouncingBall2::setPosition( double dX, double dY )
{
	m_dX = dX;
	m_dY = dY;
}

void BouncingBall2::setSpeed( double dSX, double dSY )
{
	m_dSX = dSX;
	m_dSY = dSY;
}

/**
Handle the update action, moving the object and/or handling any game logic
*/
void BouncingBall2::virtDoUpdate( int iCurrentTime )
{
	if ( getEngine()->isKeyPressed( SDLK_UP ) )
		m_dSY -= 0.01;
	if ( getEngine()->isKeyPressed( SDLK_DOWN ) )
		m_dSY += 0.01;
	if ( getEngine()->isKeyPressed( SDLK_LEFT ) )
		m_dSX -= 0.01;
	if ( getEngine()->isKeyPressed( SDLK_RIGHT ) )
		m_dSX += 0.01;
	if ( getEngine()->isKeyPressed( SDLK_SPACE ) )
		m_dSX = m_dSY = 0;

	m_dX += m_dSX;
	m_dY += m_dSY;

	if ( (m_dX+m_iStartDrawPosX) < 0 )
	{
		m_dX = - m_iStartDrawPosX;
		if ( m_dSX < 0 )
			m_dSX = -m_dSX;
	}

	if ( (m_dX+m_iStartDrawPosX+m_iDrawWidth) > (getEngine()->getWindowWidth()-1) )
	{
		m_dX = getEngine()->getWindowWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
		if ( m_dSX > 0 )
			m_dSX = -m_dSX;
	}

	if ( (m_dY+m_iStartDrawPosY) < 0 )
	{
		m_dY = -m_iStartDrawPosY;
		if ( m_dSY < 0 )
			m_dSY = -m_dSY;
	}

	if ( (m_dY+m_iStartDrawPosY+m_iDrawHeight) > (getEngine()->getWindowHeight()-1) )
	{
		m_dY = getEngine()->getWindowHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
		if ( m_dSY > 0 )
			m_dSY = -m_dSY;
	}

	// Work out current position
	m_iCurrentScreenX = (int)(m_dX+0.5);
	m_iCurrentScreenY = (int)(m_dY+0.5);

	// Ensure that the object gets redrawn on the display, if something changed
	redrawDisplay();
}

