#include "header.h"

#if defined(_MSC_VER)
#include <SDL_syswm.h>
#endif

#include <cstdio>

#include <math.h>	

#include "BaseEngine.h"
#include "DisplayableObject.h"



/*
This is a GLOBAL C-style function which 
Note that the extern "C" tells compiler to generate C-style names and code, because we are going to pass the function pointer to a function which expects a C-style function (so we need to give it one).
You can safely ignore this function for the exam and coursework. It's really an example of using extern "C" for those of you who care.
I use this so that I can see that the program ended normally rather than abnormally but it doesn't really matter.
*/
extern "C" void mydebug()
{
	puts("Ended normally (output from mydebug() function)");
}



/*
Constructor.
You need to use the initialisation list to initialise everything.
It doesn't do much else really apart from store the addresses of the two surfaces (foreground and background) to use by default.
*/
BaseEngine::BaseEngine()
	: m_pKeyStatus(NULL)
	, m_iExitWithCode(-1)
	, m_iTick(0)
	, m_iNextTickToActOn(0)
	, m_pDefaultFont(NULL)
	, m_iMouseXClickedDown(0)
	, m_iMouseYClickedDown(0)
	, m_iMouseXClickedUp(0)
	, m_iMouseYClickedUp(0)
	, m_iCurrentMouseX(0)
	, m_iCurrentMouseY(0)
	, m_iCurrentButtonStates(0)
	, m_iTimeOffset(0)
	, m_oTheForegroundSurface(this)
	, m_oTheBackgroundSurface(this)
	, m_pForegroundSurface(nullptr)
	, m_pBackgroundSurface(nullptr)
	, m_iPaused(-1)
	, m_bNotifyObjectsAboutKeys(false)
	, m_bNotifyObjectsAboutMouse(false)
	, m_bNotifyTopObjectAboutMouse(false)
	, m_bNotifyBottomObjectAboutMouse(false)
	, m_bBlockWaitingForEvents(false) // don't wait for events before continuing each loop
	, m_bNeedsRedraw(true)
	, m_bTotalRedraw(true)
	, m_iRedrawVirtualLeft(INT_MAX / 2)
	, m_iRedrawVirtualTop(INT_MAX / 2)
	, m_iRedrawVirtualRight(-INT_MAX / 2)
	, m_iRedrawVirtualBottom(-INT_MAX / 2)
	, m_iRedrawRealLeft(INT_MAX / 2)
	, m_iRedrawRealTop(INT_MAX / 2)
	, m_iRedrawRealRight(-INT_MAX / 2)
	, m_iRedrawRealBottom(-INT_MAX / 2)
	, m_iDefaultUpdatePeriod(10)
	, m_iWindowHeight(0)
	, m_iWindowWidth(0)
{
	m_pForegroundSurface = &m_oTheForegroundSurface;
	m_pBackgroundSurface = &m_oTheBackgroundSurface;
}


/*
Destructor - does nothing but note that the destructors for the contained classes will also be called automatically.
Importantly, the destructors for superclasses will also be called (including for DisplayableObjectContainer, which will destroy the contained objects by default)
*/
BaseEngine::~BaseEngine(void)
{
}





/* 
This function calls SDL functions, and the aim of theis coursework is to hide their complexity from you so you probably just want to call this and not worry about what it does.
Note that it uses the members m_iWindowWidth, m_iWindowHeight to determine window size.
Hint: don't bother to override this unless you want to do something unusual
Note: don't try to put the window memory on the graphics card (although that would be faster), because a load of functions assume it is in main memory and can be altered using a pointer.
If I put it on the graphics card then then it would be harder for you to work through the code and see what it does, which would be a worse learning C++ experience, although a lot more efficient.
*/
void BaseEngine::virtCreateWindows(const char* szCaption)
{
	// This makes the surfaces work with software rendering, so it is still a bitmapped display
	// WARNING: This is definitely not the fastest performance way to do this by any means, but it means that the bitmapped display will be more of a pointer/C++ example rather than being hidden.
	m_pSDL2Window = SDL_CreateWindow(szCaption,
		10, 40, /* If you hardcode these, allow room for the window border and title bar */
				//SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // Use this to let it position the window for you.
		m_iWindowWidth, m_iWindowHeight,
		0/*SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL*/); // Admittedly making it full screen is something some people may want to do, but not for this coursework please.
	m_pSDL2Renderer = SDL_CreateRenderer(m_pSDL2Window, -1, 0);
	// Don't change the format of the pixels - ARGB (8bits each) is assumed in many functions to manually alter the pixels, to demonstrate C++.
	m_pSDL2Texture = SDL_CreateTexture(m_pSDL2Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_iWindowWidth, m_iWindowHeight);
}


