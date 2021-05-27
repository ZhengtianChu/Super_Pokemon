#pragma once

// Dragable objects are handled in here
// In general:
//	catch the mouse down if it happens within the object, and start the drag.
//	catch mouse move to move the object the same amount as the mouse is dragged.
//	catch mouse up to end the drag and leave the object at its new position
// The base class DragableBaseClassObject implements a number of methods which can be used by subclasses, e.g.
//	implement potential mouse-down to check whether click is inside object
//	implement potential drag to move object
//	implement potential mouse up to place the object and end the drag
// The sub classes can just implement mouse up/down etc by calling the base class methods

#include "DisplayableObject.h"
#include "BaseEngine.h"



//
// Base class object - provides implementation for functionality, allowing multiple sub-classes to support it in different ways if desired.
// 
class DragableBaseClassObject : public DisplayableObject
{
public:
	// Constructors
	DragableBaseClassObject(BaseEngine* pEngine, int iWidth = 0, int iHeight = 0, bool useTopLeftFor00 = true)
		: DisplayableObject(pEngine, iWidth, iHeight, useTopLeftFor00),
		m_bIsBeingDragged(false), m_iPositionAtStartOfDragX(0), m_iPositionAtStartOfDragY(0), m_iInitialClickedDragX(0), m_iInitialClickedDragY(0)
	{
	}

	DragableBaseClassObject(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true)
		: DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, useTopLeftFor00), m_bIsBeingDragged(false)
		, m_iPositionAtStartOfDragX(0), m_iPositionAtStartOfDragY(0), m_iInitialClickedDragX(0), m_iInitialClickedDragY(0)
	{
	}

	// This is called instead of redraw directly so you could optionally redraw rectangle instead of screen.
	// Implement this to redrawRectangle or redrawScreen
	virtual void doDragRedraw()
	{
		redrawDisplay(); // It moved so do a redraw!
	}

	// Call this from mouse-down if you want to start a drag - it will check the bounds first through.
	virtual void checkStartDrag(int iX, int iY)
	{
		//std::cout << "checkStartDrag " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			// std::cout << "Drag started..." << std::endl;
			m_bIsBeingDragged = true;
			m_iInitialClickedDragX = iX;
			m_iInitialClickedDragY = iY;
			m_iPositionAtStartOfDragX = m_iCurrentScreenX;
			m_iPositionAtStartOfDragY = m_iCurrentScreenY;
			doDragRedraw();
		}
	}

	// Call from mouse moved if you want to 
	virtual void checkDoDrag(int iX, int iY)
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (m_bIsBeingDragged)
		{
			doDragRedraw(); // In case it is doing a rectangle we need to include the old position too before moving it
			m_iCurrentScreenX = m_iPositionAtStartOfDragX + iX - m_iInitialClickedDragX;
			m_iCurrentScreenY = m_iPositionAtStartOfDragY + iY - m_iInitialClickedDragY;
			doDragRedraw(); // It moved so do a redraw!
		}
	}

	// Call from mouse up to complete the drag if you wish
	virtual void checkFinishDrag(int iX, int iY)
	{
		if (m_bIsBeingDragged)
		{
			//std::cout << "checkFinishDrag " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
			m_bIsBeingDragged = false;
			doDragRedraw();
		}
	}

protected:
	bool m_bIsBeingDragged;
	int m_iPositionAtStartOfDragX, m_iPositionAtStartOfDragY;
	int m_iInitialClickedDragX, m_iInitialClickedDragY;
};






