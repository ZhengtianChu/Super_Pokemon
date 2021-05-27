#include "Scyzc3TileManager.h"
#include "header.h"

void Scyzc3TileManager::virtDrawTileAt(
	BaseEngine* pEngine,
	DrawingSurface* pSurface, int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const
{
	int iMapValue = getMapValue(iMapX, iMapY); // which was set to result of rand()

	switch (iMapValue) {
	case 0:
		wall1.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 1:
		water.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 2:
		tree.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 3:
		floor.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 4:
		door.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 5:
		key.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 6:
		door2.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 7:
		heart.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 8:
		sword.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	case 9:
		sand.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	default:
		floor.renderImageWithMask(pSurface, 0, 0, iStartPositionScreenX, iStartPositionScreenY, getTileWidth(), getTileHeight());
		break;
	}
}