/* You could override this, e.g. to create different sized surfaces, but I assume you probably won't usually do so.
Hint: don't bother to override this unless you want to do something unusual
Bigger hint: You can create your own surfaces and tell it to use those instead if you wish, which may be easier than altering these defaults.
*/
void BaseEngine::virtCreateSurfaces()
{
	// Foreground surface, for the moving objects on top of background. This is basically the window content.
	m_oTheForegroundSurface.createSurface(m_iWindowWidth, m_iWindowHeight);
	// Background surface, for the static background stuff. This is the same size as the window and is used for writing over the window contents to undraw objects.
	m_oTheBackgroundSurface.createSurface(m_iWindowWidth, m_iWindowHeight);
}


/*
Initialise SDL and create a window of the correct size.
This function is deliberately not virtual.
Instead of overriding this, I suggest to override the various functions that it calls.
*/ 
int BaseEngine::initialise(const char* szCaption, int iWindowWidth, int iWindowHeight, const char* szFontName, int iFontSize)
{
	m_iWindowWidth = iWindowWidth;
	m_iWindowHeight = iWindowHeight;

	// Initialize SDL's subsystems - must do this before doing anything else with SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	// Add my custom function to call at exit. Not needed but an interesting example of needing an extern "C" function - see mydebug() above.
	atexit(mydebug);

	// Register SDL_Quit to be called at exit; makes sure things are cleaned up when we quit.
	atexit(SDL_Quit);

	// True type font stuff - ignore this, basically it initialises the library at the start and tells the system to deinitialise it at the end.
	TTF_Init();
	atexit(TTF_Quit);

	// This looks at the font list that the font manager knows about (empty at the moment) and loads them. It also intialises data structures.
	// Ignore this as you don't need to change it.
	m_oFontManager.loadFonts();

	// Load the default font - used whenever NULL is passed
	if ( ( szFontName != NULL ) && ( strlen(szFontName) > 0 ) )
		m_pDefaultFont = m_oFontManager.getFont( szFontName, iFontSize );

	// Calls a virtual function which you could (but probably will not) modify, to create the actual window to draw inside.
	virtCreateWindows(szCaption);

	// Calls a virtual function which will tell the foreground and background surfaces to make SDL calls to create themselves (allocates memory etc).
	virtCreateSurfaces();
	
	// Initialise key status - this is an array which stores details of whether a key has been pressed or released, so you can ask whether a key is currently pressed down
	m_pKeyStatus = new bool[1 + SDL12_SDLK_LAST];
	memset( m_pKeyStatus,0,sizeof( bool )*(1 + SDL12_SDLK_LAST) );

	// Call method to allow custom sub-class initialiation.
	// Put your own initialisation in your own version of virtInitialise() to ensure it is done at the start.
	// Or just implement the function it calls: virtSetupBackgroundBuffer() and virtInitialiseObjects()
	virtInitialise();

	return 0; // success
}


/*
Initialise game/program-specific things.
Base class just calls virtSetupBackgroundBuffer() to set up the background, then virtInitialiseObjects() to create the moving objects
*/
int BaseEngine::virtInitialise()
{
	// Set up the initial background
	m_pBackgroundSurface->mySDLLockSurface();
	virtSetupBackgroundBuffer();
	m_pBackgroundSurface->mySDLUnlockSurface();

	// Create the moving objects
	virtInitialiseObjects();

	return 0; // success
}



