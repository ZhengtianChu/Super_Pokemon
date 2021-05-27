#pragma once

// Implements a simple object which moves around the maze - see MazeDemoMain.cpp for an example of use.
// Object has a memory of which direction it is moving at the moment.

#include "DisplayableObject.h"
#include "UtilMovementPosition.h"

class MazeDemoMain;


class MazeDemoObject :
	public DisplayableObject
{
public:
	MazeDemoObject(MazeDemoMain* pEngine, int iMapX, int iMapY);
	~MazeDemoObject(void);

	void virtDraw() override;
	void virtDoUpdate( int iCurrentTime ) override;

	int getXDiffForDirection( int iDir )
	{
		switch( iDir%4 )
		{
		case 0: return 0; // Up
		case 1: return 1; // Right
		case 2: return 0; // Down
		case 3: return -1; // Left
		default: return 0; // Can never happen
		}
	}

	int getYDiffForDirection( int iDir )
	{
		switch( iDir%4 )
		{
		case 0: return -1; // Up
		case 1: return 0; // Right
		case 2: return 1; // Down
		case 3: return 0; // Left
		default: return 0; // Can never happen
		}
	}

private:
	MazeDemoMain* m_pMainEngine;

	// We use one of these to interpolate the position based on a starting and ending position and time.
	MovementPosition m_oMover;

	// Current position and heading direction on the map
	int m_iMapX;
	int m_iMapY;
	int m_iDir;
};
