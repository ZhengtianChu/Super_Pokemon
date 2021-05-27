#pragma once

// This creates an object which will bounce around the screen.
// It knows how but the screen is, and how big it is itself, so can work out when to bounce.
// You can use arrow keys to change the x and y speed

#include "BaseEngine.h"

#include "SimpleDemo.h"

#include "ImageManager.h"

#include "TileManager.h"

#include "DisplayableObject.h"

/*
This draws and moves the simple rectangle on the screen.
The main feature of this class is that it maintains a speed in the x and y direction, as well as a double accuracy position.
Before drawing etc it sets the normal integer position variables to be the values of the doubles rounded off.
Note: The entirity of this class is defined inside the class definition here.
*/
class BouncingObject : public DisplayableObject
{
private:
	double m_dSX;
	double m_dSY;
	double m_dX;
	double m_dY;

public:
	// Constructor has to set up all of the position and size members
	BouncingObject(BaseEngine* pEngine, int iWidth = 100, int iHeight = 50, double dXPos = 100, double dYPos = 200, double dXSpeed = 1.5, double dYSpeed = 2.5 )
		: DisplayableObject(pEngine, 100, 50, false), m_dX(dXPos), m_dY(dYPos), m_dSX(dXSpeed), m_dSY(dYSpeed)
	{
		// See initialisation list above!!!

		// And make it visible
		setVisible(true);
	}

	// Draw the shape - just draws a rectangle
	void virtDraw() override
	{
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX - m_iDrawWidth / 2 + 1,
			m_iCurrentScreenY - m_iDrawHeight / 2 + 1,
			m_iCurrentScreenX + m_iDrawWidth / 2 - 1,
			m_iCurrentScreenY + m_iDrawHeight / 2 - 1,
			0xffff00);
	}

	// Called frequently, this should move the item
	// In this case we also accept cursor key presses to change the speed
	// Space will set the speed to zero
	void virtDoUpdate(int iCurrentTime) override
	{
		if (getEngine()->isPaused())
			return; // Do nothing if paused

					// Change speed if player presses a key
		if (getEngine()->isKeyPressed(SDLK_UP))
			m_dSY -= 0.005;
		if (getEngine()->isKeyPressed(SDLK_DOWN))
			m_dSY += 0.005;
		if (getEngine()->isKeyPressed(SDLK_LEFT))
			m_dSX -= 0.005;
		if (getEngine()->isKeyPressed(SDLK_RIGHT))
			m_dSX += 0.005;

		// Alter position for speed
		m_dX += m_dSX;
		m_dY += m_dSY;

		// Check for bounce off the edge
		if ((m_dX + m_iStartDrawPosX) < 0)
		{
			m_dX = -m_iStartDrawPosX;
			if (m_dSX < 0)
				m_dSX = -m_dSX;
		}
		if ((m_dX + m_iStartDrawPosX + m_iDrawWidth) >(getEngine()->getWindowWidth() - 1))
		{
			m_dX = getEngine()->getWindowWidth() - 1 - m_iStartDrawPosX - m_iDrawWidth;
			if (m_dSX > 0)
				m_dSX = -m_dSX;
		}
		if ((m_dY + m_iStartDrawPosY) < 0)
		{
			m_dY = -m_iStartDrawPosY;
			if (m_dSY < 0)
				m_dSY = -m_dSY;
		}
		if ((m_dY + m_iStartDrawPosY + m_iDrawHeight) >(getEngine()->getWindowHeight() - 1))
		{
			m_dY = getEngine()->getWindowHeight() - 1 - m_iStartDrawPosY - m_iDrawHeight;
			if (m_dSY > 0)
				m_dSY = -m_dSY;
		}

		// Set current position - you NEED to set the current positions
		m_iCurrentScreenX = (int)(m_dX + 0.5);
		m_iCurrentScreenY = (int)(m_dY + 0.5);

		//printf("Position %f, %f\n", m_dX, m_dY );

		// Ensure that the object gets redrawn on the display, if something changed
		redrawDisplay();
	}
};



// This is an object which can draw itself as an image.


//
// This class is a basic DisplayableObject which draws an image to the screen instead of just a square
// See: 
// Constructor: initialises the SimpleImage so you can use it
// Draw: asks the SimpleImage to draw itself at the position of the object on the screen
//
class ImageObject : public DisplayableObject
{
public:
	// Constructors
	ImageObject(BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(pEngine, 100, 100, useTopLeftFor00),
		image(pEngine->loadImage(strURL, false/*don't keep it loaded*/))
	{
		this->m_iDrawWidth = image.getWidth();
		this->m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		setVisible(true);
	}

	ImageObject(int xStart, int yStart, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, 100, 100, useTopLeftFor00),
		image(ImageManager::get()->getImagebyURL(strURL, true/*Load*/, false/*Keep it loaded*/))
	{
		m_iDrawWidth = image.getWidth();
		m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		setVisible(bVisible);
	}

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw() override
	{
		if (isVisible())
		{
			image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

private:
	SimpleImage image;
};






/*
This draws and moves an image on the screen.
Look at the lines labelled LOOK HERE
*/
class BouncingImageObject : public BouncingObject
{
public:
	// Constructor has to set tell the image what URL to load, as well as telling the base class to initialise itself.
	BouncingImageObject(BaseEngine* pEngine, std::string strURL)
		: BouncingObject(pEngine)
		, image(pEngine->loadImage(strURL, false/*don't keep it loaded*/))  // LOOK HERE
	{
		// Nothing to do since base class will do it all!
		// Note that the width and height are hard-coded in the base class - but you can fix that when you create your own class.
	}

	// Draw the shape - by asking the image to draw itself to the foreground at the specified position.
	virtual void virtDraw() override
	{
		if (isVisible())
		{  // LOOK HERE (at the line below)
			image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

private:
	// Note: image is an OBJECT, not a POINTER. This is Composition in C++ - the SimpleImage object is actually a part of BouncingImageObject
	// Automatically destroyed when the BouncingImageObject it is a part of is destroyed.
	SimpleImage image;   // LOOK HERE
};