/*
In here you need to create any displayable objects (usually moving things) that you wish to use.
Your sub-class will need to implement this function and could do it in a variety of ways - see the demo classes.
You can use the CreateObjectArray() and StoreObjectInArray() functions to create it.
*/
int BaseEngine::virtInitialiseObjects()
{
	// You can actually do the following at any time, but you will not often need to do so.
	// You will usually have a virtInitialiseObjects() method so it's a convenient place to put the calls.
	// All these actually do is set flags in the framework to tell it to pass on events to the objects themselves to handle
	// It is usually easier just to handle them in the Engine sub-class though, unless you are doing something like custom 'drag' operations.

	// Optionally tell the system to send mouse clicks and moves to the objects
	//notifyObjectsAboutMouse(true);
	
	// Optionally tell the system to send key presses and release notifications to the objects
	//notifyObjectsAboutKeys(true);
	
	// Optionally tell the system to send mouse clicks and moves to the top-most object
	//notifyTopObjectAboutMouseClick(true);
	
	// Optionally tell the system to send mouse clicks and moves to the bottom-most object
	//notifyBottomObjectAboutMouseClick(true);

	// Now we get to the part that you will usually need to implement - creating your displayable objects:

	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	// Always do this in case someone calls your function from an unusual place.
	// The key reason is you could be iterating through the array and you are about to change it. Calling this tells the main iteration functions (e.g. for draw or update all objects) to abort and start again
	drawableObjectsChanged();

	// Destroy any existing objects - if there were existing ones then get rid of these first
	// Actually unnecessary as long as you don't change createObjectArray() because it will do this for you anyway.
	destroyOldObjects(true);

	// Creates an array for the elements - always call this before trying to use the array!
	// Make it big enough for the elements you will store - so you can use 'storeObjectInArray'.
	// Otherwise you will need to use something like 'appendObjectToArray', which will resize the array when it adds to it
	//createObjectArray(3);

	// Store the pointers to the displayable objects:
	// By default the destroyOldObjects() will destroy them for you when no longer needed.
	// StoreObjectInArray( 0, new MyDisplayableObject( this, m_iPlayer1StartX, m_iPlayer1StartY);
	// StoreObjectInArray( 1, new MyDisplayableObject( this, m_iPlayer2StartX, m_iPlayer2StartY);

	// Return a count of the number of objects that you created
	return 0;
}







