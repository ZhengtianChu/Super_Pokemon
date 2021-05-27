#pragma once

#include "BaseEngine.h"

/*
The tile manager maintains an array of tiles for the screen.
It usually needs to be drawn to the BACKGROUND, and any changes need to be drawn to both background and foreground.
It remembers whereabouts on the screen it has been drawn and if you change anything it will update both the background and the foreground for you automatically if you use the correct functions.

Use:
Tell constructor how big each tile is and how big the grid of tiles is.
Use setMapSize() to say how big the array of tiles is if it changes - this will wipe any existing tile data!
Use setTopLeftPositionOnScreen() to store the position of the top-left corner on screen of the tile array. From this and the tile sizes other calculations can be made.
Use isValidTilePosition() on an x,y coordinate to find out whether it is in the tile array.
Use getMapXForScreenX() or getMapXForScreenY() to work out the tile index from a screen position.
Use drawAllTiles() or drawSomeTiles() to draw tiles to the screen.
Use setMapValue() to store the value of a tile at a specific position.
Use setAndRedrawMapValueAt() to set the map value and immediately redraw the new tile to the screen.
Override virtDrawTileAt() in your subclass to specify how each tile will be drawn.
*/
class TileManager
{
public:
	// Constructor
	// Sets the tile size, but the map size setting is left until later, since it involves allocating resources too and can be changed later
	TileManager( int iTileHeight, int iTileWidth )
		: m_pData(NULL)
		, m_iMapWidth(0)
		, m_iMapHeight(0)
		, m_iBaseScreenX(0)
		, m_iBaseScreenY(0)
		, m_iTileHeight( iTileHeight )
		, m_iTileWidth( iTileWidth )
	{
	}

	// Constructor to set both tile size and map size in one go
	TileManager(int iTileHeight, int iTileWidth, int iMapWidth, int iMapHeight )
		: m_pData(NULL)
		, m_iMapWidth(0)
		, m_iMapHeight(0)
		, m_iBaseScreenX(0)
		, m_iBaseScreenY(0)
		, m_iTileHeight(iTileHeight)
		, m_iTileWidth(iTileWidth)
	{
		setMapSize(iMapWidth, iMapHeight);
	}

	// Destructor
	virtual ~TileManager(void)
	{
		if ( m_pData != NULL )
			delete [] m_pData;
		m_pData = NULL;
	}

	// Re-size, and zero all values
	void setMapSize(int iMapWidth, int iMapHeight)
	{
		if ( m_pData != NULL )
			delete [] m_pData;
		// Set up the map data
		m_pData = new int[iMapWidth*iMapHeight];
		m_iMapWidth = iMapWidth;
		m_iMapHeight = iMapHeight;
		memset( m_pData, 0, sizeof(int)*iMapWidth*iMapHeight );
	}

	// Get a value for a specific tile
	int getMapValue(int iMapX, int iMapY) const
	{
		return m_pData[ iMapX + iMapY * m_iMapWidth ];
	}

	// Set the value of a specific tile and return the value set (allowing chaining of functions)
	int setMapValue(int iMapX, int iMapY, int iValue)
	{
		m_pData[ iMapX + iMapY * m_iMapWidth ] = iValue;
		return iValue;
	}

	// Set the value of a tile, and also redraw the tile to the background
	void setAndRedrawMapValueAt(int iMapX, int iMapY, int iValue, BaseEngine* pEngine, DrawingSurface* pSurface)
	{
		setMapValue(iMapX, iMapY, iValue);
		pSurface->mySDLLockSurface(); // Needed by some platforms to allow surface to be modified
		virtDrawTileAt(pEngine, pSurface, iMapX, iMapY, 
			m_iBaseScreenX + getTileWidth()*iMapX,
			m_iBaseScreenY + getTileHeight()*iMapY );
		pSurface->mySDLUnlockSurface(); // Needed by some platforms to allow surface to be modified
	}

// The following functions use a base tile position, to avoid needing to pass the offset each time

	// You can specify whereabouts on the screen the map should appear
	void setTopLeftPositionOnScreen( int iScreenX, int iScreenY )
	{
		m_iBaseScreenX = iScreenX;
		m_iBaseScreenY = iScreenY;
	}

	// Using the base tiles position, work out which tile the point is over
	int getMapXForScreenX( int iScreenX ) const
	{
		return (iScreenX - m_iBaseScreenX)/getTileWidth();
	}

	// Using the base tiles position, work out which tile the point is over
	int getMapYForScreenY( int iScreenY ) const
	{
		return (iScreenY - m_iBaseScreenY)/getTileHeight();
	}

	// Determine whether a screen position is inside the current tiled area
	// Assuming that the base position has been initialised
	bool isValidTilePosition( int iScreenX, int iScreenY ) const
	{
		//std::cout << "isValidTilePosition: x " << std::to_string(iScreenX) << " vs base " << std::to_string(m_iBaseScreenX)
		//	<< " y " << std::to_string(iScreenX) << " vs base " << std::to_string(m_iBaseScreenY) << std::endl;

		if ( iScreenX < m_iBaseScreenX )
			return false;
		if ( iScreenY < m_iBaseScreenY )
			return false;
		if ( iScreenX >= ( m_iBaseScreenX + getTileWidth() * m_iMapWidth ) )
			return false;
		if ( iScreenY >= ( m_iBaseScreenY + getTileHeight() * m_iMapHeight ) )
			return false;

		//std::cout << "isValidTilePosition : yes" << std::endl;
		return true;
	}

	// Return the current map width, in tiles
	int getMapWidth() const { return m_iMapWidth; }

	// Return the current map height, in tiles
	int getMapHeight() const { return m_iMapHeight; }

	// Get the size of a tile - width in pixels
	int getTileWidth() const
	{
		return m_iTileWidth;
	}

	// Get the size of a tile - height in pixels
	int getTileHeight() const
	{
		return m_iTileHeight;
	}

	// Draw all tiles, from start to end position
	void drawAllTiles(BaseEngine* pEngine, DrawingSurface* pSurface) const;
	// Draw a set of tiles
	void drawSomeTiles(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapXStart, int iMapYStart, int iWidth, int iHeight) const;

	// When a tile gets updated this is used to draw the change to the specified surface now...
	void drawToSurface(BaseEngine* pEngine, DrawingSurface* pSurface, int iMapX, int iMapY)
	{
		pSurface->mySDLLockSurface(); // Needed by some platforms to allow surface to be modified
		virtDrawTileAt(pEngine, pSurface, iMapX, iMapY, m_iBaseScreenX + getTileWidth()*iMapX, m_iBaseScreenY + getTileHeight()*iMapY);
		pSurface->mySDLUnlockSurface(); // Needed by some platforms to allow surface to be modified
	}

// THE NEXT FUNCTION IS THE ONE YOU MAY WISH TO OVERRIDE YOURSELF
// You probably don't need to change the stuff above.
// Optionally you could change the following ones too, but they are less likely to need to be changed.

	// Draw a tile at a location on the screen determined by the tile position and the base X and Y co-ordinates
	// OVERRIDE THIS TO DRAW YOUR TILE!!!
	// Note: it can be const since the function does not alter the tile manager, even though it does alter the screen
	virtual void virtDrawTileAt(
		BaseEngine* pEngine,
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const;
	
protected:
	// Map Data
	int* m_pData;
	// Map size - width
	int m_iMapWidth;
	// Map size - height
	int m_iMapHeight;
	// Base map position on the screen - to simplify things
	int m_iBaseScreenX;
	int m_iBaseScreenY;
	// Tile size
	int m_iTileHeight;
	int m_iTileWidth;
};
