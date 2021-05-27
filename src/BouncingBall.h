#ifndef BouncingBall_H
#define BouncingBall_H

// This contains various classes for the bouncing balls in the BouncingBall demo example.
// See BouncingBallMain for how they are used.
// Classes build on other classes, adding functionality.


#include "DisplayableObject.h"
#include "UtilMovementPosition.h"

#include "BouncingBallMain.h"

class BaseEngine;
class TileManager;



class BouncingBall :
	public DisplayableObject
{
public:
	BouncingBall(BouncingBallMain* pEngine, int iID, 
		int iDrawType, int iSize, int iColour,
		const char* szLabel, int iXLabelOffset, int iYLabelOffset );

	virtual ~BouncingBall(void);

	void virtDraw() override;

	void virtDoUpdate( int iCurrentTime ) override;

	// Added just to test the code for virtInitialiseObjects doesn't break anything when called...
	// Middle mouse button will reset the objects.
	// Equally valid code when adding to or removing from the object array
	void virtMouseDown(int iButton, int iX, int iY) override
	{
		if (iButton == 2)
		{
			// Re-create all of the objects
			getEngine()->virtInitialiseObjects();
		}
	}


private:
	/** Pointer to the main engine object */
	BouncingBallMain* m_pEngine;

	// Unique id for object, passed to engine to identify it
	int m_iID;

	// Type of thing to draw
	int m_iDrawType;

	// Size of thing to draw
	int m_iSize;

	// Colour index to draw with
	int m_iColour;

	// Label to apply
	const char* m_szLabel;

	// Label offset in pixels
	int m_iXLabelOffset;
	int m_iYLabelOffset;
};





class BouncingBall1 :
	public BouncingBall
{
public:
	BouncingBall1(BouncingBallMain* pEngine, int iID, 
		int iDrawType, int iSize, int iColour,
		const char* szLabel, int iXLabelOffset, int iYLabelOffset,
		TileManager* pTileManager );

	void setMovement( int iStartTime, int iEndTime, int iCurrentTime,
				int iStartX, int iStartY, int iEndX, int iEndY );

	void virtDoUpdate( int iCurrentTime ) override;

protected:
	/** Movement position calculator - give it start and end position and times 
	and it works out current position for the current time. */
	MovementPosition m_oMovement;

	// Pointer to the tile manager - we are going to use this!
	TileManager* m_pTileManager;

	// Time ball was last updated
	int m_iLastUpdatedTime;
};




class BouncingBall2 :
	public BouncingBall
{
public:
	BouncingBall2(BouncingBallMain* pEngine, int iID, 
		int iDrawType, int iSize, int iColour,
		const char* szLabel, int iXLabelOffset, int iYLabelOffset );

	// Note: does not override base class setPosition(), since parameter types are different!
	void setPosition( double dX, double dY );

	void setSpeed( double dSX, double dSY );

	void virtDoUpdate( int iCurrentTime ) override;

protected:
	double m_dX;
	double m_dY;
	double m_dSX;
	double m_dSY;
};


#endif
