#pragma once
#include "ImagePixelMapping.h"
#include "SimpleImage.h"


class Scyzc3ImagePixelMappingRotate : public ImagePixelMapping
{
public:
	Scyzc3ImagePixelMappingRotate(double r)
		: rotation(r)
	{
	}

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		// Shift offset to the centre of the image, so we can rotate around centre
		x -= image.getWidth() / 2;
		y -= image.getHeight() / 2;

		// Rotate it
		double dAngle = atan(y / (x + 0.0001));
		if (x < 0)
			dAngle += M_PI;
		double hyp = ::sqrt(x * x + y * y);
		dAngle += (double)rotation;

		x = hyp * ::cos(dAngle);
		y = hyp * ::sin(dAngle);

		// Shift offset back to the corner
		x += image.getWidth() / 2;
		y += image.getHeight() / 2;

		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}

	double getRotation() { return rotation; }
	void setRotation(double r) { rotation = r; }

protected:
	double rotation;
};

class Scyzc3ImagePixelMappingShiftXY : public ImagePixelMapping
{
public:
	Scyzc3ImagePixelMappingShiftXY(double x, double y)
		: xShift(x), yShift(y)
	{

	}

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		if (x < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;

		x += xShift;

		// Ensure it is within the size of the image
		while (x > image.getWidth())
			x -= image.getWidth();
		while (x < 0)
			x += image.getWidth();
		return true;
	}

	// Extract the current drawing position
	double getXShift() { return xShift; }
	double getYShift() { return yShift; }

	// Set the current drawing position
	void setXShift(double x) { xShift = x; }
	void setYShift(double y) { yShift = y; }

protected:
	double xShift;
	double yShift;
};