// Handle an SDL event
// You should not need to call or alter this function
// You should not even need to understand what it does, but basically it 
bool BaseEngine::handleEvent(SDL_Event* pEvent)
{
	bool bObjectHandledClick = false;

	// FIX_FOR_SPURIOUS_CLICK
	// In testing I found that a spurious click event was sent to the system in release mode, with x == y == 0 : I don't actually know why though. 
	// This code throws it away because sometimes it caused some issues for some programs.
	// If anyone knows why this happens, please do let me know!
	static bool firstClick = true;
	if (firstClick && (pEvent->type == SDL_MOUSEBUTTONDOWN) )
	{
		firstClick = false;
		if ( (pEvent->button.x == 0) && (pEvent->button.y == 0) )
			return true; // Skip if this is the rubbish one at the start which occurs when in release mode (but not debug mode) - not sure why!
	}
	// END FIX_FOR_SPURIOUS_CLICK

	// Now the real code - handle each event from SDL in turn, e.g. key press/release, mouse move, etc, by calling a relevant virtual function.

	switch (pEvent->type)
	{
	case SDL_KEYDOWN:
		//fprintf(fdebug,"Key down: %d, '%c'\n", symbolToKeycode(pEvent->key.keysym.sym), pEvent->key.keysym.sym);
		m_pKeyStatus[symbolToKeycode(pEvent->key.keysym.sym)] = true;
		virtKeyDown(pEvent->key.keysym.sym);
		if (m_bNotifyObjectsAboutKeys)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtKeyDown(pEvent->key.keysym.sym); });
		break;

	case SDL_KEYUP:
		//fprintf(fdebug, "Key up: %d, '%c'\n", symbolToKeycode(pEvent->key.keysym.sym), pEvent->key.keysym.sym);
		m_pKeyStatus[symbolToKeycode(pEvent->key.keysym.sym)] = false;
		virtKeyUp(pEvent->key.keysym.sym);
		if (m_bNotifyObjectsAboutKeys)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtKeyUp(pEvent->key.keysym.sym); });
		break;

	case SDL_MOUSEMOTION:
		//fprintf(fdebug, "SDL_MOUSEMOTION: %d, '%d'\n", pEvent->motion.x, pEvent->motion.y );
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iCurrentMouseX = m_pForegroundSurface->convertRealToVirtualXPosition(pEvent->motion.x);
		m_iCurrentMouseY = m_pForegroundSurface->convertRealToVirtualYPosition(pEvent->motion.y);
		virtMouseMoved(m_iCurrentMouseX, m_iCurrentMouseY);
		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseMoved(m_iCurrentMouseX, m_iCurrentMouseY); });
		break;

	case SDL_MOUSEBUTTONDOWN:
		//fprintf(fdebug, "SDL_MOUSEBUTTONDOWN: %d, '%d'\n", pEvent->button.x, pEvent->button.y);
		virtRealMouseDown(pEvent->button.button, pEvent->button.x, pEvent->button.y);
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iMouseXClickedDown = m_pForegroundSurface->convertRealToVirtualXPosition(pEvent->button.x);
		m_iMouseYClickedDown = m_pForegroundSurface->convertRealToVirtualYPosition(pEvent->button.y);
		virtMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown);

		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown); });

		if (m_bNotifyBottomObjectAboutMouse)
		{
			for (int i = 0; i < m_vecDisplayableObjects.size(); i++)
				if (m_vecDisplayableObjects[i] != nullptr
					&& m_vecDisplayableObjects[i]->virtSingleItemMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown))
				{
					bObjectHandledClick = true;
					break; // from for loop - this one handled it, so stop now
				}
		}

		if (m_bNotifyTopObjectAboutMouse)
		{
			for (int i = m_vecDisplayableObjects.size() - 1; i >= 0; i--)
				if (m_vecDisplayableObjects[i] != nullptr
					&& m_vecDisplayableObjects[i]->virtSingleItemMouseDown(pEvent->button.button, m_iMouseXClickedDown, m_iMouseYClickedDown))
				{
					bObjectHandledClick = true;
					break; // from for loop - this one handled it, so stop now
				}
		}

		if (!bObjectHandledClick)
		{ // No object was clocked, so notify that the background was clicked
			virtBackgroundMouseDown(pEvent->button.button, pEvent->button.x, pEvent->button.y, m_iMouseXClickedDown, m_iMouseYClickedDown);
		}
		break;

	case SDL_MOUSEBUTTONUP:
		//fprintf(fdebug, "SDL_MOUSEBUTTONUP: %d, '%d'\n", pEvent->button.x, pEvent->button.y);
		virtRealMouseUp(pEvent->button.button, pEvent->button.x, pEvent->button.y);
		// Note: If you are not using a filter this just assigns the motion.x to the m_iCurrentMouseX. If using a filter it may convert the point. See filter at top of DrawingSurface if you really need to know what this is doing.
		m_iMouseXClickedUp = m_pForegroundSurface->convertRealToVirtualXPosition(pEvent->button.x);
		m_iMouseYClickedUp = m_pForegroundSurface->convertRealToVirtualYPosition(pEvent->button.y);
		virtMouseUp(pEvent->button.button, m_iMouseXClickedUp, m_iMouseYClickedUp);
		if (m_bNotifyObjectsAboutMouse)
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) { if (pdo == nullptr) return; pdo->virtMouseUp(pEvent->button.button, m_iMouseXClickedUp, m_iMouseYClickedUp); });
		//redrawDisplay();
		break;

	case SDL_MOUSEWHEEL:
		virtMouseWheel(pEvent->wheel.x, pEvent->wheel.y, pEvent->wheel.which, pEvent->wheel.timestamp);
		break;

	case SDL_FINGERMOTION:
	case SDL_FINGERDOWN:
	case SDL_FINGERUP:
		virtTouchEvent(pEvent->tfinger.type, pEvent->tfinger.timestamp, pEvent->tfinger.touchId, pEvent->tfinger.fingerId, pEvent->tfinger.x, pEvent->tfinger.y, pEvent->tfinger.dx, pEvent->tfinger.dy, pEvent->tfinger.pressure);
		break;

	case SDL_QUIT:
		return false;
	}
	return true;
}




