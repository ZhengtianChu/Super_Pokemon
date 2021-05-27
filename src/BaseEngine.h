#ifndef BASEGAMEENGINE_H
#define BASEGAMEENGINE_H

// This is the base class for all of the main classes for the programs you will create.
// The main class usually handled key presses, mouse move/click, background drawing, creating the moving objects, and displaying information strings.
// The drawing functions delegate the actual drawing work to DrawingSurface objects, which you don't really need to look at unless you are doing something complex.

#include "DisplayableObject.h"
#include "DrawingSurface.h"

// Prototype the class, so we can use pointers of type DisplayableObject*

#include "FontManager.h"
#include "DisplayableObjectContainer.h"
#include "SimpleImage.h"
#include "ImageManager.h"


/*
Main base class for the program as a whole
Create a subclass of this class and then an instance of that class.
Override various functions, but primarily those called virt...().
*/
class BaseEngine :
	public DisplayableObjectContainer, // This object can contain and iterate through a set of displayable objects
	public FilterPoints // This object can also be used as a filter for points - e.g. restricting to only drawing inside update region
{
public:
	/* Constructor. See .cpp file. */
	BaseEngine();

	/* Virtual destructor. See .cpp file.  */
	virtual ~BaseEngine();

	//
	// Internal functions
	//

	/* Initialise the SDL and create a window of the designated size. See .cpp file. */
	int initialise(const char* szCaption, int iWindowWidth, int iWindowHeight, const char* szFontName, int iFontSize);

	/* Deinitialise everything required. See .cpp file. */
	void deinitialise(void);

	/* The main game loop - will call the draw and gamel logic functions in turn. See .cpp file. */
	int mainLoop(void);

	// Handle an SDL event.  See .cpp file. 
	bool handleEvent(SDL_Event* pEvent);

public:
	//
	// Utility functions: (call these when you need to do something)
	//

	/*
	Is the key with the specified code currently being pressed?
	*/
	bool isKeyPressed(int iKeyCode) const;

	/* Last clicked X coordinate of mouse - button down - returns the virtual position (if that matters, e.g. you applied filters) */
	int getMouseXClickedDown() const { return m_iMouseXClickedDown; }
	/* Last clicked Y coordinate of mouse - button down - returns the virtual position (if that matters, e.g. you applied filters) */
	int getMouseYClickedDown() const { return m_iMouseYClickedDown; }

	/* Last clicked X coordinate of mouse - button up - returns the virtual position (if that matters, e.g. you applied filters) */
	int getMouseXClickedUp() const { return m_iMouseXClickedUp; }
	/* Last clicked Y coordinate of mouse - button up - returns the virtual position (if that matters, e.g. you applied filters) */
	int getMouseYClickedUp() const { return m_iMouseYClickedUp; }

	/* Current X coordinate of mouse - returns the virtual position (if that matters, e.g. you applied filters)  */
	int getCurrentMouseX() const { return m_iCurrentMouseX; }
	/* Current Y coordinate of mouse - returns the virtual position (if that matters, e.g. you applied filters)  */
	int getCurrentMouseY() const { return m_iCurrentMouseY; }

	/* Current states of buttons */
	int getCurrentButtonStates() const { return m_iCurrentButtonStates; }

	/*
	Ask SDL for current details about mouse position and buttons.
	Shouldn't be needed since the information is already cached and available using the functions above.
	You can then use the functions above to get the values.
	*/
	void updateMouseInfo() 
	{
		m_iCurrentButtonStates = SDL_GetMouseState(&m_iCurrentMouseX, &m_iCurrentMouseY); 
	}



	/* Set the exit code - effectively ending the program immediately! */
	void setExitWithCode(int iCode)
	{
		m_iExitWithCode = iCode;
	}



public:
	//
	// Overridables - these are the functions that you are likely to create overrides for in sub-classes:
	//

	/* Override if necessary - a key was released. See also IsKeyPressed(). e.g. use code 'd' for the d key. */
	virtual void virtKeyUp(int iKeyCode);
	/* Override if necessary - a key was pressed. See also IsKeyPressed() */
	virtual void virtKeyDown(int iKeyCode);

	/* Override if necessary - Mouse event - mouse was moved - provides the virtual x and y positions (if that matters, e.g. you applied filters) */
	virtual void virtMouseMoved(int iX, int iY);
	/* Override if necessary - Mouse event - mouse button pressed - provides the virtual x and y positions (if that matters, e.g. you applied filters) */
	virtual void virtMouseDown(int iButton, int iX, int iY);
	/* Override if necessary - Mouse event - mouse button released - provides the virtual x and y positions (if that matters, e.g. you applied filters) */
	virtual void virtMouseUp(int iButton, int iX, int iY);

	/* You should not need this unless you need to know the real coordinate of a mouse click */
	virtual void virtRealMouseDown(int iButton, int iX, int iY) {}
	/* You should not need this unless you need to know the real coordinate of a mouse click */
	virtual void virtRealMouseUp(int iButton, int iX, int iY) {}

	/* Override to handle mouse wheel event - see SDL_MouseWheelEvent */
	virtual void virtMouseWheel(int x, int y, int which, int timestamp) {}

	/* Override to handle a touch event - see SDL_TouchFingerEvent */
	virtual void virtTouchEvent(int type, int timestamp, SDL_TouchID touchID, SDL_FingerID fingerID, float x, float y, float dx, float dy, float pressure)
	{}

	/* Override if necessary - handle initialisation, e.g. create objects */
	virtual int virtInitialise();

	/* You could override this, e.g. to create different sized windows, but I assume you probably won't usually do so.
	Hint: don't bother to override this unless you want to do something unusual
	*/
	virtual void virtCreateWindows(const char* szCaption);

	/* You could override this, e.g. to create different sized surfaces, but I assume you probably won't usually do so.
	Hint: don't bother to override this unless you want to do something unusual
	*/
	virtual void virtCreateSurfaces();

	/* Override if necessary - handle cleanup - e.g. delete allocated objects */
	virtual void virtCleanUp(void);

	/* You probably don't need this. It is used if you are handling clicks with objects and this will tell you when the user
	clicked on something that is NOT an object - i.e. the background */
	virtual void virtBackgroundMouseDown(int iButton, int iRealX, int iRealY, int iVirtualX, int iVirtualY) {}

	/*
	Methods and variables to maintain whether displayable objects are told about key and mouse operations
	If you wish to do so, you can ensure that objects are notified about events as well as the BaseEngine subclass being notified.
	You can therefore move code into objects rather than handling it in your BaseEngine sub-class.
	Note however that this will do a load of extra function calls (one per object) so by default it won't do it.
	Call these functions to tell the objects.
	Also, you may just want to know when your 'top' or 'bottom' object is clicked (e.g. to start a drag operation). In that case tell the system to notify your top (or bottom) object about mouse clicks.
	*/
public:
	void notifyObjectsAboutKeys(bool bNotifyObjectsYesNo) { m_bNotifyObjectsAboutKeys = bNotifyObjectsYesNo; }
	void notifyObjectsAboutMouse(bool bNotifyObjectsYesNo) { m_bNotifyObjectsAboutMouse = bNotifyObjectsYesNo; }
	void notifyTopObjectAboutMouseClick(bool bNotifyObjectsYesNo) { m_bNotifyTopObjectAboutMouse = bNotifyObjectsYesNo; }
	void notifyBottomObjectAboutMouseClick(bool bNotifyObjectsYesNo) { m_bNotifyBottomObjectAboutMouse = bNotifyObjectsYesNo; }

private:
	bool m_bNotifyObjectsAboutKeys;
	bool m_bNotifyObjectsAboutMouse;
	bool m_bNotifyTopObjectAboutMouse;
	bool m_bNotifyBottomObjectAboutMouse;

public:
	/*
	You probably need to override this:
	Do any setup of back buffer prior to locking the window buffer.
	Basically do the drawing of the background in here and it'll be copied to the window for you as needed.
	*/
	virtual void virtSetupBackgroundBuffer();

	/*
	Just a wrapper for a call to virtSetupBackgroundBuffer(), but it will lock the background surface first, and redraw it afterwards.
	*/
	void lockAndSetupBackground()
	{
		lockBackgroundForDrawing();
		virtSetupBackgroundBuffer();
		unlockBackgroundForDrawing();
		// Tell screen to redraw it now we set it up
		redrawDisplay();
	}

	/* You probably need to override this: Draw text labels for the window */
	virtual void virtDrawStringsUnderneath();
	virtual void virtDrawStringsOnTop();


	// Called at the start of MainLoop since one student thought this would be useful.
	virtual void virtMainLoopStartIteration() {}

	// Called in main loop before checking if it is time to run - before update and render
	virtual void virtMainLoopPreUpdate() {} // Before update
	virtual void virtMainLoopDoBeforeUpdate() {} // Just before objects update, only if they should update - i.e. not every pass
	virtual void virtMainLoopDoAfterUpdate() {} // Just after objects update, only if they should update - i.e. not every pass
	virtual void virtMainLoopPostUpdate() {} // After update

	// Change behaviour in subclass if needed.
	// If this returns false then objects will not be told to move - allowing you to freeze them all if you wish
	virtual bool virtCheckForUpdateTime(void);


	/* 
	You probably don't need to change this function, which modifies the screen. 
	Instead just draw the background and implement the obejcts to draw themselves.
	If you need to draw behind everything you can use virtPreDraw() to draw AFTER background, before everything else
	If you need to draw on top of everything, you can use virtPostDraw() to draw on top of everything
	*/
	virtual void virtRenderScreen();

	// Draw anything else before the sctrings and objects
	virtual void virtPreDraw()
	{
		// Base version does nothing
	}

	// Draw anything else afterwards
	virtual void virtPostDraw()
	{
		// Base version does nothing
	}

	//
	// Drawing functions
	//

public:

	/* Get Window width, in pixels */
	int getWindowWidth() const { return m_iWindowWidth; }

	/* Get window height, in pixels */
	int getWindowHeight() const { return m_iWindowHeight; }

	// NOTE: There are DrawingSurface objects for the foreground and background which actually do the drawing work, so many functions just delegate the work to those objects
	// These surfaces are both created to be sized as the WindowWidth and WindowHeight.
	// You could create 

	/* Foreground surface object that can be used */
	DrawingSurface* getForegroundSurface() { return m_pForegroundSurface; }
	/* Background surface object that can be used */
	DrawingSurface* getBackgroundSurface() { return m_pBackgroundSurface; }

	/* Normally you should not need to do these, but if you need for some reason to call draw methods at an unusual time
	then first lock the surface, then draw, then remember to unlock it.
	On windows it doesn't usually matter (except that you get a warning message telling you that you drew without locking),
	but it can sometimes mess up other operating systems implementations.
	In general, only draw in the usual functions and you have no problems.
	*/
	void lockForegroundForDrawing() { m_pForegroundSurface->mySDLLockSurface(); }
	void unlockForegroundForDrawing() { m_pForegroundSurface->mySDLUnlockSurface(); }

	void lockBackgroundForDrawing() { m_pBackgroundSurface->mySDLLockSurface(); }
	void unlockBackgroundForDrawing() { m_pBackgroundSurface->mySDLUnlockSurface(); }

	/* Method to fill the entire background with the same colour */
	virtual void fillBackground(unsigned int uiPixelColour) 
	{ m_pBackgroundSurface->fillSurface(uiPixelColour); }

	/* Copy all of the background buffer onto the window. 
	Note: I made it virtual in case you want to do something unusual - see starfield demo.
	*/
	virtual void copyAllBackgroundBuffer() { m_pForegroundSurface->copyEntireSurface(m_pBackgroundSurface);  }

	/* Copy the background pixels in the specified (real pixels) region onto the foreground. 
	x and y are REAL coordinates on the screen, so are NOT affected by scrolling or zooming.
	Note: I made it virtual in case you want to do something unusual, since it gets called automatically for you.
	*/
	virtual void copyBackgroundPixels( int iRealPixelX, int iRealPixelY, int iWidth, int iHeight )
	{
		m_pForegroundSurface->copyRectangleFrom(m_pBackgroundSurface, iRealPixelX, iRealPixelY, iWidth, iHeight );
	}

	// Copy all of one suface onto another.
	static void copySurface( DrawingSurface* from, DrawingSurface* to )
	{ 
		to->copyEntireSurface(from);
	}

	/* Copy a rectangle between two surfaces
	x and y are REAL coordinates on the screen, so are NOT affected by scrolling or zooming.
	*/
	static void copyRectangleBetweenSurfaces(DrawingSurface* from, DrawingSurface* to, int iRealPixelX, int iRealPixelY, int iWidth, int iHeight)
	{
		to->copyRectangleFrom(from, iRealPixelX, iRealPixelY, iWidth, iHeight);
	}

	/*
	Get a colour code for a specific index.
	The only purpose of this function is to prevents having to reinvent colours, you can just use a number without understanding RGB colour codes.
	*/
	unsigned int getColour(int iColourIndex) const;


	/* Method to set a pixel colour on the window. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	inline void setForegroundPixel( int iVirtualX, int iVirtualY, unsigned int uiColour )
	{
		m_pForegroundSurface->setPixel( iVirtualX, iVirtualY, uiColour );
	}

	/* Method to set a pixel colour in the background buffer. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	inline void setBackgroundPixel( int iVirtualX, int iVirtualY, unsigned int uiColour )
	{
		m_pBackgroundSurface->setPixel( iVirtualX, iVirtualY, uiColour );
	}

	/* Draw a line on the foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundLine(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pForegroundSurface->drawLine( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw a line on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundLine(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pBackgroundSurface->drawLine( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw a thick line on the foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundThickLine(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour, int iThickness)
	{
		m_pForegroundSurface->drawThickLine( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour, iThickness );
	}

	/* Draw a thick line on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundThickLine(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour, int iThickness)
	{
		m_pBackgroundSurface->drawThickLine( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour, iThickness );
	}


	/* Draw a region which has vertical sides on the foreground - one side may be a point, making a triangle. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundVerticalSidedRegion(
		double fVirtualX1, double fVirtualX2,
		double fVirtualY1a, double fVirtualY2a,
		double fVirtualY1b, double fVirtualY2b,
		unsigned int uiColour )
	{
		m_pForegroundSurface->drawVerticalSidedRegion( fVirtualX1, fVirtualX2, fVirtualY1a, fVirtualY2a, fVirtualY1b, fVirtualY2b, uiColour );
	}

	/* Draw a region which has vertical sides on the background - one side may be a point, making a triangle. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundVerticalSidedRegion(
		double fVirtualX1, double fVirtualX2,
		double fVirtualY1a, double fVirtualY2a,
		double fVirtualY1b, double fVirtualY2b,
		unsigned int uiColour )
	{
		m_pBackgroundSurface->drawVerticalSidedRegion( fVirtualX1, fVirtualX2, fVirtualY1a, fVirtualY2a, fVirtualY1b, fVirtualY2b, uiColour );
	}

	/* Draw a triangle on the foreground where all three points are specified.. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundTriangle(
		int iVirtualX1, int iVirtualY1,
		int iVirtualX2, int iVirtualY2,
		int iVirtualX3, int iVirtualY3,
		unsigned int uiColour )
	{
		m_pForegroundSurface->drawTriangle( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, iVirtualX3, iVirtualY3, uiColour );
	}

	/* Draw a triangle on the background where all three points are specified.. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundTriangle(
		int iVirtualX1, int iVirtualY1,
		int iVirtualX2, int iVirtualY2,
		int iVirtualX3, int iVirtualY3,
		unsigned int uiColour )
	{
		m_pBackgroundSurface->drawTriangle( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, iVirtualX3, iVirtualY3, uiColour );
	}



	/* Draw a rectangle on the foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundRectangle(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pForegroundSurface->drawRectangle( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw a rectangle on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundRectangle(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pBackgroundSurface->drawRectangle( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw an oval on the foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundOval(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pForegroundSurface->drawOval( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw an oval on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundOval(int iVirtualX1, int iVirtualY1, int iVirtualX2, int iVirtualY2, unsigned int uiColour)
	{
		m_pBackgroundSurface->drawOval( iVirtualX1, iVirtualY1, iVirtualX2, iVirtualY2, uiColour );
	}

	/* Draw a polygon on the window. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4, unsigned int uiColour )
	{
		double pXArray[4] = { fX1, fX2, fX3, fX4 };
		double pYArray[4] = { fY1, fY2, fY3, fY4 };
		m_pForegroundSurface->drawPolygon( 4, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the Foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, unsigned int uiColour )
	{
		double pXArray[5] = { fX1, fX2, fX3, fX4, fX5 };
		double pYArray[5] = { fY1, fY2, fY3, fY4, fY5 };
		m_pForegroundSurface->drawPolygon( 5, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the Foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6, unsigned int uiColour )
	{
		double pXArray[6] = { fX1, fX2, fX3, fX4, fX5, fX6 };
		double pYArray[6] = { fY1, fY2, fY3, fY4, fY5, fY6 };
		m_pForegroundSurface->drawPolygon( 6, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the Foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, unsigned int uiColour )
	{
		double pXArray[7] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7 };
		double pYArray[7] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7 };
		m_pForegroundSurface->drawPolygon( 7, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the Foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, double fX8, double fY8, unsigned int uiColour )
	{
		double pXArray[8] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7, fX8 };
		double pYArray[8] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7, fY8 };
		m_pForegroundSurface->drawPolygon( 8, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the Foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundPolygon( int iPoints, double* pXArray, double* pYArray, unsigned int uiColour )
	{
		m_pForegroundSurface->drawPolygon( iPoints, pXArray, pYArray, uiColour );
	}

	/* Draw a string to the foreground. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawForegroundString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL)
	{
		m_pForegroundSurface->drawScalableString(iX, iY, pText, uiColour, pFont);
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4, unsigned int uiColour )
	{
		double pXArray[4] = { fX1, fX2, fX3, fX4 };
		double pYArray[4] = { fY1, fY2, fY3, fY4 };
		m_pBackgroundSurface->drawPolygon( 4, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, unsigned int uiColour )
	{
		double pXArray[5] = { fX1, fX2, fX3, fX4, fX5 };
		double pYArray[5] = { fY1, fY2, fY3, fY4, fY5 };
		m_pBackgroundSurface->drawPolygon( 5, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6, unsigned int uiColour )
	{
		double pXArray[6] = { fX1, fX2, fX3, fX4, fX5, fX6 };
		double pYArray[6] = { fY1, fY2, fY3, fY4, fY5, fY6 };
		m_pBackgroundSurface->drawPolygon( 6, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, unsigned int uiColour )
	{
		double pXArray[7] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7 };
		double pYArray[7] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7 };
		m_pBackgroundSurface->drawPolygon( 7, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( double fX1, double fY1, double fX2, double fY2,
		double fX3, double fY3, double fX4, double fY4,
		double fX5, double fY5, double fX6, double fY6,
		double fX7, double fY7, double fX8, double fY8, unsigned int uiColour )
	{
		double pXArray[8] = { fX1, fX2, fX3, fX4, fX5, fX6, fX7, fX8 };
		double pYArray[8] = { fY1, fY2, fY3, fY4, fY5, fY6, fY7, fY8 };
		m_pBackgroundSurface->drawPolygon( 8, pXArray, pYArray, uiColour );
	}

	/* Draw a polygon on the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundPolygon( int iPoints, double* pXArray, double* pYArray, unsigned int uiColour )
	{
		m_pBackgroundSurface->drawPolygon( iPoints, pXArray, pYArray, uiColour );
	}

	/* Draw a string to the background. x and y are virtual coordinates on the screen and could be changed by scrolling or zooming. */
	void drawBackgroundString(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL )
	{
		m_pBackgroundSurface->drawScalableString( iX, iY, pText, uiColour, pFont );
	}


	/* Method to set a pixel colour on the window WITHOUT and checking or filtering - you probably want to use the non-raw method!
	These are actual pixel positions on the screen - so make sure that they are in range otherwise you may corrupt memory.
	*/
	inline void rawSetForegroundPixel(int iRealX, int iRealY, unsigned int uiColour)
	{
		m_pForegroundSurface->rawSetPixel(iRealX, iRealY, uiColour);
	}

	/* Method to set a pixel colour on the background WITHOUT and checking or filtering - you probably want to use the non-raw method! 
	These are actual pixel positions on the screen - so make sure that they are in range otherwise you may corrupt memory.
	*/
	inline void rawSetBackgroundPixel(int iRealX, int iRealY, unsigned int uiColour)
	{
		m_pBackgroundSurface->rawSetPixel(iRealX, iRealY, uiColour);
	}

	/* Method to get a pixel colour on the window, in-lined, but checks the supplied parameters. */
	inline unsigned int rawGetForegroundPixel(int iRealPixelX, int iRealPixelY) const
	{
		return m_pForegroundSurface->rawGetPixel(iRealPixelX, iRealPixelY);
	}

	/* Method to set a pixel colour on the window, in-lined, but checks the supplied parameters */
	inline unsigned int rawGetBackgroundPixel(int iRealPixelX, int iRealPixelY) const
	{
		return m_pBackgroundSurface->rawGetPixel(iRealPixelX, iRealPixelY);
	}

	//
	// END OF DRAWING FUNCTIONS
	//

	/* This function is used to map from an SDL symbol to a key code.
	It exists primarily because the SDL codes changed between versions so this made it backward compatible 
	*/
	static int symbolToKeycode(int iSym)
	{
		if (iSym <= SDL12_SDLK_NORMAL)
			return iSym;

		switch (iSym)
		{
		case SDLK_UP: return SDL12_SDLK_NORMAL + 1;
		case SDLK_DOWN: return SDL12_SDLK_NORMAL + 2;
		case SDLK_LEFT: return SDL12_SDLK_NORMAL + 3;
		case SDLK_RIGHT: return SDL12_SDLK_NORMAL + 4;
		case SDLK_F1: return SDL12_SDLK_NORMAL + 5;
		case SDLK_F2: return SDL12_SDLK_NORMAL + 6;
		case SDLK_F3: return SDL12_SDLK_NORMAL + 7;
		case SDLK_F4: return SDL12_SDLK_NORMAL + 8;
		case SDLK_F5: return SDL12_SDLK_NORMAL + 9;
		case SDLK_F6: return SDL12_SDLK_NORMAL + 10;
		case SDLK_F7: return SDL12_SDLK_NORMAL + 11;
		case SDLK_F8: return SDL12_SDLK_NORMAL + 12;
		case SDLK_F9: return SDL12_SDLK_NORMAL + 13;
		case SDLK_F10: return SDL12_SDLK_NORMAL + 14;
		case SDLK_F11: return SDL12_SDLK_NORMAL + 15;
		case SDLK_F12: return SDL12_SDLK_NORMAL + 16;
		case SDLK_PRINTSCREEN: return SDL12_SDLK_NORMAL + 17;
		case SDLK_SCROLLLOCK: return SDL12_SDLK_NORMAL + 18;
		case SDLK_PAUSE: return SDL12_SDLK_NORMAL + 19;
		case SDLK_INSERT: return SDL12_SDLK_NORMAL + 20;
		case SDLK_HOME: return SDL12_SDLK_NORMAL + 21;
		case SDLK_PAGEUP: return SDL12_SDLK_NORMAL + 22;
		case SDLK_DELETE: return SDL12_SDLK_NORMAL + 23;
		case SDLK_END: return SDL12_SDLK_NORMAL + 24;
		case SDLK_PAGEDOWN: return SDL12_SDLK_NORMAL + 25;
		case SDLK_RETURN: return SDL12_SDLK_NORMAL + 26;
		case SDLK_BACKSPACE: return SDL12_SDLK_NORMAL + 27;
		case SDLK_ESCAPE: return SDL12_SDLK_NORMAL + 28;
		case SDLK_TAB: return SDL12_SDLK_NORMAL + 29;
		case SDLK_SPACE: return SDL12_SDLK_NORMAL + 30;
		case SDLK_CAPSLOCK: return SDL12_SDLK_NORMAL + 31;
		case SDLK_NUMLOCKCLEAR: return SDL12_SDLK_NORMAL + 32;
		case SDLK_KP_DIVIDE: return SDL12_SDLK_NORMAL + 33;
		case SDLK_KP_MULTIPLY: return SDL12_SDLK_NORMAL + 34;
		case SDLK_KP_MINUS: return SDL12_SDLK_NORMAL + 35;
		case SDLK_KP_PLUS: return SDL12_SDLK_NORMAL + 36;
		case SDLK_KP_ENTER: return SDL12_SDLK_NORMAL + 37;
		case SDLK_LCTRL: return SDL12_SDLK_NORMAL + 38;
		case SDLK_LSHIFT: return SDL12_SDLK_NORMAL + 39;
		case SDLK_LALT: return SDL12_SDLK_NORMAL + 40;
		case SDLK_LGUI: return SDL12_SDLK_NORMAL + 41;
		case SDLK_RCTRL: return SDL12_SDLK_NORMAL + 42;
		case SDLK_RSHIFT: return SDL12_SDLK_NORMAL + 43;
		case SDLK_RALT: return SDL12_SDLK_NORMAL + 44;
		case SDLK_RGUI: return SDL12_SDLK_NORMAL + 45;
		}

		return 0; // Unknown Symbol
	}

	// These functions are used for the update of the screen or screen region

	/* Indicate that either the entire window must be redrawn. */
	void redrawDisplay()
	{
		m_bTotalRedraw = true;
		m_bNeedsRedraw = true;
	}

	/* 
	Call this to specify that this section of the screen should be redrawn. 
	Adds to any earlier sections to redraw by expanding the redraw rectangle to encompass both regions.
	If a method called redrawDisplay() it will still redraw the whole screen!
	*/
	void redrawRectangle(int virtualLeft, int virtualTop, int virtualWidth, int virtualHeight)
	{
		//std::cout << "Redraw: " << std::to_string(virtualLeft) << "," << std::to_string(virtualTop) << " : " << std::to_string(virtualWidth) << "," << std::to_string(virtualHeight) << std::endl;
		int xScale = convertVirtualPixelToClickedXPosition(1) - convertVirtualPixelToClickedXPosition(0);
		if (xScale < 1) xScale = 1;
		int yScale = convertVirtualPixelToClickedYPosition(1) - convertVirtualPixelToClickedYPosition(0);
		if (yScale < 1) yScale = 1;

		if (virtualLeft < m_iRedrawVirtualLeft)
		{
			m_iRedrawVirtualLeft = virtualLeft - xScale;
			m_iRedrawRealLeft = this->convertVirtualPixelToClickedXPosition(m_iRedrawVirtualLeft);
		}
		if ( virtualTop < m_iRedrawVirtualTop)
		{
			m_iRedrawVirtualTop = virtualTop - yScale;
			m_iRedrawRealTop = convertVirtualPixelToClickedYPosition(m_iRedrawVirtualTop);
		}
		if ((virtualLeft + virtualWidth) > m_iRedrawVirtualRight)
		{
			m_iRedrawVirtualRight = virtualLeft + virtualWidth + xScale;
			m_iRedrawRealRight = convertVirtualPixelToClickedXPosition(m_iRedrawVirtualRight);
		}
		if ((virtualTop + virtualHeight) > m_iRedrawVirtualBottom)
		{
			m_iRedrawVirtualBottom = virtualTop + virtualHeight + yScale;
			m_iRedrawRealBottom = this->convertVirtualPixelToClickedYPosition(m_iRedrawVirtualBottom);
		}

		//std::cout << "Redraw area is: " << std::to_string(m_iRedrawRealLeft) << "," << std::to_string(virtualTop) << " to " << std::to_string(m_iRedrawVirtualRight) << "," << std::to_string(m_iRedrawVirtualBottom) << std::endl;

		m_bNeedsRedraw = true; // Some parts need to be redrawn
	}



	//
	// These functions are primarily needed only if you scale or translate the screen.
	// Ignore these functions if you don't do so.
	//


	// Determine which parts of the screen should be redrawn:

	// Determine whether entirity of screen should be redrawn
	int getRedrawAllScreen() { return m_bTotalRedraw; }
	// Get the left side of a virtual region of the screen to redraw
	int getRedrawRectVirtualLeft() { return m_iRedrawVirtualLeft; }
	// Get the top of a virtual region of the screen to redraw
	int getRedrawRectVirtualTop() { return m_iRedrawVirtualTop; }
	// Get the right side of a virtual region of the screen to redraw
	int getRedrawRectVirtualRight() { return m_iRedrawVirtualRight; }
	// Get the bottom of a virtual region of the screen to redraw
	int getRedrawRectVirtualBottom() { return m_iRedrawVirtualBottom; }
	// Get the left side of the real part of the foreground surface to redraw
	int getRedrawRectRealLeft() { return m_iRedrawRealLeft; }
	// Get the top of the real part of the foreground surface to redraw
	int getRedrawRectRealTop() { return m_iRedrawRealTop; }
	// Get the right side of the real part of the foreground surface to redraw
	int getRedrawRectRealRight() { return m_iRedrawRealRight; }
	// Get the bottom of the real part of the foreground surface to redraw
	int getRedrawRectRealBottom() { return m_iRedrawRealBottom; }

	/*
	Convert a click position to a pixel position : x coordinate
	Clicks are real positions on the screen, but if a scroll/translation has been applied or a zoom level then it may be necessary to work out which virtual position was clicked.
	*/
	int convertClickedToVirtualPixelXPosition(int iRealX)
	{
		return m_pForegroundSurface->convertRealToVirtualXPosition(iRealX);
	}

	/*
	Convert a click position to a pixel position : y coordinate
	Clicks are real positions on the screen, but if a scroll/translation has been applied or a zoom level then it may be necessary to work out which virtual position was clicked.
	*/
	int convertClickedToVirtualPixelYPosition(int iRealY)
	{
		return m_pForegroundSurface->convertRealToVirtualYPosition(iRealY);
	}

	/*
	Convert a virtual pixel position to a click/pixel position on the screen : x coordinate
	A 'virtual' coordinate system is maintained and this goes from a virtual coordinate (from a scroll or zoom) to a real pixel position.
	*/
	int convertVirtualPixelToClickedXPosition(int iVirtualX)
	{
		return m_pForegroundSurface->convertVirtualToRealXPosition(iVirtualX);
	}

	/*
	Convert a virtual pixel position to a click/pixel position on the screen : y coordinate
	A 'virtual' coordinate system is maintained and this goes from a virtual coordinate (from a scroll or zoom) to a real pixel position.
	*/
	int convertVirtualPixelToClickedYPosition(int iVirtualY)
	{
		return m_pForegroundSurface->convertVirtualToRealYPosition(iVirtualY);
	}

	/*
	This implements a default filter which, if used, will restrict to only update the visible or update region of screen, preventing drawing outside of that region.
	Note: the filter needs to be applied if it is to be used.
	x and y values here will be real coordinates by the time they reach this function, so just check whether these coordinates are within the visible region
	*/
	virtual bool filter(DrawingSurface* surface, int& xReal, int& yReal, unsigned int& uiColour)
	{
		if (xReal < 0 || xReal >= this->getWindowWidth() ||
			yReal < 0 || yReal >= this->getWindowHeight())
			return false;

		// If doing a partial redraw then we also need to check that we are within the redraw region
		if ( m_bNeedsRedraw && !m_bTotalRedraw )
		{
			if (xReal < m_iRedrawRealLeft || xReal > m_iRedrawRealRight || yReal < m_iRedrawRealTop || yReal > m_iRedrawRealBottom)
				return false;
		}
		return true;
	}

protected:
	// Whether to draw the entire screen or just a part. true means redraw entire screen.
	bool m_bTotalRedraw;

	// The position of the rectangle part of the screen to redraw, in virtual pixels - i.e. pixels in the underlying drawing. 
	// These may differ from real pixels if you apply a zoom (scaling) or shift to the window, displaying things at an offset or scaled position.
	int m_iRedrawVirtualLeft;
	int m_iRedrawVirtualTop;
	int m_iRedrawVirtualRight;
	int m_iRedrawVirtualBottom;

	// The position of the rectangle part of the screen to redraw, in real pixels - i.e. pixels in the window on the screen, so 0,0 is top left.
	int m_iRedrawRealLeft;
	int m_iRedrawRealTop;
	int m_iRedrawRealRight;
	int m_iRedrawRealBottom;

public:
	//
	// TIME functions
	//

	/*
	Function to get the time, in terms of the number of ticks.
	Can be used to decide which animation to display, or whether it's time to do something yet
	*/
	inline int getRawTime() const 
	{ 
		return m_iTick; 
	}

	/*
	Function to get the time, in terms of the number of ticks.
	This version allows the subclass to change the time for pausing etc.
	The getTime() function is NOT virtual, this function is.
	This also adds on a time offset, which you can use/modify/set appropriately to handle pause, for example.
	*/
	virtual int getModifiedTime() const 
	{ 
		if ( m_iPaused >= 0)
			return (m_iPaused- m_iTimeOffset); // Still at the time it was paused
		return m_iTick - m_iTimeOffset; 
	}

	// Call this to stop the modified time from changing until you call unpause()
	void pause()
	{
		//std::cout << "paused" << std::endl;
		m_iPaused = m_iTick;
		redrawDisplay();
	}

	// Call this to allow modified time to keep ticking up
	void unpause()
	{
		//std::cout << "unpaused" << std::endl;
		m_iTimeOffset += m_iTick - m_iPaused; // Extra time it has just been paused for
		m_iPaused = -1; // Unpaused now
		redrawDisplay();
	}

	/* Determine whether program is currently paused - i.e. whether pause() function was called */
	bool isPaused() 
	{ 
		return m_iPaused != -1; 
	}

	/* Specify the next time at which to act : current time plus the required delay, in ticks */
	void setTimeToAct(int iDelay) { m_iNextTickToActOn = m_iTick + iDelay; }
	
	/* Return true if it is time to act, false otherwise */
	inline bool isTimeToAct() const { return m_iTick >= m_iNextTickToActOn; }

	/* Returns true if it is now time to act (should always be the case) or false otherwise, 
	and applies a 9ms sleep if the required delay is longer than 9ms */
	inline bool isTimeToActWithSleep() const
	{
		if (m_iTick >= m_iNextTickToActOn)
			return true;
		if (m_iNextTickToActOn - m_iTick >= 9)
			SDL_Delay(9);
		return m_iTick >= m_iNextTickToActOn;
	}


protected:
	/* Current tick/time */
	int m_iTick;

	/* A time offset which can be used to modify the 'modifiedtime' returned to caller. */
	int m_iTimeOffset;

	/* The next tick to act on - to prevent continuous updates */
	int m_iNextTickToActOn;

	/* Value to specify whether program is currently paused - which pauses the modified time - set to -1 if NOT paused */
	int m_iPaused;

	// Number of milliseconds between updates of the objects. Defaults to 10.
	// No accessor or mutator has been provided but subclasses can change this if they wish.
	int m_iDefaultUpdatePeriod;

	//
	// Other functionality for i-o and exit program
	//

protected:

	/*
	If set to other than -1 the program will exit with this exit code.
	*/
	int m_iExitWithCode;

	/* Array of key status information saying what is currently pressed. See isKeyPressed(). */
	bool *m_pKeyStatus;

	/* window width, in pixels */
	int m_iWindowWidth;

	/* window height, in pixels */
	int m_iWindowHeight;

	/* true if window needs a redraw, false if nothing has changed */
	bool m_bNeedsRedraw;

	/* Last clicked X coordinate of mouse - button down */
	int m_iMouseXClickedDown;
	/* Last clicked Y coordinate of mouse - button down */
	int m_iMouseYClickedDown;

	/* Last clicked X coordinate of mouse - button up */
	int m_iMouseXClickedUp;
	/* Last clicked Y coordinate of mouse - button up */
	int m_iMouseYClickedUp;

	/* Current X coordinate of mouse */
	int m_iCurrentMouseX;
	/* Current Y coordinate of mouse */
	int m_iCurrentMouseY;
	/* Current states of buttons */
	int m_iCurrentButtonStates;

	// Displayable objects
public:
	/*
	In your subclass implementation of this you need to create any movable objects that you wish to use.
	Sub-classes need to implement this function if they have objects moving.
	See the examples in the demos
	*/
	virtual int virtInitialiseObjects();

public:
	/*
	Draw all of the displayable objects.
	You probably don't need to change this, but you may need to disable it, e.g. to prevent objects being drawn in different states?
	This just asks the objects to draw themselves, one at a time.
	*/
	virtual void drawAllObjects();

	/* Tell all displayable objects to update themselves. Calls virtDoUpdate on each displayable object. */
	virtual void updateAllObjects(int iCurrentTime);


	//
	// Image Management 
	//
public:
	// Get an image object which you can use for drawing
	// imageURL is the URL to the image file. I suggest to use relative rather than absolute paths
	// loadPermanently is a flag to say whether to keep it loaded after you allow the resulting SimpleImage object to be destroyed. If you pass true then it will avoid reloading (using the same object), but potentially waste memory if you don't need it again.
	// Note: SimpleImage contains a smart pointer, so as long as a SimpleImage object exists to refer to that image the image will stay loaded. When the last SimpleImage refering to it vanishes it will unload the image from memory.
	// DO NOT STORE A POINTER TO THE SimpleImage - always store the (quite small) SimpleImage object itself, to ensure that the smart pointers work correctly. This should not be wasteful as the SimpleImage just wraps up a single smart pointer anyway, so is very small.
	SimpleImage loadImage(std::string imageURL, bool loadPermanently = false)
	{
		return ImageManager::get()->getImagebyURL( imageURL, true, loadPermanently );
	}
	
	//
	// Font management
	//
public:

	// Get a pointer to the default font to use for text - the one which was loaded initially
	Font * getDefaultFont() const { return m_pDefaultFont; }

	/* Get (possibly loading) a specific font. If already loaded it will just give you the existing pointer from loading it last time. */
	Font* getFont(const char* szFontName, int iFontSize)
	{
		return m_oFontManager.getFont(szFontName, iFontSize);
	}

private:
	/* Manager object for the fonts - maintains a cache of fonts to prevent multiple loading of fonts. */
	FontManager m_oFontManager;

	/* Main default display font for items. Often used if a font of NULL is specified. */
	Font* m_pDefaultFont;




	// SDL stuff:
protected:
	/* This is a flag saying whether to wait for events rather than polling for them. 
	If you set this flag then less CPU time will be used, because the system will block until an event comes in, however the functions like drawing and update won't be called until an event happens so you can't have a real time AI player for example.
	*/
	int m_bBlockWaitingForEvents;

	/* Window - used by SDL2 */
	SDL_Window *m_pSDL2Window = nullptr;
	SDL_Renderer *m_pSDL2Renderer = nullptr;
	SDL_Texture *m_pSDL2Texture = nullptr;


protected:
	/* window surface, the SDL structure. */
	DrawingSurface m_oTheForegroundSurface;

	/* Background surface - this is used as a temporary surface to draw to for the background. When objects move they are removed by redrawing this into the display window (foreground surface). */
	DrawingSurface m_oTheBackgroundSurface;


	// Pointer - allowing subclass to change which surface is used if necessary
	DrawingSurface* m_pForegroundSurface;
	DrawingSurface* m_pBackgroundSurface;
};

#endif


