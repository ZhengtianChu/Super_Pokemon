#include "header.h"

#include "BaseEngine.h"

#include "TileManager.h"



// Draw all tiles, from start to end position
void TileManager::drawAllTiles(BaseEngine* pEngine, DrawingSurface* pSurface) const
{
	pSurface->mySDLLockSurface();
	for (int iTX = 0; iTX < m_iMapWidth; iTX++)
		for (int iTY = 0; iTY < m_iMapHeight; iTY++)
		{
			virtDrawTileAt(pEngine, pSurface,
				iTX, iTY,
				m_iBaseScreenX + getTileWidth()*iTX,
				m_iBaseScreenY + getTileHeight()*iTY );
		}
	pSurface->mySDLUnlockSurface();
}

// Draw a set of the tiles now - drawing from the top left of the drawable area
void TileManager::drawSomeTiles(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapXStart, int iMapYStart, int iWidth, int iHeight ) const
{
	pSurface->mySDLLockSurface();
	for (int iTX = 0; iTX < iWidth; iTX++)
		for (int iTY = 0; iTY < iHeight; iTY++)
		{
			virtDrawTileAt(pEngine, pSurface,
				iTX + iMapXStart, iTY + iMapYStart,
				m_iBaseScreenX + getTileWidth()*iTX,
				m_iBaseScreenY + getTileHeight()*iTY);
		}
	pSurface->mySDLUnlockSurface();
}


// THE NEXT FUNCTION IS THE ONE YOU MAY WISH TO OVERRIDE YOURSELF
// You probably don't need to change the stuff above.
// Optionally you could change the following ones too, but they are less likely to need to be changed.

// Draw a tile at a location on the screen determined by the tile position and the base X and Y co-ordinates
// OVERRIDE THIS TO DRAW YOUR TILE!!!
// Note: it can be const since the function does not alter the tile manager, even though it does alter the screen
void TileManager::virtDrawTileAt(
	BaseEngine* pEngine, // We don't need this but maybe a student will so it is here to use if needed
	DrawingSurface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	int iMapValue = getMapValue(iMapX, iMapY);
	unsigned int iColour = 0x101010 * ((iMapX + iMapY + iMapValue ) % 16);
	pSurface->drawRectangle(
		iStartPositionScreenX,	// Left
		iStartPositionScreenY,	// Top
		iStartPositionScreenX + getTileWidth() - 1,		// Right
		iStartPositionScreenY + getTileHeight() - 1,	// Bottom
		iColour);	// Pixel colour
}