/*
Main loop - do not change this, instead override the various functions it calls and set various flags to change its behaviour.
This does the following:
1) Handles key presses and events. 
	Either in blocking mode (does nothing and waits for user input, so nothing else moves/redraws while it is waiting) or in normal mode (e.g. with AI controlled moving objects)

2) Calls virtCheckForUpdateTime(), which subclasses can change. If it returns true then it asks all objects to update
	By default virtCheckForUpdateTime() just checks whether it is time to do an update yet.
	For the coursework you will NOT want blocking mode, because you need to have moving/changing objects.
	The card game and jigsaw program use this to stop the program consuming CPU time while waiting for user input.
	To enter blocking mode just set m_bBlockWaitingForEvents to true in a sub-class.
3) Calls virtRenderScreen() to draw the screen
	If redrawDisplay() was called, this will copy all of the background onto the screen.
	Otherwise, if redrawRectangle() can called, it will copy that rectangle of pixels from the background
	It will then draw strings underneath, then ask objects to draw themselves, then draw strings on top, to draw the screen.
	This means that your objects either need to call redrawRectangle() to ensure that the part of the window that they occupy is redrawn, 
	or redrawDisplay() to redraw the entire window contents (which may be more time-consuming).

There is a method to ensure that you can make this work the same refresh rate even on faster PCs.
	Each time through the loop it calls virtCheckForUpdateTime() to see whether it is time to do another update yet.

You can implement various methods to do things at different times:
virtMainLoopPreUpdate() : continuously called each time around the loop, even if it is not time to do an update
virtMainLoopDoBeforeUpdate() : called for any loop when it is time to do an update - BEFORE the update functions are called on the visible objects
	This is often used in sub-classes to do something to drawing surfaces that should happen whenever the objects update
	e.g. to do a scrolling or rotating background - see ImageMappingDemo, StarfieldDemo and FlashingDemo
virtMainLoopDoAfterUpdate() : called for any loop when it is time to do an update - AFTER the update functions are called on the visible objects
virtMainLoopPostUpdate() : continuously called each time around the loop, even if it is not time to do an update, AFTER the updates would have been done
*/
int BaseEngine::mainLoop(void)
{
    SDL_Event event;
	int iKeyCode;

	// Main loop: loop until told not to
	while (m_iExitWithCode==-1)
	{
		// Hook to allow subclasses to do something here
		virtMainLoopStartIteration();

		if (m_bBlockWaitingForEvents) // Block waiting for events rather than constantly looping making changes.
		{ // Block waiting for the next event - i.e. don't do anything while there are no UI events - so no AI movement etc.
			// Wait for an event to happen - keeps cpu usage low but don't use it if you need things to move without user interactions
			if (SDL_WaitEvent(&event))
				if (!handleEvent(&event))
					return 0;
		}

		// Keep polling for subsequent events until they have all been handled, rather than handling one at a time per virtual function call.
		// Note that if m_bBlockWaitingForEvents was not set then this will ensure that all pending SDL events (mouse/key clicks etc) are handled, then continue to the next part of the code.
		// If there are no pending events then this loop ends immediately, allowing code to go to the next code.
		while (SDL_PollEvent(&event))
			if (!handleEvent(&event))
				return 0;

		// Hook to allow subclasses to do something here
		virtMainLoopPreUpdate();

		// Render stuff
		m_iTick = SDL_GetTicks();

		// Called to allow sub-classes to easily add behaviour
		if (virtCheckForUpdateTime()) // If it is time for a change...
		{
			// Do any updates for movements
			virtMainLoopDoBeforeUpdate();
			updateAllObjects(getModifiedTime());
			virtMainLoopDoAfterUpdate();
		}

		// Hook to allow subclasses to do something here
		virtMainLoopPostUpdate();

		// Render the screen - only draws if some update handler told it to draw
		virtRenderScreen();
	}
	return 0;
}




// Change behaviour in subclass if needed.
// If this returns false then objects will not be told to move - allowing you to freeze them all if you wish
// Basically all this function does is check the current time and see whether it is time to make another move. 
//		If it is then it updates the time at which to do the next move and returns true, to say call the update on each object
//		If it is not then it just returns false
bool BaseEngine::virtCheckForUpdateTime(void)
{
	// Ensure update period hasn't been set too small
	if ( m_iDefaultUpdatePeriod < 8) 
		m_iDefaultUpdatePeriod = 8;

	if (this->isTimeToAct())
	{
		this->setTimeToAct(m_iDefaultUpdatePeriod); // Max 1 per update period ms
		return true;
	}
	else
	{
		return false; // Don't move anything
	}
}



