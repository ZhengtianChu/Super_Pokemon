#pragma once

// For these purposes a sprite is assumed to be an object which can move itself and shows an image

#include "header.h"
#include "DisplayableObject.h"
#include "ImageManager.h"
#include "BaseEngine.h"

/*
Look at the dragable image class first and understand how it works, since most of this code is related to dragable images.
The only code for the flashing is in the virtDoUpdate and virtDraw methods.
*/
class SimpleFlashingDragableImageObject : public DisplayableObject
{
public:
	// Constructors
	SimpleFlashingDragableImageObject(BaseEngine* pEngine, std::string strURL1, std::string strURL2, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(pEngine, 100, 100, useTopLeftFor00),
		image1(ImageManager::get()->getImagebyURL(strURL1, true/*Load*/, false/*don't keep it loaded*/)),
		image2(ImageManager::get()->getImagebyURL(strURL2, true/*Load*/, false/*don't keep it loaded*/)),
		// Note: the above is like: ImageManager::loadImage(strURL, false/*don't keep it loaded*/)
		m_bIsBeingDragged(false),
		imageNum(0)
	{
		this->m_iDrawWidth = image1.getWidth();
		this->m_iDrawHeight = image1.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(true);
	}

	SimpleFlashingDragableImageObject(int xStart, int yStart, BaseEngine* pEngine, std::string strURL1, std::string strURL2, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, 100, 100, useTopLeftFor00),
		image1(ImageManager::get()->getImagebyURL(strURL1, true/*Load*/, false/*Keep it loaded*/)),
		image2(ImageManager::get()->getImagebyURL(strURL2, true/*Load*/, false/*Keep it loaded*/)),
		m_bIsBeingDragged(false),
		imageNum(0)
	{
		m_iDrawWidth = image1.getWidth();
		m_iDrawHeight = image1.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	// Do update switches which image is being used, and forces redraw if it changes
	virtual void virtDoUpdate(int iCurrentTime) override
	{
		if (iCurrentTime / 500 % 2 != imageNum)
		{
			imageNum = 1 - imageNum; // Switch between 0 and 1
			redrawDisplay();
		}
	}

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw() override
	{
		if (isVisible())
		{
			if (imageNum == 0)
				image1.renderImageWithMaskAndTransparency(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight, 0x000000, 90);
			else
				image2.renderImageWithMaskAndTransparency(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight, 0x000000, 90 );
		}
	}

	/* Override if necessary - Mouse event - mouse button pressed. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "Mouse down " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			//printf("Drag started...");
			m_bIsBeingDragged = true;
			m_iInitialClickedDragX = iX;
			m_iInitialClickedDragY = iY;
			m_iStartOfDragX = m_iCurrentScreenX;
			m_iStartOfDragY = m_iCurrentScreenY;
		}
	}

	/* Override if necessary - Mouse event - mouse was moved. Must call notifyObjectsAboutMouse to tell base engine to notify objects (see constructor), or this method will not be called */
	virtual void virtMouseMoved(int iX, int iY) override
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (m_bIsBeingDragged)
		{
			m_iCurrentScreenX = m_iStartOfDragX + iX - m_iInitialClickedDragX;
			m_iCurrentScreenY = m_iStartOfDragY + iY - m_iInitialClickedDragY;
			redrawDisplay(); // It moved so do a redraw!
		}
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY) override
	{
		//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		m_bIsBeingDragged = false;
	}

private:
	bool m_bIsBeingDragged;
	int m_iStartOfDragX, m_iStartOfDragY, m_iInitialClickedDragX, m_iInitialClickedDragY;
	SimpleImage image1;
	SimpleImage image2;
	int imageNum;
};
