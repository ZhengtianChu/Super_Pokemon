#include "header.h"
#include "MyDemoA.h"
#include "MyObjectB.h"
#include "ImageManager.h"

void MyDemoA::virtSetupBackgroundBuffer()
{
	fillBackground(0x000000);
	for (int iX = 0; iX < getWindowWidth(); iX++)
		for (int iY = 0; iY < this->getWindowHeight(); iY++)
			switch (rand() % 100)
			{
			case 0: setBackgroundPixel(iX, iY, 0xFF0000); break;
			case 1: setBackgroundPixel(iX, iY, 0x00FF00); break;
			case 2: setBackgroundPixel(iX, iY, 0x0000FF); break;
			case 3: setBackgroundPixel(iX, iY, 0xFFFF00); break;
			case 4: setBackgroundPixel(iX, iY, 0x00FFFF); break;
			case 5: setBackgroundPixel(iX, iY, 0xFF00FF); break;
			}

	SimpleImage image = ImageManager::loadImage("demo.png", true);
	image.renderImage(getBackgroundSurface(), 0, 0, 10, 10,
		image.getWidth(), image.getHeight());

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			tm.setMapValue(i, j, rand());
	tm.setTopLeftPositionOnScreen(50, 50);
	tm.drawAllTiles(this, getBackgroundSurface());

}


void MyDemoA::virtMouseDown(int iButton, int iX, int iY)
{
	printf("Mouse clicked at %d %d\n", iX, iY);
	if (tm.isValidTilePosition(iX, iY)) // Clicked within tiles?
	{
		int mapX = tm.getMapXForScreenX(iX); // Which column?
		int mapY = tm.getMapYForScreenY(iY); // Which row?
		int value = tm.getMapValue(mapX, mapY); // Current value?
		tm.setAndRedrawMapValueAt(mapX, mapY, value + rand(), this, getBackgroundSurface());
		redrawDisplay(); // Force background to be redrawn to foreground
	}
	else if (iButton == SDL_BUTTON_LEFT)
	{
		lockBackgroundForDrawing();
		drawBackgroundRectangle(iX - 10, iY - 10, iX + 10, iY + 10, 0xff0000);
		unlockBackgroundForDrawing();
		redrawDisplay(); // Force background to be redrawn to foreground
	}
	else if (iButton == SDL_BUTTON_RIGHT)
	{
		lockBackgroundForDrawing();
		drawBackgroundOval(iX - 10, iY - 10, iX + 10, iY + 10, 0x0000ff);
		unlockBackgroundForDrawing();
		redrawDisplay(); // Force background to be redrawn to foreground
	}

}


void MyDemoA::virtKeyDown(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_SPACE:
		lockBackgroundForDrawing();
		virtSetupBackgroundBuffer();
		unlockBackgroundForDrawing();
		redrawDisplay();
		break;
	}
}


int MyDemoA::virtInitialiseObjects()
{
	// Record the fact that we are about to change the array
	// so it doesn't get used elsewhere without reloading it
	drawableObjectsChanged();
	// Destroy any existing objects
	destroyOldObjects(true);
	// Creates an array big enough for the number of objects that you want.
	createObjectArray(1);
	// You MUST set the array entry after the last one that you create to NULL,
	// so that the system knows when to stop.
	storeObjectInArray(0, new MyObjectB(this));
	// NOTE: We also need to destroy the objects, but the method at the
	// top of this function will destroy all objects pointed at by the
	// array elements so we can ignore that here.
	setAllObjectsVisible(true);
	return 0;
}