/*
Overridable function, if necessary, for doing all of the drawing.
This function locks the surfaces so that they are ready for drawing to, 
	then optionally copies either the entire background or just some update window from background to foreground
	then calls a few virtual functions to allow your subclass to do the drawing:
		virtPreDraw() : do anything you need to draw at the very back
		virtDrawStringsUnderneath() : draw strings/text on the background
		Calls virtDraw() on each object via a call to drawAllObjects()
		virtDrawStringsOnTop(): draw strings/text which should appear on top of moving objects - i.e. objects move behind this text
		virtPostDraw() : draw anything which has to appear on the very top of the window
	finally it unlocks the surfaces and copies them onto the window being displayed. So the final image is shown to the user.
*/
void BaseEngine::virtRenderScreen(void)
{
	if ( !m_bNeedsRedraw )
		return;

	// Note: the redraw flags must only be set early in this method, not at the end, since the drawing/moving of the moving objects may
	// potentially flag a win/lose condition and a state change, which will set the redraw flag again to force a full draw after the 
	// state changes.

	// Lock foreground surface if necessary
	m_pForegroundSurface->mySDLLockSurface();
	m_pBackgroundSurface->mySDLLockSurface();

	// First clear the screen by redrawing the background
	//std::cout << "Redraw background..." << std::endl;
	if (m_bTotalRedraw)
		copyAllBackgroundBuffer();
	else // Partial redraw so only clear the part of screen we are redrawing.
	{ // Copy background pixels from the redraw region only
		//this->copyBackgroundPixels(m_iRedrawRealLeft -1, m_iRedrawRealTop -1, m_iRedrawRealRight- m_iRedrawRealLeft +2, m_iRedrawRealBottom - m_iRedrawRealTop +2);
		this->copyBackgroundPixels(m_iRedrawRealLeft, m_iRedrawRealTop, m_iRedrawRealRight - m_iRedrawRealLeft, m_iRedrawRealBottom - m_iRedrawRealTop);
		//std::cout << "Clear background, real l " << std::to_string(m_iRedrawRealLeft) << ",  r " << std::to_string(m_iRedrawRealRight)
		//	<< " : t " << std::to_string(m_iRedrawRealTop) << ",  b " << std::to_string(m_iRedrawRealBottom) << std::endl;
		//std::cout << "Clear background, virtual l " << std::to_string(m_iRedrawVirtualLeft) << ",  r " << std::to_string(m_iRedrawVirtualRight)
		//	<< " : t " << std::to_string(m_iRedrawVirtualTop) << ",  b " << std::to_string(m_iRedrawVirtualBottom) << std::endl;
	}

	// Draw anything else before the strings and objects
	virtPreDraw();

	// Draw the text for the user
	//std::cout << "virtDrawStringsUnderneath..." << std::endl;
	virtDrawStringsUnderneath();	// Draw the string underneath the other objects here
	// Then draw the changing objects
	//std::cout << "drawAllObjects..." << std::endl;
	drawAllObjects();
	// Another option for where to put the draw strings code - if you want it on top of the moving objects
	//std::cout << "virtDrawStringsOnTop..." << std::endl;
	virtDrawStringsOnTop();

	// Draw anything else afterwards
	virtPostDraw();

	// Lock foreground surface if necessary
	m_pBackgroundSurface->mySDLUnlockSurface();
	m_pForegroundSurface->mySDLUnlockSurface();
	//std::cout << "Finished draw..." << std::endl;

	// Copy surface to a texture, which is then rendered to the screen.
	m_pForegroundSurface->mySDLUpdateTexture(m_pSDL2Texture);
	SDL_RenderCopy( m_pSDL2Renderer,m_pSDL2Texture,NULL,NULL );
	SDL_RenderPresent( m_pSDL2Renderer );

	// Just drawn so no more redraw needed
	m_bNeedsRedraw = false;
	m_bTotalRedraw = false;
	m_iRedrawVirtualLeft = 0xffffff;
	m_iRedrawRealLeft = 0xffffff;
	m_iRedrawVirtualTop = 0xffffff;
	m_iRedrawRealTop = 0xffffff;
	m_iRedrawVirtualRight = -0xffffff;
	m_iRedrawRealRight = -0xffffff;
	m_iRedrawVirtualBottom = -0xffffff;
	m_iRedrawRealBottom = -0xffffff;
}












