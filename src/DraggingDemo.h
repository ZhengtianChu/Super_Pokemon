#ifndef DRAGGINGDEMO_H
#define DRAGGINGDEMO_H

#include "BaseEngine.h"
#include "ExampleDragableObjects.h"

/* This is a relatively simple demo but it illustrates a few things:
1) You can put source code anywhere - check out the contents of the .cpp file!
2) It illustrates the more complex way to draw static text, so that some appears on top of and some underneath the moving objects.
3) The object itself looks for key presses and alters its speed according to the pressing of the arrow keys or space
*/
class DraggingDemo : public BaseEngine
{
public:

	/**
	Constructor
	*/
	DraggingDemo()
	{}

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void virtSetupBackgroundBuffer() override;

	// Create any moving objects
	int virtInitialiseObjects() override;

	// Handle pressing of a key
	virtual void virtKeyDown(int iKeyCode) override;
};

#endif
