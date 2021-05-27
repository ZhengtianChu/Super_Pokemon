#include "header.h"

#include "MazeDemoTileManager.h"



// Draw a tile at a location on the screen determined by the tile
// position and the base X and Y co-ordinates
// OVERRIDE THIS TO DRAW YOUR TILE!!!
// Draw a tile
void MazeDemoTileManager::virtDrawTileAt( 
		BaseEngine* pEngine,
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const
{
	// New implementation draws things of different sizes
	switch( getMapValue(iMapX,iMapY) )
	{
	case 0:
	case 1:
		pSurface->drawRectangle(
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + getTileWidth() - 1,
			iStartPositionScreenY + getTileHeight() - 1,
			pEngine->getColour( getMapValue(iMapX,iMapY) ) );
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		pSurface->drawRectangle(
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + getTileWidth() - 1,
			iStartPositionScreenY + getTileHeight() - 1,
			pEngine->getColour( 0 ) );
		pSurface->drawOval(
			iStartPositionScreenX+ getMapValue(iMapX,iMapY)*2+5,
			iStartPositionScreenY+ getMapValue(iMapX,iMapY)*2+5,
			iStartPositionScreenX + getTileWidth() - getMapValue(iMapX,iMapY)*2 -6,
			iStartPositionScreenY + getTileHeight() - getMapValue(iMapX,iMapY)*2 -6,
			pEngine->getColour(getMapValue(iMapX,iMapY) ) );
		break;
	}
}