//
// Subclass for a basic dragable object - subclass this to provide your own drawing if you wish
//
class DragableObject : public DragableBaseClassObject
{
public:
	// Constructors
	DragableObject(BaseEngine* pEngine, int iWidth = 0, int iHeight = 0, bool useTopLeftFor00 = true, bool bVisible = true )
		: DragableBaseClassObject(pEngine, iWidth, iHeight, useTopLeftFor00)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	DragableObject(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableBaseClassObject(xStart, yStart, pEngine, iWidth, iHeight, useTopLeftFor00)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	/* Override if necessary - Mouse event - mouse button pressed. 
	You must call notifyObjectsAboutMouse (e.g. in initialise or this class) to tell base engine to notify objects (see constructor), or this will not be called 
	*/
	virtual void virtMouseDown(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse down " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkStartDrag(iX, iY);
	}

	/* Override if necessary - Mouse event - mouse was moved. 
	Must call notifyObjectsAboutMouse (e.g. in initialise or this class) to tell base engine to notify objects (see constructor), or this method will not be called 
	*/
	virtual void virtMouseMoved(int iX, int iY)
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkDoDrag(iX, iY);
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkFinishDrag(iX, iY);
		redrawDisplay(); // Force full redraw of screen
	}
};







/*
This is a dragable object which draws a specific image, from a URL provided to the constructor.
*/
class DragableImageObject : public DragableBaseClassObject
{
public:
	/*
	Constructor - specify the URL to show and whether to use the top left of the image or the centre as the position of the object on the screen.
	This method also allows an initial position on the screen to be specified.
	*/
	DragableImageObject(int xStart, int yStart, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableBaseClassObject(xStart, yStart, pEngine, 100, 100, useTopLeftFor00),
		image(ImageManager::get()->getImagebyURL(strURL, true/*Load*/, false/*Keep it loaded*/))
	{
		m_iDrawWidth = image.getWidth();
		m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	/*
	Constructor - specify the URL to show and whether to use the top left of the image or the centre as the position of the object on the screen.
	Defaults the x and y positions of the object.
	*/
	DragableImageObject(BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableImageObject( 100, 100, pEngine, strURL, useTopLeftFor00, bVisible)
	{
	}

	/*
	Drawing the object means drawing the image at the current coordinate.
	*/
	virtual void virtDraw()
	{
		if (isVisible())
		{
			image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

	/*
	Override if necessary - Mouse event - mouse button pressed. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called.
	*/
	virtual void virtMouseDown(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse down " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkStartDrag(iX, iY);
	}

	/* 
	Override if necessary - Mouse event - mouse was moved. Must call notifyObjectsAboutMouse to tell base engine to notify objects (see constructor), or this method will not be called.
	*/
	virtual void virtMouseMoved(int iX, int iY)
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkDoDrag(iX, iY);
	}

	/* 
	Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called.
	*/
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		checkFinishDrag(iX, iY);
	}

protected:
	// The image to draw
	SimpleImage image;
};











/*
MappableImageObject is a dragable object 
*/
class MappableImageObject : public DragableImageObject
{
public:
	// Constructors
	MappableImageObject(ImagePixelMapping* pMapping, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableImageObject(pEngine, strURL, useTopLeftFor00, bVisible)
		, m_pMapping(pMapping)
	{

	}

	MappableImageObject(ImagePixelMapping* pMapping, int xStart, int yStart, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableImageObject(xStart, yStart, pEngine, strURL, useTopLeftFor00, bVisible)
		, m_pMapping(pMapping)
	{
	}

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw()
	{
		if (isVisible())
		{
			image.renderImageApplyingMapping(getEngine(), getEngine()->getForegroundSurface(),
				m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight,
				m_pMapping);
		}
	}

	// Set the transparency colour
	void setTransparencyColour(int colour) { image.setTransparencyColour(colour); }

	// Retrieve the colour of a pixel - primarily used so we can get pixel 0,0 to set the transparency
	int getPixelColour(int x, int y) { return image.getPixelColour(x, y); }

	// Return the width of the loaded image
	int getImageWidth() { return image.getWidth(); }

	// Return the height of the loaded image
	int getImageHeight() { return image.getHeight(); }

protected:
	ImagePixelMapping* m_pMapping;
};
