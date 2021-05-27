#pragma once

// This class has been created to avoid you having to work out how to write SIMPLE collision detection code.
// See the MazeDemo for an example of using this for circular collision detection
// It will do Circle-Circle or Rectangle-Rectangle collision, but nothing else.
// Note that the methods are static so you need never create an object of this class!

class CollisionDetection
{
public:
	// Prevent people being able to create objects of this class:
	CollisionDetection() = delete;

	/*
	Check for collision between two rectangles
	x1l and x1r are the first rectangle's x coordinates for left and right sides
	y1t and y1b are the first rectangle's y coordinates for top and bottom
	x2l and x2r are the second rectangle's x coordinates for left and right sides
	y2t and y2b are the second rectangle's y coordinates for top and bottom
	*/
	static bool checkRectangles(int x1l, int x1r, int y1t, int y1b, int x2l, int x2r, int y2t, int y2b)
	{
		return !((x1r < x2l) || (x2r < x1l) || (y1b < y2t) || (y2b < y1t));
	}

	/*
	Check for collision between two circles
	x1c, y1c is the centre of the first circle
	x2c, y2c is the centre of the second circle
	*/
	static bool checkCircles(int x1c, int y1c, int x2c, int y2c, int gap )
	{
		return ((x1c-x2c)*(x1c - x2c) + (y1c - y2c)*(y1c - y2c)) < (gap * gap);
	}

};
