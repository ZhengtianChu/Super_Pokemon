#ifndef MOVEMENT_POSITION_H
#define MOVEMENT_POSITION_H


// Class to maintain and calculate the current movement and position
// Set the start and end positions and times, then ask it what position to draw in at a specified time.
class MovementPosition
{
protected:
	/** Start time of movement */
	int m_iStartTime;
	/** End time of movement */
	int m_iEndTime;
	/** Start X coordinate of movement */
	int m_iStartX;
	/** Start Y coordinate of movement */
	int m_iStartY;
	/** End X coordinate of movement */
	int m_iEndX;
	/** End Y coordinate of movement */
	int m_iEndY;

	/** Calculated current X coordinate for current time */
	int m_iCurrentX;
	/** Calculated current Y coordinate for current time */
	int m_iCurrentY;

public:
	/*
	Reverse the current movement, setting up a movement so that it will move back to where it just came from, as the same speed as it just moved (reversing the velocity).
	*/
	inline void reverse()
	{
		int temp = m_iStartX; m_iStartX = m_iEndX; m_iEndX = temp;
		temp = m_iStartY;  m_iStartY = m_iEndY; m_iEndY = temp;
		int iNewEndTime = m_iEndTime * 2 - m_iStartTime;
		m_iStartTime = m_iEndTime;
		m_iEndTime = iNewEndTime;
	}

	/** Constructor - initialise it to have no movement. */
	MovementPosition()
		: m_iStartTime(0), m_iEndTime(0)
		, m_iStartX(0), m_iStartY(0)
		, m_iEndX(0), m_iEndY(0)
		, m_iCurrentX(0), m_iCurrentY(0)
	{
	}

	/* 
	Set up movement position calculator - uses start and finish coordinates and time.
	iXStart, iYStart : where you want the movement to start.
	iXEnd, iYEnd : where you want the movement to end.
	iStartTime : what time (tick count) the movement should start, e.g. current time.
	iEndTime : what time the movement should end, e.g. current time + 1000 to mean 1 second (1000ms) in the future.
	*/
	inline void setup( int iXStart, int iYStart, int iXEnd, int iYEnd, int iStartTime, int iEndTime )
	{
		m_iStartX = iXStart;
		m_iStartY = iYStart;
		m_iEndX = iXEnd;
		m_iEndY = iYEnd;
		m_iStartTime = iStartTime;
		m_iEndTime = iEndTime;
		// Initially put position at start
		m_iCurrentX = m_iStartX;
		m_iCurrentY = m_iStartY;
	}

	/* 
	Calculate current position, for the current time.
	Based upon the current time and the movement details stored, work out what the current x and y coordinates should be.
	Use setup() before this to store the values (or reverse() to move back again).
	Use getX() and getY() after this to retrieve the values which were calculated and stored.
	*/
	inline void calculate( int iCurrentTime )
	{
		// At start position until start time
		if ( iCurrentTime <= m_iStartTime )
		{
			m_iCurrentX = m_iStartX;
			m_iCurrentY = m_iStartY;
			return;
		}
		// At end position after end time
		if ( iCurrentTime >= m_iEndTime )
		{
			m_iCurrentX = m_iEndX;
			m_iCurrentY = m_iEndY;
			return;
		}
		// Calculate the current position while mid-movement
		m_iCurrentX = m_iStartX 
			+ ((iCurrentTime-m_iStartTime) * (m_iEndX-m_iStartX))/(m_iEndTime-m_iStartTime);
		m_iCurrentY = m_iStartY 
			+ ((iCurrentTime-m_iStartTime) * (m_iEndY-m_iStartY))/(m_iEndTime-m_iStartTime);
	}

	/*
	Get calculated X coordinate - see calculate()
	*/
	inline int getX() const { return m_iCurrentX; }

	/*
	Get calculated Y coordinate - see calculate()
	*/
	inline int getY() const { return m_iCurrentY; }

	/*
	Check whether movement has ended.
	Returns true if the end time for the movement has been reached.
	*/
	inline bool hasMovementFinished( int iCurrentTime ) const { return iCurrentTime >= m_iEndTime; }
};

#endif
