#pragma once

// See DrawingFilter.h for implementations of these

class DrawingSurface; // Declaration that DrawingSurface is a class so we can use a pointer to it as a parameter type

/*
Points can be filtered before drawing - e.g. to ensure not drawing outside of the display.
These filters could also be used to shift coordinates (e.g. for scrolling?) or to multiply/divide, e.g. for zoom in and out, without changing drawing code!
See the file DrawingFilters.h for some examples of sub-classes which implement these features. You do NOT NEED to use these to complete the coursework, but they could simplify some otherwise-complex tasks for you.
Students who find C++ easy will find this to be powerful.
Students who are struggling with this should just ignore filters - they are only useful for advanced features anyway

Examples of use: (See ExampleFilterPointClasses.h for example implementations)
Limiting drawing to the visible window, or any redraw rectangle. Just return false from the filter() function if a coordinate outside of the redraw area is passed in and it won't try to colour the pixel.
Translation of coordinates by adding 100 to x, moving the display to the side. The filter would take in a virtual position of x and increase it by 100 to get the real position. Or a real position of x and decrease it by 100 to get a virtual position.
Zoom in/zoom out: this means multiplying of dividing the current virtual drawing position to get the real position on the screen.
A filter could be applied then ask another filter to apply its change too, allowing you to chain filters for things like zoom and translation.
*/
class FilterPoints
{
public:
	/*
	This asks the filter to decide upon whether to draw a point, and also asks it to convert the coordinates from a virtual point on a display to a real pixel position, if desired.
	surface : the surface that is being drawn to, e.g. to find the size of the displayable region.
	x : the virtual x position to draw. If no mapping is performed then this will not be changed. If some kind of translation is performed then this value, which is passed in by reference, should be modified appropriately.
	y : the virtual y position to draw. If no mapping is performed then this will not be changed. If some kind of translation is performed then this value, which is passed in by reference, should be modified appropriately.
	uiColour : the pixel colour to draw with. In theory you could change this. The current filters for zooming use this to draw groups of pixels rather than just one when zoomed in.
	*/
	virtual bool filter(DrawingSurface* surface, int& x, int& y, unsigned int& uiColour) = 0;

	/*
	If your filter is implementing virtual positions then this converts from a real screen pixel position to a virtual position, according to the translation that the filter applies. 
	*/
	virtual int filterConvertRealToVirtualXPosition(int x) { return x; }
	/*
	If your filter is implementing virtual positions then this converts from a real screen pixel position to a virtual position, according to the translation that the filter applies.
	*/
	virtual int filterConvertRealToVirtualYPosition(int y) { return y; }
	/*
	If your filter is implementing virtual positions then this converts from a virtual pixel position to a real screen position, according to the translation that the filter applies.
	*/
	virtual int filterConvertVirtualToRealXPosition(int x) { return x; }
	/*
	If your filter is implementing virtual positions then this converts from a virtual pixel position to a real screen position, according to the translation that the filter applies.
	*/
	virtual int filterConvertVirtualToRealYPosition(int y) { return y; }
};