/* Deinitialise everything - delete all resources
You will not change this, and will instead implement the virtCleanUp() function to do your cleaning up of resources.
*/
void BaseEngine::deinitialise(void)
{
	// Call method to allow custom sub-class cleanup
	virtCleanUp();

	// Free memory for key status record
	delete[] m_pKeyStatus;
}


/*
Overridable function, for adding custom clean-up in sub-classes.
Your implementation should probably reverse your virtInitialise() implementation, freeing resources that you allocated.
*/
void BaseEngine::virtCleanUp(void)
{
}





/*
Determine whether a specific key is currently pressed
Utility function that you called.
Just checks its array which tracks key presses and releases and returns the status.
*/
bool BaseEngine::isKeyPressed(int iKeyCode) const
{
	return m_pKeyStatus[symbolToKeycode(iKeyCode)];
}


/* 
Override this to handle a key being released - it will be called when an SDL event it received saying that the key was released.
Note that the fact that the key has been released will have already been stored for use by isKeyPressed() by the time this is called.
*/
void BaseEngine::virtKeyUp(int iKeyCode)
{
}

/* Override this to handle a key being pressed - it will be called when an SDL event it received saying that the key was pressed.
Note that the fact that the key has been pressed will have already been stored for use by isKeyPressed() by the time this is called.
*/
void BaseEngine::virtKeyDown( int iKeyCode )
{
}


/* Override this to handle the fact that a mouse has moved
Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish: e.g. see getCurrentMouseX(), getCurrentMouseY()
*/
void BaseEngine::virtMouseMoved( int iX, int iY )
{
	//printf( "BaseEngine::MouseMoved %d, %d\n", iX, iY );
	//DrawForegroundThickLine( m_iMouseXClickedDown, m_iMouseYClickedDown, iX, iY, 0xff0000, 1 );
}

/* Override this to handle the fact that a mouse button has been pressed
Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
*/
void BaseEngine::virtMouseDown( int iButton, int iX, int iY )
{
	// Test code if you want it?
	// printf( "BaseEngine::MouseDown %d, %d, %d\n", iButton, iX, iY );
}

/*
Override this to handle the fact that a mouse button has been released
Note: these coordinates are automatically stored for you and there are methods to retrieve them if you wish
*/
void BaseEngine::virtMouseUp( int iButton, int iX, int iY )
{
	// Test code if you want it?
	// printf( "BaseEngine::MouseUp %d, %d, %d\n", iButton, iX, iY );
}



















/* 
You should implement this in your subclass to draw the background for your program.
The default just fills the background with yellow.
*/
void BaseEngine::virtSetupBackgroundBuffer()
{
	// Put any special code to call different render functions for different states here
	fillBackground( 0xffff00 );
}






// 
// You probably need to override the following functions to draw text onto the screen
// The key question is whether you want the text to appear on top of or underneath the moving objects.
// i.e. can objects move over the text, or underneath it if they are in the same position.
// See also the comments on virtRenderScreen() above to see how this works.
//



/* Draw the string that moving objects should be drawn on top of.
You can either do everything for strings in this function if you wish, ignoring UnDrawStrings() and DrawStringsOnTop(),
or you can put the background removal in UnDrawStrings(), then use this to draw strings behind moving objects and 
the three below, or you can split the contents.
*/
void BaseEngine::virtDrawStringsUnderneath()
{
	// Example Version 1: If using only this function:
	//		First clear the area where they were originally drawn - i.e. undraw previous text
	//		e.g. CopyBackgroundBuffer()
	//		Then draw the new labels/strings
	//		e.g. m_pFont->DrawString(m_pForegroundSurface, 100, 180, "The DrawStringsUnderneath method needs to be overridden", 0xff00ff);
	//		or DrawForegroundString( 150,10,"Example text",0xffffff,NULL );
	// OR Example Version 2 - assuming the string removal is in UnDrawStrings() above
	//		Just draw any strings you want to draw...
	//		e.g. DrawForegroundString( 150,10,"Example text",0xffffff,NULL );
	// For efficiency I recommend version 2, so that the undrawing is only done when needed, rather than every time the strings are drawn,
	// however, that may be harder for some of you to understand hence the option to put it all in here if you wish.
}

