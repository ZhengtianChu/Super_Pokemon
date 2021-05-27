#pragma once

// Class created to allow people to specify an alignment for drag operations
// This stores a pixel multiple and ensures that all final draw positions line up on that pixel multiple.

class DragAlignment
{
public:
	virtual void align(int x, int y)
	{
		// Default does nothing
	}
};

class DragAlignmentMultiple : public DragAlignment
{
	int m_iXAlign, m_iYAlign;

public:
	// Constructor takes the multipliers to use for alignments
	DragAlignmentMultiple(int iXAlign, int iYAlign)
		: m_iXAlign(iXAlign), m_iYAlign(iYAlign)
	{

	}

	// Align on a multiple of the sizes passed in
	virtual void align(int x, int y) override
	{
		// Default does nothing
		x -= x%m_iXAlign;
		y -= y%m_iYAlign;
	}
};
