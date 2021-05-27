#pragma once

#include "BaseEngine.h"
#include "TileManager.h"
#include "MazeDemoTileManager.h"

// This looks more complex than it really is.
// When going through this example initially, try changing virtInitialiseObjects() to not create the objects, so you can see the basics.
// The objects do the work really.
// It is a useful example to see the basics of supporting states in the program - via switches.

class MazeDemoMain :
	public BaseEngine
{
public:
	MazeDemoMain(void);
	~MazeDemoMain(void);

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;


	// Get a reference to the current tile manager - which is defined below!
	MazeDemoTileManager& GetTileManager() { return m_oTiles; }

private:
	// Contained object (composition) for the tile manager
	MazeDemoTileManager m_oTiles;

public:
	// State number - so we can support different states and demonstrate the basics.
	enum State { stateInit, stateMain, statePaused };

private:
	// Current state
	State m_state;
};