/* Draw any string which should appear on top of moving objects - i.e. objects move behind these.
There are two differences between this and the Underneath() function above:
1: These strings appear on top of the moving objects (i.e. they are drawn AFTER the moving objects)
2: Because of when this function is called, you cannot remove the strings in it. i.e. you need to implement UnDrawStrings()
*/
void BaseEngine::virtDrawStringsOnTop()
{
	// Example:
	// DrawForegroundString( 150,60,"Example text",0xffffff,NULL );
}












/*
Draws draws the displayable objects in their new positions.
You do not change this - instead you just implement the virtDraw() methods on the objects.
*/
void BaseEngine::drawAllObjects()
{
	for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		if (*it) 
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		//std::cout << "starting draw loop..." << std::endl;
		m_bDrawableObjectsChanged = false; // Moved to inside loop to avoid infinite loop
		finished = true;
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					//std::cout << "draw " << (int)(*it) << std::endl;
					(*it)->setHasActed(true);
					(*it)->virtDraw();
				}
				if (m_bDrawableObjectsChanged)
				{
					finished = false;
					break; // from the for statement - will repeat loop again
				}
			}
		}
	}
}




/* Tell all displayable objects to update themselves. Calls virtDoUpdate on each displayable object.
You do not change this - instead you just implement the virtDoUpdate() methods on the objects.
*/
void BaseEngine::updateAllObjects(int iCurrentTime)
{
	for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		if (*it) 
			(*it)->setHasActed(false);

	bool finished = false;
	while (!finished)
	{
		m_bDrawableObjectsChanged = false; // Moved to inside loop to avoid infinite loop
		finished = true;
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
		{
			if (*it)
			{
				if (!(*it)->getHasActed())
				{
					(*it)->setHasActed(true);
					(*it)->virtDoUpdate(iCurrentTime);
				}
				if (m_bDrawableObjectsChanged)
				{
					finished = false;
					break; // from the for statement - will repeat loop again
				}
			}
		}
	}
}






/*
Utility function that you may find useful to avoid having to understand the colour codes.
This gets a colour code for a specific index from 0 to 40 inclusive. So if you want 40 different colours you could use these.
Used to avoid creating arbitrary colours and instead have a set of useful values.
*/
unsigned int BaseEngine::getColour(int iColourIndex) const
{
	switch ( iColourIndex )
	{
	case 0: return 0x000000;
	case 1: return 0xff0000;
	case 2: return 0x00ff00;
	case 3: return 0x0000ff;
	case 4: return 0xff00ff;
	case 5: return 0xffff00;
	case 6: return 0x00ffff;
	case 7: return 0xffffff;
	case 8:	return 0xffc0c0;
	case 9: return 0xc0ffc0;
	case 10: return 0xc0c0ff;
	case 11: return 0xc00000;
	case 12: return 0x00c000;
	case 13: return 0x0000c0;
	case 14: return 0xc000c0;
	case 15: return 0xc0c000;
	case 16: return 0x00c0c0;
	case 17: return 0xc0c0c0;
	case 18: return 0xff8080;
	case 19: return 0x80ff80;
	case 20: return 0x8080ff;
	case 21: return 0x800000;
	case 22: return 0x008000;
	case 23: return 0x000080;
	case 24: return 0x800080;
	case 25: return 0x808000;
	case 26: return 0x008080;
	case 27: return 0x808080;
	case 28: return 0xc08080;
	case 29: return 0x80c080;
	case 30: return 0x8080c0;
	case 31: return 0x400000;
	case 32: return 0x004000;
	case 33: return 0x000040;
	case 34: return 0x400040;
	case 35: return 0x404000;
	case 36: return 0x004040;
	case 37: return 0x404040;
	case 38: return 0xc04040;
	case 39: return 0x40c040;
	case 40: return 0x4040c0;
	}
	return 0xcccccc; // Anything else is grey
}





