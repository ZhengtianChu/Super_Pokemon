#pragma once
#include "State.h"
#include "Scyzc3ImagePixelMapping.h"
#include "Scyzc3FilterPoints.h"

class Scyzc3Engine;
class ImagePixelMapping;
class MappableImageObject;

class StartUpState :
    public State, public ImagePixelMapping
{
public:
    void Handle(Context* c);
    StartUpState(Scyzc3Engine* pEngine);
    ~StartUpState();
    void virtSetupBackgroundBuffer();
    void virtMouseDown(int iButton, int iX, int iY);
    int virtInitialiseObjects();
    void virtDrawStringsOnTop();

    void virtMainLoopDoBeforeUpdate();

    void copyAllBackgroundBuffer();
    void virtKeyDown(int ikeyDown) {};
    bool mapCoordinates(double& x, double& y, const SimpleImage& image);

private:
    Scyzc3Engine* m_engine;
    SimpleImage image;
    int m_iOffset = 0;
    Scyzc3ImagePixelMappingRotate rotator;
    Scyzc3ImagePixelMappingShiftXY shifter1;
    MappableImageObject* ob;
    MappableImageObject* ob2;
    Scyzc3FilterPoints m_filterScaling;
    Scyzc3FilterPointsTranslation m_filterTranslation;
    int flag = 0;
  
};

